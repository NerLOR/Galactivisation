//
// Created by lorenz on 13.02.22.
//

#ifndef GALACTIVISATION_BORDER_H
#define GALACTIVISATION_BORDER_H

#include "Position.h"


class Border {
public:
    Position *p1;
    Position *p2;

    Border(Position *p1, Position *p2);
};


#endif //GALACTIVISATION_BORDER_H
