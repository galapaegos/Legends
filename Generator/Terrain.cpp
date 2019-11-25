#include "stdafx.h"

#include "Terrain.h"

void create_terrain(Terrain &t, const int &width, const int &height) {
	t.grid_width = width;
	t.grid_height = height;

	const int total = width * height;
	t.elevation.resize(total);
	t.moisture.resize(total);

	t.biome.resize(total);

	t.rainfall.resize(total);
	t.temperature.resize(total);
	t.atmosphere.resize(total);


	t.geode.resize(total);
}