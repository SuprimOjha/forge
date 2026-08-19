#pragma once

#include "forge/models/project_info.hpp"
#include "forge/core/process_runner.hpp"
#include <vector>

namespace forge {

class FixEngine {
public:
    static std::vector<FixAction> resolveFixes(const ProjectInfo& project);
    static bool executeFix(const FixAction& fix, const std::string& projectPath, ProcessResult& resultOut);
    static bool verifyFix(const FixAction& fix, const ProjectInfo& updatedProject);
};

} // namespace forge