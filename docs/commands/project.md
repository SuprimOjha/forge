# Project inspection: project

The `project` command is intended to detect and analyze the current software project. It is useful as an initial inspection when entering an unfamiliar workspace.

Example:

```sh
forge project
```

Run it from the directory you want Forge to inspect. Detection results depend on files present in that directory and on the detector implementation. If the result looks incorrect, check that you selected the project root and that expected manifest or build files exist.

Use `forge project --help` to inspect supported options in your version.