#pragma once

#include "stdafx.h"

#include "PerlinNoise.h"

#include "Terrain.h"

class Generator {
public:
	Generator();

	void generate(const TerrainSettings &ts);

	TerrainWorld world;

private:
	// Create a series of seeds based off a 'root seed'.
	void establish_seeds();

	void create_elevation(PerlinNoise &n);
	void create_moisture(PerlinNoise &n);
	void create_temperature(PerlinNoise &n);
	void create_atmosphere(PerlinNoise &n);
	void create_drainage(PerlinNoise &n);

	void identify_biomes(PerlinNoise &n);

	void create_oceans(PerlinNoise &n);
	void create_rivers(PerlinNoise &n);

	void smooth(std::vector<float32> &grid);

	void calculate_gradients(std::vector<float32> &input, std::vector<glm::vec3> gradients);

	TerrainSettings settings;
};