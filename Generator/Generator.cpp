#include "stdafx.h"

#include "Generator.h"

#include <glm/glm.hpp>

Generator::Generator() {

}

void Generator::generate(const TerrainSettings &ts) {
	settings = ts;

	PerlinNoise pn(256, settings.map.seed);

	// Initialize structures here:
	initialize_terrain(world, settings.map.world_width, settings.map.world_height);

	// Using a fractal generate a reasonable land
	create_elevation(pn);

	// Create an ocean.
	create_oceans(pn);

	// Creating the atmospheric winds
	create_moisture(pn);

	// Temperature map
	create_temperature(pn);

	// Identify biomes based on temperature, rainfaill and elevation.
	identify_biomes(pn);

	// Derive drainage map from biomes
	create_drainage(pn);
}

void Generator::establish_seeds() { }

void Generator::create_elevation(PerlinNoise &n) {
	float64 i_width = 1.0/world.width;
	float64 i_height = 1.0/world.height;

	float64 maxNoiseVal = FLT_MIN;
	float64 weighting_pass = settings.pass_weight;
	for (int i = 0; i < settings.passes; i++) {
		std::vector<float32> elevation;
		elevation.resize(world.width*world.height, 0.f);

		for (int y = 0; y < world.height; y++) {
			for (int x = 0; x < world.width; x++) {
				int32 idx = y * world.width + x;

				float64 fx = ((x + settings.elevation.offset_x) * settings.elevation.frequency_x)*i_width;
				float64 fy = ((y + settings.elevation.offset_y) * settings.elevation.frequency_y)*i_height;

				float64 amplitude = 1.0;
				for (int l = 0; l < settings.elevation.fractalsum_layers; l++) {
					float64 v = n.noise(fx, fy, 0.f);
					elevation[idx] += v * amplitude;

					fx *= settings.elevation.frequency_multiplier;
					fy *= settings.elevation.frequency_multiplier;

					amplitude *= settings.elevation.amplitude_multiplier;
				}

				if (elevation[idx] > maxNoiseVal) {
					maxNoiseVal = elevation[idx];
				}
			}
		}

		smooth(elevation);

		// 
		float64 terrain_min = FLT_MAX, terrain_max = FLT_MIN;
		for (int y = 0; y < world.height; y++) {
			for (int x = 0; x < world.width; x++) {
				int idx = y * world.width + x;

				if (terrain_max < elevation[idx])
					terrain_max = elevation[idx];

				if (terrain_min > elevation[idx])
					terrain_min = elevation[idx];

				world.elevation[idx] += elevation[idx] * settings.pass_weight;
			}
		}
		printf(" Terrain [%f, %f]\n", terrain_min, terrain_max);

		weighting_pass *= settings.pass_weight;
	}

	// Normalize our terrain value
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int32 idx = y * world.width + x;
			world.elevation[idx] /= maxNoiseVal;
			world.elevation[idx] *= settings.elevation.range;
			world.elevation[idx] += settings.elevation.offset;
		}
	}
}

void Generator::create_moisture(PerlinNoise &n) {
	float64 i_width = 1.0 / world.width;
	float64 i_height = 1.0 / world.height;

	float64 maxNoiseVal = 0.f;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int32 idx = y * world.width + x;

			float64 fx = ((x + settings.moisture.offset_x) * settings.moisture.frequency_x)*i_width;
			float64 fy = ((y + settings.moisture.offset_y) * settings.moisture.frequency_y)*i_height;

			float64 amplitude = 1.0;
			for (int l = 0; l < settings.moisture.fractalsum_layers; l++) {
				float64 v = n.noise(fx, fy, 0.f);
				world.moisture[idx] += v * amplitude;

				fx *= settings.moisture.frequency_multiplier;
				fy *= settings.moisture.frequency_multiplier;

				amplitude *= settings.moisture.amplitude_multiplier;
			}

			if (world.moisture[idx] > maxNoiseVal) {
				maxNoiseVal = world.moisture[idx];
			}
		}
	}

	// Normalize our moisture
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int32 idx = y * world.width + x;
			world.moisture[idx] /= maxNoiseVal;
		}
	}
}

