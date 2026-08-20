#pragma once

namespace forge {

/**
 * @brief Parses git history and generates release changelogs and version notes.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runRelease(int argc, char* argv[]);

} // namespace forge