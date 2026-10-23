# Build system

The project is configured through the root `CMakeLists.txt`. It declares project version 0.1.0, requires CMake 3.20, uses C++20 without compiler extensions, and builds an executable named `forge`.

Compiler warnings are enabled using `/W4` on MSVC and `-Wall -Wextra -Wpedantic` on other supported compiler configurations. Windows builds link `ws2_32`.

When adding source files or platform-specific dependencies, update CMake and verify the build with the intended compiler. A successful build on one toolchain does not guarantee that every platform is supported.