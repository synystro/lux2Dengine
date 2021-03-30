#pragma once

#include <SDL2/SDL.h>
#include <string>

class Entity;

class TextManager {
    public:
        static Entity& DisplayText(std::string textID, int posX, int posY, std::string text, int fontSize, std::string fontFamily, SDL_Color fontColor);
};