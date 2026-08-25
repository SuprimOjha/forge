#pragma once

namespace forge {

/**
 * @brief Bundles source files and manifests into a timestamped archive file.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runArchive(int argc, char* argv[]);

} // namespace forge