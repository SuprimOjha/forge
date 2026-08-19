#ifndef FORGE_COMMANDS_BUILD_HPP
#define FORGE_COMMANDS_BUILD_HPP

#include <vector>
#include <string>

namespace forge {

int runBuild(const std::vector<std::string>& extraArgs = {});

} // namespace forge

#endif // FORGE_COMMANDS_BUILD_HPP