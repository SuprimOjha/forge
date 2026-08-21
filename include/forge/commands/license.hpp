#pragma once

namespace forge {

/**
 * @brief Scans project workspace and dependencies for license compliance and risk levels.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runLicense(int argc, char* argv[]);

} // namespace forge