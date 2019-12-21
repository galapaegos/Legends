#ifndef _Geode_h_
#define _Geode_h_

// Types of rocks found in each layer
enum Geode_Type {
	Geode_Unknown = -1,
	Geode_Iron,
	Geode_Copper,
	Geode_Aluminum,
	Geode_Bronze
};

struct Geode {
	Geode_Type type;
	int32 seed;
};

#endif
