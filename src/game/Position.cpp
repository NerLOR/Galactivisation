//
// Created by lorenz on 29.01.21.
//

#include "Position.h"

Position::Position(long long x, long long y) {
    this->x = x;
    this->y = y;
}

Position &Position::operator+=(Vector &vec) {
    this->x += (long long) vec.x;
    this->y += (long long) vec.y;
    return *this;
}

Position &Position::operator-=(Vector &vec) {
    this->x -= (long long) vec.x;
    this->y -= (long long) vec.y;
    return *this;
}

Position &Position::operator+=(Position &pos) {
    this->x += pos.x;
    this->y += pos.y;
    return *this;
}

Position &Position::operator-=(Position &pos) {
    this->x -= pos.x;
    this->y -= pos.y;
    return *this;
}

bool Position::isOnSegment(Position *a, Position *b, Position *c) {
    return (b->x <= std::max(a->x, c->x) && b->x >= std::min(a->x, c->x) &&
            b->y <= std::max(a->y, c->y) && b->y >= std::min(a->y, c->y));
}

int Position::orientation(Position *a, Position *b, Position *c) {
    long long val = (b->y - a->y) * (c->x - b->x) - (b->x - a->x) * (c->y - b->y);
    return (val == 0) ? 0 : (val > 0) ? 1 : 2;
}

bool Position::intersect(Position *a1, Position *a2, Position *b1, Position *b2) {
    int o1 = orientation(a1, a2, b1);
    int o2 = orientation(a1, a2, b2);
    int o3 = orientation(b1, b2, a1);
    int o4 = orientation(b1, b2, a2);
    return (o1 != o2 && o3 != o4) ||
           (o1 == 0 && isOnSegment(a1, b1, a2)) ||
           (o2 == 0 && isOnSegment(a1, b2, a2)) ||
           (o3 == 0 && isOnSegment(b1, a1, b2)) ||
           (o4 == 0 && isOnSegment(b1, a2, b2));
}
