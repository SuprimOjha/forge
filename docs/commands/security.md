# Security inspection: secret

The `secret` command is intended to detect potential secrets in a project.

```sh
forge secret
forge secret --help
```

Detection cannot prove that a repository contains no credentials. Never commit API keys, passwords, or private tokens. If a real secret is exposed, revoke or rotate it immediately; deleting it from the latest commit does not invalidate it or remove it from history. Review findings privately before sharing logs.