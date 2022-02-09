
#include "voronoi.h"

#include <queue>
#include <vector>
#include <set>
#include <cmath>

namespace utils::voronoi {
    using Coord = double;

    enum class BLItemType {
        Arc,
        Edge,
    };

    enum class EventType {
        None,
        SiteEvent,
        CircleEvent,
    };

    class Point {
    public:
        Coord x = 0;
        Coord y = 0;

        Point() = default;;

        Point(Coord x, Coord y) {
            this->x = x;
            this->y = y;
        }
    };

    class Vector {
    public:
        double x = 0;
        double y = 0;

        Vector() = default;

        Vector(double x, double y) {
            this->x = x;
            this->y = y;
        }
    };

    class Edge {
    public:
        Point start;
        Vector dir;
        bool vertical = false;
        bool horizontal = false;
        // y = a * x + b
        double a = 0;
        Coord b = 0;

        Edge() = default;

        Edge(Point start, Point &left, Point &right) : start(0, 0), dir(0, 0) {
            this->start = start;
            dir = Vector((double) (right.y - left.y), (double) -(right.x - left.x));
            assert(!(dir.x == 0 && dir.y == 0));
            vertical = (dir.x == 0);
            horizontal = (dir.y == 0);
            NormalizeDirection();
            CalcCoefficients();
        }

        Edge(Point start, Vector dir) : start(start), dir(dir) {
            assert(!(dir.x == 0 && dir.y == 0));
            vertical = (dir.x == 0);
            horizontal = (dir.y == 0);
            NormalizeDirection();
            CalcCoefficients();
        }

        void NormalizeDirection() {
            double len = std::hypot(dir.x, dir.y);
            dir.x /= len;
            dir.y /= len;
        }

        void CalcCoefficients() {
            if (vertical) {
                a = 0;
                b = 0;
            } else {
                a = dir.y / dir.x;
                b = start.y - (Coord) (a * (double) start.x);
            }
        }

        Coord GetY(Coord x) const {
            if (vertical)
                throw std::exception();
            else if (horizontal)
                return start.y;
            return (Coord) (a * (double) x) + b;
        }

        Coord GetX(Coord y) const {
            if (vertical)
                return start.x;
            else if (horizontal)
                throw std::exception();
            return (Coord) ((double) (y - b) / a);
        }

        bool Intersect(Edge &e2, Point &intersection) const {
            if ((vertical && e2.vertical) || (horizontal && e2.horizontal))
                return false;

            Coord x, y;
            if (horizontal) {
                y = GetY(0);
                x = e2.GetX(y);
            } else if (vertical) {
                x = GetX(0);
                y = e2.GetY(x);
            } else if (e2.horizontal) {
                y = e2.GetY(0);
                x = GetX(y);
            } else if (e2.vertical) {
                x = e2.GetX(0);
                y = GetY(x);
            } else {
                x = (Coord) ((double) (e2.b - b) / (a - e2.a));
                y = GetY(x);
            }

            if (((double) (x - start.x) * dir.x < 0) ||
                ((double) (y - start.y) * dir.y < 0) ||
                ((double) (x - e2.start.x) * e2.dir.x < 0) ||
                ((double) (y - e2.start.y) * e2.dir.y < 0))
            {
                return false;
            }

            intersection = {x, y};
            return true;
        }
    };

    class CompleteEdge {
    public:
        Point a;
        Point b;

        CompleteEdge(Point a, Point b) {
            this->a = a;
            this->b = b;
        }
    };

    class CircleEvent;

    class Arc {
    public:
        Point focus;
        CircleEvent *event = nullptr;

        bool GetY(Coord x, Coord directrixY, Coord &res) const {
            if (focus.y == directrixY)
                return false;

            double a = 1.0 / (2.0 * ((double) focus.y - (double) directrixY));
            double c = ((double) focus.y + (double) directrixY) / 2.0;
            double w = (double) x - (double) focus.x;

            res = (Coord) (a * w * w + c);
            return true;
        }
    };

