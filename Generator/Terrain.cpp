#include "stdafx.h"

#include "Terrain.h"

void initialize_terrain(TerrainWorld &t, const int &width, const int &height) {
	t.world_width = width;
	t.world_height = height;

	const int total = width * height;
	t.elevation.resize(total);
	t.moisture.resize(total);

	t.biome.resize(total);

	t.rainfall.resize(total);
	t.temperature.resize(total);
	t.atmosphere.resize(total);
	t.drainage.resize(total);

	t.tectonic_plate_depth.resize(total);

	t.geode.resize(total);

	t.regions.resize(total);
}

void initialize_region(TerrainRegion &t, const int &width, const int &height) {
	t.region_height = width;
	t.region_height = height;

	const int total = width * height;
}

void initialize_local(TerrainLocal &t, const int &width, const int &height) {
	t.local_height = width;
	t.local_height = height;

	const int total = width * height;
}
