#pragma once

#include <string>

std::string getCurrentDirectory();

bool createDirectory(const std::string &path);
bool directoryExists(const std::string &path);

bool fileExists(const std::string &file);
