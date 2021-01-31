//
// Created by lorenz on 29.01.21.
//

#include "Vector.h"

Vector::Vector(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector &Vector::operator+=(Vector &vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector &Vector::operator-=(Vector &vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector &Vector::operator*=(double multi) {
    this->x *= multi;
    this->y *= multi;
    return *this;
}


Vector &Vector::operator/=(double div) {
    this->x /= div;
    this->y /= div;
    return *this;
}
