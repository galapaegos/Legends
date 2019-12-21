#include "stdafx.h"

#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(const int &hash_size) {
	hashsize = hash_size;
	permutation.resize(hash_size*2);
	gradients.resize(hash_size);
}

PerlinNoise::PerlinNoise(const int &hash_size, const uint32 &seed) : PerlinNoise(hash_size) {
	std::mt19937 generator(seed);
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	auto dice = std::bind(distribution, generator);

	for (int i = 0; i < hash_size; i++) {
		float64 theta = acos(2 * dice() - 1);
		float64 phi = 2 * dice() * M_PI;

		float64 x = cos(phi) * sin(theta);
		float64 y = sin(phi) * sin(theta);
		float64 z = cos(theta);

		gradients[i] = glm::vec<3, double>(x, y, z);

		permutation[i] = i;
	}

	std::uniform_int_distribution<unsigned> distributionInt;
	auto diceInt = std::bind(distributionInt, generator);

	for (int i = 0; i < hashsize; i++) {
		std::swap(permutation[i], permutation[diceInt() & (hashsize - 1)]);
	}

	for (int i = 0; i < hashsize; i++) {
		permutation[hashsize + i] = permutation[i];
	}
}

float64 PerlinNoise::noise(const float64 &x, const float64 &y, const float64 &z) {
	int xi0 = int(std::floor(x)) & (hashsize - 1);
	int yi0 = int(std::floor(y)) & (hashsize - 1);
	int zi0 = int(std::floor(z)) & (hashsize - 1);

	int xi1 = (xi0 + 1) & (hashsize - 1);
	int yi1 = (yi0 + 1) & (hashsize - 1);
	int zi1 = (zi0 + 1) & (hashsize - 1);

	float64 tx = x - int(std::floor(x));
	float64 ty = y - int(std::floor(y));
	float64 tz = z - int(std::floor(z));

	float64 u = fade(tx);
	float64 v = fade(ty);
	float64 w = fade(tz);

	float64 x0 = tx;
	float64 x1 = tx - 1;
	float64 y0 = ty;
	float64 y1 = ty - 1;
	float64 z0 = tz;
	float64 z1 = tz - 1;

	float64 a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
	float64 b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
	float64 c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
	float64 d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
	float64 e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
	float64 f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
	float64 g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
	float64 h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

	float64 du = fadeDeriv(tx);
	float64 dy = fadeDeriv(ty);
	float64 dz = fadeDeriv(tz);

	float64 k0 = a;
	float64 k1 = (b - a);
	float64 k2 = (c - a);
	float64 k3 = (e - a);
	float64 k4 = (a + d - b - c);
	float64 k5 = (a + f - b - e);
	float64 k6 = (a + g - c - e);
	float64 k7 = (b + c + e + h - a - d - f - g);

	float64 res = ((k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w) + 1.0) * 0.5;

	return res;
}

float64 PerlinNoise::noise(const float64 &x, const float64 &y, const float64 &z, float64 *gx, float64 *gy, float64 *gz) {
	int xi0 = int(std::floor(x)) & (hashsize - 1);
	int yi0 = int(std::floor(y)) & (hashsize - 1);
	int zi0 = int(std::floor(z)) & (hashsize - 1);

	int xi1 = (xi0 + 1) & (hashsize - 1);
	int yi1 = (yi0 + 1) & (hashsize - 1);
	int zi1 = (zi0 + 1) & (hashsize - 1);

	float64 tx = x - int(std::floor(x));
	float64 ty = y - int(std::floor(y));
	float64 tz = z - int(std::floor(z));

	float64 u = fade(tx);
	float64 v = fade(ty);
	float64 w = fade(tz);

	float64 x0 = tx;
	float64 x1 = tx - 1;
	float64 y0 = ty;
	float64 y1 = ty - 1;
	float64 z0 = tz;
	float64 z1 = tz - 1;

	float64 a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
	float64 b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
	float64 c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
	float64 d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
	float64 e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
	float64 f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
	float64 g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
	float64 h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

	float64 dx = fadeDeriv(tx);
	float64 dy = fadeDeriv(ty);
	float64 dz = fadeDeriv(tz);

	float64 k0 = a;
	float64 k1 = (b - a);
	float64 k2 = (c - a);
	float64 k3 = (e - a);
	float64 k4 = (a + d - b - c);
	float64 k5 = (a + f - b - e);
	float64 k6 = (a + g - c - e);
	float64 k7 = (b + c + e + h - a - d - f - g);

	*gx = dx * (k1 + k4 * v + k5 * w + k7 * v * w);
	*gy = dy * (k2 + k4 * u + k6 * w + k7 * v * w);
	*gz = dz * (k3 + k5 * u + k6 * v + k7 * v * w);


	float64 res = ((k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w) + 1.0) * 0.5;

	return res;
}

float64 PerlinNoise::fade(const float64 &t) const {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float64 PerlinNoise::fadeDeriv(const float64 &t) const {
	return 30 * t * t * (t * (t - 2) + 1);
}

float64 PerlinNoise::lerp(const float64 &t, const float64 &a, const float64 &b) const {
	return a + t * (b - a);
}

float64 PerlinNoise::grad(const int &hash, const float64 &x, const float64 &y, const float64 &z) const {
	int h = hash & 15;

	float64 u = h < 8 ? x : y;
	float64 v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float64 PerlinNoise::gradientDotV(const int32 perm, const float64 &x, const float64 &y, const float64 &z) {
	switch (perm & 15) {
	case 0: return x + y;
	case 1: return -x + y;
	case 2: return x - y;
	case 3: return -x - y;
	case 4: return x + z;
	case 5: return -x + z;
	case 6: return x - z;
	case 7: return -x - z;
	case 8: return y + z;
	case 9: return -y + z;
	case 10: return y - z;
	case 11: return -y - z;
	case 12: return y + x;
	case 13: return -x + y;
	case 14: return -y + z;
	case 15: return -y - z;
	}

	return -1.f;
}

int32 PerlinNoise::hash(const int &x, const int &y, const int &z) const {
	return permutation[permutation[permutation[x] + y] + z];
}
