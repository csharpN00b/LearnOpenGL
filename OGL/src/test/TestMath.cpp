#include "../FMT.h"
#include "../Math/Math.h"
#include "../Math/Matrix4f.h"


#define PRINT_VEC(vec) fmt::print("{3}: [{0}, {1}, {2}]\n", vec.x, vec.y, vec.z, #vec)

using namespace Logl;

void TestVector3()
{
	vec3 v1(1.0, 0.0, 0.0);
	vec3 v2(0.0, 1.0, 0.0);
	auto v3 = v1 + v2;
	auto v4 = v3;
	v4 *= 2;
	auto v5 = v4 / 2;
	auto v6 = -v2 + v3;
	auto v7 = CrossProduct(v1, v2);
	auto v8 = v1 * 2;
	v8 += v7;
	auto v9 = 2 * v1;
	v9 /= 2;
	vec3 v10(v9);

	mat4 matrix;
	auto v11 = matrix * v10;

	PRINT_VEC(v11);
	PRINT_VEC(v10);
	PRINT_VEC(v9);
	PRINT_VEC(v8);
	PRINT_VEC(v7);
	PRINT_VEC(v6);
	PRINT_VEC(v5);
	PRINT_VEC(v4);
	PRINT_VEC(v3);
	PRINT_VEC(v2);
	PRINT_VEC(v1);

	PRINT("|v8|: {}\n", VectorMag(v8));
	PRINT("v1*v2: {}\n", v1 * v2);
	PRINT("v1 == v2: {}\n", v1 == v2 ? "true" : "false");
	PRINT("v3 == v5: {}\n", v3 == v5 ? "true" : "false");
	PRINT("v6 != v1: {}\n", v6 != v1 ? "true" : "false");
	PRINT("v9 == v1: {}\n", v9 == v1 ? "true" : "false");
	v8.normalize();
	PRINT("v8.normalize(): ");
	PRINT_VEC(v8);
}

void TestLogl()
{
	mat4 m1;
	mat4 m2 = mat4::Rotate(/*Radians(60.f)*/PI / 3, vec3(1.0f, 0.0f, 1.0f));
	mat4 m3 = mat4::Translate(vec3(10.0f, 0.0f, 0.0f));
	mat4 m4 = mat4::Scale(2.0f);
	//Matrix4f m5 = Matrix4f::Mirror({ 0.0f, 1.0f, 0.0f });

	vec3 v(2.0f, 3.0f, 5.0f);
	auto m = m4 * m3 * m2;
	auto v1 = m * v;

	auto v2 = m2 * v;
	v2 = m3 * v2;
	v2 = m4 * v2;

	PRINT("Logl:\n");
	PRINT("Radians(60.f): {}\n", Radians(60.f));
	PRINT_VEC(v);
	PRINT_VEC(v1);
	PRINT_VEC(v2);
}

