#pragma once

namespace forge {

/**
 * @brief Checks local or remote TCP port status, availability, and connectivity.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runPort(int argc, char* argv[]);

} // namespace forge