#pragma once

namespace forge {

/**
 * @brief Scans workspace for heavy build artifacts/caches and offers cleanup.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runSweep(int argc, char* argv[]);

} // namespace forge