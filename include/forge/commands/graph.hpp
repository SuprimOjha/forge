#pragma once

namespace forge {

/**
 * @brief Analyzes and renders dependency graphs in ASCII or DOT format.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runGraph(int argc, char* argv[]);

} // namespace forge