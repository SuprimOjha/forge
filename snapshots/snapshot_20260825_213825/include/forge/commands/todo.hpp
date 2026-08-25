#pragma once

namespace forge {

/**
 * @brief Scans source files across the workspace for TODO, FIXME, and HACK technical debt tags.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runTodo(int argc, char* argv[]);

} // namespace forge