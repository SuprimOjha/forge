# Suggested repairs: fix

The `fix` command is intended to detect problems and offer supported fixes. Automated repairs can change files or invoke external tools, so inspect the proposed action before proceeding.

```sh
forge fix
forge fix --help
```

Prefer small, reversible changes and keep a clean version-control checkpoint before repair operations. After applying a fix, rerun the original diagnostic or build to verify the result. A command reporting that a fix was attempted is not, by itself, proof that the underlying issue is resolved.