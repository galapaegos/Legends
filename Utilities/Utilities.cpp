#include "stdafx.h"

#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#include "Utilities.h"

std::string getCurrentDirectory() {
	wchar_t path[255];
	GetCurrentDirectory(255, path);

	return convertToString(path);
}

bool createDirectory(const std::string &path) {
	std::wstring wpath = convertToWString(path);
	int ret = SHCreateDirectoryEx(NULL, wpath.c_str(), NULL);

	return (ret == ERROR_SUCCESS) ? true : false;
}

bool directoryExists(const std::string &path) {
	std::wstring wpath = convertToWString(path);
	return PathIsDirectory(wpath.c_str()) != 0;
}

bool fileExists(const std::string &file) {
	std::wstring wpath = convertToWString(file);
	return PathFileExists(wpath.c_str()) != 0;
}