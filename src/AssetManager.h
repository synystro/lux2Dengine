#pragma once

#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include "./TextureManager.h"
#include "./FontManager.h"
#include "./EntityManager.h"

class AssetManager {
    private:
        EntityManager* entityManager;
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
    public:
        AssetManager(EntityManager* entityManager);
        ~AssetManager();
        void ClearData();
        void AddTexture(std::string textureID, const char* filePath);
        void AddFont(std::string fontID, const char* filePath, int fontSize);
        SDL_Texture* GetTexture(std::string textureID);
        TTF_Font* GetFont(std::string fontID);
};