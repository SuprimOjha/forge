# Packaging and archives

Forge provides `package`, `pack`, `archive`, and `snapshot` commands for related packaging or project-capture workflows.

```sh
forge package --help
forge pack --help
forge archive --help
forge snapshot --help
```

Before distributing an artifact, inspect it for secrets, local configuration, generated output, and files that should not be published. Verify archives in a temporary directory and confirm output paths and overwrite behavior first.