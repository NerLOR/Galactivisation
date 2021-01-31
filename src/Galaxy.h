//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_GALAXY_H
#define GALACTIVISATION_GALAXY_H

#include "SpaceObj.h"

class Galaxy {
protected:
    long x0 = -500000000000000000;
    long y0 = -500000000000000000;
    long x1 = 500000000000000000;
    long y1 = 500000000000000000;
    SpaceObj *objs[1000000]{};
    unsigned int objsLen = 0;
    unsigned long tick = 0;
    long systemBorderReach = 10000000000;
    SDL_Surface *preRender = nullptr;

public:
    Camera cam;

    Galaxy() = default;

    void AddSpaceObject(SpaceObj *obj);

    void Init(SDL_Renderer *renderer);

    void Calc(double d);

    void Render(SDL_Renderer *renderer);

    [[nodiscard]] unsigned long GetDiameter() const;

    void PreRender();
};

#endif //GALACTIVISATION_GALAXY_H
