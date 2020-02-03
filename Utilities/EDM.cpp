#include "stdafx.h"

#include "Utilities/EDM.h"
#include "Utilities/Threshold.h"

void edm(std::vector<float32> &input, const int &width, const int &height, std::vector<float32> &grid, const float32 &level) {
	// Copy over contents.
	grid = input;
	threshold(grid, width, height, level);

	// Forward/backwards for X axis
	for (int j = 0; j < height; j++) {
		int32 df = width;
		int32 db = width;
		//forwards
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;


			if (grid[idx] != 0)
				df = df + 1;
			else
				df = 0;

			grid[idx] = float32(df * df);
		}

		// backwards
		for (int i = width - 1; i >= 0; i--) {
			int idx = j * width + i;

			if (grid[idx] != 0)
				db = db + 1;
			else
				db = 0;

			float32 db2 = float32(db * db);
			if (grid[idx] > db2)
				grid[idx] = db2;
		}
	}

	// Forwards/Backwards for Y axis
	for (int32 i = 0; i < width; i++) {
		float32 *buffer = new float32[height];

		for (int j = 0; j < height; j++) {
			int idx = j * width + i;
			buffer[j] = grid[idx];
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
					int32 w = int32(buffer[j + n]) + n * n;

					if (w < int32(d))
						d = float32(w);
				}
			}

			grid[idx] = d;
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int idx = j * width + i;

			grid[idx] = sqrt(grid[idx]);
		}
	}
}
