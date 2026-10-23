# Configuration guidance

Forge exposes configuration through its `config` command and includes a core configuration implementation. Confirm the configuration path, supported keys, and precedence rules from current source or command help.

Good practices:
- Keep shared defaults portable.
- Never store secrets in tracked configuration.
- Back up settings before changing them.
- Document new keys where they are defined.
- Handle missing or malformed configuration gracefully.

When reporting a configuration bug, include relevant redacted settings and reproducible steps.