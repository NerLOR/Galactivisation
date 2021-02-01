//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_GALAXY_H
#define GALACTIVISATION_GALAXY_H

#include <vector>
#include <bits/stdc++.h>
#include "SpaceObj.h"

#define NUM_SYSTEMS 200000

class Galaxy {
protected:
    long long x0 = -500000000000000000;
    long long y0 = -500000000000000000;
    long long x1 = 500000000000000000;
    long long y1 = 500000000000000000;
    std::vector<SpaceObj*> objs{};
    unsigned int objsLen = 0;
    unsigned long long tick = 0;
    long long systemBorderReach = 10000000000;
    SDL_Texture *preRender = nullptr;

public:
    Camera cam;

    Galaxy() = default;

    void AddSpaceObject(SpaceObj *obj);

    void Init(SDL_Renderer *renderer);

    void Calc(double d);

    void Render(SDL_Renderer *renderer);

    [[nodiscard]] unsigned long long GetDiameter() const;

    void PreRender(SDL_Renderer *renderer);

    void OnResize(SDL_Renderer *renderer);

    void SortSpaceObjects();
};

#endif //GALACTIVISATION_GALAXY_H
