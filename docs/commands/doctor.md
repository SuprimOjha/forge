# Environment diagnostics: doctor

The `doctor` command checks aspects of the development environment and installed tools. It is a useful first step when a build or project workflow fails.

```sh
forge doctor
forge doctor --help
```

The current dispatcher provides dedicated help for this command and rejects an unrecognized option. The specific checks and diagnostic messages depend on the implementation and host environment.

Treat a diagnostic as evidence to investigate, not an automatic guarantee that a project is healthy. Record the command output when reporting an issue.