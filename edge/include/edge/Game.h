#pragma once

#include <sol/sol.hpp>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace edge
{
class Game {
public:
    void start();

private:
    void init();
    void loop();
    void render();
    void cleanup();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Texture* screenTexture = nullptr;

    sol::state lua;
    sol::table luaGame;

    // TODO: put into a JSON file
    int screenWidth = 256;
    int screenHeight = 240;
    int screenScale = 3;
};
}
