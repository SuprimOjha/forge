#include "forge/commands/check.hpp"
#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"

#include <iostream>
#include <vector>

namespace forge {

int runCheck() {
    std::cout << "\nForge Check\n";
    std::cout << "────────────────────────────\n\n";

    const std::vector<ToolInfo> tools = {
        detectTool("Git", "git"),
        detectTool("CMake", "cmake"),
        detectTool("Python", "python"),
        detectTool("Node.js", "node"),
        detectTool("Docker", "docker")
    };

    bool allReady = true;

    for (const auto& tool : tools) {
        if (tool.installed) {
            std::cout << "✓ " << tool.name << "\n";
        } else {
            std::cout << "✗ " << tool.name << " not found\n";
            allReady = false;
        }
    }

    std::cout << "\n";

    if (allReady) {
        std::cout << "Result: environment ready\n";
        return 0;
    }

    std::cout << "Result: environment has missing tools\n";
    return 1;
}

} // namespace forge