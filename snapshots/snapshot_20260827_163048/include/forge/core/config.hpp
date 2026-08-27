#pragma once

#include "forge/models/config.hpp"

#include <string>

namespace forge {

std::string getConfigDirectory();

std::string getConfigPath();

Config loadConfig();

bool saveConfig(const Config& config);

}