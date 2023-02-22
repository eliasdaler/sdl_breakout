#include "LuaBindings.h"

#include <sol/sol.hpp>

#include "Globals.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace
{
/////////////////////
// Texture
/////////////////////

struct Texture {
    // _gc
    void destroy()
    {
        assert(texture);
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

std::tuple<sol::object, const char*> loadTexture(const char* path, sol::this_state s)
{
    sol::state_view lua(s);

    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        return std::make_tuple(sol::nil, SDL_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (!texture) {
        SDL_FreeSurface(loadedSurface);
        return std::make_tuple(sol::nil, SDL_GetError());
    }

    sol::object luaTexture{lua, sol::in_place_type<Texture>,
                           // ctor params
                           texture, loadedSurface->w, loadedSurface->h};
    SDL_FreeSurface(loadedSurface);

    return std::make_tuple(luaTexture, nullptr);
}

void registerTexture(sol::state& lua, sol::table& edge)
{
    // clang-format off
    auto dsu = edge.new_usertype<Texture>("Texture");
    // clang-format on
    dsu[sol::meta_function::garbage_collect] = &Texture::destroy;

    edge["loadTexture"] = loadTexture;
}

/////////////////////
// Sprite
/////////////////////

struct Sprite {
    Sprite(Texture* texture) : texture(texture)
    {
        setTextureRect(0, 0, texture->width, texture->height);
    }

    void draw(double x, double y) const
    {
        assert(texture);
        SDL_Rect renderQuad = {(int)x, (int)y, textureRect.w, textureRect.h};
        SDL_RenderCopy(gRenderer, texture->texture, &textureRect, &renderQuad);
    }

    void setTextureRect(int x, int y, int w, int h) { textureRect = {x, y, w, h}; }

    Texture* texture = nullptr;
    SDL_Rect textureRect;
};

void registerSprite(sol::state& lua, sol::table& edge)
{
    // sprite
    auto su = edge.new_usertype<Sprite>("Sprite");
    su["setTextureRect"] = &Sprite::setTextureRect;
    su["getTextureRect"] = [](Sprite& sprite) {
        auto st = sprite.textureRect;
        return std::make_tuple(st.x, st.y, st.w, st.h);
    };
    su["draw"] = &Sprite::draw;

    edge["newSprite"] = [&lua](Texture* texture) {
        return sol::object(lua, sol::in_place_type<Sprite>, texture);
    };
}

/////////////////////
// Font
/////////////////////

struct Font {
    // _gc
    void destroy()
    {
        assert(font);
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Font* font = nullptr;
};

std::tuple<sol::object, const char*> loadFont(const char* path, int size, sol::this_state s)
{
    sol::state_view lua(s);
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        return std::make_tuple(sol::nil, SDL_GetError());
    }
    return std::make_tuple(sol::object(lua, sol::in_place_type<Font>, font), nullptr);
}

void registerFont(sol::state& lua, sol::table& edge)
{
    auto fu = edge.new_usertype<Font>("Font");
    fu[sol::meta_function::garbage_collect] = &Font::destroy;

    edge["loadFont"] = loadFont;
}

/////////////////////
// DrawableString
/////////////////////

struct DrawableString {
    DrawableString(SDL_Texture* texture, int width, int height) :
        texture(texture), width(width), height(height)
    {}

    // _gc
    void destroy()
    {
        assert(texture);
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    void draw(int x, int y, int r, int g, int b) const
    {
        assert(texture);

        SDL_SetTextureColorMod(texture, r, g, b);
        SDL_Rect rect{x, y, width, height};
        SDL_RenderCopy(gRenderer, texture, NULL, &rect);
    }

    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

void registerDrawableString(sol::state& lua, sol::table& edge)
{
    // clang-format off
    auto dsu = edge.new_usertype<DrawableString>("DrawableString");
    dsu[sol::meta_function::garbage_collect] = &DrawableString::destroy;
    dsu["draw"] = &DrawableString::draw;

    edge["newDrawableString"] = [&lua](const Font& font, const char* text) {
        const SDL_Color white = {255, 255, 255, 255};

        auto surface = TTF_RenderUTF8_Solid(font.font, text, white);
        assert(surface);
        auto texture = SDL_CreateTextureFromSurface(gRenderer, surface);
        assert(texture);

        int width = surface->w;
        int height = surface->h;
        SDL_FreeSurface(surface);

        return sol::object(lua, sol::in_place_type<DrawableString>, texture, width, height);
    };
}

/////////////////////
// Sound
/////////////////////

struct Sound {
    Sound(Mix_Chunk* s) : sound(s) {}

    // _gc
    void destroy()
    {
        assert(sound);
        Mix_FreeChunk(sound);
        sound = nullptr;
    }

    void play() const {
        assert(sound);
        Mix_PlayChannel(-1, sound, 0);
    }

    Mix_Chunk* sound = nullptr;
};

std::tuple<sol::object, const char*> loadSound(const char* path, sol::this_state s)
{
    sol::state_view lua(s);
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        return std::make_tuple(sol::nil, SDL_GetError());
    }
    return std::make_tuple(sol::object(lua, sol::in_place_type<Sound>, sound), nullptr);
}

void registerSound(sol::state& lua, sol::table& edge)
{
    auto su = edge.new_usertype<Sound>("Sound");
    su[sol::meta_function::garbage_collect] = &Sound::destroy;
    su["play"] = &Sound::play;

    edge["loadSound"] = loadSound;
}

/////////////////////
// Music
/////////////////////

struct Music {
    Music(Mix_Music* m) : music(m) {}

    // _gc
    void destroy()
    {
        assert(music);
        Mix_FreeMusic(music);
        music = nullptr;
    }

    void play() const {
        assert(music);
        Mix_PlayMusic(music, -1);
    }

    void stop() const {
        assert(music);
        Mix_HaltMusic(); // TODO: halt specific channel
    }

    Mix_Music* music = nullptr;
};

std::tuple<sol::object, const char*> loadMusic(const char* path, sol::this_state s)
{
    sol::state_view lua(s);
    Mix_Music* music = Mix_LoadMUS(path);
    if (!music) {
        return std::make_tuple(sol::nil, SDL_GetError());
    }
    return std::make_tuple(sol::object(lua, sol::in_place_type<Music>, music), nullptr);
}

void registerMusic(sol::state& lua, sol::table& edge)
{
    auto mu = edge.new_usertype<Music>("Music");
    mu[sol::meta_function::garbage_collect] = &Music::destroy;
    mu["play"] = &Music::play;
    mu["stop"] = &Music::stop;

    edge["loadMusic"] = loadMusic;
}

/////////////////////
// Free functions
/////////////////////

bool isKeyPressed(int scancode)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    return currentKeyStates[scancode];
}

void drawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
    // save prev color
    Uint8 pr, pg, pb, pa;
    SDL_GetRenderDrawColor(gRenderer, &pr, &pg, &pb, &pa);

    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(gRenderer, &rect);

    SDL_SetRenderDrawColor(gRenderer, pr, pg, pb, pa); // restore prev color
}

} // end of anonymous namespace

namespace edge
{
namespace luab
{
    void registerLuaBindings(sol::state& lua)
    {
        sol::table edge = lua["edge"].force();

        // keyboard
        edge["isKeyPressed"] = isKeyPressed;
        edge["KEY_UP"] = SDL_SCANCODE_UP;
        edge["KEY_DOWN"] = SDL_SCANCODE_DOWN;
        edge["KEY_LEFT"] = SDL_SCANCODE_LEFT;
        edge["KEY_RIGHT"] = SDL_SCANCODE_RIGHT;
        edge["KEY_Z"] = SDL_SCANCODE_Z;

        // graphics
        edge["drawRect"] = drawRect;

        // usertypes
        registerTexture(lua, edge);
        registerSprite(lua, edge);
        registerFont(lua, edge);
        registerDrawableString(lua, edge);

        registerSound(lua, edge);
        registerMusic(lua, edge);
    }
} // end of namespace luab
} // end of namespace edge
