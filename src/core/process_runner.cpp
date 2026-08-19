#include "forge/core/process_runner.hpp"

#include <cstdlib>
#include <array>
#include <memory>
#include <cstdio>

namespace forge {

ProcessResult ProcessRunner::run(const std::string& command, const std::string& workingDir) {
    ProcessResult result;
    
    std::string execCmd = command;
#if defined(_WIN32)
    if (!workingDir.empty()) {
        execCmd = "cd /d \"" + workingDir + "\" && " + command;
    }
    FILE* pipe = _popen(execCmd.c_str(), "r");
#else
    if (!workingDir.empty()) {
        execCmd = "cd \"" + workingDir + "\" && " + command;
    }
    FILE* pipe = popen(execCmd.c_str(), "r");
#endif

    if (!pipe) {
        result.stdErr = "Failed to spawn process pipe.";
        result.exitCode = -1;
        result.success = false;
        return result;
    }

    std::array<char, 256> buffer;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result.stdOut += buffer.data();
    }

#if defined(_WIN32)
    result.exitCode = _pclose(pipe);
#else
    result.exitCode = pclose(pipe);
#endif

    result.success = (result.exitCode == 0);
    return result;
}

} // namespace forge