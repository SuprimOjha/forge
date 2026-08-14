#include "forge/core/detector.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace forge {

namespace fs = std::filesystem;

#ifdef _WIN32

std::vector<std::string> getWindowsPaths(const std::string& command) {
    std::vector<std::string> paths;

    if (command == "git") {
        paths = {
            R"(C:\Program Files\Git\cmd\git.exe)",
            R"(C:\Program Files\Git\bin\git.exe)"
        };
    }
    else if (command == "cmake") {
        paths = {
            R"(C:\Program Files\CMake\bin\cmake.exe)"
        };
    }
  else if (command == "python") {
    paths = {
        R"(C:\Users\User\AppData\Local\Programs\Python\Python312\python.exe)",
        R"(C:\Users\User\AppData\Local\Programs\Python\Python311\python.exe)"
    };
}
    else if (command == "node") {
        paths = {
            R"(C:\Program Files\nodejs\node.exe)"
        };
    }
    else if (command == "docker") {
        paths = {
            R"(C:\Program Files\Docker\Docker\resources\bin\docker.exe)"
        };
    }

    return paths;
}

#endif

std::string getVersion(const std::string& executable) {
    std::string command = "\"" + executable + "\" --version 2>&1";

#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif

    if (!pipe) {
        return "";
    }

    std::array<char, 256> buffer{};
    std::string output;

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe)) {
        output += buffer.data();
    }

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    // Remove trailing newline characters.
    while (!output.empty() &&
           (output.back() == '\n' || output.back() == '\r')) {
        output.pop_back();
    }

    return output;
}

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

    // First try the normal Windows PATH.
    const std::string whereCommand =
        "where " + command + " >nul 2>&1";

    if (std::system(whereCommand.c_str()) == 0) {
        info.installed = true;

        const std::string versionCommand =
            "where " + command;

        FILE* pipe = _popen(versionCommand.c_str(), "r");

        if (pipe) {
            std::array<char, 512> buffer{};
            std::string executablePath;

            if (fgets(buffer.data(),
                      static_cast<int>(buffer.size()),
                      pipe)) {
                executablePath = buffer.data();
            }

            _pclose(pipe);

            while (!executablePath.empty() &&
                   (executablePath.back() == '\n' ||
                    executablePath.back() == '\r')) {
                executablePath.pop_back();
            }

            if (!executablePath.empty()) {
                info.version = getVersion(executablePath);
            }
        }

        return info;
    }

    // If PATH lookup failed, check common Windows installation paths.
    for (const auto& path : getWindowsPaths(command)) {

        if (fs::exists(path)) {
            info.installed = true;
            info.version = getVersion(path);

            return info;
        }
    }

#else

    const std::string checkCommand =
        "command -v " + command + " >/dev/null 2>&1";

    if (std::system(checkCommand.c_str()) == 0) {
        info.installed = true;
        info.version = getVersion(command);
    }

#endif

    return info;
}

}