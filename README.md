This is a small breakout game that I made to test a new approach of separating Lua and C++ code when working with SDL.

The entire game logic is written in Lua.

The small "engine" is written in C++ with SDL.

![image](https://i.ibb.co/1LdJktH/gameplay-breakout.gif)

This is a port of my old test game, [Ebiten Breakout](https://github.com/eliasdaler/ebiten_breakout).

It's in public domain, so do whatever you want with it.

The process of its short development was documented here: https://github.com/eliasdaler/edw.is/discussions/4

I'll be continuing doing stuff with this new "engine", I'll probably release it's iterative versions some day!

--

## Building

Stuff you'll need:

* A C++20 compiler (but you can also downgrade to C++17, which sol2 still uses)
* git
* CMake 3.18+

```sh
git submodule update --init --recursive
mkdir build
cd build
cmake .. # add -DBUILD_SHARED_LIBS=OFF to link statically
cmake --build . --target breakout
./games/breakout/breakout
```

---

Font used:  "Press Start 2P", Designed by CodeMan38 (licensed under the Open Font License)
