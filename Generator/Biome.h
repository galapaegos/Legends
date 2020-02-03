#ifndef _TerrainBiome_h_
#define _TerrainBiome_h_

// Biome descriptions
// https://ucmp.berkeley.edu/glossary/gloss5/biome/grassland.html
enum Biome {
	Biome_Unknown = 0,

	// Locations - warm shallow waters, found near barriers along continents, fringing islands
	// Temperatures - 
	// atolls.
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Coral,			// 1

	// Locations - where freshwater streams or rivers merge with the ocean
	// Temperatures -
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Estuaries,		// 2


	// Temperatures - 3C
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Ocean,			// 3

	// Temperatures -
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Lakes,			// 4

	// Temperatures -
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Ponds,			// 5

	// Temperatures -
	// Precipitation -
	// Soil - 
	// Flora - 
	// Fauna - 
	Aquatic_Wetlands,		// 6

	// Temeprature - 20-25C annual, 43.5-49C high and as low as -18C at night
	// Precipitation - very low or concentrated in short bursts, 1.5cm to 28cm a year
	// Soil - course-textured, shallow, rocky, gravely with good drainage and no subsurface water
	// Canopy - very rare
	// Flora - ground-hugging shrubs, short woody trees, leaves are replete with water-conserving 
	// characteristics. yuccas, ocotillo, turpentine bush, prickly pears, false mesquite, sotol, 
	// ephedras, agaves, brittlebush
	// Fauna - small nocturnal carnivores; burrowers, kangaroo rats. insects, arachnids, reptiles,
	// birds
	Deserts_Hot_and_Dry,	// 7

	// Temperatures - 21-27 in summer, max is 38C, nights get dwn to 10C
	// Precipitation - 2-4cm annually
	// Soil - sandy, fine-textured rock fragments, gravel or sand, with good drainage. No 
	// subsurface water
	// Flora - spiny plants, plants have silvery or glossly leaves allowing them to reflect
	// more radiant energy. Creosote bush, bur sage, white thorn, cat claw, mesquite, brittle bushes
	// lyciums, jujube
	// Fauna - Insects, jack rabbits, kangaroo rats, rabbits, skunks, grasshoppers, ants, lizards,
	// snakes, burrowing owls, thrasher
	Deserts_Semiarid,		// 8

	// Temperatures - 13-24C summer, winter is -4 to 5C and below.
	// Precipitation - 8-13cm annually
	// Soil - fine-textured with moderate salt content. fairly porous with good drainage.
	// Flora - plants have extensive root systems close to the surface. salt bush, buckwheat bush,
	// black bush, rice grass, little leaf horsebrush, black sage, chrysothamnus
	// Fauna - toads, fairy shrimps, insects, mammals (coyote & badger), great horned owl, golden
	// eagle, bald eagle, lizards and snakes
	Deserts_Coastal,		// 9

	// Temperatures - 21-26C summer, -2-4C winter
	// Precipitation - 15-26cm annual precipitation
	// Soil - heavy, silty, salty. contains alluvial fans where soil is relatively porous and 
	// drainage is good so that most salt leaches out
	// Flora - ~10% ground covered, but where sagebush it is ~85%. plants are 15cm to 122cm. most
	// plants are deciduous, most having spiny leaves
	// Fauna - jack rabbits, badgers, kit fox, coyote, lizards, deer
	Deserts_Cold,			// 10

	// Temperature - 20-25C, ~temperatures differ no more than 5C
	// Precipitation - evenly distributed throughout year, over 20 cm
	// Soil - nutrient poor and acidic; decomposition is rapid and soils are subject to heavy leaching
	// Canopy - multilayered and continuous, allowing little light penetration
	// Flora - Many types of trees; Trees are 25-35m tall, butressed trunks and shallow roots,
	// mostly evergreen w/ large dark green leaves. 
	// orchids, bromeliads, vines, ferns, mosses, palms
	// Fauna - birds, bats, small mammals, insects
	Forests_Tropical,		// 11

	// Temperature - varies from -30C to 30C
	// Precipitation - 75-150cm distributed evenly throughout year
	// Soil - fertile, enriched with decaying litter
	// Canopy - moderately dense, allows light to penetrate through
	// Flora - 3-4 tree species, broad leaves lost annually like oak, hickory, beech, hemlock,
	// maple, basswood, cottonwood, elm, willow, spring-flowering herbs
	// Fauna - squirrels, rabbits, skunks, birds, deer, mountain lion, bobcat, timber wolf, fox,
	// blackbear
	Forests_Temperate,		// 12

	// Temperature - very low
	// Precipitation - primarily snow, 40-100cm annually
	// Soil - thin, nutrient-poor, acidic
	// Canopy - low light penetration, understory is limited
	// Flora - consists of cold-tolerant evergreen conifers with needle-like leaves like pine,
	// fir, spruce
	// Fauna - woodpeckers, hawks, moose, bear, weasel, lynx, fox, wolf, deer, hares, chipmunks,
	// shrews, bats
	Forests_Boreal,			// 13

	// grassland scattered with individual trees; 
	// warm or hot climates, 
	// anual rainfall is 50.8 to 127 cm per year, in 6-8 months of the year
	// porous soil, rapid drainage of water
	// thin layer of humus (orgaic portion of soil created by partial decomposition of plant or animal matter
	// has both dry and rainy season, seasonal fires
	// giraffes, zebras, buffaloes, kangaroos, mice, moles, gophers, groudn squirrels, snakes, worms,
	// termites, beetles, lions, lepoards, hyenas, elephants
	Grasslands_Savanna,		// 14
	
	// grasses dominat vegetation, trees and shrubs are absent
	// annual rainfall is 50.8 to 88.9cm
	// summer 38C, winter -40C
	// gazelles, zebras, rhinoceroses, wild horses, lions, wolves, prairie dogs,  jack rabbits,
	// deer, mic, coyotes, foxes, skunks, badgers, blackbirds, grouses, meadowlarks, quails, sparrows,
	// hawks, owls, snakes, grasshoppers, leafhoppers, spiders
	Grasslands_Temperate,	// 15
	
	// dry areas of grasslands with hot summers and cold winters
	// annual rainfall 25.4 to 50.8cm per year
	// tall plants, blue grama, buffalo grass, cacti, sagesbrush, speargrass, sunflowers
	// badgers, hawks, owls, snakes
	// Loose soil can cause dust storms
	Grasslands_Steppes,		// 16

	Tundra_Artic,			// 17
	Tundra_Alpine,			// 18
};


Biome identify_biome(const float32 &ocean_level, const float32 &elevation, const float32 &temperature, const float32 &moisture);

#endif
