#include "forge/commands/env.hpp"

#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace forge {

namespace {

std::string getOperatingSystem() {

#ifdef _WIN32
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif

}


std::string getArchitecture() {

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    return "x64";
#elif defined(_M_IX86) || defined(__i386__)
    return "x86";
#elif defined(_M_ARM64) || defined(__aarch64__)
    return "ARM64";
#elif defined(_M_ARM) || defined(__arm__)
    return "ARM";
#else
    return "Unknown";
#endif

}

}


int runEnv() {

    std::cout
        << "\n"
        << "Forge Environment\n"
        << "────────────────────────────\n\n";


    /*
     * Forge
     */

    std::cout
        << "Forge\n"
        << "  Version: 0.1.0\n\n";


    /*
     * System
     */

    std::cout
        << "System\n"
        << "  OS: "
        << getOperatingSystem()
        << "\n"

        << "  Architecture: "
        << getArchitecture()
        << "\n\n";


    /*
     * Development tools
     */

    std::vector<ToolInfo> tools = {

        detectTool("Git", "git"),

        detectTool("CMake", "cmake"),

        detectTool("Python", "python"),

        detectTool("Node.js", "node"),

        detectTool("Docker", "docker")

    };


    std::cout
        << "Development Tools\n\n";


    for (const auto& tool : tools) {

        if (tool.installed) {

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

        } else {

            std::cout
                << "✗ "
                << tool.name
                << " not found\n";

        }

        std::cout << "\n";
    }


    return 0;
}

}