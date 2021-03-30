#pragma once

#include "./Game.h"

class FontManager {
    private:

    public:
        static TTF_Font* LoadFont(const char* fileName, int fontSize);
        static TTF_Font* ChangeFontSize(const char* family, int size);
        static void Draw(SDL_Texture* texture, SDL_Rect position);
};