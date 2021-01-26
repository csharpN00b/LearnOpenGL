#pragma once

#include "Math.h"

namespace Logl
{
	class vec3
	{
	public:
		vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{}

		vec3(float nx, float ny, float nz)
			: x(nx), y(ny), z(nz)
		{}

		vec3(const vec3& obj)
			: x(obj.x), y(obj.y), z(obj.z)
		{}

		vec3& operator=(const vec3& obj)
		{
			if (&obj == this)
				return *this;

			x = obj.x;
			y = obj.y;
			z = obj.z;
			return *this;
		}

		vec3 normalize() const
		{
			auto magSq = x * x + y * y + z * z;
			if (magSq > 0.0f || !IsEqual(magSq, 1.0f))
			{
				auto k = 1 / sqrt(magSq);
				return { x * k, y * k, z * k };
			}

			return { x, y, z };
		}

		void zero()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

	public:
		vec3 operator-() const { return vec3(-x, -y, -z); }

		vec3 operator+(const vec3& obj) const { return vec3(x + obj.x, y + obj.y, z + obj.z); }

		vec3 operator-(const vec3& obj) const { return vec3(x - obj.x, y - obj.y, z - obj.z); }

		// Dot Product
		float operator*(const vec3& obj) const { return x * obj.x + y * obj.y + z * obj.z; };

		vec3 operator*(const float val) const { return vec3(val * x, val * y, val * z); }

		vec3 operator/(const float val) const { return vec3(x / val, y / val, z / val); }

		vec3& operator+=(const vec3& obj)
		{
			x += obj.x;
			y += obj.y;
			z += obj.z;
			return *this;
		}

		vec3& operator-=(const vec3& obj)
		{
			x -= obj.x;
			y -= obj.y;
			z -= obj.z;
			return *this;
		}

		vec3& operator*=(const float val)
		{
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}

		vec3& operator/=(const float val)
		{
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}

		bool operator==(const vec3& obj) const { return IsEqual(x, obj.x) && IsEqual(y, obj.y) && IsEqual(z, obj.z); }

		bool operator!=(const vec3& obj) const { return !IsEqual(x, obj.x) || !IsEqual(y, obj.y) || !IsEqual(z, obj.z); }

	public:
		float x, y, z;
	};

	// non-member functions
	// may be used in vector expression

	inline vec3 CrossProduct(const vec3& v1, const vec3& v2)
	{
		return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	// Magnitude
	inline float VectorMag(const vec3& obj) { return sqrt(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z); }

	inline vec3 operator*(const float val, const vec3& obj) { return vec3(val * obj.x, val * obj.y, val * obj.z); }

	inline float Distance(const vec3& v1, const vec3& v2)
	{
		auto dx = v1.x - v2.x;
		auto dy = v1.y - v2.y;
		auto dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	//extern const vec3 kZeroVector;
}