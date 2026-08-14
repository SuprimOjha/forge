#include "forge/commands/doctor.hpp"
#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace forge {

int runDoctor() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "\n";
    std::cout << "Forge Doctor\n";
    std::cout << "────────────────────────────\n\n";

    std::vector<ToolInfo> tools = {
        detectTool("Git", "git"),
        detectTool("CMake", "cmake"),
        detectTool("Python", "python"),
        detectTool("Node.js", "node"),
        detectTool("Docker", "docker")
    };

    std::cout << "Development Tools\n\n";

    for (const auto& tool : tools) {

        if (tool.installed) {
            std::cout << "✓ "
                      << tool.name
                      << "\n";
        } else {
            std::cout << "✗ "
                      << tool.name
                      << " not found\n";
        }
    }

    std::cout << "\n";

    return 0;
}

}