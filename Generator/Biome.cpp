#include "stdafx.h"

#include "Biome.h"

Biome identify_biome(const float32 &ocean_level, const float32 &elevation, const float32 &temperature, const float32 &moisture) {
	// Aquatic:
	if (elevation < ocean_level) {
		if (ocean_level - elevation < 0.1)
			return Biome::Aquatic_Coral;
		else
			return Biome::Aquatic_Ocean;
	}
	else {
		// Cold regions
		if (temperature < 15.f) {
			if (elevation > 0.6f) {
				// Mountainous tundra region
				return Biome::Tundra_Alpine;
			} else {
				// Cold Tundra region
				return Biome::Tundra_Artic;
			}
		} else {
			// Moist regions
			if (moisture > 50.f) {
				if (temperature > 20) {
					return Biome::Forests_Tropical;
				}
				else if(temperature > 12) {
					return Biome::Forests_Temperate;
				}
				else
				{
					return Biome::Forests_Boreal;
				}
			} else {
				// Dry regions
				if (elevation - ocean_level < 0.1) {
					return Biome::Deserts_Coastal;
				}
				else if (temperature > 20) {
					return Biome::Deserts_Hot_and_Dry;
				}
			}
		}
	}

	return Biome::Biome_Unknown;
}