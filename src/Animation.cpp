#include "./Animation.h"

Animation::Animation() {

}

Animation::Animation(unsigned int index, unsigned framesCount, unsigned int animationSpeed) {
    this->index = index;
    this->framesCount = framesCount;
    this->speed = animationSpeed;
}