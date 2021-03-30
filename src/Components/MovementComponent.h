#pragma once

#include "../Component.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"

class MovementComponent : public Component {
    private:
        unsigned int speed = 0;
        TransformComponent *transform;
        SpriteComponent *sprite;
        int currentDir;
        bool mLeft = false;
        bool mRight = false;
        bool mUp = false;
        bool mDown = false;
        bool isFlying = false;
    public:
        MovementComponent(unsigned int movementSpeed) {
            speed = movementSpeed;            
        }
        unsigned int Speed() { return speed; }
        enum Direction { LEFT, RIGHT, UP, DOWN };

        void Init() override {
            if(!owner->HasComponent<TransformComponent>()) {
                std::cerr << "Tried to add MovementComponent to " << owner->name << " without adding a TransformComponent first.";
                return;
            }

            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
        }
        
        void ToggleFlight() {
            isFlying = !isFlying;
            sprite->ToggleAnimation();
            if(!isFlying) {
                owner->SetHeight(Height::GROUND);
                Stop(static_cast<Direction>(currentDir));                
            } else {
                owner->SetHeight(Height::FLYING);
            }
        }

        void Move(Direction direction) {
            if(owner->type == EntityType::FLYING && !isFlying)
                return;

            switch(direction) {
                case LEFT:
                    transform->velocity.x = static_cast<int>(speed * -1);
                    mLeft = true;
                    mRight = false;
                    break;
                case RIGHT:
                    transform->velocity.x = static_cast<int>(speed); 
                    mRight = true;
                    mLeft = false;
                    break;
                case UP:
                    transform->velocity.y = static_cast<int>(speed * -1);
                    mUp = true;
                    mDown = false;
                    break;
                case DOWN:
                    transform->velocity.y = static_cast<int>(speed);
                    mDown = true;
                    mUp = false;
                    break;
            }
            if(sprite->IsAnimated())
                UpdateAnimation();
        }

        void UpdateAnimation() {
            if(mRight) {
                sprite->Play("RightAnimation");
                currentDir = RIGHT;
            } else if(mLeft) {
                sprite->Play("LeftAnimation");
                currentDir = LEFT;
            } else if(mUp) {
                sprite->Play("UpAnimation");
                currentDir = UP;
            } else if(mDown) {
                sprite->Play("DownAnimation");
                currentDir = DOWN;
            }
        }

        void Stop(Direction direction) {
            switch(direction) {
                case LEFT:
                    if(mRight)
                        Move(RIGHT);
                    else if(transform->velocity.x < 0)
                        transform->velocity.x = 0;
                    mLeft = false;
                    break;
                case RIGHT:
                    if(mLeft)
                        Move(LEFT);
                    else if(transform->velocity.x > 0)
                        transform->velocity.x = 0;
                    mRight = false;
                    break;
                case UP:
                    if(mDown)
                        Move(DOWN);
                    else if(transform->velocity.y < 0)
                        transform->velocity.y = 0;
                    mUp = false;
                    break;
                case DOWN:
                    if(mUp)
                        Move(UP);
                    else if(transform->velocity.y > 0)
                        transform->velocity.y = 0;
                    mDown = false;
                    break;
                default:
                    break;
            }
            if(sprite->IsAnimated())
                UpdateAnimation();
        }

        void Reset() {
            transform->velocity = { 0, 0};
            
            mLeft = false;
            mRight = false;
            mUp = false;
            mDown = false;

            if(sprite->IsAnimated())
                sprite->Play("DownAnimation");
        }
};