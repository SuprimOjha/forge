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

    int installedCount = 0;

    for (const auto& tool : tools) {

        if (tool.installed) {

            ++installedCount;

            std::cout
                << "✓ "
                << tool.name
                << "\n";

            if (!tool.path.empty()) {

                std::cout
                    << "  Path: "
                    << tool.path
                    << "\n";
            }

        } else {

            std::cout
                << "✗ "
                << tool.name
                << " not found\n";
        }

        std::cout << "\n";
    }

    std::cout
        << installedCount
        << "/"
        << tools.size()
        << " development tools detected.\n\n";

    return 0;
}

}