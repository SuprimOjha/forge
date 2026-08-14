#include "forge/core/detector.hpp"
#include "forge/core/path.hpp"

namespace forge {

ToolInfo detectTool(
    const std::string& name,
    const std::string& command
) {
    ToolInfo info;

    info.name = name;
    info.command = command;
    info.installed = commandAvailable(command);

    if (info.installed) {
        info.path = findCommandPath(command);
    }

    return info;
}

}