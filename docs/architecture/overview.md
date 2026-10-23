# Architecture overview

Forge is a C++ command-line executable organized into command handlers, core services, and output helpers. The root CMake target compiles the entry point at `src/main.cpp`, command implementations under `src/commands/`, core implementations under `src/core/`, and UI output code under `src/ui/`.

The `include/forge/` tree contains public project headers used by the implementation. This separation gives individual commands a place for workflow-specific logic while shared services can be reused.

When changing architecture, keep command dispatch, shared behavior, and user-facing output understandable. Confirm source/header relationships in CMake whenever adding a new compilation unit.