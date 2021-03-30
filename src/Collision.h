#pragma once

#include <SDL2/SDL.h>
#include <string>

class ColliderComponent;

class Collision {
    public:
        static bool CheckRectCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
        static bool CheckTagCollision(ColliderComponent *colA, ColliderComponent *colB, std::string tagA, std::string tagB);
};