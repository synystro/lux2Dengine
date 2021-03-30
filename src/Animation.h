#pragma once

class Animation {
    private:
    public:
        unsigned int index;
        unsigned int framesCount;
        unsigned int speed;
        Animation();
        Animation(unsigned int index, unsigned int framesCount, unsigned int speed);
};