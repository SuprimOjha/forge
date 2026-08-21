#pragma once

namespace forge {

/**
 * @brief Inspects line and file diffs against recent commits or snapshot states.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runDiff(int argc, char* argv[]);

} // namespace forge