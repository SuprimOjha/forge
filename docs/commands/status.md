# Workspace status: status

The `status` command reports project or repository status. It is useful before making changes and after running a build, cleanup, or repair operation.

```sh
forge status
forge status --help
```

Use version control as the authoritative source for Git changes. Forge's status view may summarize only selected aspects of the workspace. If output appears incomplete, compare it with `git status --short` and inspect the command implementation before filing a bug.