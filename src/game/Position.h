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

    Position();

    Position(long long x, long long y);

    Position &operator+=(Vector &vec);

    Position &operator-=(Vector &vec);

    Position &operator+=(Position &pos);

    Position &operator-=(Position &pos);

    static bool IsOnSegment(Position &a, Position &b, Position &c);

    static int GetOrientation(Position &a, Position &b, Position &c);

    static bool DoLinesIntersect(Position &a1, Position &a2, Position &b1, Position &b2);

    static bool DoLineSegmentsIntersect(Position &a1, Position &a2, Position &b1, Position &b2);

    long long GetDistanceTo(Position &b) const;
};

#endif //GALACTIVISATION_POSITION_H
