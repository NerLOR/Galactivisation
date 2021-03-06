//
// Created by lorenz on 30.01.21.
//

#include "Galaxy.h"

#include <utility>
#include "StaticSpaceObj.h"

void Galaxy::AddSystem(System *system) {
    numSystems++;
    systems.push_back(system);
}

void Galaxy::Init(SDL_Renderer *renderer) {
    cam.Init(renderer, GetDiameter());
    AddSystem(new System(0, 0));
    unsigned long long dia = GetDiameter();

    double cycles = 3.5;
    double incr = 1.0 / 64;
    double a = 0;
    auto b = (double) 40000000000000000;
    int c = 2;

    int pointsSize = (int) (cycles * (M_PI / incr + 1) * c);
    Position *points[pointsSize];

    int num = 0;
    for (double i = 0; i <= cycles * M_PI; i += incr) {
        for (int j = 0; j < c; j++) {
            points[num] = new Position((long long) ((a + b * i) * cos(i + j * M_PI * 2 / c)),
                                       (long long) ((a + b * i) * sin(i + j * M_PI * 2 / c)));
            num++;
        }
    }

    for (int i = 0; i < NUM_SYSTEMS; i++) {
        long long x, y, hy;
        int tries = 0;
        while (true) {
            do {
                x = (long long) ((double) rand() / RAND_MAX * (double) dia) - x1;
                y = (long long) ((double) rand() / RAND_MAX * (double) dia) - y1;
                hy = hypot(x, y);
            } while (hy > dia / 2 || hy < 50000000000000000);
            auto len = (double) dia;
            int point = 0;
            for (int j = 0; j < num; j++) {
                double a = hypot(x - points[j]->x, y - points[j]->y);
                if (a < len) {
                    len = a;
                    point = j;
                }
            }
            double dis = hypot(points[point]->x, points[point]->y);
            dis -= (double) dia / 4;
            dis *= 4;
            *(long long *) &dis &= 0x7FFFFFFFFFFFFFFF;
            if (dis < 300000000000000000) {
                dis = 300000000000000000;
            }
            double p = (len / (double) dia * dis / (double) dia * 50);
            if (p < ((double) rand() / RAND_MAX)) {
                break;
            }
            tries++;
        }
        AddSystem(new System(x, y));
        if (i + 1 == NUM_SYSTEMS || (i & 0xF) == 0) {
            printf("\r%5.1f%%", (double) (i + 1) / NUM_SYSTEMS * 100);
            fflush(stdout);
        }
    }
    printf("\n");

    for (int i = 0; i < num; i++) {
        delete points[i];
    }
    InitSystems();
}

void Galaxy::Calc(double d) {
    unsigned long long lastTick = tick;
    tick += (unsigned long long) d;
    cam.Calc(tick, d);
    printf("\r%4.1f FPS - %llu.%02llu.%02llu ", 1000.0 / d, tick / 5000000, (tick / 50000) % 100, (tick / 1000) % 50);
    fflush(stdout);
    if (lastTick % 1000 > tick % 1000) {
        // TODO Calc in other thread
        for (auto system : systems) {
            system->Calc(tick, (double) (tick - lastTick));
        }
    }
}

void Galaxy::Render(SDL_Renderer *renderer) {
    long long cx1, cy1, cx2, cy2;
    cam.Render(renderer, &cx1, &cy1, &cx2, &cy2);
    if (preRender != nullptr && cam.mppx > 200000000000000) {
        SDL_Rect dst;
        auto *zero = new Position(0, 0);
        cam.GetScreenPos(zero, &dst, GetDiameter(), GetDiameter());
        SDL_RenderCopy(renderer, preRender, nullptr, &dst);
        delete zero;
    } else {
        for (auto system : systems) {
            long long idx = system->GetPos()->x;
            if (idx < cx1) {
                continue;
            } else if (idx < cx2) {
                system->Render(renderer, &cam);
            } else {
                break;
            }
        }
    }
}

unsigned long long Galaxy::GetDiameter() const {
    return x1 - x0;
}

void Galaxy::PreRender(SDL_Renderer *renderer) {
    if (preRender != nullptr) {
        SDL_DestroyTexture(preRender);
    }
    long long mppx = 100000000000000; // / 2000;
    int size = (int) (GetDiameter() / mppx);
    printf("Size: %i\n", size);
    SDL_Surface *surf = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
    SDL_Renderer *preRenderer = SDL_CreateSoftwareRenderer(surf);
    SDL_SetRenderDrawColor(preRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(preRenderer);
    SDL_SetRenderDrawColor(preRenderer, 0xFF, 0xFF, 0xFF, 0x00);
    for (auto system : systems) {
        Position *pos = system->GetPos();
        auto x = (float) pos->x / (float) mppx + (float) size / 2;
        auto y = (float) pos->y / (float) mppx + (float) size / 2;
        SDL_FRect rect;
        rect.w = 3;
        rect.h = 3;
        rect.x = x - rect.w / 2;
        rect.y = y - rect.h / 2;
        SDL_RenderFillRectF(preRenderer, &rect);
    }
    SDL_DestroyRenderer(preRenderer);
    preRender = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void Galaxy::OnResize(SDL_Renderer *renderer) {
    cam.Init(renderer, GetDiameter());
    PreRender(renderer);
}

void Galaxy::InitSystems() {
    std::sort(systems.begin(), systems.end(), SpaceObj::Compare);
    printf("Sorted!\n");
    for (int i = 0; i < systems.size(); i++) {
        System *sys1 = systems[i];
        Position *pos1 = sys1->GetPos();
        for (int j = i + 1; j < systems.size(); j++) {
            System *sys2 = systems[j];
            Position *pos2 = sys2->GetPos();
            if (pos2->x - pos1->x > systemReach * 2) {
                break;
            }
            unsigned long long dist = hypot(pos2->x - pos1->x, pos2->y - pos1->y);
            if (dist > systemReach * 2) {
                continue;
            }
            sys1->AddNeighbor(sys2);
        }
    }
    printf("Calculated neighbors...\n");
    long long div = 10000000000;
    auto *a = new Position(0, 0);
    auto *b = new Position(0, 0);
    auto *c = new Position(0, 0);
    auto *d = new Position(0, 0);
    for (auto sys1 : systems) {
        Position *p1 = sys1->GetPos();
        a->x = p1->x / div;
        a->y = p1->y / div;
        auto neighbors = sys1->GetNeighbors();
        cont:
        for (auto sys2 : *neighbors) {
            Position *p2 = sys2->GetPos();
            c->x = p2->x / div;
            c->y = p2->y / div;
            for (auto sys3 : *sys2->GetNeighbors()) {
                if (sys3 == sys1) continue;
                Position *p3 = sys3->GetPos();
                d->x = p3->x / div;
                d->y = p3->y / div;
                double d1 = hypot(p2->x - p3->x, p2->y - p3->y);
                for (auto sys4 : *neighbors) {
                    if (sys4 == sys2 || sys4 == sys3) continue;
                    Position *p4 = sys4->GetPos();
                    b->x = p4->x / div;
                    b->y = p4->y / div;
                    // Intersect p1<->p4 x p2<->p3
                    double d2 = hypot(p1->x - p4->x, p1->y - p4->y);
                    bool intersect = Position::intersect(a, b, c, d);
                    if (intersect && d2 > d1) {
                        sys1->RemoveNeighbor(sys4);
                        goto cont;
                    }
                }
            }
        }
    }
    delete a;
    delete b;
    delete c;
    delete d;
    printf("Finished!\n");
}
