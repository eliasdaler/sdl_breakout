#include <edge/Game.h>

int main()
{
    edge::GameParams params{
        .screenWidth = 640,
        .screenHeight = 480,
        .screenScale = 1,
        .windowTitle = "Minimal",
    };

    edge::Game game;
    game.start(params);
}
