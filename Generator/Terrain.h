#ifndef _Terrain_h_
#define _Terrain_h_

#include "stdafx.h"

#include "Biome.h"
#include "Geode.h"

#include <glm/vec4.hpp>

struct ElevationSettings {
	float32 elevation_offset_x = 0.0f;
	float32 elevation_offset_y = 0.0f;

	float32 elevation_frequency_x = 5.0f;
	float32 elevation_frequency_y = 5.0f;

	float32 elevation_frequency = 0.02f;
	float32 elevation_frequency_multiplier = 1.8f;
	float32 elevation_amplitude_multiplier = 0.35f;

	int32 elevation_fractalsum_layers = 5;

	int32 elevation_range = 200;
	int32 elevation_offset = 50;

	int32 terrain_ocean_bin = 64;
};

struct MoistureSettings {
	float32 moisture_offset_x = 0.0f;
	float32 moisture_offset_y = 0.0f;

	float32 moisture_frequency_x = 0.0f;
	float32 moisture_frequency_y = 0.0f;

	float32 moisture_frequency = 0.02f;
	float32 moisture_frequency_multiplier = 1.8f;
	float32 moisture_amplitude_multiplier = 0.35f;

	int32 moisture_fractalsum_layers = 5;
};

// 
struct TerrainSettings {
	ElevationSettings elevation;
	MoistureSettings moisture;
};

struct TerrainLocal {
	int local_width;
	int local_height;

	std::vector<float64> elevation;
	std::vector<float64> moisture;

	// Numerical value tracking the amount of liquid above this tile
	std::vector<float64> rainfall;
	std::vector<float64> temperature;
	std::vector<float64> atmosphere;
	std::vector<float64> drainage;

	// Vector with strength as w.
	std::vector<glm::vec<4, float>> wind;
};

struct TerrainRegion {
	int region_width;
	int region_height;

	std::vector<float64> elevation;
	std::vector<float64> moisture;

	// Numerical value tracking the amount of liquid above this tile
	std::vector<float64> rainfall;
	std::vector<float64> temperature;
	std::vector<float64> atmosphere;
	std::vector<float64> drainage;

	// Vector with strength as w.
	std::vector<glm::vec<4, float>> wind;

	// Regional biomes.
	std::vector<Biome> biome;

	// Type of possible geodes found within the ground
	std::vector<int32> geode;

	std::vector<TerrainLocal> locals;
};

//Details paramters about this indexed location
struct TerrainWorld {
	int world_width;
	int world_height;

	std::vector<float64> elevation;
	std::vector<float64> moisture;

	// Desert, swamp, etc
	std::vector<Biome> biome;

	// Numerical value tracking the amount of liquid above this tile
	std::vector<float64> rainfall;
	std::vector<float64> temperature;
	std::vector<float64> atmosphere;
	std::vector<float64> drainage;

	// Vector with strength as w.
	std::vector<glm::vec<4, float>> wind;

	std::vector<int32> tectonic_plate_depth;

	// Type of possible geodes found within the ground
	std::vector<int32> geode;

	std::vector<TerrainRegion> regions;
};

// Setup data structures
void initialize_terrain(TerrainWorld &t, const int &width, const int &height);
void initialize_region(TerrainRegion &t, const int &width, const int &height);
void initialize_local(TerrainLocal &t, const int &width, const int &height);

#endif
