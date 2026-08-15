#include "forge/core/config.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>

namespace forge {

std::string getConfigDirectory() {

#ifdef _WIN32

    const char* appData = std::getenv("APPDATA");

    if (appData != nullptr) {
        return std::string(appData) + "/forge";
    }

    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile != nullptr) {
        return std::string(userProfile) + "/.forge";
    }

    return ".forge";

#else

    const char* home = std::getenv("HOME");

    if (home != nullptr) {
        return std::string(home) + "/.forge";
    }

    return ".forge";

#endif
}

std::string getConfigPath() {

    return getConfigDirectory() + "/config.txt";
}

Config loadConfig() {

    Config config;

    config.version = "1";
    config.editor = "";
    config.shell = "";

    std::ifstream file(getConfigPath());

    if (!file.is_open()) {
        return config;
    }

    std::string line;

    while (std::getline(file, line)) {

        const std::size_t separator = line.find('=');

        if (separator == std::string::npos) {
            continue;
        }

        const std::string key =
            line.substr(0, separator);

        const std::string value =
            line.substr(separator + 1);

        if (key == "version") {
            config.version = value;
        }
        else if (key == "editor") {
            config.editor = value;
        }
        else if (key == "shell") {
            config.shell = value;
        }
    }

    return config;
}

bool saveConfig(const Config& config) {

    const std::filesystem::path directory =
        getConfigDirectory();

    std::error_code error;

    std::filesystem::create_directories(
        directory,
        error
    );

    if (error) {
        return false;
    }

    std::ofstream file(getConfigPath());

    if (!file.is_open()) {
        return false;
    }

    file << "version=" << config.version << "\n";
    file << "editor=" << config.editor << "\n";
    file << "shell=" << config.shell << "\n";

    return true;
}

}