#ifndef FORGE_COMMANDS_BUILD_HPP
#define FORGE_COMMANDS_BUILD_HPP

#include <string>
#include <vector>

namespace forge {

int runBuild(const std::vector<std::string>& extraArgs = {});

}

#endif