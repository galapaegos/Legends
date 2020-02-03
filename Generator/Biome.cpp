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
		if (temperature < 0.f) {
			if (elevation > 0.6f) {
				// Mountainous tundra region
				return Biome::Tundra_Alpine;
			} else {
				// Cold Tundra region
				return Biome::Tundra_Artic;
			}
		} else {
			if (temperature > 20.f && moisture < 5.f)
				return Biome::Deserts_Semiarid;
			else if (temperature > 20.f && moisture < 15.f)
				return Biome::Deserts_Hot_and_Dry;
			else if (temperature > 20.f && moisture < 20.f)
				return Biome::Deserts_Hot_and_Dry;
			else if (temperature > 20.f && moisture > 20.f)
				return Biome::Forests_Tropical;
			else if (temperature < 20.f && moisture > 75.f)
				return Biome::Forests_Temperate;
			else if (temperature < 10.f && moisture < 100.f)
				return Biome::Forests_Boreal;
		}

		return Biome::Grasslands_Temperate;
	}

	return Biome::Biome_Unknown;
}