//
// Created by lorenz on 30.01.21.
//

#include "StaticSpaceObj.h"

SDL_Texture *sunTexture;

StaticSpaceObj::StaticSpaceObj(long long x, long long y, double orientation, long long tpr) : SpaceObj(Position(0, 0)) {
    this->pos.x = x;
    this->pos.y = y;
    this->baseOrientation = orientation;
    this->ticksPerRevolution = tpr;
    this->size = 700000000;
}

void StaticSpaceObj::Calc(unsigned long long t, double d) {
    if (ticksPerRevolution == 0) {
        orientation = baseOrientation;
    } else {
        orientation = baseOrientation + (double) t / (double) ticksPerRevolution;
    }
}

void StaticSpaceObj::Render(SDL_Renderer *renderer, Camera *cam) {
    SDL_Rect dst;
    bool visible = cam->GetScreenPos(&this->pos, &dst, size, size);
    if (visible) {
        if (dst.w < 1) dst.w = 1;
        if (dst.h < 1) dst.h = 1;
        SDL_RenderCopyEx(renderer, sunTexture, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
    }
}

SDL_Texture **StaticSpaceObj::GetSunTexture() {
    return &sunTexture;
}
