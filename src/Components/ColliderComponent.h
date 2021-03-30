#pragma once

#include <SDL2/SDL.h>
#include "../Game.h"
#include "./TransformComponent.h"
#include "../Collision.h"

#include <iostream>

extern EntityManager entityManager;
class Game;

class ColliderComponent : public Component {
    private:
        void UpdatePosition() {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->size.x * transform->scale;
            collider.h = transform->size.y * transform->scale;

            targetRect.x = collider.x - Game::camera.x;
            targetRect.y = collider.y - Game::camera.y;
        }
    public:        
        SDL_Rect collider;
        SDL_Rect sourceRect;
        SDL_Rect targetRect;
        TransformComponent* transform;
        std::string tag;

        ColliderComponent(std::string tag, int x, int y, int w, int h) {
            this->tag = tag;
            this->collider = {x, y, w, h};            
        }                

        void Init() override {
            if(!owner->HasComponent<TransformComponent>()) {
                std::cerr << "Tried to add ColliderComponent to " << owner->name << " without adding a TransformComponent first.";
                return;
            }

            entityManager.AddCollider(this);  

            transform = owner->GetComponent<TransformComponent>();
            sourceRect = {0,0, transform->size.x, transform->size.y};
            targetRect = {collider.x, collider.y, collider.w, collider.h};                   
        }
        void Update(float deltaTime) override {
            UpdatePosition();
        }
        void Render() {
            if(Game::debugMode) {
                SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(Game::renderer, &targetRect);
            }
        }
        void OnCollision(ColliderComponent *otherCollider) {
            // if player collides with a projectile
            if(Collision::CheckTagCollision(this, otherCollider, "PLAYER", "PROJECTILE")) {                
                if(this->owner->GetHeight() == otherCollider->owner->GetHeight()) {
                    Game game;
                    game.GameOver();
                }
                
            }
            if(Collision::CheckTagCollision(this, otherCollider, "PLAYER", "OBJECTIVE")) {
                if(this->owner->GetHeight() == otherCollider->owner->GetHeight()) {
                    Game game;
                    game.GameWon();
                }
            }            
        }
};