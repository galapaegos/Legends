#pragma once

#include "stdafx.h"

#include "PerlinNoise.h"
#include "TerrainSpot.h"

class Generator {
public:
	Generator();

	void generate(const int32 &x, const int32 &y, const int32 &length, const uint32 &seed);

	std::vector<TerrainSpot> getTerrain() const { return terrain; }

	void getGridSize(int *width, int *height) const { *width = grid_width; *height = grid_height; }

private:
	void create_land(PerlinNoise &n);
	void create_ocean(PerlinNoise &n);
	void create_rivers(PerlinNoise &n);
	
	void generate_civilizations(PerlinNoise &n);
	void generate_populations(PerlinNoise &n);

	void progress_step(PerlinNoise &n);

	void apply_evaporation(int16 &rainfall);

	int grid_width, grid_height;
	std::vector<TerrainSpot> terrain;
};