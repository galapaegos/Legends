#ifndef _Terrain_h_
#define _Terrain_h_

#include "stdafx.h"

#include "Biome.h"
#include "Geode.h"

#include <glm/glm.hpp>

struct MapSettings {
	int32 world_width = 256;
	int32 world_height = 256;

	int32 region_width = 8;
	int32 region_height = 8;

	int32 local_width = 128;
	int32 local_height = 128;

	int32 seed = 2019;
};

struct ElevationSettings {
	float32 offset_x = 0.0f;
	float32 offset_y = 0.0f;

	float32 frequency_x = 5.0f;
	float32 frequency_y = 5.0f;

	float32 frequency = 0.02f;
	float32 frequency_multiplier = 1.8f;
	float32 amplitude_multiplier = 0.35f;

	int32 fractalsum_layers = 5;

	int32 range = 200;
	int32 offset = 50;

	int32 terrain_ocean_bin = 64;
};

struct MoistureSettings {
	float32 offset_x = 0.0f;
	float32 offset_y = 0.0f;

	float32 frequency_x = 0.0f;
	float32 frequency_y = 0.0f;

	float32 frequency = 0.02f;
	float32 frequency_multiplier = 1.8f;
	float32 amplitude_multiplier = 0.35f;

	int32 fractalsum_layers = 5;
};

struct TemperatureSettings {
	int32 polar_shift = 0;
	float32 temperature = 30.f;
	float32 range = 5.f;
	float32 scaling = 0.25f;
	float32 factor = 0.01f;
};

struct DrainageSettings {
	float32 aquatic_coral = 0.01f;
	float32 aquatic_coral_offset = 10.f;
	float32 aquatic_estuaries = 0.05f;
	float32 aquatic_estuaries_offset = 10.f;
	float32 aquatic_ocean = 0.01f;
	float32 aquatic_ocean_offset = 10.f;
	float32 aquatic_lakes = 0.1f;
	float32 aquatic_lakes_offset = 10.f;
	float32 aquatic_ponds = 0.1f;
	float32 aquatic_ponds_offset = 10.f;
	float32 aquatic_wetlands = 0.05f;
	float32 aquatic_wetlands_offset = 10.f;

	float32 deserts_hot_and_dry = 20.f;
	float32 deserts_hot_and_dry_offset = 10.f;
	float32 deserts_semiarid = 10.f;
	float32 deserts_semiarid_offset = 10.f;
	float32 deserts_coastal = 5.f;
	float32 deserts_coastal_offset = 10.f;
	float32 deserts_cold = 25.f;
	float32 deserts_cold_offset = 10.f;

	float32 forests_tropical = 15.f;
	float32 forests_tropical_offset = 20.f;
	float32 forests_temperate = 5.f;
	float32 forests_temperate_offset = 20.f;
	float32 forests_boreal = 5.f;
	float32 forests_boreal_offset = 20.f;

	float32 grasslands_savanna = 50.f;
	float32 grasslands_savanna_offset = 45.f;
	float32 grasslands_temperate = 65.f;
	float32 grasslands_temperate_offset = 50.f;
	float32 grasslands_steppes = 40.f;
	float32 grasslands_steppes_offset = 50.f;

	float32 tundra_artic = 40.f;
	float32 tundra_artic_offset = 20.f;
	float32 tundra_alpine = 50.f;
	float32 tundra_alpine_offset = 20.f;
};

// 
struct TerrainSettings {
	int32 passes = 1;
	float32 pass_weight = 0.5;

	MapSettings map;
	ElevationSettings elevation;
	MoistureSettings moisture;
	TemperatureSettings temperature;
	DrainageSettings drainage;
};

//Details paramters about this indexed location
struct TerrainWorld {
	int width;
	int height;

	// [0 -> 255(m? or km?)]
	std::vector<float32> elevation;

	// [0 -> 150cm]
	std::vector<float32> moisture;

	// Desert, swamp, etc
	std::vector<uint8> biome;

	// [0 -> 150cm]
	std::vector<float32> rainfall;

	// [-50C to 50C]
	std::vector<float32> temperature;

	// 
	std::vector<float32> atmosphere;

	// [0 -> 150cm] 
	std::vector<float32> drainage;

	// Vector with strength as w.
	std::vector<glm::vec<4, float32>> wind;

	std::vector<glm::vec<3, float32>> elevation_gradients;

	//std::vector<int32> tectonic_plate_depth;

	// Type of possible geodes found within the ground
	std::vector<int32> geode;

	float32 sea_level;
	float32 world_age;
};

// Setup data structures
void initialize_terrain(TerrainWorld &t, const int &width, const int &height);

void convert_to_windmap(std::vector<float32> &input, const int &width, const int &height, 
	std::vector<glm::vec<4, uint8>> &windmap);
void convert_to_heatmap(std::vector<float32> &input, const int &width, const int &height, 
	std::vector<glm::vec<4, uint8>> &heatmap);

void convert_to_drainage(std::vector<float32> &input, const int &width, const int &height,
	std::vector<glm::vec<4, uint8>> &heatmap);

void save_world(TerrainWorld &t, const std::string &path);

void renormalize(std::vector<float32> &input, const int &width, const int &height);

void print_settings(TerrainSettings &ts);

#endif
