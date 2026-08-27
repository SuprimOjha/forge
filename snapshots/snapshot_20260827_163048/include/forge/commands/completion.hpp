#pragma once

namespace forge {

/**
 * @brief Generates shell completion scripts for Bash, Zsh, Fish, and PowerShell.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runCompletion(int argc, char* argv[]);

} // namespace forge