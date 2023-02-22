#include <edge/Game.h>

#include <SDL.h>

int main(int argc, char* argv[])
{
    edge::GameParams params{
        .screenWidth = 256,
        .screenHeight = 240,
        .screenScale = 3,
        .windowTitle = "SDL Breakout",
    };

    edge::Game game;
    game.start(params);
    return 0;
}
