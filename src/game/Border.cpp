//
// Created by lorenz on 13.02.22.
//

#include "Border.h"

Border::Border(Position p1, Position p2, System *sys1, System *sys2) {
    this->p1 = p1;
    this->p2 = p2;
    this->sys1 = sys1;
    this->sys2 = sys2;
}
