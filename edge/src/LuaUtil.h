#pragma once

#include <sol/sol.hpp>

namespace luautil
{
template<typename... T>
void safeCall(sol::state& lua, const char* name, T&&... args)
{
    sol::protected_function f = lua[name];
    sol::protected_function_result result = f(std::forward<T>(args)...);
    if (!result.valid()) {
        sol::error err = result;
        std::cout << err.what();
    }
}

template<typename... T>
void safeSelfCall(sol::table& t, const char* name, T&&... args)
{
    sol::protected_function f = t[name];
    sol::protected_function_result result = f(t, std::forward<T>(args)...);
    if (!result.valid()) {
        sol::error err = result;
        std::cout << err.what();
    }
}
}
