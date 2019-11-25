#ifndef _Terrain_h_
#define _Terrain_h_

#include "stdafx.h"

#include "Biome.h"
#include "Geode.h"

#include <glm/vec4.hpp>

// Configurable details user can specify for this region
struct TerrainSettings {
	float64 terrain_offset_x = 0.0;
	float64 terrain_offset_y = 0.0;

	float64 terrain_frequency_x = 5.0;
	float64 terrain_frequency_y = 5.0;

	float64 terrain_frequency = 0.02;
	float64 terrain_frequency_multiplier = 1.8;
	float64 terrain_amplitude_multiplier = 0.35;

	int32 terrain_fractalsum_layers = 5;

	int32 terrain_elevation_range = 200;
	int32 terrain_elevation_offset = 50;

	float64 moisture_offset_x = 0.0;
	float64 moisture_offset_y = 0.0;

	float64 moisture_frequency_x = 0.0;
	float64 moisture_frequency_y = 0.0;

	float64 moisture_frequency = 0.02;
	float64 moisture_frequency_multiplier = 1.8;
	float64 moisture_amplitude_multiplier = 0.35;

	int32 moisture_fractalsum_layers = 5;
};

//Details paramters about this indexed location
struct Terrain {
	int grid_width;
	int grid_height;

	std::vector<float64> elevation;
	std::vector<float64> moisture;

	// Vector with strength as w.
	std::vector<glm::vec<4, float>> wind;

	// Desert, swamp, etc
	std::vector<Biome> biome;

	// Numerical value tracking the amount of liquid above this tile
	std::vector<int16> rainfall;
	std::vector<int16> temperature;
	std::vector<float64> atmosphere;

	std::vector<int16> tectonic_plate_depth;

	// Type of possible geodes found within the ground
	std::vector<uint16> geode;
};

void create_terrain(Terrain &t, const int &width, const int &height);

#endif
