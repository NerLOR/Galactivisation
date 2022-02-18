//
// Created by lorenz on 03.02.21.
//

#ifndef GALACTIVISATION_SYSTEM_H
#define GALACTIVISATION_SYSTEM_H

#include <vector>
#include <set>

class System;

#include "SpaceObj.h"
#include "StaticSpaceObj.h"
#include "Border.h"

class System : public SpaceObj {
protected:
    std::vector<StaticSpaceObj *> suns{};
    std::vector<SpaceObj *> objs{};
    std::vector<Border *> borders{};
    std::set<System *> neighbors{};
    int owner = 0;
    unsigned long long reach;

public:
    System(long long x, long long y, unsigned long long reach);

    void Calc(unsigned long long t, double d) override;

    void Render(SDL_Renderer *renderer, Camera *cam) override;

    void AddNeighbor(System *neighbor);

    void RemoveNeighbor(System *neighbor);

    std::set<System *> *GetNeighbors();

    void AddBorder(Border *b);
};


#endif //GALACTIVISATION_SYSTEM_H
