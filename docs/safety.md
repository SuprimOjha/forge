# Safe usage

Forge contains commands that may inspect files, run processes, apply fixes, or remove artifacts. Use a cautious workflow:

1. Start with read-only inspection.
2. Read command-specific help.
3. Check the working tree and back up important data.
4. Run destructive or process-launching operations only in trusted workspaces.
5. Review changes afterward with version control.
6. Report unexpected behavior with reproducible steps.

Forge output is not a complete security assurance. Understand a command's scope before relying on it in production.