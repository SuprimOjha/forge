#pragma once

#include <string>

namespace forge {

bool commandAvailable(const std::string& command);

std::string findCommandPath(const std::string& command);

std::string executeCommand(const std::string& command);

std::string trim(const std::string& value);

}