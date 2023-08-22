#include "object.h"

void INIT() {
    if (IMG_Init(IMG_INIT_PNG) < 0)
        cout << "IMG could not be initiaiized: " << IMG_GetError();

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
        cout << "SDL could not be initiaiized: " << SDL_GetError();
}