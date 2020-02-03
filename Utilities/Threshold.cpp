#include "stdafx.h"

#include "Utilities/Threshold.h"

void threshold(std::vector<float32> &input, const int &width, const int &height, const float32 &level) {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			if (input[idx] < level)
				input[idx] = 0.;
		}
	}
}
