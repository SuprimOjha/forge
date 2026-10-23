# Clean generated artifacts: clean

The `clean` command is intended to remove project build artifacts. Cleaning can delete generated files and may be destructive if a project stores important data in a build-like directory.

Before using it:

1. Run `forge clean --help`.
2. Inspect the target project and any configured output directories.
3. Commit or back up important changes.
4. Review the command output and verify that only generated files were removed.

Do not run cleanup commands against unfamiliar directories until their behavior and target paths are understood.