    class Event {
    public:
        Point pos;
        EventType type;

        Event() {
            type = EventType::None;
            pos = Point(0, 0);
        }
    };

    class SiteEvent : public Event {
        System *system;

    public:
        Point point;

        explicit SiteEvent(System *sys)  {
            type = EventType::SiteEvent;
            system = sys;
            point = {(Coord) system->GetPos()->x, (Coord) system->GetPos()->y};
            pos = point;
        }
    };

    class BLItem;

    struct CircleEvent : public Event {
    public:
        Point center;
        BLItem *squeezed = nullptr;
        bool valid = true;

        CircleEvent(Point p, Point center, BLItem *squeezed) {
            type = EventType::CircleEvent;
            pos = p;
            this->center = center;
            this->squeezed = squeezed;
        }
    };

    struct EventComparison : std::binary_function<Event, Event, bool> {
        bool operator()(const Event *a, const Event *b) const {
            return a->pos.y == b->pos.y ? a->pos.x > b->pos.x : a->pos.y < b->pos.y;
        }
    };

    class BLItem {
    public:
        BLItemType type;
        union {
            Arc arc;
            Edge edge;
        };

        BLItem *parent = nullptr;
        BLItem *left = nullptr;
        BLItem *right = nullptr;

        explicit BLItem(Point focus) {
            type = BLItemType::Arc;
            arc.focus = focus;
            arc.event = nullptr;
        }

        BLItem(Point start, Point &left, Point &right) {
            type = BLItemType::Edge;
            edge = Edge(start, left, right);
        }

        BLItem(Point start, Vector dir) {
            type = BLItemType::Edge;
            edge = Edge(start, dir);
        }

        void SetLeftChild(BLItem *l) {
            assert(type == BLItemType::Edge);
            assert(l != nullptr);
            left = l;
            l->parent = this;
        }

        void SetRightChild(BLItem *r) {
            assert(type == BLItemType::Edge);
            assert(r != nullptr);
            right = r;
            r->parent = this;
        }

        void SetChildren(BLItem *l, BLItem *r) {
            assert(type == BLItemType::Edge);
            assert(l != nullptr && r != nullptr);
            left = l;
            right = r;
            l->parent = this;
            r->parent = this;
        }

        void SetParentFromItem(BLItem *item) {
            assert(item != nullptr);
            if (item->parent == nullptr) {
                parent = nullptr;
                return;
            }

            if (item->parent->left == item) {
                item->parent->SetLeftChild(this);
            } else {
                assert(item->parent->right == item);
                item->parent->SetRightChild(this);
            }
        }

        BLItem *GetFirstAncestorLeft() {
            BLItem *cur = this;
            while (cur->parent) {
                if (cur->parent->right == cur) {
                    assert(cur->parent->type == BLItemType::Edge);
                    return cur->parent;
                }
                cur = cur->parent;
            }
            return nullptr;
        }

        BLItem *GetFirstAncestorRight() {
            BLItem *cur = this;
            while (cur->parent) {
                if (cur->parent->left == cur) {
                    assert(cur->parent->type == BLItemType::Edge);
                    return cur->parent;
                }
                cur = cur->parent;
            }
            return nullptr;
        }

        BLItem *GetFirstLeafLeft() const {
            BLItem *cur = left;
            if (!cur)
                return nullptr;
            while (cur->right) cur = cur->right;
            assert(cur->type == BLItemType::Arc);
            return cur;
        }

        BLItem *GetFirstLeafRight() const {
            BLItem *cur = right;
            if (!cur)
                return nullptr;
            while (cur->left) cur = cur->left;
            assert(cur->type == BLItemType::Arc);
            return cur;
        }
    };

