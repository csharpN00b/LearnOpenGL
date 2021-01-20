#pragma once

#include "Vector3.h"

namespace Logl
{
	struct vec4
	{
		float x, y, z, w;

		vec4(float nx, float ny, float nz, float nw)
			: x(nx), y(ny), z(nz), w(nw)
		{
		}

		vec4(const Vector3& vector)
			: x(vector.x), y(vector.y), z(vector.z), w(1.0f)
		{
		}

		float operator*(const vec4& v2)
		{
			return x * v2.x + y * v2.y + z * v2.z + w * v2.w;
		}
	};


	class Matrix4f
	{
	public:
		Matrix4f()
		{
			Identity();
		}

		Matrix4f(const float array[])
		{
			for (int i = 0; i < 16; i++)
				m_Data[i] = array[i];
		}

		Matrix4f(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44
			)
		{
			m_Data[0] = m11;  m_Data[1] = m12;  m_Data[2] = m13;  m_Data[3] = m14;
			m_Data[4] = m21;  m_Data[5] = m22;  m_Data[6] = m23;  m_Data[7] = m24;
			m_Data[8] = m31;  m_Data[9] = m32;  m_Data[10] = m33; m_Data[11] = m34;
			m_Data[12] = m41; m_Data[13] = m42; m_Data[14] = m43; m_Data[15] = m44;
		}

		void Identity()
		{
			SetRow(1, 1.0f, 0.0f, 0.0f, 0.0f);
			SetRow(2, 0.0f, 1.0f, 0.0f, 0.0f);
			SetRow(3, 0.0f, 0.0f, 1.0f, 0.0f);
			SetRow(4, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4f Transposed() const
		{
			Matrix4f matrix;
			matrix.SetRow(1, GetCol(1));
			matrix.SetRow(2, GetCol(2));
			matrix.SetRow(3, GetCol(3));
			matrix.SetRow(4, GetCol(4));
			return matrix;
		}


		static inline Matrix4f Rotate(float angle, const Vector3& axis)
		{
			auto v = axis;
			v.normalize();

			float cosA = cos(angle);
			float sinA = sin(angle);
			float k = 1 - cosA;
			auto x = v.x;
			auto y = v.y;
			auto z = v.z;
			float xx = v.x * v.x;
			float yy = v.y * v.y;
			float zz = v.z * v.z;
			float xy = v.x * v.y;
			float yz = v.y * v.z;
			float xz = v.x * v.z;

			return {
				k * xx + cosA,		k* xy - z * sinA,	k* xz + y * sinA,	0.0f,
				k* xy + z * sinA,	k * yy + cosA,		k* yz - x * sinA,	0.0f,
				k* xz - y * sinA,	k* yz + x * sinA,	k * zz + cosA,		0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static inline Matrix4f Translate(const Vector3& v)
		{
			return {
				1.0f, 0.0f, 0.0f, v.x,
				0.0f, 1.0f, 0.0f, v.y,
				0.0f, 0.0f, 1.0f, v.z,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		static inline Matrix4f Scale(const Vector3& direction, float k)
		{
			auto v = direction;
			v.normalize();

			float k1 = k - 1;
			float xx = v.x * v.x;
			float yy = v.y * v.y;
			float zz = v.z * v.z;
			float xy = v.x * v.y;
			float yz = v.y * v.z;
			float xz = v.x * v.z;

			return {
				1 + k1 * xx, k1* xy, k1* xz, 0.0f,
				k1 * xy, 1 + k1 * yy, k1* yz, 0.0f,
				k1 * xz, k1* yz, 1 + k1 * zz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static inline Matrix4f Scale(float k)
		{
			return {
				k,0.0f,0.0f, 0.0f,
				0.0f, k,0.0f, 0.0f,
				0.0f, 0.0f,k, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		/*
		static inline Matrix4f OrthoProjection(const Vector3& normal)
		{
			auto v = normal;
			v.normalize();

			float xx = v.x * v.x;
			float yy = v.y * v.y;
			float zz = v.z * v.z;
			float xy = v.x * v.y;
			float yz = v.y * v.z;
			float xz = v.x * v.z;

			return {
				1-xx, -xy, -xz, 0.0f,
				-xy, 1-yy, -yz, 0.0f,
				-xz, -yz, 1 - zz,0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static inline Matrix4f Mirror(const Vector3& axis)
		{
			auto v = axis;
			v.normalize();

			float xx = v.x * v.x;
			float yy = v.y * v.y;
			float zz = v.z * v.z;
			float xy = v.x * v.y;
			float yz = v.y * v.z;
			float xz = v.x * v.z;

			return {
				1-2*xx, -2*xy, -2*xz, 0.0f,
				-2*xy, 1-2*yy, -2*yz, 0.0f,
				-2*xz, -2*yz, 1-2*zz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
		*/

	private:
		void SetRow(int row, float x, float y, float z, float w)
		{
			int i = (row - 1) * 4;
			m_Data[i++] = x;
			m_Data[i++] = y;
			m_Data[i++] = z;
			m_Data[i++] = w;
		}

		void SetRow(int row, vec4 v)
		{
			int i = (row - 1) * 4;
			m_Data[i++] = v.x;
			m_Data[i++] = v.y;
			m_Data[i++] = v.z;
			m_Data[i++] = v.w;
		}

		void SetCol(int col, float v1, float v2, float v3, float v4)
		{
			int i = (col - 1);
			m_Data[i] = v1;
			m_Data[i + 4] = v2;
			m_Data[i + 8] = v3;
			m_Data[i + 12] = v4;
		}

		void SetCol(int col, vec4 v)
		{
			int i = (col - 1);
			m_Data[i] = v.x;
			m_Data[i + 4] = v.y;
			m_Data[i + 8] = v.z;
			m_Data[i + 12] = v.w;
		}

		vec4 GetRow(int row) const
		{
			int i = (row - 1) * 4;
			return { m_Data[i], m_Data[i + 1], m_Data[i + 2], m_Data[i + 3] };
		}

		vec4 GetCol(int col) const
		{
			int i = (col - 1);
			return { m_Data[i], m_Data[i + 4], m_Data[i + 8], m_Data[i + 12] };
		}

		vec4 operator[](const int row) const
		{
			int i = (row - 1) * 4;
			return { m_Data[i], m_Data[i + 1], m_Data[i + 2], m_Data[i + 3] };
		}

	public:
		Matrix4f operator*(const float val) const
		{
			float data[16];
			for (int i = 0; i < 16; i++)
				data[i] = m_Data[i] * val;
			return Matrix4f(data);
		}

		Matrix4f operator*(const Matrix4f& matrix) const
		{
			float data[16];
			int i = 0;
			for (int row = 1; row <= 4; row++)
			{
				for (int col = 1; col <= 4; col++)
				{
					data[i++] = GetRow(row) * matrix.GetCol(col);
				}
			}
			return Matrix4f(data);
		}

		Vector3 operator*(const Vector3& vector) const
		{
			vec4 vec(vector);
			auto k = 1 / (GetRow(4) * vec);
			return Vector3(GetRow(1) * vec*k, GetRow(2) * vec*k, GetRow(3) * vec*k);
		}


	public:
		float m_Data[16];
	};

}