#pragma once

#include <algorithm>

#include "Vector3f.h"

namespace Logl
{

#define MAT4_VALUE(matrix, i, j) matrix.m_Data[4*(i)+(j)]

	class mat4
	{
	private:
		struct vec4
		{
			float x, y, z, w;

			vec4(float nx, float ny, float nz, float nw)
				: x(nx), y(ny), z(nz), w(nw)
			{
			}

			vec4(const vec3& vector)
				: x(vector.x), y(vector.y), z(vector.z), w(1.0f)
			{
			}

			float operator*(const vec4& v2)
			{
				return x * v2.x + y * v2.y + z * v2.z + w * v2.w;
			}
		};

	public:
		mat4()
		{
			Identity();
		}

		mat4(const float array[])
		{
			for (int i = 0; i < 16; i++)
				m_Data[i] = array[i];
		}

		mat4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44
			)
		{
			m_Data[0] = m11;  m_Data[1] = m21;  m_Data[2] = m31;  m_Data[3] = m41;
			m_Data[4] = m12;  m_Data[5] = m22;  m_Data[6] = m32;  m_Data[7] = m42;
			m_Data[8] = m13;  m_Data[9] = m23;  m_Data[10] = m33; m_Data[11] = m43;
			m_Data[12] = m14; m_Data[13] = m24; m_Data[14] = m34; m_Data[15] = m44;
		}

