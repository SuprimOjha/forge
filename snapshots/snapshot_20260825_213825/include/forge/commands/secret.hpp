#pragma once

namespace forge {

/**
 * @brief Manages encrypted/masked global secrets and environment variables (~/.forge/secrets.json).
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runSecret(int argc, char* argv[]);

} // namespace forge