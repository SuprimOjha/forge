# Core services

Forge's core implementation includes project detection, path utilities, configuration, process execution, and a fix engine. These services support command handlers and reduce the need to duplicate shared logic.

Changes to a core service can affect several commands. Before modifying one, search its callers and preserve existing assumptions about paths, exit codes, process failures, and missing tools. Keep error messages actionable and avoid silently converting failures into success.

The precise contracts should be confirmed by reading the current declarations and implementations in `include/forge/` and `src/core/`.