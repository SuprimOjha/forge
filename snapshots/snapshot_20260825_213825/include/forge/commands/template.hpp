#pragma once

namespace forge {

/**
 * @brief Manages project scaffolding templates and custom template generation (~/.forge/templates/).
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runTemplate(int argc, char* argv[]);

} // namespace forge