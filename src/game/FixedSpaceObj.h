//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_FIXEDSPACEOBJ_H
#define GALACTIVISATION_FIXEDSPACEOBJ_H

#include "SpaceObj.h"

class FixedSpaceObj : public SpaceObj {
    SpaceObj *parent;
    long long orbitRadius;
    long long ticksPerRotation;
    double baseRotation;

public:
    FixedSpaceObj(SpaceObj *parent, long long orbitRadius, double tpr, double orientation);

    void Calc(unsigned long long t, double d) override;

    void Render(SDL_Renderer *renderer, Camera *cam) override;
};


#endif //GALACTIVISATION_FIXEDSPACEOBJ_H
