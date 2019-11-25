#include "stdafx.h"

#include "Generator.h"

Generator::Generator() {

}

void Generator::generate(const TerrainSettings &ts, const int32 &map_w, const int32 &map_h, const int32 &length, const uint32 &seed) {
	PerlinNoise pn(256, seed);

	terrain_settings = ts;

	// Initialize structures here:
	create_terrain(terrain, map_w, map_h);

	// Skipping tectonic plates, etc:

	// Using a fractal generate a reasonable land
	create_land(pn);

	// Create an ocean.
	create_ocean(pn);

	// Using a fractal generate a reasonable moisture
	create_moisture(pn);

	// Combine both land & moisture to identify biomes.
	identify_biomes(pn);
}

void Generator::create_land(PerlinNoise &n) {
	float64 i_width = 1.0/terrain.grid_width;
	float64 i_height = 1.0/terrain.grid_height;

	// set elevation
	// set temperature
	// running rivers
	// formming lakes and minerals

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int32 idx = y * terrain.grid_width + x;

			float64 fx = ((x + terrain_settings.terrain_offset_x) * terrain_settings.terrain_frequency_x)*i_width;
			float64 fy = ((y + terrain_settings.terrain_offset_y) * terrain_settings.terrain_frequency_y)*i_height;

			float64 amplitude = 1.0;
			for (int l = 0; l < terrain_settings.terrain_fractalsum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				terrain.value[idx] += v;

				fx *= terrain_settings.terrain_frequency_multiplier;
				fy *= terrain_settings.terrain_frequency_multiplier;

				amplitude *= terrain_settings.terrain_amplitude_multiplier;
			}

			if (terrain.value[idx] > maxNoiseVal) {
				maxNoiseVal = terrain.value[idx];
			}
		}
	}

	// Range of values
	int terrain_elevation_range = 200;

	// Natural offset
	int terrain_elevation_offset = 50;

	// Normalize our terrain value
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int32 idx = y * terrain.grid_width + x;
			terrain.value[idx] /= maxNoiseVal;
			terrain.value[idx] *= terrain_elevation_range;
			terrain.value[idx] += terrain_elevation_offset;
		}
	}

	// TODO: Now that we have our values, simulate rain and evaporation over the entire surface
	// https://hal.inria.fr/inria-00402079/document
	// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.90.8141&rep=rep1&type=pdf

	int rainfall_iteraions = 5;
	int rainfall_amount = 10;

	int drainage = 10;

	// First pass for performing water effects
	for (int i = 0; i < rainfall_iteraions; i++) {
		// First iteration is to place out a number of rainfall's randomly.  The amount of 
		// rainfall should be dependent on the biome
		for (int y = 0; y < terrain.grid_height; y++) {
			for (int x = 0; x < terrain.grid_width; x++) {
				int idx = y * terrain.grid_width + x;

				float64 fx = x * terrain.grid_width;
				float64 fy = y * terrain.grid_height;

				terrain.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
			}
		}

		// Next we will find gradients at each location of rainfalls, and erode dirt.
		// Determine how much soil to move and location to move them.
		for (int y = 0; y < terrain.grid_height; y++) {
			for (int x = 0; x < terrain.grid_width; x++) {
				int idx = y * terrain.grid_width + x;

				// Get current value
				uint16 rain = terrain.rainfall[idx];
			}
		}

		// Finally evaporation an amount, depositing dirt in location
		for (int y = 0; y < terrain.grid_height; y++) {
			for (int x = 0; x < terrain.grid_width; x++) {
				int idx = y * terrain.grid_width + x;

				apply_evaporation(terrain.rainfall[idx]);
			}
		}
	}
}

