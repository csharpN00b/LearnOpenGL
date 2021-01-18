#pragma once

#include "Base.h"

class Vector3
{
public:
	Vector3(float nx = 0.0f, float ny = 0.0f, float nz = 0.0f) 
		: x(nx), y(ny), z(nz)
	{}

	Vector3(const Vector3& obj)
		: x(obj.x), y(obj.y), z(obj.z)
	{}

	Vector3& operator=(const Vector3& obj)
	{
		x = obj.x;
		y = obj.y;
		z = obj.z;
		return *this;
	}

	void normalize()
	{
		auto magSq = x * x + y * y + z * z;
		if (magSq > 0.0f)
		{
			auto k = 1 / sqrt(magSq);
			x *= k;
			y *= k;
			z *= k;
		}
	}

	void zero()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

public:

	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	Vector3 operator+(const Vector3& obj) const { return Vector3(x + obj.x, y + obj.y, z + obj.z); }
	
	Vector3 operator-(const Vector3& obj) const { return Vector3(x - obj.x, y - obj.y, z - obj.z); }

	float operator*(const Vector3& obj) const { return x * obj.x + y * obj.y + z * obj.z; };

	Vector3 operator*(const float val) const { return Vector3(val * x, val * y, val * z); }

	Vector3 operator/(const float val) const { return Vector3(x / val, y / val, z / val); }

	Vector3& operator+=(const Vector3& obj)
	{
		x += obj.x;
		y += obj.y;
		z += obj.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& obj)
	{
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		return *this;
	}

	Vector3& operator*=(const float val)
	{
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	Vector3& operator/=(const float val)
	{
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}

	bool operator==(const Vector3& obj) const { return IsEqual(x, obj.x) && IsEqual(y, obj.y) && IsEqual(z, obj.z); }

	bool operator!=(const Vector3& obj) const { return !IsEqual(x, obj.x) || !IsEqual(y, obj.y) || !IsEqual(z, obj.z); }

public:
	float x, y, z;
};


inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

inline float VectorMag(const Vector3& obj) { return sqrt(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z); }

inline Vector3 operator*(const float val, const Vector3& obj) { return Vector3(val * obj.x, val * obj.y, val * obj.z); }

inline float Distance(const Vector3& v1, const Vector3& v2)
{
	auto dx = v1.x - v2.x;
	auto dy = v1.y - v2.y;
	auto dz = v1.z - v2.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

//extern const Vector3 ZeroVector;