# Maintenance commands: sweep, strip, todo

Forge lists `sweep` for finding unnecessary artifacts, `strip` for removing files, and `todo` for finding TODO/FIXME markers.

```sh
forge sweep
forge strip --help
forge todo
```

Finding files and deleting files have different risk levels. Inspect results before removal and read `strip` help before use. TODO/FIXME output is a discovery aid, not a priority or ownership decision.