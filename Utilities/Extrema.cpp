#include "stdafx.h"

void extrema(std::vector<float32> &input, float *minimum, float *maximum) {
	*minimum = FLT_MAX;
	*maximum = -FLT_MIN;
	for (int i = 0; i < input.size(); i++) {
		auto v = input[i];
		if (v < *minimum)
			*minimum = v;
		if (v > *maximum)
			*maximum = v;
	}
}
