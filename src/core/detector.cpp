#include "forge/core/detector.hpp"

#include <cstdlib>

namespace forge {

ToolInfo detectTool(
    const std::string& name,
    const std::string& command
) {
    ToolInfo info;

    info.name = name;
    info.command = command;
    info.installed = false;
    info.version = "";

#ifdef _WIN32

    const std::string checkCommand =
        "where " + command + " >nul 2>&1";

#else

    const std::string checkCommand =
        "command -v " + command + " >/dev/null 2>&1";

#endif

    const int result =
        std::system(checkCommand.c_str());

    info.installed = (result == 0);

    return info;
}

}