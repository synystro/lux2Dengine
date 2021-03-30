#pragma once

#include "./Game.h"

class TextureManager {
    private:
    public:
        static SDL_Texture* LoadTexture(const char* fileName);
        static void Draw(SDL_Texture* texture, SDL_Rect sourceRect,
        SDL_Rect targetRect, SDL_RendererFlip flip);
};