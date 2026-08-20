#pragma once

namespace forge {

/**
 * @brief Measures and benchmarks command execution time and return status.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runBench(int argc, char* argv[]);

} // namespace forge