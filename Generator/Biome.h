#ifndef _TerrainBiome_h_
#define _TerrainBiome_h_

enum Biome {
	Terrestrial_Temperate,
	Terrestrial_Coniferous,
	Terrestrial_Woodland,
	Terrestrial_Tundra,
	Terrestrial_Grassland,
	Terrestrial_Desert,
	Terrestrial_TropicalSavanna,
	Terrestrial_TropicalForest,
	Marine_Plankton,
	Marine_Balanoid,
	Marine_Pelecypod,
	Marine_Coral,
};


Biome identify_biome(const double &elevation, const double &temperature, const double &moisture);

#endif
