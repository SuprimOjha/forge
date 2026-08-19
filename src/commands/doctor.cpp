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

    /*
     * Development Tools & C++ Compilers
     */

    std::vector<ToolInfo> tools = {
        detectTool("Git", "git"),
        detectTool("CMake", "cmake"),
        detectTool("GCC (g++)", "g++"),
        detectTool("Clang (clang++)", "clang++"),
        detectTool("MSVC (cl)", "cl"),
        detectTool("Python", "python"),
        detectTool("Node.js", "node")
    };

    std::cout << "Development Tools & Compilers\n\n";

    int installedCount = 0;
    int workingCount = 0;

    for (const auto& tool : tools) {

        if (tool.installed) {

            ++installedCount;

            if (tool.working) {
                ++workingCount;
            }

            std::cout
                << "✓ "
                << tool.name
                << "\n";

            if (!tool.version.empty()) {
                std::cout
                    << "  Version: "
                    << tool.version
                    << "\n";
            }

            if (!tool.path.empty()) {
                std::cout
                    << "  Path: "
                    << tool.path
                    << "\n";
            }

            if (tool.working) {
                std::cout
                    << "  Status: Ready\n";
            } else {
                std::cout
                    << "  Status: Installed but not responding\n";
            }

        } else {

            std::cout
                << "✗ "
                << tool.name
                << "\n"
                << "  Status: Not installed\n";
        }

        std::cout << "\n";
    }

    std::cout << "Summary\n";

    std::cout
        << installedCount
        << "/"
        << tools.size()
        << " tools installed\n";

    std::cout
        << workingCount
        << "/"
        << tools.size()
        << " tools ready\n\n";

    /*
     * Exit code:
     *
     * 0 = all tools ready
     * 1 = one or more tools are missing/broken
     */

    if (workingCount == static_cast<int>(tools.size())) {
        return 0;
    }

    return 1;
}

} // namespace forge