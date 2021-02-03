//
// Created by lorenz on 03.02.21.
//

#include "System.h"

System::System(long x, long y) : SpaceObj(Position(x, y)) {
    suns.push_back(new StaticSpaceObj(x, y, 0, 0));
}

void System::Calc(unsigned long long int t, double d) {
    for (auto sun : suns) {
        sun->Calc(t, d);
    }
    for (auto obj : objs) {
        obj->Calc(t, d);
    }
}

void System::Render(SDL_Renderer *renderer, Camera *cam) {
    SDL_Rect dst;
    bool visible = cam->GetScreenPos(&this->pos, &dst, 1000000000000, 1000000000000);
    if (visible) {
        if (dst.w < 12) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            SDL_RenderDrawPoint(renderer, dst.x + dst.w / 2, dst.y + dst.h / 2);
        } else {
            for (auto sun : suns) {
                sun->Render(renderer, cam);
            }
            if (dst.w >= 48) {
               for (auto obj : objs) {
                   obj->Render(renderer, cam);
               }
            }
        }
    }
}
