#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"
#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"

class AssetManager;

class Game {
    private:
        static bool isLevelComplete;
        SDL_Window *window;
    public:
        Game();
        ~Game();        
        int ticksLastFrame = 0;
        bool IsRunning() const; 
        void GameWon();
        void GameOver();
        void RespawnPlayer();
        static bool isRespawning;
        static bool isRunning;       
        static bool debugMode;
        static SDL_Renderer *renderer;
        static AssetManager* assetManager;
        static SDL_Event sdlEvent;
        static SDL_Rect camera;
        void LoadLevel(int levelID);        
        void Init(int width, int height);
        void SetPlayer();
        void ProcessInput();
        void Update();
        void Render();
        void Terminate();
        void HandleCameraMovement();
        void CheckCollisions();        
};