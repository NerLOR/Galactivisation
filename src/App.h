//
// Created by lorenz on 30.01.21.
//

#ifndef GALACTIVISATION_APP_H
#define GALACTIVISATION_APP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game/Game.h"

class App {
protected:
    bool running = true;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    unsigned short ticksPerFrame = 1000;
    struct {SDL_Surface *surf; SDL_Texture **variable;} surfaces[1000];
    int surfaceNum = 0;

public:
    Game game{};

    App();

    bool Init();

    bool Cleanup();

    void SetFps(unsigned short fps);

    void OnEvent(SDL_Event *evt);

    void Main();

    bool LoadTexture(const char *path, SDL_Texture **var);

    SDL_Renderer *GetRenderer();

    SDL_Window *GetWindow();

    bool Resize(int w, int h, bool fullscreen);
};


#endif //GALACTIVISATION_APP_H
