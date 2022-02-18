//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_GALAXY_H
#define GALACTIVISATION_GALAXY_H

#include <vector>
#include <bits/stdc++.h>
#include <queue>

#include "StaticSpaceObj.h"
#include "System.h"
#include "Border.h"

#define NUM_SYSTEMS 200000

class Galaxy {
public:
    long long x0 = -500000000000000000;
    long long y0 = -500000000000000000;
    long long x1 = 500000000000000000;
    long long y1 = 500000000000000000;

protected:
    std::vector<System *> systems{};
    unsigned int numSystems = 0;
    unsigned long long tick = 0;
    SDL_Texture *preRender = nullptr;
    unsigned long long systemReach = 4000000000000000;
    std::vector<Border *> borders{};

public:
    Camera cam;

    Galaxy() = default;

    void AddSystem(System *system);

    void Init(SDL_Renderer *renderer);

    void Calc(double d);

    void Render(SDL_Renderer *renderer);

    [[nodiscard]] unsigned long long GetDiameter() const;

    void PreRender(SDL_Renderer *renderer);

    void OnResize(SDL_Renderer *renderer);

    std::vector<System *> *GetSystems();

    std::vector<Border *> *GetBorders();

    void InitSystems();
};

#endif //GALACTIVISATION_GALAXY_H
