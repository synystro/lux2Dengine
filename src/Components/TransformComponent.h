#pragma once

#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>
#include "../Game.h"

class TransformComponent: public Component {
    public:
        glm::vec2 position;
        glm::vec2 spawnPos;
        glm::vec2 velocity;
        glm::vec2 size;
        int scale;

        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
            position = glm::vec2(posX, posY);
            spawnPos = glm::vec2(posX, posY);
            velocity = glm::vec2(velX, velY);
            size = glm::vec2(w, h);
            scale = s;
        }

        void Init() override {

        }
        void Update(float deltaTime) override {
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
        void Render() override {
        }
};