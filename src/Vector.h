//
// Created by lorenz on 29.01.21.
//

#ifndef GALACTIVISATION_VECTOR_H
#define GALACTIVISATION_VECTOR_H

class Vector {
public:
    double x;
    double y;

    Vector(double x, double y);

    Vector &operator+=(Vector &vec);

    Vector &operator-=(Vector &vec);

    Vector &operator*=(double multi);

    Vector &operator/=(double div);
};


#endif //GALACTIVISATION_VECTOR_H
