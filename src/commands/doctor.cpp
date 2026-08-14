#include "forge/commands/doctor.hpp"
#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"
#include "forge/ui/output.hpp"

#include <iostream>
#include <vector>

namespace forge {

int runDoctor() {

    ui::init();

    ui::header("Forge Doctor");

    std::vector<ToolInfo> tools = {
        detectTool("Git", "git"),
        detectTool("CMake", "cmake"),
        detectTool("Python", "python"),
        detectTool("Node.js", "node"),
        detectTool("Docker", "docker")
    };

    std::cout << "Development Tools\n\n";

    int installedCount = 0;

    for (auto& tool : tools) {

        if (tool.installed) {

            installedCount++;

            tool.version =
                detectToolVersion(tool.command);

            ui::success(tool.name);

            if (!tool.version.empty()) {
                std::cout
                    << "  Version: "
                    << tool.version
                    << '\n';
            }

            if (!tool.path.empty()) {
                std::cout
                    << "  Path: "
                    << tool.path
                    << "\n\n";
            }

        } else {

            ui::error(tool.name + " not found");
            std::cout << '\n';
        }
    }

    std::cout
        << "Summary\n"
        << installedCount
        << "/"
        << tools.size()
        << " tools detected\n\n";

    return 0;
}

}