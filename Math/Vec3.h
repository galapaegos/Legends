#pragma once

template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vec3 operator * (const T &r) const { return Vec3(x * r, y * r, z * r); }
	Vec3 operator - (const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3& operator *= (const T &r) { x *= r, y *= r, z *= r; return *this; }
	T length2() const { return x * x + y * y + z * z; }
	Vec3& operator /= (const T &r) { x /= r, y /= r, z /= r; return *this; }
	Vec3 cross(const Vec3 &v) const
	{
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}
	Vec3& normalize()
	{
		T len2 = length2();
		if (len2 > 0) {
			T invLen = T(1) / sqrt(len2);
			x *= invLen, y *= invLen, z *= invLen;
		}
		return *this;
	}

	T x, y, z;
};