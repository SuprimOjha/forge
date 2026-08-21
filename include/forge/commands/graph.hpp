#pragma once

namespace forge {

/**
 * @brief Generates an include dependency graph across C++ header and source files.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runGraph(int argc, char* argv[]);

} // namespace forge