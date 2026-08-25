#pragma once

namespace forge {

/**
 * @brief Sends HTTP requests and analyzes server responses and latency.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runRequest(int argc, char* argv[]);

} // namespace forge