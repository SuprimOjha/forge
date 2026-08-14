#pragma once

#include <string>

namespace forge::ui {

void init();

void success(const std::string& message);
void error(const std::string& message);
void warning(const std::string& message);
void info(const std::string& message);

void header(const std::string& title);

}