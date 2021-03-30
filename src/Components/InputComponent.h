#pragma once

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"
#include "./MovementComponent.h"
#include "../Controls.h"

extern InputManager inputManager;

class InputComponent : public Component {
    private:
    TransformComponent *transform;
    SpriteComponent *sprite;
    MovementComponent *movement;
    public: 
        void Init() override {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
            movement = owner->GetComponent<MovementComponent>();

            if(!movement)
                std::cout << "input component added before required component (movement)!";
        }
        void Update(float deltaTime) override {
            if(movement) {
                HandleMovement();
            }
        }
        void HandleMovement() {
            MovementComponent* movement = owner->GetComponent<MovementComponent>();            
            switch(Game::sdlEvent.type) {                
                case SDL_KEYDOWN:
                    MovementComponent::Direction direction;
                    switch(Game::sdlEvent.key.keysym.sym) {
                        case UP:
                            direction = MovementComponent::Direction::UP;
                            break;
                        case LEFT:
                            direction = MovementComponent::Direction::LEFT;
                            break;
                        case DOWN:
                            direction = MovementComponent::Direction::DOWN;
                            break;
                        case RIGHT:
                            direction = MovementComponent::Direction::RIGHT;
                            break;
                        case TOGGLE_FLIGHT:
                            if(owner->type == EntityType::FLYING) {
                                if(!inputManager.CheckKeyPressed(TOGGLE_FLIGHT)) {
                                    movement->ToggleFlight();
                                }
                            }                            
                            
                            break;
                        default:
                            break;
                    }
                    movement->Move(direction);
                    break;
                case SDL_KEYUP:
                    switch(Game::sdlEvent.key.keysym.sym) {
                        case UP:
                            direction = MovementComponent::Direction::UP;
                            break;
                        case LEFT:
                            direction = MovementComponent::Direction::LEFT;
                            break;
                        case DOWN:
                            direction = MovementComponent::Direction::DOWN;
                            break;
                        case RIGHT:
                            direction = MovementComponent::Direction::RIGHT;
                            break;
                        case TOGGLE_FLIGHT:
                            inputManager.SetKeyReleased(TOGGLE_FLIGHT);
                            break;
                        default:
                            break;
                    }
                    movement->Stop(direction);
                    break;
                default:
                    break;
            }
        }
};