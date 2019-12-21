#ifndef _Utilities_TiffObject_h_
#define _Utilities_TiffObject_h_

#include <string>
#include <vector>

#include "stdafx.h"

#include <glm/glm.hpp>

#include "tiff.h"
#include "tiffio.h"

void warningHandler(const char *module, const char *fmt, va_list ap);
void errorHandler(const char *module, const char *fmt, va_list ap);

void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, const std::vector<uint8> &grid);
void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, const std::vector<float32> &grid);
void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, const std::vector<glm::vec<4, uint8>> &grid);

#endif
