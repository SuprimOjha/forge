#pragma once

namespace forge {

/**
 * @brief Analyzes open ports and tests TCP socket service health.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runPort(int argc, char* argv[]);

} // namespace forge