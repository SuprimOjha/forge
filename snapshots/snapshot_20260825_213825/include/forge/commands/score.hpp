#pragma once

namespace forge {

/**
 * @brief Evaluates project workspace health, readiness, docs, and debt into a 0-100 score.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runScore(int argc, char* argv[]);

} // namespace forge