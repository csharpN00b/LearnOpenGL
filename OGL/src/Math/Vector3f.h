#pragma once

#include "Core/Base.h"
#include "Math.h"

namespace Logl
{
	struct vec3
	{
		// -- Data --
#if 1
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
		};
#else
		union { float x, r; };
		union { float y, g; };
		union { float z, b; };
#endif
		// -- Implicit basic constructors --

		vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{}

		vec3(const vec3& v)
			: x(v.x), y(v.y), z(v.z)
		{}

		// -- Explicit basic constructors --

		explicit vec3(float n)
			: x(n), y(n), z(n)
		{}

		vec3(float nx, float ny, float nz)
			: x(nx), y(ny), z(nz)
		{}

		// -- Component accesses --

		float& operator[](unsigned int i)
		{
			ASSERT(i < 3);
			switch (i)
			{
			default:
			case 0: return x;
			case 1: return y;
			case 2: return z;
			}
		}

		float const& operator[](unsigned int i) const
		{
			ASSERT(i < 3);
			switch (i)
			{
			default:
			case 0: return x;
			case 1: return y;
			case 2: return z;
			}
		}

		// -- Unary arithmetic operators --

		vec3& operator=(const vec3& v)
		{
			if (&v == this)
				return *this;

			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		vec3& operator+=(const vec3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		vec3& operator-=(const vec3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		vec3& operator*=(float k)
		{
			x *= k;
			y *= k;
			z *= k;
			return *this;
		}

		vec3& operator/=(float k)
		{
			x /= k;
			y /= k;
			z /= k;
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
	};

	// -- Unary operators --

	inline vec3 operator-(const vec3& v) { return vec3(-v.x, -v.y, -v.z); }

	// -- Binary operators --

	inline vec3 operator+(const vec3& v1, const vec3& v2) { return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

	inline vec3 operator-(const vec3& v1, const vec3& v2) { return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }

	inline vec3 operator*(const vec3& v1, const vec3& v2) { return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

	inline vec3 operator/(const vec3& v1, const vec3& v2) { return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }


	inline vec3 operator*(float k, const vec3& v) { return vec3(k * v.x, k * v.y, k * v.z); }

	inline vec3 operator*(const vec3& v, float k) { return vec3(k * v.x, k * v.y, k * v.z); }

	inline vec3 operator/(const vec3& v1, float k) { return vec3(v1.x / k, v1.y / k, v1.z / k); }

	// -- Boolean operators --

	inline bool operator==(const vec3& v1, const vec3& v2) { return IsEqual(v1.x, v2.x) && IsEqual(v1.y, v2.y) && IsEqual(v1.z, v2.z); }

	inline bool operator!=(const vec3& v1, const vec3& v2) { return !IsEqual(v1.x, v2.x) || !IsEqual(v1.y, v2.y) || !IsEqual(v1.z, v2.z); }

	// non-member functions
	// may be used in vector expression

	inline float dot(const vec3& v1, const vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; };

	inline vec3 cross(const vec3& v1, const vec3& v2)
	{
		return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	inline float length(const vec3& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

	inline float distance(const vec3& v1, const vec3& v2)
	{
		auto dx = v1.x - v2.x;
		auto dy = v1.y - v2.y;
		auto dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	//extern const vec3 kZeroVector;
}