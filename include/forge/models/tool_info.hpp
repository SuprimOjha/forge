#pragma once

#include <string>

namespace forge {

struct ToolInfo {
    std::string name;
    std::string command;
    bool installed;
    std::string version;
};

}