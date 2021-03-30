#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"

class TextComponent: public Component {
    private:
        SDL_Rect position;
        std::string text;
        std::string family;
        SDL_Color color;
        SDL_Texture* texture;
    public:
        TextComponent(int x, int y, std::string text, std::string family, const SDL_Color& color) {
            this->position.x = x;
            this->position.y = y;
            this->text = text;
            this->family = family;
            this->color = color;

            SetText(text, family);
        }
        SDL_Rect GetOffsetRect() {
            SDL_Rect offsetRect;

            offsetRect.x = position.x - (position.w / 2);
            offsetRect.y = position.y - (position.h / 2);
            offsetRect.w = position.w;
            offsetRect.h = position.h;

            return offsetRect;            
        }
        void SetText(std::string text, std::string family) {
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(family), text.c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
        }
        void Render() override {            
            FontManager::Draw(texture, GetOffsetRect());
        }
};