#include "forge/commands/doctor.hpp"
#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"
#include "forge/ui/output.hpp"
#include "forge/commands/doctor.hpp"
#include "forge/core/detector.hpp"
#include "forge/models/tool_info.hpp"
#include "forge/ui/output.hpp"
#include <iostream>
#include <vector>
#include <vector>
#include <iostream>

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

    ui::info("Development Tools");
    std::cout << '\n';

    for (const auto& tool : tools) {

        if (tool.installed) {

            ui::success(tool.name);

        } else {

            ui::error(tool.name + " not found");

        }

    }

    std::cout << '\n';

    return 0;
}

}