#include "stdafx.h"

#include "Generator.h"

Generator::Generator() {

}

void Generator::generate(const int32 &map_w, const int32 &map_h, const int32 &length, const uint32 &seed) {
	PerlinNoise pn(256, seed);

	grid_width = map_w;
	grid_height = map_h;

	terrain.resize(map_w * map_h);

	create_land(pn);
	create_ocean(pn);
}

void Generator::create_land(PerlinNoise &n) {

	//Generate world our grid, provide a starting initial value
	float64 offset_x = 0.0;
	float64 offset_y = 0.0;
	float64 frequency_x = 5.0;
	float64 frequency_y = 5.0;

	float64 f_gridwidth = 1.0 / float64(grid_width);
	float64 f_gridheight= 1.0 / float64(grid_height);

	// Using a Fractal sum for utilizing different layers
	int number_fractalSum_layers = 5;

	float64 frequency = 0.02;
	float64 frequencyMultiplier = 1.8;
	float64 amplitudeMultiplier = 0.35;

	// set elevation
	// set temperature
	// running rivers
	// formming lakes and minerals

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int32 idx = y * grid_width + x;

			float64 fx = ((x + offset_x) * frequency_x)*f_gridwidth;
			float64 fy = ((y + offset_y) * frequency_y)*f_gridheight;

			float64 amplitude = 1.0;
			for (int l = 0; l < number_fractalSum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				terrain[idx].terrain_value += v;

				fx *= frequencyMultiplier;
				fy *= frequencyMultiplier;

				amplitude *= amplitudeMultiplier;
			}

			if (terrain[idx].terrain_value > maxNoiseVal) {
				maxNoiseVal = terrain[idx].terrain_value;
			}
		}
	}

	// Range of values
	int terrain_elevation_range = 200;

	// Natural offset
	int terrain_elevation_offset = 50;

	// Normalize our terrain value
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int32 idx = y * grid_width + x;
			terrain[idx].terrain_value /= maxNoiseVal;
			terrain[idx].terrain_value *= terrain_elevation_range;
			terrain[idx].terrain_value += terrain_elevation_offset;
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
		for (int y = 0; y < grid_height; y++) {
			for (int x = 0; x < grid_width; x++) {
				int idx = y * grid_width + x;

				float64 fx = x * f_gridwidth;
				float64 fy = y * f_gridheight;

				terrain[idx].terrain_rainfall += n.noise(fx, fy, 0.0)*rainfall_amount;
			}
		}

		// Next we will find gradients at each location of rainfalls, and erode dirt.
		// Determine how much soil to move and location to move them.
		for (int y = 0; y < grid_height; y++) {
			for (int x = 0; x < grid_width; x++) {
				int idx = y * grid_width + x;

				// Get current value
				uint16 rain = terrain[idx].terrain_rainfall;
			}
		}

		// Finally evaporation an amount, depositing dirt in location
		for (int y = 0; y < grid_height; y++) {
			for (int x = 0; x < grid_width; x++) {
				int idx = y * grid_width + x;

				apply_evaporation(terrain[idx].terrain_rainfall);
			}
		}
	}
}
void Generator::create_ocean(PerlinNoise &n) {
	// Find min/max of the terrain values
	float64 terrain_min = FLT_MAX, terrain_max = FLT_MIN;

	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int idx = y * grid_width + x;

			if (terrain_max < terrain[idx].terrain_value)
				terrain_max = terrain[idx].terrain_value;

			if (terrain_min > terrain[idx].terrain_value)
				terrain_min = terrain[idx].terrain_value;
		}
	}

	std::vector<int> histogram;
	float64 offset = terrain_max - terrain_min;

	// Find histogram of 256 bins
	histogram.resize(256);

	// Create a histogram of the terrain values.
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int idx = y * grid_width + x;

			// Normalize terrain value
			float64 t = (terrain[idx].terrain_value - terrain_min) / offset;

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
	float64 f_gridwidth = 1.0 / float64(grid_width);
	float64 f_gridheight = 1.0 / float64(grid_height);

	int rainfall_iteraions = 5;
	int rainfall_amount = 10;

	int drainage = 10;

	// First iteration is to place out a number of rainfall's randomly.  The amount of 
	// rainfall should be dependent on the biome
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int idx = y * grid_width + x;

			float64 fx = x * f_gridwidth;
			float64 fy = y * f_gridheight;

			terrain[idx].terrain_rainfall += n.noise(fx, fy, 0.0)*rainfall_amount;
		}
	}

	// Next we will find gradients at each location of rainfalls, and erode dirt.
	// Determine how much soil to move and location to move them.
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int idx = y * grid_width + x;

			// Get current value
			uint16 rain = terrain[idx].terrain_rainfall;
		}
	}

	// Finally evaporation an amount, depositing dirt in location
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			int idx = y * grid_width + x;

			apply_evaporation(terrain[idx].terrain_rainfall);
		}
	}
}

void Generator::apply_evaporation(int16 &rainfall) {
	//Applying a very dumb implementation of evaporation.
	// Evaporation will need to take into account other parameters, such as current biome and temperature
	rainfall *= 0.2;
}