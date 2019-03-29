#include "stdafx.h"

#include <stdarg.h>

#include <codecvt>
#include <locale>

std::string convertToString(const std::wstring &wstr) {
	std::string result;
	try {
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		result = converter.to_bytes(wstr);
	}

	catch (std::exception &e) {
		printf("%s\n", e.what());
	}

	return result;
}

std::wstring convertToWString(const std::string &str) {
	std::wstring result;
	try {
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		result = converter.from_bytes(str);
	}

	catch (std::exception &e) {
		printf("%s\n", e.what());
	}
	return result;
}

std::string format(const char *fmt, ...) {
	va_list args;

	char buffer[2048];
	memset(buffer, 0, 2048);

	va_start(args, fmt);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, args);

	va_end(args);

	return std::string(buffer);
}