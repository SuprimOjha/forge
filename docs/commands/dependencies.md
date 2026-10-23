# Dependency analysis: deps and graph

Forge includes `deps` for dependency and coupling analysis, `graph` for dependency relationships, and `build-graph` for build dependencies.

```sh
forge deps
forge graph
forge build-graph
```

These views can help orient a developer in an unfamiliar codebase. Their results depend on what the implementation can detect. Do not assume they resolve every language, package manager, generated dependency, or dynamic relationship. Use each command's help to learn supported arguments.