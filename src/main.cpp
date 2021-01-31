
#include <iostream>
#include <SDL2/SDL.h>

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

#include "Vector.cpp"
#include "Position.cpp"
#include "Camera.cpp"
#include "SpaceObj.cpp"
#include "StaticSpaceObj.cpp"
#include "FixedSpaceObj.cpp"
#include "Galaxy.cpp"
#include "Game.cpp"
#include "App.cpp"

//  -> 1 ms (physics)
// Tertiae, Ter.: Tick -> 1 sec (IG "1 hr") (IRL 0:00:01)
// Secundae, Sec.: Tock -> 60 Ticks (IG "1 day") (IRL 0:01:00)
// Primare, Prim.: Tuck -> 60 Tocks (IG "1 yr") (IRL 1:00:00)

// Tick = 1ms
// ater -> 0:00:01
// asec -> 50 ater -> 0:00:50
// apri -> 100 asec -> 1:23:20

int main() {
    auto *app = new App();
    app->Init();
    app->SetFps(60);
    app->LoadTexture("res/sun.png", &sunTexture);
    app->Resize(1080, 720, false);
    app->game.Init(app->GetRenderer());
    app->Main();
    return 0;
}
