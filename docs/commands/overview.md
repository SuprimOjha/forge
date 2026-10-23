# Command reference overview

Forge exposes commands for several workflow areas:

- Workspace: `project`, `init`, `status`, `tree`
- Diagnostics: `doctor`, `check`, `env`, `env-check`, `health`
- Metrics and review: `score`, `metrics`, `stats`, `audit`
- Analysis: `deps`, `graph`, `build-graph`, `diff`, `arch`, `profile`
- Maintenance: `secret`, `license`, `sweep`, `strip`, `todo`
- Packaging: `package`, `pack`, `archive`, `snapshot`, `release`, `release-notes`
- Utilities: `search`, `cheat`, `dashboard`, `bench`, and more

This list is based on the command dispatcher. Discover current flags with `forge <command> --help`; do not assume every command accepts the same options.