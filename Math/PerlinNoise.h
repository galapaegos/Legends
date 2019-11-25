#pragma once

#include "stdafx.h"

#include <glm/vec3.hpp>

// Parts of this code were used from www.scratchapixel.com/
class PerlinNoise {
public:
	PerlinNoise(const int &hash_size, const uint32 &seed);

	float64 noise(const float64 &x, const float64 &y, const float64 &z);
	float64 noise(const float64 &x, const float64 &y, const float64 &z, float64 *gx, float64 *gy, float64 *gz);

private:
	PerlinNoise(const int &hash_size);

	float64 fade(const float64 &t) const;
	float64 fadeDeriv(const float64 &t) const;
	float64 lerp(const float64 &t, const float64 &a, const float64 &b) const;
	float64 grad(const int &hash, const float64 &x, const float64 &y, const float64 &z) const;

	float64 gradientDotV(const int32 perm, const float64 &x, const float64 &y, const float64 &z);

	int32 hash(const int &x, const int &y, const int &z) const;

	std::vector<int32> permutation;
	std::vector<glm::vec<3, double>> gradients;

	int hashsize;
};