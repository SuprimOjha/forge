#pragma once

namespace forge {

/**
 * @brief Scans workspace source files for TODO, FIXME, HACK, and BUG comments.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runTodo(int argc, char* argv[]);

} // namespace forge