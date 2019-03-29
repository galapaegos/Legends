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

//Details paramters about this indexed location
class TerrainSpot {
public:
	TerrainSpot();

	// Seed for this piece
	float64 terrain_value;

	float64 terrain_eroded_value;

	// mountain
	// hill
	// plain
	// river
	// ocean
	uint16 terrain_type;

	// Desert, swamp, etc
	TerrainBiome terrain_biome;

	// Numerical value tracking the amount of liquid above this tile
	int16 terrain_rainfall;

	int16 terrain_temperature;

	// Type of possible geodes found within the ground
	uint16 terrain_geode;
};
