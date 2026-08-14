#pragma once

#include <string>

namespace forge {

struct ToolInfo {

    std::string name;
    std::string command;
    std::string path;
    std::string version;

    bool installed = false;
};

}