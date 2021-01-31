//
// Created by lorenz on 30.01.21.
//

#include "Camera.h"

Camera::Camera() : pos(Position(0, 0)), posTarget(Position(0, 0)) {

}

void Camera::Calc(unsigned long t, double d) {
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    auto *posChange = new Vector(0, 0);
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
        posChange->y -= 0.5;
    }
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
        posChange->y += 0.5;
    }
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
        posChange->x -= 0.5;
    }
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
        posChange->x += 0.5;
    }
    *posChange *= d * (double) zoom / 1000;
    posTarget += *posChange;
    delete posChange;

    orientation += d * dOrientation;
    dOrientation -= 1 * d * dOrientation;
    double mult = d / 100;
    if (mult > 1) {
        zoom = zoomTarget;
        pos = posTarget;
    } else {
        zoom += (long) ((double) ((long) (zoomTarget - zoom)) * mult);
        posChange = new Vector((double) (posTarget.x - pos.x), (double) (posTarget.y - pos.y));
        *posChange *= mult;
        pos += *posChange;
        delete posChange;
    }
}

bool Camera::GetScreenPos(Position *obj_pos, SDL_Rect *rect, unsigned long w, unsigned long h) {
    auto *half_size = new Vector((double) w / 2, (double)  h / 2);
    Position rel_pos_1 = *obj_pos;
    Position rel_pos_2 = *obj_pos;
    rel_pos_1 -= *half_size;
    rel_pos_2 += *half_size;
    rel_pos_1 -= pos;
    rel_pos_2 -= pos;
    delete half_size;
    long x1 = rel_pos_1.x / (long) mppx + screenW / 2;
    long y1 = rel_pos_1.y / (long) mppx + screenH / 2;
    long x2 = rel_pos_2.x / (long) mppx + screenW / 2;
    long y2 = rel_pos_2.y / (long) mppx + screenH / 2;
    rect->x = (int) x1;
    rect->y = (int) y1;
    rect->w = (int) (x2 - x1);
    rect->h = (int) (y2 - y1);
    return x2 >= 0 && y2 >= 0 && x1 <= screenW && y1 <= screenH;
}

void Camera::Render(SDL_Renderer *renderer, long *x1, long *y1, long *x2, long *y2) {
    mppx = zoom / screenW;
    SDL_GetMouseState(&mouseX, &mouseY);
    mousePosX = (long) ((mouseX - screenW / 2) * mppx);
    mousePosY = (long) ((mouseY - screenH / 2) * mppx);
    *x1 = pos.x - screenW * (long) mppx / 2;
    *y1 = pos.y - screenH * (long) mppx / 2;
    *x2 = pos.x + screenW * (long) mppx / 2;
    *y2 = pos.y + screenH * (long) mppx / 2;
}

void Camera::Init(SDL_Renderer *renderer, unsigned long diameter) {
    screenW = 0;
    screenH = 0;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
    zoomMax = (long) ((double) diameter * ((double) screenW / screenH + 0.25));
}
