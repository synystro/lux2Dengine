#pragma once

#include "./FontManager.h"

TTF_Font* FontManager::LoadFont(const char* fileName, int fontSize) {
    return TTF_OpenFont(fileName, fontSize);
}

TTF_Font* FontManager::ChangeFontSize(const char* family, int size) {
    return TTF_OpenFont(family, size);
}

void FontManager::Draw(SDL_Texture *texture, SDL_Rect position) {
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}