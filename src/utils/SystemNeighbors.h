//
// Created by lorenz on 08.02.21.
//

#ifndef GALACTIVISATION_SYSTEMNEIGHBORS_H
#define GALACTIVISATION_SYSTEMNEIGHBORS_H

#include "../game/Galaxy.h"
#include "../game/Position.h"

#include <iostream>
#include <queue>
#include <set>
#include <cmath>

namespace SystemNeighbors {
    double X0 = 0, X1 = 0, Y0 = 0, Y1 = 0;

    struct arc;
    struct seg;

    struct event {
        double x;
        Position p;
        arc *a;
        bool valid;

        event(double xx, Position pp, arc *aa) : x(xx), p(pp), a(aa), valid(true) {}
    };

    struct arc {
        Position p;
        arc *prev, *next;
        event *e;

        seg *s0, *s1;

        explicit arc(Position pp, arc *a = nullptr, arc *b = nullptr)
                : p(pp), prev(a), next(b), e(nullptr), s0(nullptr), s1(nullptr) {}
    };

    std::vector<seg *> output;

    struct seg {
        Position start, end;
        bool done;

        explicit seg(Position p) : start(p), end(0, 0), done(false) {
            output.push_back(this);
        }

        void finish(Position p) {
            if (done) return;
            end = p;
            done = true;
        }
    };

    arc *root = nullptr;

    void process_point();

    void process_event();

    void front_insert(Position p);

    bool circle(Position a, Position b, Position c, double *x, Position *o);

    void check_circle_event(arc *i, double x0);

    bool intersect(Position p, arc *i, Position *result = nullptr);

    Position intersection(Position p0, Position p1, double l);

    void finish_edges();

    void calculate(Galaxy *galaxy);

    struct gt {
        bool operator()(Position a, Position b) {
            return a.x == b.x ? a.y > b.y : a.x > b.x;
        }

        bool operator()(event *a, event *b) {
            return a->x > b->x;
        }
    };
}

#endif //GALACTIVISATION_SYSTEMNEIGHBORS_H
