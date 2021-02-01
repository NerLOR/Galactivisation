//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_SPACEOBJ_H
#define GALACTIVISATION_SPACEOBJ_H

#include <SDL2/SDL_render.h>
#include "Position.h"
#include "Camera.h"

class SpaceObj {
protected:
    Position pos;
    long mass = 1;
    double orientation = 0.0;
    double baseOrientation = 0.0;
    long ticksPerRevolution = 1;

public:
    explicit SpaceObj(Position pos);

    explicit SpaceObj(Position *pos);

    Position *GetPos();

    virtual void Calc(unsigned long t, double d) = 0;

    virtual void Render(SDL_Renderer *renderer, Camera *cam) = 0;

    static bool Compare(SpaceObj *a, SpaceObj *b);
};

#endif //GALACTIVISATION_SPACEOBJ_H
