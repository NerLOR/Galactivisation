//
// Created by lorenz on 30.01.21.
//

#include "Galaxy.h"
#include "StaticSpaceObj.h"

void Galaxy::AddSpaceObject(SpaceObj *obj) {
    SpaceObj *cache = nullptr;
    bool inserted = false;
    for (int i = 0; i < objsLen; i++) {
        if (inserted) {
            cache = objs[i];
            objs[i] = obj;
            obj = cache;
        } else if (objs[i]->GetPos()->x > obj->GetPos()->x) {
            inserted = true;
            cache = objs[i];
            objs[i] = obj;
            obj = cache;
            objsLen++;
        }
    }
    if (!inserted) objs[objsLen++] = obj;
}

void Galaxy::Init(SDL_Renderer *renderer) {
    cam.Init(renderer, GetDiameter());
    AddSpaceObject(new StaticSpaceObj(0, 0, 0, 1));
    unsigned long dia = GetDiameter();

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
            points[num] = new Position((long) ((a + b * i) * cos(i + j * M_PI * 2 / c)),
                                       (long) ((a + b * i) * sin(i + j * M_PI * 2 / c)));
            num++;
        }
    }

    for (int i = 0; i < 200000; i++) {
        long x, y, hy;
        int tries = 0;
        while (true) {
            do {
                x = (long) ((double) rand() / RAND_MAX * (double) dia) - x1;
                y = (long) ((double) rand() / RAND_MAX * (double) dia) - y1;
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
            *(long*) &dis &= 0x7FFFFFFFFFFFFFFF;
            if (dis < 300000000000000000) {
                dis = 300000000000000000;
            }
            double p = (len / (double) dia * dis / (double) dia * 50);
            //printf("%f\n", p);
            if (p < ((double) rand() / RAND_MAX)) {
                break;
            }
            tries++;
        }
        AddSpaceObject(new StaticSpaceObj(x, y, 0, 1));
    }

    for (int i = 0; i < num; i++) {
        delete points[i];
    }
}

void Galaxy::Calc(double d) {
    tick += (long) d;
    cam.Calc(tick, d);
    for (unsigned int i = 0; i < objsLen; i++) {
        objs[i]->Calc(tick, d);
    }
}

void Galaxy::Render(SDL_Renderer *renderer) {
    long cx1, cy1, cx2, cy2;
    cam.Render(renderer, &cx1, &cy1, &cx2, &cy2);
    if (cx1 > objs[0]->GetPos()->x || cx2 < objs[objsLen - 1]->GetPos()->x) {
        for (int i = 0; i < objsLen; i++) {
            long idx = objs[i]->GetPos()->x;
            if (idx < cx1) {
                continue;
            } else if (idx < cx2) {
                objs[i]->Render(renderer, &cam);
            } else {
                break;
            }
        }
    } else {
        for (int i = 0; i < objsLen; i++) {
            objs[i]->Render(renderer, &cam);
        }
    }
}

unsigned long Galaxy::GetDiameter() const {
    return x1 - x0;
}

void Galaxy::PreRender() {
    if (preRender != nullptr) {
        SDL_FreeSurface(preRender);
    }
    int size = (int) (GetDiameter() / 100000000000000000);

    preRender = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
    for (int i = 0; i < objsLen; i++) {
        Position *pos = objs[i]->GetPos();
    }
}
