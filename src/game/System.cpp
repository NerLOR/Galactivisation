//
// Created by lorenz on 03.02.21.
//

#include "System.h"

System::System(long long x, long long y, unsigned long long reach) : SpaceObj(Position(x, y)) {
    suns.push_back(new StaticSpaceObj(x, y, 0, 0));
    this->reach = reach;
}

void System::Calc(unsigned long long int t, double d) {
    for (auto sun : suns) {
        sun->Calc(t, d);
    }
    for (auto obj : objs) {
        obj->Calc(t, d);
    }
}

void System::Render(SDL_Renderer *renderer, Camera *cam) {
    SDL_Rect dst, neigh_dst;
    bool visible = cam->GetScreenPos(&this->pos, &dst, 1000000000000, 1000000000000);
    if (visible) {
        if (dst.w < 12) {
            for (auto neighbor : neighbors) {
                SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0x00);
                bool neigh_visible = cam->GetScreenPos(&neighbor->pos, &neigh_dst, 1, 1);
                SDL_RenderDrawLine(renderer, dst.x + dst.w / 2, dst.y + dst.h / 2, neigh_dst.x, neigh_dst.y);
                if (neigh_visible) {
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
                    SDL_RenderDrawPoint(renderer, neigh_dst.x, neigh_dst.y);
                }
            }
            if (hypot(cam->mouseX - dst.x, cam->mouseY - dst.y) < 10) {
                SDL_Point p1, p2;
                SDL_SetRenderDrawColor(renderer, 0x00, 0xC0, 0x40, 0x00);
                for (auto b : borders) {
                    if (cam->GetLineScreenPos(&b->p1, &b->p2, &p1, &p2)) {
                        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            SDL_RenderDrawPoint(renderer, dst.x + dst.w / 2, dst.y + dst.h / 2);
        } else {
            for (auto sun : suns) {
                sun->Render(renderer, cam);
            }
            if (dst.w >= 48) {
               for (auto obj : objs) {
                   obj->Render(renderer, cam);
               }
            }
        }
    }
}

void System::AddNeighbor(System *neighbor) {
    if (neighbor == this || neighbors.contains(neighbor)) return;
    neighbors.insert(neighbor);
    neighbor->neighbors.insert(this);
}

void System::RemoveNeighbor(System *neighbor) {
    if (neighbor == this || !neighbors.contains(neighbor)) return;
    neighbors.erase(neighbor);
    neighbor->neighbors.erase(this);
}

std::set<System *> *System::GetNeighbors() {
    return &neighbors;
}

void System::AddBorder(Border *b) {
    borders.push_back(b);
    System *other = (b->sys1 == this) ? b->sys2 : b->sys1;
    if (!neighbors.contains(other) &&
            pos.GetDistanceTo(other->pos) < reach &&
        Position::DoLineSegmentsIntersect(b->p1, b->p2, pos, other->pos))
    {
        AddNeighbor(other);
    }
}
