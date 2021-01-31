//
// Created by lorenz on 30.01.21.
//

#include "Game.h"

Game::Game() {
    galaxy = new Galaxy();
}

bool Game::Init(SDL_Renderer *renderer) {
    galaxy->Init(renderer);
    return true;
}

void Game::OnEvent(SDL_Event *evt) {
    Camera *cam = &galaxy->cam;
    if (evt->type == SDL_MOUSEWHEEL) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        x -= cam->screenW / 2;
        y -= cam->screenH / 2;
        long posX = (long) (x * cam->mppx);
        long posY = (long) (y * cam->mppx);
        if (evt->wheel.y < 0) {
            cam->zoomTarget *= 2;
            if (cam->zoomTarget > cam->zoomMax) {
                cam->zoomTarget = cam->zoomMax;
            } else {
                cam->posTarget.x -= posX / 3;
                cam->posTarget.y -= posY / 3;
            }
        } else if (evt->wheel.y > 0) {
            cam->zoomTarget /= 2;
            if (cam->zoomTarget < cam->zoomMin) {
                cam->zoomTarget = cam->zoomMin;
            } else {
                cam->posTarget.x += posX / 3;
                cam->posTarget.y += posY / 3;
            }
        }
    } else if (evt->type == SDL_KEYUP) {
        if (evt->key.keysym.sym == SDLK_m) {
            cam->zoomTarget = cam->zoomMax;
            cam->posTarget.x = 0;
            cam->posTarget.y = 0;
        }
    }
}

void Game::Calc(double d) {
    galaxy->Calc(d);
}

void Game::Render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    galaxy->Render(renderer);
    SDL_RenderPresent(renderer);
}

void Game::OnResize(SDL_Renderer *renderer) {
    galaxy->OnResize(renderer);
    galaxy->cam.Init(renderer, galaxy->GetDiameter());
}
