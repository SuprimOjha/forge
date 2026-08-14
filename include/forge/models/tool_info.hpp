#pragma once

#include <string>

namespace forge {

struct ToolInfo {

    std::string name;
    std::string command;

    bool installed = false;

    std::string version;
    std::string path;
};

}