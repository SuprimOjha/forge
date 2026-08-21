#pragma once

namespace forge {

/**
 * @brief Manages custom command shortcuts and aliases (~/.forge/aliases.json).
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runAlias(int argc, char* argv[]);

} // namespace forge