//
// Created by lorenz on 30.01.21.
//

#include "Galaxy.h"
#include "StaticSpaceObj.h"

void Galaxy::AddSystem(StaticSpaceObj *system) {
    numSystems++;
    systems.push_back(system);
}

void Galaxy::Init(SDL_Renderer *renderer) {
    cam.Init(renderer, GetDiameter());
    AddSystem(new StaticSpaceObj(0, 0, 0, 1));
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
            *(long long*) &dis &= 0x7FFFFFFFFFFFFFFF;
            if (dis < 300000000000000000) {
                dis = 300000000000000000;
            }
            double p = (len / (double) dia * dis / (double) dia * 50);
            if (p < ((double) rand() / RAND_MAX)) {
                break;
            }
            tries++;
        }
        AddSystem(new StaticSpaceObj(x, y, 0, 1));
        if (i + 1 == NUM_SYSTEMS || (i & 0xF) == 0) {
            printf("\r%5.1f%%", (double) (i + 1) / NUM_SYSTEMS * 100);
            fflush(stdout);
        }
    }
    printf("\n");

    for (int i = 0; i < num; i++) {
        delete points[i];
    }
    SortSystems();
}

void Galaxy::Calc(double d) {
    unsigned long long lastTick = tick;
    tick += (long long) d;
    cam.Calc(tick, d);
    printf("\r%4.1f FPS - %llu.%02llu.%02llu ", 1000.0 / d, tick / 5000000, (tick / 50000) % 100, (tick / 1000) % 50);
    fflush(stdout);
    if (lastTick % 1000 > tick % 1000) {
        for (auto system : systems) {
            system->Calc(tick, d);
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

void Galaxy::SortSystems() {
    std::sort(systems.begin(), systems.end(), SpaceObj::Compare);
}
