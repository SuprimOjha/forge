# Building Forge from source

This guide covers the CMake build for the Forge CLI. It reflects the current top-level `CMakeLists.txt`; command availability can vary as the project evolves.

## Prerequisites

- CMake 3.20 or newer
- A C++ compiler with C++20 support
- Git, if you are cloning the repository

Check your toolchain versions before building:

```bash
cmake --version
```

On Linux, GCC or Clang can be used if the installed version supports the C++20 features used by Forge. On Windows, use a recent Visual Studio C++ toolchain or another supported C++20 compiler.

## Get the source

```bash
git clone https://github.com/SuprimOjha/forge.git
cd forge
```

If you already have a local checkout, update it before starting a clean build.

## Configure and build

Use a separate build directory so generated files stay out of the source tree:

```bash
cmake -S . -B build
cmake --build build --config Release
```

The first command configures the project and detects the available compiler. The second builds the `forge` executable. If configuration fails, read the first CMake error and verify that CMake and your C++ compiler are installed and discoverable.

## Run the executable

The executable path depends on the generator:

- **Single-configuration generators** (commonly Makefiles or Ninja): `build/forge` on Linux/macOS, or `build/forge.exe` on Windows.
- **Visual Studio and other multi-configuration generators:** commonly `build/Release/forge.exe` on Windows when building the Release configuration.

Run the help command to verify that the executable starts:

```bash
./build/forge --help
```

For a Visual Studio build, use the executable under `build/Release` instead. You can also run the `doctor` command to inspect the local development environment:

```bash
./build/forge doctor
```

## Clean rebuild

Delete only the generated build directory, then configure and build again:

```bash
cmake -S . -B build
cmake --build build --config Release
```

If you need a fully fresh configuration, remove the `build` directory first. Do not delete source directories or project configuration files as a cleanup shortcut.

## Troubleshooting

### CMake cannot find a compiler

Install a C++20-capable compiler and ensure it is available in your terminal's environment. On Windows, open a Developer Command Prompt if your Visual Studio compiler is not on the regular PATH.

### The compiler is too old

Upgrade the compiler to a version that supports the C++20 features used by the codebase, then remove the old build directory and configure again.

### The command is not found after building

Run Forge using the path produced by your selected CMake generator. Installing or adding Forge to `PATH` is a separate step; a successful build does not automatically install the executable system-wide.

## Reporting build issues

When opening an issue, include your operating system, CMake version, compiler and version, the exact configure/build command, and the first relevant error message. Remove personal paths, access tokens, and other sensitive information from logs before posting them publicly.
