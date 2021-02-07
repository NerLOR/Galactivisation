//
// Created by lorenz on 29.01.21.
//

#ifndef GALACTIVISATION_POSITION_H
#define GALACTIVISATION_POSITION_H

#include "Vector.h"

class Position {
public:
    long long x;
    long long y;

    Position(long long x, long long y);

    Position &operator+=(Vector &vec);

    Position &operator-=(Vector &vec);

    Position &operator+=(Position &pos);

    Position &operator-=(Position &pos);

    static bool isOnSegment(Position *a, Position *b, Position *c);

    static int orientation(Position *a, Position *b, Position *c);

    static bool intersect(Position *a1, Position *a2, Position *b1, Position *b2);
};


#endif //GALACTIVISATION_POSITION_H