    bool IntersectEdgeArc(Edge &edge, Arc &arc, Coord directrixY, Point &intersection) {
        if (edge.vertical) {
            if (directrixY == arc.focus.y) {
                if (edge.start.x == arc.focus.x) {
                    intersection = arc.focus;
                    return true;
                }
                return false;
            }
            Coord y;
            assert(arc.GetY(edge.start.x, directrixY, y));
            intersection = {edge.start.x, y};
            return true;
        }

        if (arc.focus.y == directrixY) {
            if ((double) (arc.focus.x - edge.start.x) * edge.dir.x < 0)
                return false;

            intersection = {arc.focus.x, (Coord) (edge.a * (double) arc.focus.x) + edge.b};
            return true;
        }

        // y = a_0 + a_1x + a_2x^2
        double a2 = 1.0 / (2 * ((double) arc.focus.y - (double) directrixY));
        double a1 = -edge.a - 2 * a2 * (double) arc.focus.x;
        double a0 = a2 * (double) arc.focus.x * (double) arc.focus.x + ((double) arc.focus.y + (double) directrixY) / 2.0 - edge.b;

        double disc = a1 * a1 - 4 * a2 * a0;
        if (disc < 0)
            return false;

        double rootDisc = std::sqrt(disc);
        Coord x1 = (Coord) (-a1 + rootDisc) / (2 * a2);
        Coord x2 = (Coord) (-a1 - rootDisc) / (2 * a2);
        double x1dot = ((double) x1 - (double) edge.start.x) * edge.dir.x;
        double x2dot = ((double) x2 - (double) edge.start.x) * edge.dir.x;

        Coord x;
        if ((x1dot >= 0) && (x2dot < 0)) {
            x = x1;
        } else if ((x1dot < 0) && (x2dot >= 0)) {
            x = x2;
        } else if ((x1dot >= 0) && (x2dot >= 0)) {
            x = (x1dot < x2dot) ? x1 : x2;
        } else {
            x = (x1dot < x2dot) ? x2 : x1;
        }

        Coord y;
        assert(arc.GetY(x, directrixY, y));
        intersection = {x, y};
        return true;
    }

    class Voronoi {
        Point P0;
        Point P1;

    public:
        Coord sweepLine;
        std::vector<CompleteEdge *> edges{};
        std::priority_queue<Event *, std::vector<Event *>, EventComparison> events{};
        BLItem *root;

        Voronoi(Point p0, Point p1) : P0(p0), P1(p1) {
            sweepLine = P1.y;
            root = nullptr;
        }

        void AddSystems(std::vector<System *> *systems) {
            for (auto system: *systems) {
                events.push(new SiteEvent(system));
            }
        }

        void Calc() {
            while (!events.empty()) {
                Event *event = events.top(); events.pop();
                sweepLine = event->pos.y;
                if (event->type == EventType::SiteEvent) {
                    ProcessSiteEvent((SiteEvent *) event);
                } else if (event->type == EventType::CircleEvent) {
                    ProcessCircleEvent((CircleEvent *) event);
                } else {
                    throw std::exception();
                }
            }
            FinishEdges();
        }

        void FinishEdges() {
            FinishEdge(root);
            delete root;
            root = nullptr;
        }

        void ProcessSiteEvent(SiteEvent *event) {
            AddArcToBeachLine(*event);
            delete event;
        }

        void ProcessCircleEvent(CircleEvent *event) {
            if (event->valid) RemoveArcFromBeachLine(*event);
            delete event;
        }

        BLItem *GetActiveArcForXCoord(Coord x, Coord directrixY) const {
            BLItem *cur = root;
            while (cur->type != BLItemType::Arc) {
                assert(cur->type == BLItemType::Edge);
                BLItem *left = cur->GetFirstLeafLeft();
                BLItem *right = cur->GetFirstLeafRight();
                assert((left != nullptr) && (left->type == BLItemType::Arc));
                assert((right != nullptr) && (right->type == BLItemType::Arc));

                BLItem *fromLeft = left->GetFirstAncestorRight();
                BLItem *fromRight = right->GetFirstAncestorLeft();
                assert((fromLeft != nullptr) && (fromLeft == fromRight));
                assert(fromLeft->type == BLItemType::Edge);
                Edge &sep = fromLeft->edge;

                Point leftIntersect;
                Point rightIntersect;
                bool didLeftIntersect = IntersectEdgeArc(sep, left->arc, directrixY, leftIntersect);
                bool didRightIntersect = IntersectEdgeArc(sep, right->arc, directrixY, rightIntersect);

                Coord xx = (!didLeftIntersect && didRightIntersect) ? rightIntersect.x : leftIntersect.x;
                if (x < xx) {
                    cur = cur->left;
                } else {
                    cur = cur->right;
                }
            }

            assert(cur->type == BLItemType::Arc);
            return cur;
        }

