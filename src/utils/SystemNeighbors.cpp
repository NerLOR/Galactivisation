//
// Created by lorenz on 08.02.21.
//

#include "SystemNeighbors.h"

namespace SystemNeighbors {
    std::priority_queue<System *, std::vector<System *>, gt> points;
    std::priority_queue<event *, std::vector<event *>, gt> events;

    void calculate(Galaxy *galaxy) {
        for (auto sys : *galaxy->GetSystems()) {
            points.push(sys);
        }

        // Process the queues; select the top element with smaller x coordinate.
        while (!points.empty()) {
            if (!events.empty() && events.top()->x <= points.top()->GetPos()->y) {
                process_event();
            } else {
                process_point();
            }
        }

        // After all points are processed, do the remaining circle events.
        while (!events.empty()) {
            process_event();
        }

        finish_edges(); // Clean up dangling edges.
    }

    void process_point() {
        // Get the next Position from the queue.
        System *p = points.top();
        points.pop();

        // Add a new arc to the parabolic front.
        front_insert(p->GetPos());
    }

    void process_event() {
        // Get the next event from the queue.
        event *e = events.top();
        events.pop();

        if (e->valid) {
            // Start a new edge.
            seg *s = new seg(e->p);

            // Remove the associated arc from the front.
            arc *a = e->a;
            if (a->prev) {
                a->prev->next = a->next;
                a->prev->s1 = s;
            }
            if (a->next) {
                a->next->prev = a->prev;
                a->next->s0 = s;
            }

            // Finish the edges before and after a.
            if (a->s0) a->s0->finish(e->p);
            if (a->s1) a->s1->finish(e->p);

            // Recheck circle events on either side of p:
            if (a->prev) check_circle_event(a->prev, e->x);
            if (a->next) check_circle_event(a->next, e->x);
        }
        delete e;
    }

    void front_insert(Position p) {
        if (!root) {
            root = new arc(p);
            return;
        }

        // Find the current arc(s) at height p.y (if there are any).
        for (arc *i = root; i; i = i->next) {
            Position z, zz;
            if (intersect(p, i, &z)) {
                // New parabola intersects arc i.  If necessary, duplicate i.
                if (i->next && !intersect(p, i->next, &zz)) {
                    i->next->prev = new arc(i->p, i, i->next);
                    i->next = i->next->prev;
                } else {
                    i->next = new arc(i->p, i);
                }
                i->next->s1 = i->s1;

                // Add p between i and i->next.
                i->next->prev = new arc(p, i, i->next);
                i->next = i->next->prev;

                i = i->next; // Now i points to the new arc.

                // Add new half-edges connected to i's endpoints.
                i->prev->s1 = i->s0 = new seg(z);
                i->next->s0 = i->s1 = new seg(z);

                // Check for new circle events around the new arc:
                check_circle_event(i, p.x);
                check_circle_event(i->prev, p.x);
                check_circle_event(i->next, p.x);

                return;
            }
        }

        // Special case: If p never intersects an arc, append it to the list.
        arc *i;
        for (i = root; i->next; i = i->next); // Find the last node.

        i->next = new arc(p, i);
        // Insert segment between p and i
        Position start;
        start.x = X0;
        start.y = (i->next->p.y + i->p.y) / 2;
        i->s1 = i->next->s0 = new seg(start);
    }

// Look for a new circle event for arc i.
    void check_circle_event(arc *i, double x0) {
        // Invalidate any old event.
        if (i->e && i->e->x != x0) i->e->valid = false;
        i->e = nullptr;

        if (!i->prev || !i->next) return;

        double x;
        Position o;

        if (circle(i->prev->p, i->p, i->next->p, &x, &o) && x > x0) {
            // Create new event.
            i->e = new event(x, o, i);
            events.push(i->e);
        }
    }

// Find the rightmost Position on the circle through a,b,c.
    bool circle(Position a, Position b, Position c, double *x, Position *o) {
        // Check that bc is a "right turn" from ab.
        if ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) > 0)
            return false;

        // Algorithm from O'Rourke 2ed p. 189.
        double A = b.x - a.x, B = b.y - a.y,
                C = c.x - a.x, D = c.y - a.y,
                E = A * (a.x + b.x) + B * (a.y + b.y),
                F = C * (a.x + c.x) + D * (a.y + c.y),
                G = 2 * (A * (c.y - b.y) - B * (c.x - b.x));

        if (G == 0) return false;  // points are co-linear.

        // Position o is the center of the circle.
        o->x = (D * E - B * F) / G;
        o->y = (A * F - C * E) / G;

        // o.x plus radius equals max x coordinate.
        *x = o->x + sqrt(pow(a.x - o->x, 2) + pow(a.y - o->y, 2));
        return true;
    }

// Will a new parabola at Position p intersect with arc i?
    bool intersect(Position p, arc *i, Position *res) {
        if (i->p.x == p.x) return false;

        double a, b;
        if (i->prev) { // Get the intersection of i->prev, i.
            a = intersection(i->prev->p, i->p, p.x).y;
        }
        if (i->next) { // Get the intersection of i->next, i.
            b = intersection(i->p, i->next->p, p.x).y;
        }

        if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
            res->y = p.y;

            // Plug it back into the parabola equation.
            res->x = (i->p.x * i->p.x + (i->p.y - res->y) * (i->p.y - res->y) - p.x * p.x)
                     / (2 * i->p.x - 2 * p.x);

            return true;
        }
        return false;
    }

// Where do two parabolas intersect?
    Position intersection(Position p0, Position p1, double l) {
        Position res, p = p0;

        if (p0.x == p1.x) {
            res.y = (p0.y + p1.y) / 2;
        } else if (p1.x == l) {
            res.y = p1.y;
        } else if (p0.x == l) {
            res.y = p0.y;
            p = p1;
        } else {
            // Use the quadratic formula.
            double z0 = 2 * (p0.x - l);
            double z1 = 2 * (p1.x - l);

            double a = 1 / z0 - 1 / z1;
            double b = -2 * (p0.y / z0 - p1.y / z1);
            double c = (p0.y * p0.y + p0.x * p0.x - l * l) / z0
                       - (p1.y * p1.y + p1.x * p1.x - l * l) / z1;

            res.y = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        }
        // Plug back into one of the parabola equations.
        res.x = (p.x * p.x + (p.y - res.y) * (p.y - res.y) - l * l) / (2 * p.x - 2 * l);
        return res;
    }

    void finish_edges() {
        // Advance the sweep line so no parabolas can cross the bounding box.
        double l = X1 + (X1 - X0) + (Y1 - Y0);

        // Extend each remaining segment to the new parabola intersections.
        for (arc *i = root; i->next; i = i->next) {
            if (i->s1) {
                i->s1->finish(intersection(i->p, i->next->p, l * 2));
            }
        }
    }
}
