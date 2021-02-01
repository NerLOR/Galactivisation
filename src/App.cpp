//
// Created by lorenz on 30.01.21.
//

#include "App.h"

App::App() {

}

bool App::Resize(int w, int h, bool fullscreen) {
    if (renderer != nullptr) SDL_DestroyRenderer(renderer);
    if (window != nullptr) SDL_DestroyWindow(window);
    window = SDL_CreateWindow("Galactivisation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (window == nullptr) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        return false;
    }

    for (int i = 0; i < surfaceNum; i++) {
         *surfaces[i].variable = SDL_CreateTextureFromSurface(renderer, surfaces[i].surf);
    }

    game.OnResize(renderer);
    return true;
}

bool App::Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Unable to initialize game engine: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "Unable to initialize SDL_image: %s\n", IMG_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    return true;
}

bool App::Cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void App::SetFps(unsigned short fps) {
    ticksPerFrame = 1000 / fps;
}

void App::OnEvent(SDL_Event *evt) {
    if (evt->type == SDL_QUIT || (evt->type == SDL_KEYUP && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE && evt->key.state == SDL_RELEASED)) {
        running = false;
    } else if (evt->type == SDL_KEYUP && evt->key.keysym.scancode == SDL_SCANCODE_F11 && evt->key.state == SDL_RELEASED) {
        Resize(1080, 720, (SDL_GetWindowFlags(window) & (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN)) == 0);
    } else {
        game.OnEvent(evt);
    }
}

void App::Main() {
    SDL_Event Event;
    int start_time, end_time, delay_time;
    int last_time = SDL_GetTicks();
    while (running) {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        game.Calc((double) (start_time - last_time));
        game.Render(renderer);
        end_time = SDL_GetTicks();
        delay_time = ticksPerFrame - (end_time - start_time);
        if (delay_time > 0) SDL_Delay(delay_time);
        last_time = start_time;
    }
    printf("\n");
    Cleanup();
}

bool App::LoadTexture(const char *path, SDL_Texture **var) {
    SDL_Surface *surface = IMG_Load(path);
    if (surface == nullptr) {
        fprintf(stderr, "Unable to load image: %s\n", IMG_GetError());
        return false;
    }
    surfaces[surfaceNum++] = {surface, var};
    return true;
}

SDL_Renderer *App::GetRenderer() {
    return renderer;
}

SDL_Window *App::GetWindow() {
    return window;
}