void Generator::create_temperature(PerlinNoise &n) {
	// Setup initial temperature
	const int32 half_height = world.height / 2;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			float64 fx = x * world.width;
			float64 fy = y * world.height;

			float current_temp = settings.temperature.temperature + n.noise(fx, fy, 0.f) * settings.temperature.range;
			world.temperature[idx] = current_temp - (settings.temperature.polar_shift + half_height - y)*settings.temperature.scaling;
		}
	}

	// Find min/max of the temperatures
	int32 temp_min = 500;
	int32 temp_max = -500;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			auto temp = world.temperature[idx];
			if (temp < temp_min)
				temp_min = temp;
			if (temp > temp_max)
				temp_max = temp;
		}
	}

	printf(" Initial temperature range: [%i %i]\n", temp_min, temp_max);

	// Adjust temperature based on elevation:
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			// 
			const float temp_scaling = (world.elevation[idx] - world.sea_level) * settings.temperature.factor;

			world.temperature[idx] += world.temperature[idx]*temp_scaling;
		}
	}

	temp_min = 500;
	temp_max = -500;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			auto temp = world.temperature[idx];
			if (temp < temp_min)
				temp_min = temp;
			if (temp > temp_max)
				temp_max = temp;
		}
	}

	printf(" Scaling temperature range: [%i %i]\n", temp_min, temp_max);
}

void Generator::create_atmosphere(PerlinNoise &n) {
	const int32 prevailing_winds = 5;
	const int32 offset = world.height/prevailing_winds;
	const float32 power = 0.01;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			float32 strength = n.noise(x, y, 0);
			if (offset % 3 == 0) {
				world.wind[idx] = glm::vec4(-1*strength, 1.f - strength, 0, 0);
			}
			else if (offset % 3 == 1) {

				world.wind[idx] = glm::vec4(strength*power, (1.f - strength)*power, 0, 0);
			}
			else if (offset % 3 == 2) {
				world.wind[idx] = glm::vec4(1*strength, 1.f - strength, 0, 0);
			}

			// 
			world.atmosphere[idx] = strength;
		}
	}
}

void Generator::create_drainage(PerlinNoise &n) {
	// [10 -> 150cm]
	const float32 drainage_power = 1.f;
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			float64 fx = x * world.width;
			float64 fy = y * world.height;

			switch (world.biome[idx]) {
			case Biome::Aquatic_Coral:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_coral * drainage_power + settings.drainage.aquatic_coral_offset;
				break;

			case Biome::Aquatic_Estuaries:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_estuaries * drainage_power + settings.drainage.aquatic_estuaries_offset;
				break;

			case Biome::Aquatic_Lakes:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_lakes * drainage_power + settings.drainage.aquatic_lakes_offset;
				break;

			case Biome::Aquatic_Ocean:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_ocean * drainage_power + settings.drainage.aquatic_ocean_offset;
				break;

			case Biome::Aquatic_Ponds:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_ponds * drainage_power + settings.drainage.aquatic_ponds_offset;
				break;

			case Biome::Aquatic_Wetlands:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.aquatic_wetlands * drainage_power + settings.drainage.aquatic_wetlands_offset;
				break;

			case Biome::Deserts_Coastal:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.deserts_coastal * drainage_power + settings.drainage.deserts_coastal_offset;
				break;

			case Biome::Deserts_Cold:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.deserts_cold * drainage_power + settings.drainage.deserts_cold_offset;
				break;

			case Biome::Deserts_Hot_and_Dry:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.deserts_hot_and_dry * drainage_power + settings.drainage.deserts_hot_and_dry_offset;
				break;

			case Biome::Deserts_Semiarid:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.deserts_semiarid * drainage_power + settings.drainage.deserts_semiarid_offset;
				break;

			case Biome::Forests_Boreal:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.forests_boreal * drainage_power + settings.drainage.forests_boreal_offset;
				break;

			case Biome::Forests_Temperate:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.forests_temperate * drainage_power + settings.drainage.forests_temperate_offset;
				break;

			case Biome::Forests_Tropical:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.forests_tropical * drainage_power + settings.drainage.forests_tropical_offset;
				break;

			case Biome::Grasslands_Savanna:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.grasslands_savanna * drainage_power + settings.drainage.grasslands_savanna_offset;
				break;

			case Biome::Grasslands_Steppes:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.grasslands_steppes * drainage_power + settings.drainage.grasslands_steppes_offset;
				break;

			case Biome::Grasslands_Temperate:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.grasslands_temperate * drainage_power + settings.drainage.grasslands_temperate_offset;
				break;

			case Biome::Tundra_Alpine:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.tundra_alpine * drainage_power + settings.drainage.tundra_alpine_offset;
				break;

			case Biome::Tundra_Artic:
				world.drainage[idx] = n.noise(fx, fy, 0.0) * settings.drainage.tundra_artic * drainage_power + settings.drainage.tundra_artic_offset;
				break;
			}
		}
	}
}

