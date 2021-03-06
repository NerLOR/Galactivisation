//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_STATICSPACEOBJ_H
#define GALACTIVISATION_STATICSPACEOBJ_H

#include <vector>
#include "SpaceObj.h"

SDL_Texture *sunTexture;

class StaticSpaceObj : public SpaceObj {
protected:
    unsigned long long size;

public:
    StaticSpaceObj(long long x, long long y, double orientation, long long tpr);

    void Calc(unsigned long long t, double d) override;

    void Render(SDL_Renderer *renderer, Camera *cam) override;
};


#endif //GALACTIVISATION_STATICSPACEOBJ_H
