# Command dispatch

The main entry point reads the first command-line argument and routes recognized command names to their corresponding handlers. Global help is handled separately, and the dispatcher includes dedicated handling for some commands such as `doctor` and `build`.

When adding a command:

1. Define a focused header and implementation when consistent with existing patterns.
2. Include the command header in `src/main.cpp`.
3. Register the command in the dispatcher and help output.
4. Add the implementation source to `CMakeLists.txt`.
5. Test normal use, help, invalid input, and error paths.

Avoid documenting options until the implementation actually supports them.