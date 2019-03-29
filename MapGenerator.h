#pragma once

#include "stdafx.h"

class MapGenerator {
public:
	MapGenerator(const int &seed);
	~MapGenerator();

	void generate();
};