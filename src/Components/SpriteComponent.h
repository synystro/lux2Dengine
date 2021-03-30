#pragma once

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect sourceRect;
        SDL_Rect targetRect;
        bool isAnimated;
        bool isAnimationOn;
        int framesCount;
        int animationSpeed;
        bool isFixed;
        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
        unsigned int animationIndex = 0;
    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
        bool IsAnimated() { return isAnimated; }

        SpriteComponent(std::string assetTextureID) {
            isAnimated = false;
            isAnimationOn = false;
            isFixed = false;
            SetTexture(assetTextureID);
        }
        SpriteComponent(std::string assetTextureID, int framesCount, int speed, bool hasDirections, bool isFixed) {
            isAnimated = true;
            isAnimationOn = true;
            this->framesCount = framesCount;
            this->animationSpeed = speed;
            this->isFixed = isFixed;

            if(hasDirections) {
                Animation downAnimation = Animation(0, framesCount, animationSpeed);
                Animation rightAnimation = Animation(1, framesCount, animationSpeed);
                Animation leftAnimation = Animation(2, framesCount, animationSpeed);
                Animation upAnimation = Animation(3, framesCount, animationSpeed);

                animations.emplace("DownAnimation", downAnimation);
                animations.emplace("RightAnimation", rightAnimation);
                animations.emplace("LeftAnimation", leftAnimation);
                animations.emplace("UpAnimation", upAnimation);

                this->animationIndex = 0;
                this->currentAnimationName = "DownAnimation";
            } else {
                Animation noDirAnimation = Animation(0, framesCount, speed);
                animations.emplace("NoDirAnimation", noDirAnimation);
                this->animationIndex =0;
                this->currentAnimationName = "NoDirAnimation";
            }

            Play(this->currentAnimationName);

            SetTexture(assetTextureID);
        }
        void Play(std::string animationName) {
            framesCount = animations[animationName].framesCount;
            animationIndex = animations[animationName].index;
            animationSpeed = animations[animationName].speed;
            currentAnimationName = animationName;
        }
        void ToggleAnimation() {
            if(isAnimated) {
                isAnimationOn = !isAnimationOn;
            } else {
                std::cerr << "Tried to toggle animation of non-animated sprite for entity: " << owner->name << "\n";
            }
        }
        void SetTexture(std::string assetTextureID) {
            texture = Game::assetManager->GetTexture(assetTextureID);
        }
        void Init() override {
            transform = owner->GetComponent<TransformComponent>();
            sourceRect.x = 0;
            sourceRect.y = 0;
            sourceRect.w = transform->size.x;
            sourceRect.h = transform->size.y;
        }
        void Update(float deltaTime) override {
            if(isAnimationOn) {
                sourceRect.x = sourceRect.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % framesCount);
            }
            sourceRect.y = animationIndex * transform->size.y;

            targetRect.x = static_cast<int>(transform->position.x - (isFixed ? 0 : Game::camera.x));
            targetRect.y = static_cast<int>(transform->position.y - (isFixed ? 0 : Game::camera.y));
            targetRect.w = transform->size.x * transform->scale;
            targetRect.h = transform->size.y * transform->scale;
        }
        void Render() override {
            TextureManager::Draw(texture, sourceRect, targetRect, spriteFlip);
        }
};