//
// Created by lorenz on 13.02.22.
//

#ifndef GALACTIVISATION_BORDER_H
#define GALACTIVISATION_BORDER_H

class Border;

#include "Position.h"
#include "System.h"

class Border {
public:
    Position p1;
    Position p2;
    System *sys1;
    System *sys2;

    Border(Position p1, Position p2, System *sys1, System *sys2);
};

#endif //GALACTIVISATION_BORDER_H
