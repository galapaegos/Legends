#include "stdafx.h"

#include "Generator.h"

Generator::Generator() {

}

void Generator::generate(const TerrainSettings &ts, const int32 &map_w, const int32 &map_h, const int32 &length, const uint32 &seed) {
	PerlinNoise pn(256, seed);

	settings = ts;

	// Initialize structures here:
	initialize_terrain(world, map_w, map_h);

	// Skipping tectonic plates, etc:

	// Using a fractal generate a reasonable land
	create_land(pn);

	// Create an ocean.
	create_oceans(pn);

	// Using a fractal generate a reasonable moisture
	create_moisture(pn);

	// Combine both land & moisture to identify biomes.
	identify_biomes(pn);
}

void Generator::create_land(PerlinNoise &n) {
	float64 i_width = 1.0/world.world_width;
	float64 i_height = 1.0/world.world_height;

	// set elevation
	// set temperature
	// running rivers
	// formming lakes and minerals

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int32 idx = y * world.world_width + x;

			float64 fx = ((x + settings.elevation.elevation_offset_x) * settings.elevation.elevation_frequency_x)*i_width;
			float64 fy = ((y + settings.elevation.elevation_offset_y) * settings.elevation.elevation_frequency_y)*i_height;

			float64 amplitude = 1.0;
			for (int l = 0; l < settings.elevation.elevation_fractalsum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				world.elevation[idx] += v;

				fx *= settings.elevation.elevation_frequency_multiplier;
				fy *= settings.elevation.elevation_frequency_multiplier;

				amplitude *= settings.elevation.elevation_amplitude_multiplier;
			}

			if (world.elevation[idx] > maxNoiseVal) {
				maxNoiseVal = world.elevation[idx];
			}
		}
	}

	// Normalize our terrain value
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int32 idx = y * world.world_width + x;
			world.elevation[idx] /= maxNoiseVal;
			world.elevation[idx] *= settings.elevation.elevation_range;
			world.elevation[idx] += settings.elevation.elevation_offset;
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
		for (int y = 0; y < world.world_height; y++) {
			for (int x = 0; x < world.world_width; x++) {
				int idx = y * world.world_width + x;

				float64 fx = x * world.world_width;
				float64 fy = y * world.world_height;

				world.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
			}
		}

		// Next we will find gradients at each location of rainfalls, and erode dirt.
		// Determine how much soil to move and location to move them.
		for (int y = 0; y < world.world_height; y++) {
			for (int x = 0; x < world.world_width; x++) {
				int idx = y * world.world_width + x;

				// Get current value
				uint16 rain = world.rainfall[idx];
			}
		}

		// Finally evaporation an amount, depositing dirt in location
		for (int y = 0; y < world.world_height; y++) {
			for (int x = 0; x < world.world_width; x++) {
				int idx = y * world.world_width + x;

				//apply_evaporation(terrain.rainfall[idx]);
			}
		}
	}
}

void Generator::create_moisture(PerlinNoise &n) {
	float64 i_width = 1.0 / world.world_width;
	float64 i_height = 1.0 / world.world_height;

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int32 idx = y * world.world_width + x;

			float64 fx = ((x + settings.moisture.moisture_offset_x) * settings.moisture.moisture_frequency_x)*i_width;
			float64 fy = ((y + settings.moisture.moisture_offset_y) * settings.moisture.moisture_frequency_y)*i_height;

			float64 amplitude = 1.0;
			for (int l = 0; l < settings.moisture.moisture_fractalsum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				world.moisture[idx] += v;

				fx *= settings.moisture.moisture_frequency_multiplier;
				fy *= settings.moisture.moisture_frequency_multiplier;

				amplitude *= settings.moisture.moisture_amplitude_multiplier;
			}

			if (world.moisture[idx] > maxNoiseVal) {
				maxNoiseVal = world.moisture[idx];
			}
		}
	}

	// Normalize our moisture
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int32 idx = y * world.world_width + x;
			world.elevation[idx] /= maxNoiseVal;
		}
	}
}

void Generator::create_temperature(PerlinNoise &n) {
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			float64 fx = x * world.world_width;
			float64 fy = y * world.world_height;

			world.temperature[idx] += int16(n.noise(fx, fy, 0.0));
		}
	}
}

void Generator::create_atmosphere(PerlinNoise &n) {
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			float64 fx = x * world.world_width;
			float64 fy = y * world.world_height;

			world.atmosphere[idx] += int16(n.noise(fx, fy, 0.0));
		}
	}
}

void Generator::create_drainage(PerlinNoise &n) {}

void Generator::identify_biomes(PerlinNoise &n) {
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int32 idx = y * world.world_width + x;

			world.biome[idx] = identify_biome(world.elevation[idx], 0, world.moisture[idx]);

		}
	}
}

void Generator::create_oceans(PerlinNoise &n) {
	// Find min/max of the terrain values
	float64 terrain_min = FLT_MAX, terrain_max = FLT_MIN;

	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			if (terrain_max < world.elevation[idx])
				terrain_max = world.elevation[idx];

			if (terrain_min > world.elevation[idx])
				terrain_min = world.elevation[idx];
		}
	}

	std::vector<int> histogram;
	float64 offset = terrain_max - terrain_min;

	// Find histogram of 256 bins
	histogram.resize(256);

	// Create a histogram of the terrain values.
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			// Normalize terrain value
			float64 t = (world.elevation[idx] - terrain_min) / offset;

			// Place into 256 bin
			histogram[int(t*255)]++;
		}
	}
	
	// Our ocean location is based off this:  25% is the ocean level?
	float64 ocean_level = histogram[64];

	printf("Ocean level = %f\n [%f,%f]\n", (64/255.0)*offset + terrain_min, terrain_min, terrain_max);

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
	float64 width = 1.0 / world.world_width;
	float64 height = 1.0 / world.world_height;

	int rainfall_iteraions = 5;
	int rainfall_amount = 10;

	int drainage = 10;

	// First iteration is to place out a number of rainfall's randomly.  The amount of 
	// rainfall should be dependent on the biome
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			float64 fx = x * width;
			float64 fy = y * height;

			world.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
		}
	}

	// Next we will find gradients at each location of rainfalls, and erode dirt.
	// Determine how much soil to move and location to move them.
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			// Get current value
			uint16 rain = world.rainfall[idx];
		}
	}

	// Finally evaporation an amount, depositing dirt in location
	for (int y = 0; y < world.world_height; y++) {
		for (int x = 0; x < world.world_width; x++) {
			int idx = y * world.world_width + x;

			//apply_evaporation(terrain.rainfall[idx]);
		}
	}
}

void Generator::apply_evaporation(float64 &rainfall) {
	//Applying a very dumb implementation of evaporation.
	// Evaporation will need to take into account other parameters, such as current biome and temperature
	rainfall *= 0.2;
}