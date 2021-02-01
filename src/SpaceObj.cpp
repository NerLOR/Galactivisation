//
// Created by lorenz on 30.01.21.
//

#include "SpaceObj.h"

SpaceObj::SpaceObj(Position pos) : pos(pos) {

}

SpaceObj::SpaceObj(Position *pos) : pos(*pos) {

}

Position *SpaceObj::GetPos() {
    return &pos;
}

bool SpaceObj::Compare(SpaceObj *a, SpaceObj *b) {
    return a->GetPos()->x < b->GetPos()->x;
}
