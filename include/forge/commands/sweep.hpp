#pragma once

namespace forge {

/**
 * @brief Scans and sweeps workspace junk files, caches, logs, and temp artifacts.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runSweep(int argc, char* argv[]);

} // namespace forge