# Testing and verification

Documentation changes should be checked for correct paths, command names, and code fences. Code changes should be built and tested with the intended toolchain before being described as verified.

Suggested baseline:

```sh
cmake -S . -B build
cmake --build build --config Release
forge --help
forge --version
```

These commands cover configuration, compilation, and basic CLI startup when the executable is available. They are not a comprehensive automated test suite. If tests are added, document how to run them.