#pragma once

namespace forge {

/**
 * @brief Executes dev commands with optional live file watching and auto-restarting.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runRun(int argc, char* argv[]);

} // namespace forge