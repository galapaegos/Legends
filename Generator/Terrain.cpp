#include "stdafx.h"

#include "Terrain.h"

#include "TiffObject.h"

void initialize_terrain(TerrainWorld &t, const int &width, const int &height) {
	t.width = width;
	t.height = height;

	// Clear before initializing:
	t.elevation.clear();
	t.moisture.clear();
	t.biome.clear();

	t.rainfall.clear();
	t.temperature.clear();
	t.atmosphere.clear();
	t.drainage.clear();

	t.elevation_gradients.clear();

	t.geode.clear();

	// initialize map
	const int total = width * height;
	t.elevation.resize(total, 0);
	t.moisture.resize(total, 0);
	t.biome.resize(total, Biome::Biome_Unknown);

	t.rainfall.resize(total, 0);
	t.temperature.resize(total, 0);
	t.atmosphere.resize(total, 0);
	t.drainage.resize(total, 0);

	t.elevation_gradients.resize(total);

	t.wind.resize(total);

	t.geode.resize(total, Geode_Type::Geode_Unknown);

	t.sea_level = 0.f;
	t.world_age = 0.f;
}

void convert_to_windmap(std::vector<glm::vec4> &input, const int &width, const int &height, 
	std::vector<glm::vec<4, uint8>> &windmap) {
	// Initialize the heatmap
	windmap.resize(width*height);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			windmap[idx] = glm::normalize(input[idx]) * glm::vec4(255);
		}
	}
}

void convert_to_heatmap(std::vector<float32> &input, const int &width, const int &height, 
	std::vector<glm::vec<4, uint8>> &heatmap) {
	// Initialize the heatmap
	heatmap.resize(width*height);

	// -50 to 50 C
	// These are in F
	// -30 - purple
	// -20 - blue
	// -10 - baby blue
	//   0 - sky blue
	//  10 - green
	//  20 - yellow
	//  30 - orange
	//  40 - red
	//  50 - blood red

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			float32 temperature = input[idx];

			if (temperature > 50)
				heatmap[idx] = glm::vec<4, uint8>(128, 0, 0, 255);
			else if(temperature > 40)
				heatmap[idx] = glm::vec<4, uint8>(255, 0, 0, 255);
			else if(temperature > 30)
				heatmap[idx] = glm::vec<4, uint8>(255, 255, 0, 255);
			else if (temperature > 20)
				heatmap[idx] = glm::vec<4, uint8>(128, 128, 0, 255);
			else if (temperature > 10)
				heatmap[idx] = glm::vec<4, uint8>(0, 255, 0, 255);
			else if (temperature > 0)
				heatmap[idx] = glm::vec<4, uint8>(0, 128, 255, 255);
			else if (temperature > -10)
				heatmap[idx] = glm::vec<4, uint8>(0, 128, 128, 255);
			else if (temperature > -20)
				heatmap[idx] = glm::vec<4, uint8>(0, 0, 255, 255);
			else
				heatmap[idx] = glm::vec<4, uint8>(255, 0, 255, 255);
		}
	}
}

void convert_to_drainage(std::vector<float32> &input, const int &width, const int &height,
	std::vector<glm::vec<4, uint8>> &drainage_map) {
	drainage_map.resize(width*height);

	// Drainage:
	// 100 - deep green
	//  80 - green
	//  60 - light green
	//  40 - yellow
	//  20 - light yellow
	//   0 - white
	for (int32 j = 0; j < height; j++) {
		for (int32 i = 0; i < width; i++) {
			int idx = j * width + i;

			float32 drainage = input[idx];
			if (drainage > 100)
				drainage_map[idx] = glm::vec<4, uint8>(0, 128, 0, 255);
			else if (drainage > 80)
				drainage_map[idx] = glm::vec<4, uint8>(0, 255, 0, 255);
			else if (drainage > 60)
				drainage_map[idx] = glm::vec<4, uint8>(0, 255, 128, 255);
			else if (drainage > 40)
				drainage_map[idx] = glm::vec<4, uint8>(255, 255, 0, 255);
			else if (drainage > 20)
				drainage_map[idx] = glm::vec<4, uint8>(255, 255, 128, 255);
			else
				drainage_map[idx] = glm::vec<4, uint8>(255, 255, 255, 255);
		}
	}
}

void renormalize(std::vector<float32> &input, const int &width, const int &height) {
	float32 input_min = FLT_MAX;
	float32 input_max = -FLT_MIN;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			float32 value = input[idx];
			if (value < input_min)
				input_min = value;
			if (value > input_max)
				input_max = value;
		}
	}

	const float32 total = input_max - input_min;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			float32 value = input[idx];
			input[idx] = (value - input_min) / total;
		}
	}
}

void save_world(TerrainWorld &t, const std::string &path) {
	// Make directory if it doesn't exist
	if (!directoryExists(path))
		createDirectory(path);

	const int width = t.width;
	const int height = t.height;

	//std::vector<glm::vec<4, uint8>> heatmap;
	//convert_to_heatmap(t.temperature, width, height, heatmap);

	//std::vector<glm::vec<4, uint8>> drainage;
	//convert_to_drainage(t.drainage, width, height, drainage);

	std::vector<glm::vec<4, uint8>> windmap;
	convert_to_windmap(t.wind, width, height, windmap);

	write_tiff_file(path + "\\elevation.tiff", width, height, t.elevation);
	write_tiff_file(path + "\\heatmap.tiff", width, height, t.temperature);
	write_tiff_file(path + "\\drainage.tiff", width, height, t.drainage);
	write_tiff_file(path + "\\biomes.tiff", width, height, t.biome);
	write_tiff_file(path + "\\rainfall.tiff", width, height, t.rainfall);
	write_tiff_file(path + "\\windmap.tiff", width, height, windmap);
	write_tiff_file(path + "\\moisture.tiff", width, height, t.moisture);
	write_tiff_file(path + "\\atmosphere.tiff", width, height, t.atmosphere);
}

void print_settings(TerrainSettings &ts) {
	printf("Map: [%i %i] [%i %i] [%i %i]\n", ts.map.world_width, ts.map.world_height,
		ts.map.region_width, ts.map.region_height, ts.map.local_width, ts.map.local_height);
	printf("  seed - %i\n", ts.map.seed);
	
	printf("Elevation: \n");
	printf("  offset: %f %f\n", ts.elevation.offset_x, ts.elevation.offset_y);
	printf("  frequency: [%f %f] %f %f %f\n", ts.elevation.frequency_x, ts.elevation.frequency_y, ts.elevation.frequency, 
		ts.elevation.frequency_multiplier, ts.elevation.amplitude_multiplier);
	printf("  layers: %i\n", ts.elevation.fractalsum_layers);
	printf("  terrain: %i %i\n", ts.elevation.offset, ts.elevation.range);
	printf("  ocean bin: %i\n", ts.elevation.terrain_ocean_bin);

	printf("Moisture: \n");
	printf("  offset: %f %f\n", ts.moisture.offset_x, ts.moisture.offset_y);
	printf("  frequency: [%f %f] %f %f %f\n", ts.moisture.frequency_x, ts.moisture.frequency_y, ts.moisture.frequency,
		ts.moisture.frequency_multiplier, ts.moisture.amplitude_multiplier);
	printf("  layers: %i\n", ts.moisture.fractalsum_layers);

	printf("Temperature: \n");
	printf("  polor position: %i\n", ts.temperature.polar_shift);
	printf("  temperature: %f:%f [%f]\n", ts.temperature.temperature, ts.temperature.range, ts.temperature.scaling);
}
