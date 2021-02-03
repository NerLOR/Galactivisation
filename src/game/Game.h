//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_GAME_H
#define GALACTIVISATION_GAME_H

#include <SDL2/SDL.h>
#include "Galaxy.h"

class Game {
protected:
    Galaxy *galaxy;

public:
    Game();

    bool Init(SDL_Renderer *renderer);

    void OnEvent(SDL_Event *evt);

    void Calc(double d);

    void Render(SDL_Renderer *renderer);

    void OnResize(SDL_Renderer *renderer);
};


#endif //GALACTIVISATION_GAME_H
