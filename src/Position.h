//
// Created by lorenz on 29.01.21.
//

#ifndef GALACTIVISATION_POSITION_H
#define GALACTIVISATION_POSITION_H

#include "Vector.h"

class Position {
public:
    long x;
    long y;

    Position(long x, long y);

    Position &operator+=(Vector &vec);

    Position &operator-=(Vector &vec);

    Position &operator+=(Position &pos);

    Position &operator-=(Position &pos);
};


#endif //GALACTIVISATION_POSITION_H
