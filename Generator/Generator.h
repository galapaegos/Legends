#pragma once

#include "stdafx.h"

#include "PerlinNoise.h"

#include "Terrain.h"

class Generator {
public:
	Generator();

	void generate(const TerrainSettings &ts, const int32 &x, const int32 &y, const int32 &length, const uint32 &seed);

	TerrainWorld world;

private:
	void create_land(PerlinNoise &n);
	void create_moisture(PerlinNoise &n);
	void create_temperature(PerlinNoise &n);
	void create_atmosphere(PerlinNoise &n);
	void create_drainage(PerlinNoise &n);

	void identify_biomes(PerlinNoise &n);

	void create_oceans(PerlinNoise &n);
	void create_rivers(PerlinNoise &n);
	
	void generate_civilizations(PerlinNoise &n);
	void generate_populations(PerlinNoise &n);

	void progress_step(PerlinNoise &n);

	void apply_evaporation(float64 &rainfall);

	TerrainSettings settings;
};