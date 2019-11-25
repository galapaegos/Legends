#pragma once

#include "stdafx.h"

#include "Vec3.h"

// 
enum TerrainBiome {
	Terrestrial_Temperate,
	Terrestrial_Coniferous,
	Terrestrial_Woodland,
	Terrestrial_Tundra,
	Terrestrial_Grassland,
	Terrestrial_Desert,
	Terrestrial_TropicalSavanna,
	Terrestrial_TropicalForest,
	Marine_Plankton,
	Marine_Balanoid,
	Marine_Pelecypod,
	Marine_Coral,
};

// Types of rocks found in each layer
enum TerrainGeode {
	Geode_Iron,
	Geode_Copper,
	Geode_Aluminum,
	Geode_Bronze
};

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

	std::vector<float64> value;
	std::vector<float64> moisture;

	// Desert, swamp, etc
	std::vector<TerrainBiome> biome;

	// Numerical value tracking the amount of liquid above this tile
	std::vector<int16> rainfall;

	std::vector<int16> temperature;

	// Type of possible geodes found within the ground
	std::vector<uint16> geode;
};

void create_terrain(Terrain &t, const int &width, const int &height);
