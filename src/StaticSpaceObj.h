//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_STATICSPACEOBJ_H
#define GALACTIVISATION_STATICSPACEOBJ_H

#include "SpaceObj.h"

SDL_Texture *sunTexture;

class StaticSpaceObj : public SpaceObj {
protected:
    unsigned long size;
    Position *border[8]{};
    int owner = 0;
    int borderSize = 0;

public:
    StaticSpaceObj(long x, long y, double orientation, long tpr);

    void Calc(unsigned long t, double d) override;

    void Render(SDL_Renderer *renderer, Camera *cam) override;
};


#endif //GALACTIVISATION_STATICSPACEOBJ_H
