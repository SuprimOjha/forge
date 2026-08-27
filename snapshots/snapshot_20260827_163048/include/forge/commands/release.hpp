#pragma once

namespace forge {

/**
 * @brief Bundles binaries, headers, and assets into versioned release distributions with SHA-256 checksums.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runRelease(int argc, char* argv[]);

} // namespace forge