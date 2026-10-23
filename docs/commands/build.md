# Build projects: build

The `build` command is described in the dispatcher as an automatic build workflow, with help text mentioning CMake-based C++ and Node.js projects.

```sh
forge build
forge build --help
```

Run builds from the intended project root and read the output carefully. Builds can execute project-defined tools, compiler processes, and scripts, so only run them in workspaces you trust. If a build fails, compare Forge's output with the project's native build instructions to isolate whether the issue is in the project, toolchain, or wrapper command.