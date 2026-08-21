#pragma once

namespace forge {

/**
 * @brief Scans workspace files for open source licenses and header compliance.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code (0 on success, 1 on failure).
 */
int runLicense(int argc, char* argv[]);

} // namespace forge