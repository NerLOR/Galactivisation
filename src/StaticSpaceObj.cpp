//
// Created by lorenz on 30.01.21.
//

#include "StaticSpaceObj.h"

StaticSpaceObj::StaticSpaceObj(long x, long y, double orientation, long tpr) : SpaceObj(Position(0, 0)) {
    this->pos.x = x;
    this->pos.y = y;
    this->baseOrientation = orientation;
    this->ticksPerRevolution = tpr;
    this->size = 700000000;
}

void StaticSpaceObj::Calc(unsigned long t, double d) {
    orientation = baseOrientation + (double) t / (double) ticksPerRevolution;
}

void StaticSpaceObj::Render(SDL_Renderer *renderer, Camera *cam) {
    SDL_Rect dst;
    bool visible = cam->GetScreenPos(&this->pos, &dst, size, size);
    bool marked = false;
    if (visible) {
        if (dst.w < 1) dst.w = 1;
        if (dst.h < 1) dst.h = 1;
        if (dst.w <= 1 && dst.h <= 1) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            SDL_RenderDrawPoint(renderer, dst.x, dst.y);
            if (hypot(dst.x - cam->mouseX, dst.y - cam->mouseY) < 15) {
                dst.x -= 7;
                dst.y -= 7;
                dst.w = 15;
                dst.h = 15;
                marked = true;
                SDL_SetRenderDrawColor(renderer, 0x00, 0xC0, 0x80, 0x00);
                SDL_RenderDrawRect(renderer, &dst);
            }
        } else {
            SDL_RenderCopyEx(renderer, sunTexture, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
        }
        if (cam->zoom > 2000000000000000 && cam->zoom < 100000000000000000) {
            if (marked) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xC0, 0x80, 0x00);
                DrawCircle(renderer, dst.x + dst.w / 2, dst.y + dst.h / 2, 2000000000000000 / cam->mppx);
            } else {
                SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0x00);
            }
        }
    }
}