void Generator::identify_biomes(PerlinNoise &n) {
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int32 idx = y * world.width + x;

			world.biome[idx] = identify_biome(world.sea_level, world.elevation[idx], world.temperature[idx], world.moisture[idx]);
		}
	}
}

void Generator::create_oceans(PerlinNoise &n) {
	// Find min/max of the terrain values
	float64 terrain_min = FLT_MAX, terrain_max = FLT_MIN;

	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			if (terrain_max < world.elevation[idx])
				terrain_max = world.elevation[idx];

			if (terrain_min > world.elevation[idx])
				terrain_min = world.elevation[idx];
		}
	}

	std::vector<int> histogram;
	float64 offset = terrain_max - terrain_min;

	printf("elevation: %f %f [%f]\n", terrain_min, terrain_max, offset);

	// Find histogram of 256 bins
	histogram.resize(256);

	// Create a histogram of the terrain values.
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			// Normalize terrain value
			float64 t = (world.elevation[idx] - terrain_min) / offset;

			// Place into 256 bin
			histogram[int(t*255)]++;
		}
	}
	
	// Our ocean location is based off this:  25% is the ocean level?
	world.sea_level = histogram[settings.elevation.terrain_ocean_bin];

	printf("Ocean level = %f\n [%f,%f]\n", (settings.elevation.terrain_ocean_bin /255.0)*offset + terrain_min, 
		terrain_min, terrain_max);

	// Run a EDM, which is the ocean tiles.  Don't walk into the land
}

void Generator::create_rivers(PerlinNoise &n) {
	// Running EDM on elevation
	std::vector<float32> grid;
	edm(world.elevation, grid);

	// Figure out the gradients of the terrain.
	calculate_gradients(world.elevation, world.elevation_gradients);

	// Find locations along the ocean, walk them back up mountains.


	// Find locations that are below sea level
}

void Generator::smooth(std::vector<float32> &grid) {
	std::vector<float32> avg;
	avg.resize(world.width*world.height, 0.);

	// Averaging up/down/left/right
	for (int j = 0; j < world.height; j++) {
		for (int i = 0; i < world.width; i++) {
			int idx = j * world.width + i;

			float32 average = 0.f;
			int counter = 5;
			average += grid[idx]*5;
			
			if (i - 1 >= 0) {
				average += grid[j * world.width + i - 1];
				counter ++;
			}
			
			if (i + 1 < world.width) {
				average += grid[j * world.width + i + 1];
				counter++;
			}

			if (j - 1 >= 0) {
				average += grid[(j - 1) * world.width + i];
				counter++;
			}

			if (j + 1 < world.height) {
				average += grid[(j + 1) * world.width + i];
				counter++;
			}

			avg[idx] = average / float(counter);
		}
	}

	grid = avg;
}

