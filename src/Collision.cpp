#include "./Collision.h"
#include "./Components/ColliderComponent.h"

bool Collision::CheckRectCollision(const SDL_Rect& rectA, const SDL_Rect& rectB) {
    return (
        rectA.x + rectA.w >= rectB.x &&
        rectB.x + rectB.w >= rectA.x &&
        rectA.y + rectA.h >= rectB.y &&
        rectB.y + rectB.h >= rectA.y
    );
}

bool Collision::CheckTagCollision(ColliderComponent *colA, ColliderComponent *colB, std::string tagA, std::string tagB) {
    if
    ((colA->tag == tagA && colB->tag == tagB)
    ||
    (colA->tag == tagB && colB->tag == tagA)) {
        return true;
    }
    return false;
}