        void CheckCircle(BLItem *arc) {
            BLItem *leftEdge = arc->GetFirstAncestorLeft();
            BLItem *rightEdge = arc->GetFirstAncestorRight();

            if ((leftEdge == nullptr) || (rightEdge == nullptr))
                return;

            Point center;
            if (!leftEdge->edge.Intersect(rightEdge->edge, center))
                return;

            double radius = hypot(arc->arc.focus.x - center.x, arc->arc.focus.y - center.y);
            Coord eventY = center.y - radius;
            assert(arc->type == BLItemType::Arc);

            if (arc->arc.event) {
                if (arc->arc.event->pos.y >= eventY)
                    return;
                assert(arc->arc.event->type == EventType::CircleEvent);
                arc->arc.event->valid = false;
            }

            auto *newEvt = new CircleEvent({center.x, eventY}, center, arc);
            events.push(newEvt);

            arc->arc.event = newEvt;
        }

        void AddArcToBeachLine(SiteEvent &event) {
            Point p = event.point;
            if (!root) {
                root = new BLItem(p);
                return;
            }

            BLItem *repl = GetActiveArcForXCoord(p.x, sweepLine);
            assert((repl != nullptr) && (repl->type == BLItemType::Arc));
            auto newArc = new BLItem(p);

            if (repl->arc.focus.y == p.y) {
                auto newEdge = new BLItem({p.x / 2 + repl->arc.focus.x / 2,P1.y}, {0, -1});

                if (repl->parent) {
                    if (repl == repl->parent->left) {
                        repl->parent->SetLeftChild(newEdge);
                    } else {
                        assert(repl == repl->parent->right);
                        repl->parent->SetRightChild(newEdge);
                    }
                } else {
                    root = newEdge;
                }

                if (p.x < repl->arc.focus.x) {
                    newEdge->SetChildren(newArc, repl);
                } else {
                    newEdge->SetChildren(repl, newArc);
                }

                return;
            }

            auto splitLeft = new BLItem(repl->arc.focus);
            auto splitRight = new BLItem(repl->arc.focus);

            Coord intersectionY;
            assert(repl->arc.GetY(p.x, sweepLine, intersectionY));

            Point start = {p.x, intersectionY};
            auto edgeLeft = new BLItem(start, repl->arc.focus, newArc->arc.focus);
            auto edgeRight = new BLItem(start, newArc->arc.focus, repl->arc.focus);

            assert(repl->left == nullptr);
            assert(repl->right == nullptr);
            edgeLeft->SetParentFromItem(repl);
            edgeLeft->SetChildren(splitLeft, edgeRight);
            edgeRight->SetChildren(newArc, splitRight);

            if (root == repl) root = edgeLeft;

            if (repl->arc.event) {
                assert(repl->arc.event->type == EventType::CircleEvent);
                assert(repl->arc.event->valid);
                repl->arc.event->valid = false;
            }

            assert(repl->arc.event == nullptr || repl->arc.event->valid == false);
            delete repl;

            CheckCircle(splitLeft);
            CheckCircle(splitRight);
        }

