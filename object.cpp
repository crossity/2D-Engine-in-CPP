#include <string>
#include <vector>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vec2.h"
#include "vec3.h"

using namespace std;

SDL_Texture* DuplicateTexture(SDL_Texture* tex, SDL_Renderer* renderer) {
    Uint32 format;
    int w, h;
    SDL_BlendMode blendmode;
    SDL_Texture* renderTarget;
    SDL_Texture* newTex;

    // Get all properties from the texture we are duplicating
    SDL_QueryTexture(tex, &format, NULL, &w, &h);
    SDL_GetTextureBlendMode(tex, &blendmode);

    // Save the current rendering target (will be NULL if it is the current window)
    renderTarget = SDL_GetRenderTarget(renderer);

    // Create a new texture with the same properties as the one we are duplicating
    newTex = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);

    // Set its blending mode and make it the render target
    SDL_SetTextureBlendMode(newTex, SDL_BLENDMODE_NONE);
    SDL_SetRenderTarget(renderer, newTex);

    // Render the full original texture onto the new one
    SDL_RenderCopy(renderer, tex, NULL, NULL);

    // Change the blending mode of the new texture to the same as the original one
    SDL_SetTextureBlendMode(newTex, blendmode);

    // Restore the render target
    SDL_SetRenderTarget(renderer, renderTarget);

    // Return the new texture
    return newTex;
}

struct object {
    vec2 pos;
    vec2 size;
    SDL_Texture *texture;
    vec3 color;
    int alpha;

    object() {
        this->pos = vec2();
        this->size = vec2();
        this->texture = nullptr;
        color = vec3(255, 255, 255);
        alpha = 255;
    }
    object(vec2 pos, vec2 size) {
        this->pos = pos;
        this->size = size;
        this->texture = nullptr;
        color = vec3(255, 255, 255);
        alpha = 255;
    }
    object(vec2 pos, vec2 size, string path, SDL_Renderer *renderer) {
        this->pos = pos;
        this->size = size;
        this->texture = nullptr;
        this->texture = IMG_LoadTexture(renderer, path.c_str());
        if (this->texture == nullptr)
            cout << "ERROR: could not load image" << endl;
        color = vec3(255, 255, 255);
        alpha = 255;
    }

    ~object() {
        SDL_DestroyTexture(this->texture);
        this->alpha = -1;
    }
};

unsigned char int_to_uint8(int a) {
    return a;
}

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, vec3 color, int alpha, vec2 pos, vec2 size, vec2 uv_start = vec2(), vec2 uv_end = vec2(1, 1)) {
    SDL_Vertex verts[3];
    SDL_Color c{int_to_uint8(color.x), int_to_uint8(color.y), int_to_uint8(color.z), int_to_uint8(alpha)};

    verts[0].position.x = pos.x;
    verts[0].position.y = pos.y;
    verts[0].tex_coord.x = uv_start.x;
    verts[0].tex_coord.y = uv_start.y;
    verts[0].color = c;

    verts[1].position.x = pos.x + size.x;
    verts[1].position.y = pos.y + size.y;
    verts[1].tex_coord.x = uv_end.x;
    verts[1].tex_coord.y = uv_end.y;
    verts[1].color = c;

    verts[2].position.x = pos.x;
    verts[2].position.y = pos.y + size.y;
    verts[2].tex_coord.x = uv_start.x;
    verts[2].tex_coord.y = uv_end.y;
    verts[2].color = c;
    SDL_RenderGeometry(renderer, texture, verts, 3, NULL, 0);

    verts[0].position.x = pos.x;
    verts[0].position.y = pos.y;
    verts[0].tex_coord.x = uv_start.x;
    verts[0].tex_coord.y = uv_start.y;
    verts[0].color = c;

    verts[1].position.x = pos.x + size.x;
    verts[1].position.y = pos.y;
    verts[1].tex_coord.x = uv_end.x;
    verts[1].tex_coord.y = uv_start.y;
    verts[1].color = c;

    verts[2].position.x = pos.x + size.x;
    verts[2].position.y = pos.y + size.y;
    verts[2].tex_coord.x = uv_end.x;
    verts[2].tex_coord.y = uv_end.y;
    verts[2].color = c;
    SDL_RenderGeometry(renderer, texture, verts, 3, NULL, 0);
}

struct camera : object {
    int height, width;
    SDL_Surface *surface;
    SDL_Renderer *renderer;

    camera() : object() {
        this->height = 0;
        this->width = 0;
        this->surface = SDL_CreateRGBSurface(0, this->width, this->height, 32, 0, 0, 0, 0);
        this->renderer = SDL_CreateSoftwareRenderer(this->surface);
    }
    camera(vec2 pos, int width, int height) : object(pos, vec2()) {
        this->width = width;
        this->height = height;
        this->surface = SDL_CreateRGBSurface(0, this->width, this->height, 32, 0, 0, 0, 0);
        this->renderer = SDL_CreateSoftwareRenderer(this->surface);
    }

    void render(vector<object> &objects) {
        for (int i = 0; i < objects.size(); i++)
            draw_texture(this->renderer, objects[i].texture, objects[i].color, objects[i].alpha, objects[i].pos, objects[i].size);
    }

    ~camera() {
        SDL_DestroyRenderer(this->renderer);
        SDL_FreeSurface(this->surface);
    }
};