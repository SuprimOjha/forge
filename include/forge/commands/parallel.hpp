#pragma once

namespace forge {

/**
 * @brief Executes multiple independent shell commands or build targets concurrently across a thread pool.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runParallel(int argc, char* argv[]);

} // namespace forge