#include <iostream>
#include "./Constants.h"
#include "./Game.h"

int main(int argc, char *args[]) {
    Game game;

    game.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

    while(game.IsRunning()) {
        game.ProcessInput();
        game.Update();
        game.Render();
    }

    game.Terminate();

    return 0;
}