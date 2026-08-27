#pragma once

namespace forge {

/**
 * @brief Scans local service ports or checks remote target socket availability.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runPort(int argc, char* argv[]);

} // namespace forge