		void Identity()
		{
			SetRow(1, 1.0f, 0.0f, 0.0f, 0.0f);
			SetRow(2, 0.0f, 1.0f, 0.0f, 0.0f);
			SetRow(3, 0.0f, 0.0f, 1.0f, 0.0f);
			SetRow(4, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		mat4 Transposed() const
		{
			mat4 matrix;
			matrix.SetRow(1, GetCol(1));
			matrix.SetRow(2, GetCol(2));
			matrix.SetRow(3, GetCol(3));
			matrix.SetRow(4, GetCol(4));
			return matrix;
		}

		friend mat4 Inverse(mat4 matrix);

		const float* ValuePtr() const { return &m_Data[0]; }


	public:
		static inline mat4 Rotate(float angle, const vec3& axis)
		{
			auto v = axis.normalize();

			float cosA = cosf(angle);
			float sinA = sinf(angle);
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

			return mat4(
				k * xx + cosA,		k* xy - z * sinA,	k* xz + y * sinA,	0.0f,
				k* xy + z * sinA,	k * yy + cosA,		k* yz - x * sinA,	0.0f,
				k* xz - y * sinA,	k* yz + x * sinA,	k * zz + cosA,		0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Translate(const vec3& v)
		{
			return mat4(
				1.0f, 0.0f, 0.0f, v.x,
				0.0f, 1.0f, 0.0f, v.y,
				0.0f, 0.0f, 1.0f, v.z,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Scale(const vec3& direction, float k)
		{
			auto v = direction.normalize();

			float k1 = k - 1;
			float xx = v.x * v.x;
			float yy = v.y * v.y;
			float zz = v.z * v.z;
			float xy = v.x * v.y;
			float yz = v.y * v.z;
			float xz = v.x * v.z;

			return mat4(
				1 + k1 * xx, k1* xy, k1* xz, 0.0f,
				k1 * xy, 1 + k1 * yy, k1* yz, 0.0f,
				k1 * xz, k1* yz, 1 + k1 * zz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Scale(float k)
		{
			return mat4(
				k,0.0f,0.0f, 0.0f,
				0.0f, k,0.0f, 0.0f,
				0.0f, 0.0f,k, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Scale(float kx, float ky, float kz)
		{
			return mat4(
				kx, 0.0f, 0.0f, 0.0f,
				0.0f, ky, 0.0f, 0.0f,
				0.0f, 0.0f, kz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			auto rw = 1.0f / (right - left);
			auto rh = 1.0f / (top - bottom);
			auto rl = 1.0f / (zFar - zNear);

			return mat4(
				2.0f*rw, 0.0f, 0.0f, -(right + left) * rw,
				0.0f, 2.0f*rh, 0.0f, -(top + bottom) * rh,
				0.0f, 0.0f, -2.0f*rl, -(zFar + zNear) * rl,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static inline mat4 Perspective(float fovy, float aspect, float zNear, float zFar)
		{
			auto a = 1.0f / tanf(fovy / 2.0f);
			auto rl = 1.0f / (zFar - zNear);
			return mat4(
				a/aspect, 0.0f, 0.0f, 0.0f,
				0.0f, a, 0.0f, 0.0f,
				0.0f, 0.0f, -(zFar+zNear)*rl, -2 * zFar * zNear * rl,
				0.0f, 0.0f, -1.0f, 0.0f
			);
		}

		static inline mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up)
		{
			auto direction = (eye - target).normalize();
			auto right = CrossProduct(up, direction).normalize();
			auto cameraUp = CrossProduct(direction, right).normalize();
			mat4 m1 = mat4(
				right.x, right.y, right.z, 0,
				cameraUp.x, cameraUp.y, cameraUp.z, 0,
				direction.x, direction.y, direction.z, 0,
				0, 0, 0, 1
			);
			mat4 m2 = Translate(-eye);
			return m1 * m2;
		}

		static inline mat4 Viewport(int x, int y, int width, int height)
		{
			return mat4::Translate(vec3((float)x, (float)y, 0.0f)) * mat4::Scale(width * 0.5f, height * 0.5f, 0.5f) * mat4::Translate(vec3(1.0f, 1.0f, 1.0f));
		}

		/*
		static inline mat4 OrthoProjection(const vec3& normal)
		{
			auto v = normal.normalize();

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

		static inline mat4 Mirror(const vec3& axis)
		{
			auto v = axis.normalize();

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
		void SetRow(int row, float v1, float v2, float v3, float v4)
		{
			int i = (row - 1);
			m_Data[i] = v1;
			m_Data[i + 4] = v2;
			m_Data[i + 8] = v3;
			m_Data[i + 12] = v4;
		}

		void SetRow(int row, vec4 v)
		{
			int i = (row - 1);
			m_Data[i] = v.x;
			m_Data[i + 4] = v.y;
			m_Data[i + 8] = v.z;
			m_Data[i + 12] = v.w;
		}

		void SetCol(int col, float x, float y, float z, float w)
		{
			int i = (col - 1) * 4;
			m_Data[i++] = x;
			m_Data[i++] = y;
			m_Data[i++] = z;
			m_Data[i++] = w;
		}

		void SetCol(int col, vec4 v)
		{
			int i = (col - 1) * 4;
			m_Data[i++] = v.x;
			m_Data[i++] = v.y;
			m_Data[i++] = v.z;
			m_Data[i++] = v.w;
		}


		vec4 GetRow(int row) const
		{
			int i = (row - 1);
			return { m_Data[i], m_Data[i + 4], m_Data[i + 8], m_Data[i + 12] };
		}

		vec4 GetCol(int col) const
		{
			int i = (col - 1) * 4;
			return { m_Data[i], m_Data[i + 1], m_Data[i + 2], m_Data[i + 3] };
		}


		vec4 operator[](const int row) const
		{
			int i = (row - 1) * 4;
			return { m_Data[i], m_Data[i + 4], m_Data[i + 8], m_Data[i + 12] };
		}

	public:
		mat4 operator*(const float val) const
		{
			float data[16];
			for (int i = 0; i < 16; i++)
				data[i] = m_Data[i] * val;
			return mat4(data);
		}

		mat4 operator*(const mat4& matrix) const
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
			return mat4(data).Transposed();
		}

		vec3 operator*(const vec3& vector) const
		{
			vec4 vec(vector);
			auto k = 1 / (GetRow(4) * vec);
			return vec3(GetRow(1) * vec*k, GetRow(2) * vec*k, GetRow(3) * vec*k);
		}

	public:
		bool IsEqual(const mat4& matrix) const
		{
			for (int i = 0; i < 16; i++)
			{
				if (!Logl::IsEqual(m_Data[i], matrix.m_Data[i]))
					return false;
			}
			return true;
		}


	private:
		float m_Data[16]; // column-major ordering
	};


	inline mat4 Inverse(mat4 matrix)
	{
		unsigned int indxc[4], indxr[4], ipiv[4];
		unsigned int i, j, k, l, ll;
		unsigned int icol = 0;
		unsigned int irow = 0;
		float pivinv, dum, big;

		for (j = 0; j < 4; j++)
			ipiv[j] = 0;

		for (i = 0; i < 4; i++)
		{
			big = 0.0f;
			for (j = 0; j < 4; ++j)
			{
				if (ipiv[j] != 1)
				{
					for (k = 0; k < 4; k++)
					{
						if (ipiv[k] == 0)
						{
							if (fabs(MAT4_VALUE(matrix, j, k)) >= big)
							{
								big = fabs(MAT4_VALUE(matrix, j, k));
								irow = j;
								icol = k;
							}
						}
						else if (ipiv[k] > 1)
							return matrix;
					}
				}
			}
			++(ipiv[icol]);
			if (irow != icol)
				for (l = 0; l < 4; ++l)
					std::swap(MAT4_VALUE(matrix, irow, l), MAT4_VALUE(matrix, icol, l));

			indxr[i] = irow;
			indxc[i] = icol;
			if (IsZero(MAT4_VALUE(matrix, icol, icol)))
				return matrix;

			pivinv = 1.0f / MAT4_VALUE(matrix, icol, icol);
			MAT4_VALUE(matrix, icol, icol) = 1;
			for (l = 0; l < 4; ++l)
				MAT4_VALUE(matrix, icol, l) *= pivinv;
			for (ll = 0; ll < 4; ++ll)
			{
				if (ll != icol)
				{
					dum = MAT4_VALUE(matrix, ll, icol);
					MAT4_VALUE(matrix, ll, icol) = 0;
					for (l = 0; l < 4; ++l)
						MAT4_VALUE(matrix, ll, l) -= MAT4_VALUE(matrix, icol, l) * dum;
				}
			}
		}
		for (int lx = 4; lx > 0; --lx)
		{
			if (indxr[lx - 1] != indxc[lx - 1])
			{
				for (k = 0; k < 4; k++)
					std::swap(MAT4_VALUE(matrix, k, indxr[lx - 1]), MAT4_VALUE(matrix, k, indxc[lx - 1]));
			}
		}

		return matrix;
	}
}