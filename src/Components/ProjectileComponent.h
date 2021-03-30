#pragma once

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ProjectileComponent: public Component {
    private:
        Entity *shotOwner;
        TransformComponent *transform;
        TransformComponent *shotOwnerTransform;
        glm::vec2 spawnPos;
        int speed;
        int range;
        float angleRad;
        bool loop;
    public:
        ProjectileComponent(Entity *shotOwner, int speed, int angleDeg, int range, bool loop) {
            this->shotOwner = shotOwner;
            
            this->speed = speed;
            this->range = range;
            this->angleRad = glm::radians(static_cast<float>(angleDeg));
            this->loop = loop;
        }
        void Init() override {
            transform = owner->GetComponent<TransformComponent>();
            shotOwnerTransform = shotOwner->GetComponent<TransformComponent>();
            spawnPos = transform->position;
            transform->velocity = glm::vec2(
                glm::cos(angleRad) * speed,
                glm::sin(angleRad) * speed
            );
        }
        void Update(float deltaTime) override {
            if(glm::distance(transform->position, spawnPos) > range) {
                if(loop) {
                    transform->position = shotOwnerTransform->position;
                    spawnPos = transform->position;
                } else {
                    owner->Terminate();
                }
            }
        }
};