        void RemoveArcFromBeachLine(CircleEvent &event) {
            BLItem *squeezed = event.squeezed;
            assert(event.type == EventType::CircleEvent);
            assert(event.valid);
            assert(squeezed->arc.event == &event);

            BLItem *leftEdge = squeezed->GetFirstAncestorLeft();
            BLItem *rightEdge = squeezed->GetFirstAncestorRight();
            assert(leftEdge != nullptr && rightEdge != nullptr);

            BLItem *leftArc = leftEdge->GetFirstLeafLeft();
            BLItem *rightArc = rightEdge->GetFirstLeafRight();
            assert(leftArc != nullptr && rightArc != nullptr);
            assert(leftArc != rightArc);

            edges.emplace_back(new CompleteEdge(leftEdge->edge.start, event.center));
            edges.emplace_back(new CompleteEdge(event.center, rightEdge->edge.start));

            auto newItem = new BLItem(event.center, leftArc->arc.focus, rightArc->arc.focus);

            BLItem *higher = nullptr;
            BLItem *temp = squeezed;
            while (temp->parent != nullptr) {
                temp = temp->parent;
                if (temp == leftEdge) higher = leftEdge;
                if (temp == rightEdge) higher = rightEdge;
            }
            assert(higher != nullptr && higher->type == BLItemType::Edge);

            newItem->SetParentFromItem(higher);
            newItem->SetLeftChild(higher->left);
            newItem->SetRightChild(higher->right);

            assert(squeezed->parent == nullptr || squeezed->parent->type == BLItemType::Edge);
            BLItem *remaining = nullptr;
            BLItem *parent = squeezed->parent;
            if (parent->left == squeezed) {
                remaining = parent->right;
            } else {
                assert(parent->right == squeezed);
                remaining = parent->left;
            }
            assert(parent == leftEdge || parent == rightEdge);
            assert(parent != higher);

            remaining->SetParentFromItem(parent);

            if ((root == leftEdge) || (root == rightEdge))
                root = newItem;

            assert(squeezed->type == BLItemType::Arc);
            if (squeezed->arc.event != nullptr) {
                assert(squeezed->arc.event->type == EventType::CircleEvent);
                assert(squeezed->arc.event->valid);
                squeezed->arc.event->valid = false;
            }
            delete leftEdge;
            delete squeezed;
            delete rightEdge;

            CheckCircle(leftArc);
            CheckCircle(rightArc);
        }

        void FinishEdge(BLItem *item) {
            if (item->type == BLItemType::Arc)
                return;

            Coord x, y;
            Edge e = item->edge;
            if (e.vertical) {
                x = e.GetX(0);
                y = (e.dir.y > 0) ? P1.y : P0.y;
            } else if (e.horizontal) {
                y = e.GetY(0);
                x = (e.dir.x > 0) ? P1.x : P0.x;
            } else if (std::abs(e.a) > 1) {
                y = (e.dir.y > 0) ? P1.y : P0.y;
                x = e.GetX(y);
                if (x > P1.x) {
                    x = P1.x;
                    y = e.GetY(x);
                } else if (x < P0.x) {
                    x = P0.x;
                    y = e.GetY(x);
                }
            } else {
                x = (e.dir.x > 0) ? P1.x : P0.x;
                y = e.GetY(x);
                if (y > P1.y) {
                    y = P1.y;
                    x = e.GetX(y);
                } else if (y < P0.y) {
                    y = P0.y;
                    x = e.GetX(y);
                }
            }
            edges.push_back(new CompleteEdge(e.start, {x, y}));

            FinishEdge(item->left);
            FinishEdge(item->right);
        }
    };

    void CalcSystemNeighbors(Galaxy *galaxy) {
        auto vor = new Voronoi({(Coord) galaxy->x0, (Coord) galaxy->y0}, {(Coord) galaxy->x1, (Coord) galaxy->y1});
        vor->AddSystems(galaxy->GetSystems());
        vor->Calc();
        auto *borders = galaxy->GetBorders();
        for (auto *e : vor->edges) {
            borders->push_back(new Border(
                    new Position((long long) e->a.x, (long long) e->a.y),
                    new Position((long long) e->b.x, (long long) e->b.y)
            ));
        }
    }
}
