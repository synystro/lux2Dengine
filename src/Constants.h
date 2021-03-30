#pragma once

#include <SDL2/SDL.h>

/* game window */

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

/* layers */
enum class LayerType {
    TILE = 0,
    VEGETATION = 1,
    ENEMY = 2,
    PLAYER = 3,
    PROJECTILE = 4,
    UI = 5
};

/* heights (z axis) */
enum class Height {
    GROUND = 0,
    FLYING = 1,
    FREEPATHING = 2
};

/* entity types */
enum class EntityType {
    GROUND = 0,
    FLYING = 1,
    UI = 3
};

const unsigned int NUM_LAYERS = 6;

/* colors */

const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color COLOR_RED = {255, 255, 0, 0};
const SDL_Color COLOR_GREEN = {0, 255, 0, 255};
const SDL_Color COLOR_BLUE = {0, 0, 255, 255};

/* text ids */

const std::string WIN_TEXT_ID = "Win";
const std::string LOSS_TEXT_ID = "Loss";
const std::string TUTORIAL_FLY_TEXT_ID = "FlyTutorial";
const std::string TUTORIAL_MOVE_TEXT_ID = "MoveTutorial";
