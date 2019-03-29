#pragma once

#include <string>

#include "tiff.h"

class Generator;

void read_tiff(Generator &g, const std::string &path);
void write_tiff(Generator &g, const std::string &path);
