# Build

**Make sure to run first!**

```sh
./tools/init_submodules.sh
```

Static:

```sh
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=prod -DBUILD_SHARED_LIBS=OFF..
cmake --build . --target game
```

Shared:
```sh
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=prod -DBUILD_SHARED_LIBS=ON ..
cmake --build . --target game
```
