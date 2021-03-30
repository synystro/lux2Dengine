#pragma once

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../../lib/glm/glm.hpp"

class TileComponent : public Component {
    public:
        SDL_Texture *texture;
        SDL_Rect sourceRect;
        SDL_Rect targetRect;
        glm::vec2 position;

        TileComponent(int sourceRectX, int sourceRectY, int posX, int posY, int size, int scale, std::string assetTextureID) {
            texture = Game::assetManager->GetTexture(assetTextureID);

            sourceRect.x = sourceRectX;
            sourceRect.y = sourceRectY;
            sourceRect.w = size;
            sourceRect.h = size;

            targetRect.x = posX;
            targetRect.y = posY;
            targetRect.w = size * scale;
            targetRect.h = size * scale;

            position.x = posX;
            position.y = posY;
        }
        ~TileComponent() {
            SDL_DestroyTexture(texture);
        }
        void Update(float deltaTime) override {
            targetRect.x = position.x - Game::camera.x;
            targetRect.y = position.y - Game::camera.y;
        }
        void Render() override {
            TextureManager::Draw(texture, sourceRect, targetRect, SDL_FLIP_NONE);
        }
};