void Generator::create_moisture(PerlinNoise &n) {
	float64 i_width = 1.0 / terrain.grid_width;
	float64 i_height = 1.0 / terrain.grid_height;

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int32 idx = y * terrain.grid_width + x;

			float64 fx = ((x + terrain_settings.moisture_offset_x) * terrain_settings.moisture_frequency_x)*i_width;
			float64 fy = ((y + terrain_settings.moisture_offset_y) * terrain_settings.moisture_frequency_y)*i_height;

			float64 amplitude = 1.0;
			for (int l = 0; l < terrain_settings.moisture_fractalsum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				terrain.moisture[idx] += v;

				fx *= terrain_settings.moisture_frequency_multiplier;
				fy *= terrain_settings.moisture_frequency_multiplier;

				amplitude *= terrain_settings.moisture_amplitude_multiplier;
			}

			if (terrain.moisture[idx] > maxNoiseVal) {
				maxNoiseVal = terrain.moisture[idx];
			}
		}
	}

	// Normalize our moisture
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int32 idx = y * terrain.grid_width + x;
			terrain.value[idx] /= maxNoiseVal;
		}
	}
}

void Generator::identify_biomes(PerlinNoise &n) {
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int32 idx = y * terrain.grid_width + x;

			terrain.moisture[idx];
			terrain.value[idx];

			terrain.biome[idx];
		}
	}
}

void Generator::create_ocean(PerlinNoise &n) {
	// Find min/max of the terrain values
	float64 terrain_min = FLT_MAX, terrain_max = FLT_MIN;

	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int idx = y * terrain.grid_width + x;

			if (terrain_max < terrain.value[idx])
				terrain_max = terrain.value[idx];

			if (terrain_min > terrain.value[idx])
				terrain_min = terrain.value[idx];
		}
	}

	std::vector<int> histogram;
	float64 offset = terrain_max - terrain_min;

	// Find histogram of 256 bins
	histogram.resize(256);

	// Create a histogram of the terrain values.
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int idx = y * terrain.grid_width + x;

			// Normalize terrain value
			float64 t = (terrain.value[idx] - terrain_min) / offset;

			// Place into 256 bin
			histogram[int(t*255)]++;
		}
	}
	
	// Our ocean location is based off this:  25% is the ocean level?
	float64 ocean_level = histogram[64];

	printf("Ocean level = %f\n [%f,%f]", (64/255.0)*offset + terrain_min, terrain_min, terrain_max);

	// Run a EDM, which is the ocean tiles.  Don't walk into the land
}

void Generator::create_rivers(PerlinNoise &n) {
	// First we need gradients of the terrain.

	// Find locations along the ocean, walk them back up mountains.

	// Find locations that are below sea level
}

void Generator::generate_civilizations(PerlinNoise &n) {

}
void Generator::generate_populations(PerlinNoise &n) {

}

void Generator::progress_step(PerlinNoise &n) {
	float64 width = 1.0 / terrain.grid_width;
	float64 height = 1.0 / terrain.grid_height;

	int rainfall_iteraions = 5;
	int rainfall_amount = 10;

	int drainage = 10;

	// First iteration is to place out a number of rainfall's randomly.  The amount of 
	// rainfall should be dependent on the biome
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int idx = y * terrain.grid_width + x;

			float64 fx = x * width;
			float64 fy = y * height;

			terrain.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
		}
	}

	// Next we will find gradients at each location of rainfalls, and erode dirt.
	// Determine how much soil to move and location to move them.
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int idx = y * terrain.grid_width + x;

			// Get current value
			uint16 rain = terrain.rainfall[idx];
		}
	}

	// Finally evaporation an amount, depositing dirt in location
	for (int y = 0; y < terrain.grid_height; y++) {
		for (int x = 0; x < terrain.grid_width; x++) {
			int idx = y * terrain.grid_width + x;

			apply_evaporation(terrain.rainfall[idx]);
		}
	}
}

void Generator::apply_evaporation(int16 &rainfall) {
	//Applying a very dumb implementation of evaporation.
	// Evaporation will need to take into account other parameters, such as current biome and temperature
	rainfall *= 0.2;
}