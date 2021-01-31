//
// Created by lorenz on 29.01.21.
//

#include "Position.h"

Position::Position(long x, long y) {
    this->x = x;
    this->y = y;
}

Position &Position::operator+=(Vector &vec) {
    this->x += (long) vec.x;
    this->y += (long) vec.y;
    return *this;
}

Position &Position::operator-=(Vector &vec) {
    this->x -= (long) vec.x;
    this->y -= (long) vec.y;
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

