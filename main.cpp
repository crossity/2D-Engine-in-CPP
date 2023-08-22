#include "engine.h"

#define w_Height 200
#define w_Width  200

#define Height 100
#define Width  100

using namespace std;

void surface_on_screen(SDL_Renderer *renderer, int w, int h, SDL_Surface *surface) {
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destr;
    destr.x = 0;
    destr.y = 0;
    destr.w = w;
    destr.h = h;

    SDL_RenderCopy(renderer, tex, NULL, &destr);

    SDL_DestroyTexture(tex);
}

void texture_on_screen(SDL_Renderer *renderer, int w, int h, SDL_Texture *tex) {
    SDL_Rect destr;
    destr.x = 0;
    destr.y = 0;
    destr.w = w;
    destr.h = h;

    SDL_RenderCopy(renderer, tex, NULL, &destr);
}

int main() {
    INIT();

    SDL_Window *window = nullptr;

    window = SDL_CreateWindow("3D Eingine", 20, 20, w_Width, w_Height, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool run = true;

    camera cam = camera(vec2(), Width, Height);
    SDL_SetRenderDrawBlendMode(cam.renderer, SDL_BLENDMODE_BLEND);

    object obj = object(vec2(0, 0), vec2(20, 20), "textures/grass.png", cam.renderer);
    obj.alpha = 100;
    vector<object> scene = {obj};
    obj.pos = vec2(10, 10);
    scene.push_back(obj);

    SDL_Texture *tex = nullptr;
    tex = IMG_LoadTexture(cam.renderer, "textures/grass.png");
    if (tex == nullptr)
        cout << "ERROR" << endl;

    while (run) {
        SDL_Event event;
    
        while(SDL_PollEvent(&event)){
            switch(event.type) {
                case SDL_QUIT:
                    run = false;
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(cam.renderer, 255, 255, 255, 0);
        SDL_RenderClear(cam.renderer);
        cam.render(scene);

        surface_on_screen(renderer, w_Width, w_Height, cam.surface);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
