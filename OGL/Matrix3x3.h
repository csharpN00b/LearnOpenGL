#pragma once

#include "Vector3.h"

class Matrix3x3
{
public:
	/*
	* 构造函数
	* 单位矩阵 identity matrix
	* 转置 transposed matrix
	* 标量和矩阵乘法
	* 向量与矩阵的乘法
	* 矩阵乘法
	*/
	Matrix3x3()
	{
		m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
		m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
		m31 = 0.0f; m32 = 0.0f; m33 = 0.1f;
	}

	Matrix3x3(float nm11, float nm12, float nm13,
			  float nm21, float nm22, float nm23,
			  float nm31, float nm32, float nm33
		)
	{
		m11 = nm11; m12 = nm12; m13 = nm13;
		m21 = nm21; m22 = nm22; m23 = nm23;
		m31 = nm31; m32 = nm32; m33 = nm33;
	}

	void Identity()
	{
		m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
		m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
		m31 = 0.0f; m32 = 0.0f; m33 = 0.1f;
	}

	Matrix3x3 Transposed() const
	{
		return Matrix3x3(m11, m21, m31,
						 m12, m22, m32,
						 m13, m23, m33);
	}

public:
	Matrix3x3 operator*(const float val) const
	{
		return Matrix3x3(m11*val, m21*val, m31*val,
						 m12*val, m22*val, m32*val,
						 m13*val, m23*val, m33*val);
	}

	Matrix3x3 operator*(const Matrix3x3& matrix) const
	{
		return Matrix3x3(m11 * matrix.m11 + m12 * matrix.m21 + m13 * matrix.m31,
						 m11 * matrix.m12 + m12 * matrix.m22 + m13 * matrix.m32,
						 m11 * matrix.m13 + m12 * matrix.m23 + m13 * matrix.m33,
						 m21 * matrix.m11 + m22 * matrix.m21 + m23 * matrix.m31,
						 m21 * matrix.m12 + m22 * matrix.m22 + m23 * matrix.m32,
						 m21 * matrix.m13 + m22 * matrix.m23 + m23 * matrix.m33,
						 m31 * matrix.m11 + m32 * matrix.m21 + m33 * matrix.m31,
						 m31 * matrix.m12 + m32 * matrix.m22 + m33 * matrix.m32,
						 m31 * matrix.m13 + m32 * matrix.m23 + m33 * matrix.m33);
	}

	Vector3 operator*(const Vector3& vector) const
	{
		return Vector3(m11 * vector.x + m12 * vector.y + m13 * vector.z,
					   m21 * vector.x + m22 * vector.y + m23 * vector.z,
					   m31 * vector.x + m32 * vector.y + m33 * vector.z);
	}

public:
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;
};