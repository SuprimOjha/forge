#pragma once

#include <string>
#include <vector>

namespace forge {

struct ProjectInfo {

    std::string name;
    std::string path;
    std::string type;

    std::string packageManager;

    std::vector<std::string> detectedFiles;
    std::vector<std::string> frameworks;

    bool gitRepository = false;
};

}