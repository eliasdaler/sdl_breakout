#include "edge/Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <filesystem>

#include "Globals.h"
#include "LuaBindings.h"
#include "LuaUtil.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace
{
std::filesystem::path getExecutableDir()
{
#ifdef _WIN32
    std::wstring buf;
    buf.resize(MAX_PATH);
    do {
        unsigned int len = GetModuleFileNameW(NULL, &buf[0], static_cast<unsigned int>(buf.size()));
        if (len < buf.size()) {
            buf.resize(len);
            break;
        }

        buf.resize(buf.size() * 2);
    } while (buf.size() < 65536);

    return std::filesystem::path(buf).parent_path();
#else
    if (std::filesystem::exists("/proc/self/exe")) {
        return std::filesystem::read_symlink("/proc/self/exe").parent_path();
    }
    return std::filesystem::path();
#endif
}

SDL_Texture* createBlankTexture(SDL_Renderer* renderer, int width, int height)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        printf("Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }
    return texture;
}

}

namespace edge
{
void GameParams::validate()
{
    assert(screenWidth > 0);
    assert(screenHeight > 0);
    assert(screenScale > 0);
}

void Game::start(GameParams params)
{
    params.validate();
    this->params = params;

    init();
    loop();
    cleanup();
}

void Game::init()
{
    std::filesystem::current_path(getExecutableDir());

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        std::exit(1);
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        std::exit(1);
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }

    window = SDL_CreateWindow(params.windowTitle.c_str(),
                              // pos
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              // size
                              params.screenWidth * params.screenScale,
                              params.screenHeight * params.screenScale, 0);

    if (!window) {
        printf("Couldn't create window! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    gRenderer = renderer;

    screenTexture = createBlankTexture(renderer, params.screenWidth, params.screenHeight);

    // bg color
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, SDL_ALPHA_OPAQUE);

    { // lua setup
        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math,
                           sol::lib::string);
        // set up package path
        const auto cwd = std::filesystem::current_path();
        std::string ppath = cwd.string() + "/scripts/?.lua;";
        ppath += cwd.string() + "/edge/?.lua;";
        lua["package"]["path"] = ppath;

        luab::registerLuaBindings(lua);
    }

    // everything ready, start game
    {
        sol::protected_function_result result =
            lua.safe_script_file("scripts/game.lua", &sol::script_pass_on_error);
        if (result.valid()) {
            luaGame = result;
        } else {
            sol::error err = result;
            std::cout << err.what() << std::endl;
            cleanup();
            std::exit(1);
        }
    }
}

void Game::loop()
{
    // Fix your timestep! game loop
    const float dt = 1 / 60.f; // 1 / FPS
    float accumulator = dt; // so that we get at least 1 update before render
    float frame_time = 0.f;

    uint32_t current_time = SDL_GetTicks();

    bool is_running = true;
    while (is_running) {
        uint32_t new_time = SDL_GetTicks();
        frame_time = (new_time - current_time) / 1000.f; // time in SDL is stored in ms
        current_time = new_time;
        accumulator += frame_time;

        while (accumulator >= dt) {
            { // event processing
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        is_running = false;
                    }
                }
            }

            // update
            luautil::safeSelfCall(luaGame, "update", dt);

            accumulator -= dt;
        }

        render();
    }
}

void Game::render()
{
    SDL_SetRenderTarget(renderer, screenTexture);

    // render to virtual screen
    SDL_RenderClear(renderer);
    luautil::safeSelfCall(luaGame, "draw");
    SDL_SetRenderTarget(renderer, nullptr);

    // render to screen to window
    SDL_Rect src = {0, 0, params.screenWidth, params.screenHeight};
    SDL_Rect dst = {0, 0, params.screenWidth * params.screenScale,
                    params.screenHeight * params.screenScale};
    SDL_RenderCopy(renderer, screenTexture, &src, &dst);

    SDL_RenderPresent(renderer);
}

void Game::cleanup()
{
    // shut down Lua
    luaGame = sol::nil;
    lua = sol::state{}; // hack - this will kill the state and do a garbage collection

    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

} // end of namespace edge
