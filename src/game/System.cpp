//
// Created by lorenz on 03.02.21.
//

#include "System.h"

System::System(long x, long y) : SpaceObj(Position(x, y)) {
    suns.push_back(new StaticSpaceObj(x, y, 0, 0));
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
                if (borderPoints.size() > 1) {
                    SDL_Rect dst1, dst2;
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xC0, 0x40, 0x00);
                    for (int i = 0; i < borderPoints.size(); i++) {
                        Position *p1 = borderPoints[i];
                        Position *p2 = borderPoints[(i == 0) ? borderPoints.size() - 1 : i - 1];
                        cam->GetScreenPos(p1, &dst1, 1, 1);
                        cam->GetScreenPos(p2, &dst2, 1, 1);
                        //SDL_RenderDrawLine(renderer, dst1.x, dst1.y, dst2.x, dst2.y);
                        SDL_RenderDrawPoint(renderer, dst1.x, dst2.y);
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

void System::AddBorderPoint(Position *p) {
    borderPoints.push_back(p);
}
