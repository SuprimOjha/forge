#pragma once

#include <string>

namespace forge {

struct ProcessResult {
    int exitCode = -1;
    std::string stdOut;
    std::string stdErr;
    bool success = false;
};

class ProcessRunner {
public:
    static ProcessResult run(const std::string& command, const std::string& workingDir = "");
};

} // namespace forge