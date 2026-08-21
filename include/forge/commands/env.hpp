#pragma once

namespace forge {

/**
 * @brief Validates environment variables and scans configuration files for missing variables or exposed secrets.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runEnv(int argc, char* argv[]);

} // namespace forge