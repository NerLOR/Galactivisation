//
// Created by lorenz on 03.02.21.
//

#ifndef GALACTIVISATION_SYSTEM_H
#define GALACTIVISATION_SYSTEM_H

#include <vector>
#include <set>
#include "SpaceObj.h"
#include "StaticSpaceObj.h"

class System : public SpaceObj {
protected:
    std::vector<StaticSpaceObj *> suns{};
    std::vector<SpaceObj *> objs{};
    std::vector<Position *> borderPoints{};
    std::set<System *> neighbors{};
    int owner = 0;

public:
    System(long x, long y);

    void Calc(unsigned long long t, double d) override;

    void Render(SDL_Renderer *renderer, Camera *cam) override;

    void AddNeighbor(System *neighbor);

    void RemoveNeighbor(System *neighbor);

    std::set<System *> *GetNeighbors();

    void AddBorderPoint(Position *p);
};


#endif //GALACTIVISATION_SYSTEM_H