void Generator::edm(std::vector<float32> &elevation, std::vector<float32> &edm) {
	const int width = world.width;
	const int height = world.height;
	const float32 sea_level = world.sea_level;

	// Copy over contents.
	edm = elevation;
	threshold(edm, sea_level);

	// Forward/backwards for X axis
	for (int j = 0; j < height; j++) {
		int32 df = width;
		int32 db = width;
		//forwards
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			
			if (edm[idx] != 0)
				df = df + 1;
			else 
				df = 0;

			edm[idx] = df*df;
		}

		// backwards
		for (int i = width - 1; i >= 0; i--) {
			int idx = j * width + i;

			if (edm[idx] != 0)
				db = db + 1;
			else
				db = 0;

			if (edm[idx] > db * db)
				edm[idx] = db * db;
		}
	}

	// Forwards/Backwards for Y axis
	for (int32 i = 0; i < width; i++) {
		float32 *buffer = new float32[height];

		for (int j = 0; j < height; j++) {
			int idx = j * width + i;
			buffer[j] = edm[idx];
		}

		for (int j = 0; j < height; j++) {
			int idx = j * width + i;
			
			float32 d = buffer[j];
			if (d != 0) {
				int32 rMax = int32(sqrt(d));

				int32 rStart = 0;
				int32 rEnd = 0;

				if (rMax < j)
					rStart = rMax;
				else
					rStart = j;
				
				if (rMax < (height - 1 - j))
					rEnd = rMax;
				else
					rEnd = height - 1 - j;

				for (int n = -rStart; n <= rEnd; n++) {
					int32 w = buffer[j + n] + n * n;

					if (w < d)
						d = w;
				}
			}

			edm[idx] = d;
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			edm[idx] = sqrt(edm[idx]);
		}
	}
}

void Generator::threshold(std::vector<float32> &input, const float &threshold) {
	const int width = world.width;
	const int height = world.height;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			if (input[idx] < threshold)
				input[idx] = 0.;
		}
	}
}

void Generator::calculate_gradients(std::vector<float32> &input, std::vector<glm::vec3> gradients) {
	const int width = world.width;
	const int height = world.height;

	gradients.resize(width*height);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			gradients[idx] = glm::vec3(0, 0, input[idx]);
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			glm::vec3 v1 = gradients[idx];
			glm::vec3 v2;
			glm::vec3 v3;

			if (i + 1 < width)
				v2 = gradients[j * width + i + 1];
			else
				v2 = gradients[j * width + i - 1];

			if (j + 1 < height)
				v3 = gradients[(j + 1) * width + i];
			else
				v3 = gradients[(j - 1) * width + i];
			
			glm::vec3 u = v2 - v1;
			glm::vec3 v = v3 - v1;

			glm::vec3 n = glm::cross(u, v);

			gradients[idx] = glm::normalize(gradients[idx] + n);
		}
	}
}

/*
void Generator::progress_step(PerlinNoise &n) {
	float64 width = 1.0 / world.width;
	float64 height = 1.0 / world.height;

	int rainfall_iteraions = 5;
	int rainfall_amount = 10;

	int drainage = 10;

	// First iteration is to place out a number of rainfall's randomly.  The amount of 
	// rainfall should be dependent on the biome
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			float64 fx = x * width;
			float64 fy = y * height;

			world.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
		}
	}

	// Next we will find gradients at each location of rainfalls, and erode dirt.
	// Determine how much soil to move and location to move them.
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			// Get current value
			uint16 rain = uint16(world.rainfall[idx]);
		}
	}

	// Finally evaporation an amount, depositing dirt in location
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			//apply_evaporation(terrain.rainfall[idx]);
		}
	}
}
*/

/*
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
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			float64 fx = x * world.width;
			float64 fy = y * world.height;

			world.rainfall[idx] += int16(n.noise(fx, fy, 0.0)*rainfall_amount);
		}
	}

	// Next we will find gradients at each location of rainfalls, and erode dirt.
	// Determine how much soil to move and location to move them.
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			// Get current value
			uint16 rain = uint16(world.rainfall[idx]);
		}
	}

	// Finally evaporation an amount, depositing dirt in location
	for (int y = 0; y < world.height; y++) {
		for (int x = 0; x < world.width; x++) {
			int idx = y * world.width + x;

			//apply_evaporation(terrain.rainfall[idx]);
		}
	}
}	
*/
