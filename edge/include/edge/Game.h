#pragma once

#include <string>

#include <sol/sol.hpp>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace edge
{
struct GameParams {
    void validate();

    int screenWidth = 256;
    int screenHeight = 224;
    int screenScale = 3;

    std::string windowTitle = "Game";
};

class Game {
public:
    void start(GameParams params);

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

    GameParams params;
};
}
