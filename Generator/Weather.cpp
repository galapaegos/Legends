#include "stdafx.h"

#include "Generator/Weather.h"

void move_weather_patterns(TerrainWorld &world, const float &delta_time) {

}

void apply_rainfall(TerrainWorld &world, const float &delta_time) {

}

void apply_drainage(TerrainWorld &world, const float &delta_time) {

}

void move_water(TerrainWorld &world, const float &delta_time) {

}

void evaporation(TerrainWorld &world, const float &delta_time) {

}

void advance_weather(TerrainWorld &world, const float &time) {
	float delta_time = 0.1;
	while (delta_time < time) {

		// Move weather patterns
		move_weather_patterns(world, delta_time);
		
		// Apply rainfall to each location
		apply_rainfall(world, delta_time);

		// Apply drain water into soil
		apply_drainage(world, delta_time);

		// Move water to adjacent squares
		move_water(world, delta_time);

		// evaptoration 
		evaporation(world, delta_time);
	}
}