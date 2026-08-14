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

    if (!commandAvailable(command)) {
        return info;
    }

    info.installed = true;

    info.path =
        findCommandPath(command);

    info.version =
        trim(
            executeCommand(
                command + " --version"
            )
        );

    return info;
}

}