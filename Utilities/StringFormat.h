#pragma once

#include <string>

std::string convertToString(const std::wstring &wstr);
std::wstring convertToWString(const std::string &str);

std::string format(const char *fmt, ...);

