# Build from source

Forge uses CMake and requires a C++20-capable compiler. The root `CMakeLists.txt` declares CMake 3.20 as the minimum version and enables standard warning flags.

## Configure and build

```sh
cmake -S . -B build
cmake --build build --config Release
```

On Windows with MinGW, a generator can be selected explicitly:

```sh
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

If configuration fails, check that CMake and the compiler are installed and visible in the same terminal. Build commands may differ depending on the generator and compiler toolchain.