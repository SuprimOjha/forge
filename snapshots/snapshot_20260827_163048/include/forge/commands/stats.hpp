#pragma once

namespace forge {

/**
 * @brief Analyzes workspace metrics: line count, code vs comments vs blanks, and file sizes.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runStats(int argc, char* argv[]);

} // namespace forge