#include "forge/core/path.hpp"

#include <array>
#include <cstdio>
#include <memory>
#include <string>

namespace forge {

namespace {

std::string executeCommand(const std::string& command) {

    std::array<char, 256> buffer{};
    std::string result;

#ifdef _WIN32

   const std::string fullCommand =
    "cmd.exe /c \"" + command + " 2>nul\"";

    FILE* rawPipe =
        _popen(fullCommand.c_str(), "r");

    if (!rawPipe) {
        return "";
    }

    std::unique_ptr<FILE, decltype(&_pclose)>
        pipe(rawPipe, _pclose);

#else

    FILE* rawPipe =
        popen(command.c_str(), "r");

    if (!rawPipe) {
        return "";
    }

    std::unique_ptr<FILE, decltype(&pclose)>
        pipe(rawPipe, pclose);

#endif

    while (fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }

    return result;
}

std::string trim(const std::string& value) {

    const auto start =
        value.find_first_not_of(" \t\r\n");

    if (start == std::string::npos) {
        return "";
    }

    const auto end =
        value.find_last_not_of(" \t\r\n");

    return value.substr(
        start,
        end - start + 1
    );
}

}

bool commandAvailable(const std::string& command) {

#ifdef _WIN32

    return !executeCommand(
        "where " + command
    ).empty();

#else

    return !executeCommand(
        "command -v " + command
    ).empty();

#endif
}

std::string findCommandPath(const std::string& command) {

#ifdef _WIN32

    const std::string output =
        executeCommand("where " + command);

#else

    const std::string output =
        executeCommand("command -v " + command);

#endif

    if (output.empty()) {
        return "";
    }

    const auto newline =
        output.find_first_of("\r\n");

    if (newline == std::string::npos) {
        return trim(output);
    }

    return trim(output.substr(0, newline));
}
}