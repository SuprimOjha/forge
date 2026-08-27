#pragma once

namespace forge {

/**
 * @brief Manages global CLI user settings.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runConfig(int argc, char* argv[]);

} // namespace forge