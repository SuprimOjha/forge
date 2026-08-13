#pragma once

#include <string>

#include "forge/models/tool_info.hpp"

namespace forge {

ToolInfo detectTool(
    const std::string& name,
    const std::string& command
);

}