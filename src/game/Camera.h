//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_CAMERA_H
#define GALACTIVISATION_CAMERA_H

#include <SDL2/SDL.h>
#include "Position.h"
#include "Vector.h"

class Camera {
public:
    Position pos;
    Position posTarget;
    double orientation = 0.0;
    double dOrientation = 0.0;
    unsigned long long zoom = 2000000000;
    unsigned long long zoomTarget = 2000000000;
    unsigned long long zoomMax = 2000000000;
    unsigned long long zoomMin = 1000000;
    unsigned long long mppx = 0;
    int screenW = 800;
    int screenH = 600;
    long long mousePosX = 0;
    long long mousePosY = 0;
    int mouseX = 0;
    int mouseY = 0;

    explicit Camera();

    void Calc(unsigned long long t, double d);

    bool GetScreenPos(Position *obj_pos, SDL_Rect *rect, unsigned long long w, unsigned long long h);

    bool GetScreenPosF(Position *obj_pos, SDL_FRect *rect, unsigned long long w, unsigned long long h);

    void Render(SDL_Renderer *renderer, long long *x1, long long *y1, long long *x2, long long *y2);

    void Init(SDL_Renderer *renderer, unsigned long long diameter);

    void SetPos(long long x, long long y, long long zoom);

};


#endif //GALACTIVISATION_CAMERA_H
