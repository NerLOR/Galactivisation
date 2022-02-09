//
// Created by lorenz on 30.01.21.
//

#include "Camera.h"

Camera::Camera() : pos(Position(0, 0)), posTarget(Position(0, 0)) {

}

void Camera::Calc(unsigned long long t, double d) {
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
        zoom += (long long) ((double) ((long long) (zoomTarget - zoom)) * mult);
        posChange = new Vector((double) (posTarget.x - pos.x), (double) (posTarget.y - pos.y));
        *posChange *= mult;
        pos += *posChange;
        delete posChange;
    }
}

bool Camera::GetScreenPos(Position *obj_pos, SDL_Rect *rect, unsigned long long w, unsigned long long h) {
    auto *half_size = new Vector((double) w / 2, (double)  h / 2);
    Position rel_pos_1 = *obj_pos;
    Position rel_pos_2 = *obj_pos;
    rel_pos_1 -= *half_size;
    rel_pos_2 += *half_size;
    rel_pos_1 -= pos;
    rel_pos_2 -= pos;
    delete half_size;
    long long sw2 = screenW / 2;
    long long sh2 = screenH / 2;
    long long x1 = rel_pos_1.x / (long long) mppx + sw2;
    long long y1 = rel_pos_1.y / (long long) mppx + sh2;
    long long x2 = rel_pos_2.x / (long long) mppx + sw2;
    long long y2 = rel_pos_2.y / (long long) mppx + sh2;
    rect->x = (int) x1;
    rect->y = (int) y1;
    rect->w = (int) (x2 - x1);
    rect->h = (int) (y2 - y1);
    return x2 >= 0 && y2 >= 0 && x1 <= screenW && y1 <= screenH;
}

bool Camera::GetLineScreenPos(Position *op1, Position *op2, SDL_Point *l1, SDL_Point *l2) {
    unsigned long long w = screenW * mppx;
    unsigned long long h = screenH * mppx;
    auto *half_size = new Vector((double) w / 2, (double)  h / 2);
    Position rp1 = pos;
    Position rp2 = pos;
    rp1 -= *half_size;
    rp2 += *half_size;
    delete half_size;
    Position p1 = *op1;
    Position p2 = *op2;

    if ((p1.x < rp1.x && p2.x < rp1.x) || (p1.x > rp2.x && p2.x > rp2.x) ||
        (p1.y < rp1.y && p2.y < rp1.y) || (p1.y > rp2.y && p2.y > rp2.y)) {
        return false;
    }

    if (p1.x == p2.x) {
        if (p1.y < rp1.y) p1.y = rp1.y;
        else if (p1.y > rp2.y) p1.y = rp2.y;
        if (p2.y < rp1.y) p2.y = rp1.y;
        else if (p2.y > rp2.y) p2.y = rp2.y;
    } else if (p1.y == p2.y) {
        if (p1.x < rp1.x) p1.x = rp1.x;
        else if (p1.x > rp2.x) p1.x = rp2.x;
        if (p2.x < rp1.x) p2.x = rp1.x;
        else if (p2.x > rp2.x) p2.x = rp2.x;
    } else {
        // y = a * x + b
        double a = (double) (p2.y - p1.y) / (double) (p2.x - p1.x);
        double b = (double) p1.y - a * (double) p1.x;
        double b_min, b_max;
        if (a > 0) {
            b_max = (double) rp2.y - a * (double) rp1.x;
            b_min = (double) rp1.y - a * (double) rp2.x;
        } else {
            b_max = (double) rp2.y - a * (double) rp2.x;
            b_min = (double) rp1.y - a * (double) rp1.x;
        }
        if (b < b_min || b > b_max)
            return false;

        if (p1.x < rp1.x) {
            p1.x = rp1.x;
            p1.y = (long long) (a * (double) p1.x + b);
        } else if (p2.x < rp1.x) {
            p2.x = rp1.x;
            p2.y = (long long) (a * (double) p2.x + b);
        }
        if (p1.x > rp2.x) {
            p1.x = rp2.x;
            p1.y = (long long) (a * (double) p1.x + b);
        } else if (p2.x > rp2.x) {
            p2.x = rp2.x;
            p2.y = (long long) (a * (double) p2.x + b);
        }

        if (p1.y < rp1.y) {
            p1.y = rp1.y;
            p1.x = (long long) (((double) p1.y - b) / a);
        } else if (p2.y < rp1.y) {
            p2.y = rp1.y;
            p2.x = (long long) (((double) p2.y - b) /a);
        }
        if (p1.y > rp2.y) {
            p1.y = rp2.y;
            p1.x = (long long) (((double) p1.y - b) / a);
        } else if (p2.y > rp2.y) {
            p2.y = rp2.y;
            p2.x = (long long) (((double) p2.y - b) / a);
        }
    }

    SDL_Rect dst1, dst2;
    GetScreenPos(&p1, &dst1, 0, 0);
    GetScreenPos(&p2, &dst2, 0, 0);
    l1->x = dst1.x;
    l1->y = dst1.y;
    l2->x = dst2.x;
    l2->y = dst2.y;
    return true;
}

void Camera::Render(SDL_Renderer *renderer, long long *x1, long long *y1, long long *x2, long long *y2) {
    mppx = zoom / screenW;
    SDL_GetMouseState(&mouseX, &mouseY);
    mousePosX = (long long) ((mouseX - screenW / 2) * mppx);
    mousePosY = (long long) ((mouseY - screenH / 2) * mppx);
    *x1 = pos.x - screenW * (long long) mppx / 2;
    *y1 = pos.y - screenH * (long long) mppx / 2;
    *x2 = pos.x + screenW * (long long) mppx / 2;
    *y2 = pos.y + screenH * (long long) mppx / 2;
}

void Camera::Init(SDL_Renderer *renderer, unsigned long long diameter) {
    screenW = 0;
    screenH = 0;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
    zoomMax = (long long) ((double) diameter * ((double) screenW / screenH + 0.25));
}

void Camera::SetPos(long long x, long long y, long long zoom) {
    pos.x = x;
    pos.y = y;
    posTarget.x = x;
    posTarget.y = y;
    this->zoom = zoom;
    this->zoomTarget = zoom;
}
