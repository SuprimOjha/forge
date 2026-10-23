# Troubleshooting

## The command is not found

Confirm the executable exists and its directory is on `PATH`. Reopen the terminal after changing environment variables.

## CMake cannot find a compiler

Install a C++20-capable compiler and ensure it is visible to the terminal running CMake. On Windows, avoid mixing incompatible compiler environments.

## A source file is not compiled

Check that it is listed in `CMakeLists.txt` and rerun CMake configuration.

## A command behaves unexpectedly

Run `forge <command> --help`, capture the exact output, and compare behavior with the implementation.

## Reporting an issue

Include OS, compiler, CMake version, Forge version or commit, command, and a minimal reproduction. Redact secrets and private paths.