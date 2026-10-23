# Configuration audit: audit

The `audit` command is described as auditing project configuration. Use it as one input when reviewing a repository's setup and maintenance needs.

```sh
forge audit
forge audit --help
```

An audit can only identify issues covered by its implemented checks. A clean result is not a guarantee that a project is secure or correctly configured. Review findings, reproduce important issues, and report false positives with a minimal example and the Forge version.