//
// Created by lorenz on 30.01.21.
//

#include "FixedSpaceObj.h"

FixedSpaceObj::FixedSpaceObj(SpaceObj *parent, long orbitRadius, double tpr, double orientation) : SpaceObj(Position(0, 0)) {
    this->parent = parent;
    this->orbitRadius = orbitRadius;
    this->baseRotation = orientation;
    this->ticksPerRotation = tpr;
}

void FixedSpaceObj::Calc(unsigned long t, double d) {
    double pos = baseRotation + (double) t / ticksPerRotation * 2 * M_PI;
    this->pos.x = (long) (sin(pos) * (double) orbitRadius + (double) parent->GetPos()->x);
    this->pos.y = (long) (cos(pos) * (double) orbitRadius + (double) parent->GetPos()->y);
    orientation = baseOrientation + (double) t / ticksPerRevolution;
}

void FixedSpaceObj::Render(SDL_Renderer *renderer, Camera *cam) {

}
