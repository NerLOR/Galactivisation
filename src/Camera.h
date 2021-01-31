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
    unsigned long zoom = 2000000000;
    unsigned long zoomTarget = 2000000000;
    unsigned long zoomMax = 2000000000;
    unsigned long zoomMin = 1000000;
    unsigned long mppx = 0;
    int screenW = 800;
    int screenH = 600;
    long mousePosX = 0;
    long mousePosY = 0;
    int mouseX = 0;
    int mouseY = 0;

    explicit Camera();

    void Calc(unsigned long t, double d);

    bool GetScreenPos(Position *obj_pos, SDL_Rect *rect, unsigned long w, unsigned long h);

    void Render(SDL_Renderer *renderer, long *x1, long *y1, long *x2, long *y2);

    void Init(SDL_Renderer *renderer, unsigned long diameter);

};


#endif //GALACTIVISATION_CAMERA_H
