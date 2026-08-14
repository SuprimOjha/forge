#include "forge/core/detector.hpp"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <array>
#include <sstream>

namespace forge {

namespace {

std::string executeCommand(const std::string& command) {

    std::array<char, 256> buffer{};
    std::string result;

#ifdef _WIN32
    const std::string fullCommand =
        "cmd.exe /c \"" + command + "\"";
#else
    const std::string fullCommand = command;
#endif

    std::unique_ptr<FILE, decltype(&_pclose)> pipe(
        _popen(fullCommand.c_str(), "r"),
        _pclose
    );

    if (!pipe) {
        return "";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }

    return result;
}

std::string trim(const std::string& value) {

    const auto first =
        value.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    const auto last =
        value.find_last_not_of(" \t\r\n");

    return value.substr(
        first,
        last - first + 1
    );
}

} // anonymous namespace


ToolInfo detectTool(
    const std::string& name,
    const std::string& command
) {

    ToolInfo info;

    info.name = name;
    info.command = command;

#ifdef _WIN32

    const std::string whereCommand =
        "where " + command;

#else

    const std::string whereCommand =
        "command -v " + command;

#endif

    const std::string pathOutput =
        executeCommand(whereCommand);

    if (pathOutput.empty()) {
        return info;
    }

    info.installed = true;

    std::istringstream paths(pathOutput);

    std::getline(paths, info.path);

    info.path = trim(info.path);

    return info;
}


std::string detectToolVersion(
    const std::string& command
) {

    const std::string output =
        executeCommand(command + " --version");

    return trim(output);
}

} // namespace forge