#include "../FMT.h"
#include "../Math/Math.h"
#include "../Math/Matrix4f.h"


#define PRINT_VEC(vec) fmt::print("{3}: [{0}, {1}, {2}]\n", vec.x, vec.y, vec.z, #vec)

namespace Logl
{

	void TestVector3()
	{
		Vector3 v1(1.0, 0.0, 0.0);
		Vector3 v2(0.0, 1.0, 0.0);
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
		Vector3 v10(v9);

		Matrix4f matrix;
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

	void TestMatrix4f()
	{
		Matrix4f m1;
		Matrix4f m2 = Matrix4f::Rotate(/*Radians(60.f)*/PI/3, Vector3(1.0f, 0.0f, 1.0f));
		Matrix4f m3 = Matrix4f::Translate(Vector3(10.0f, 0.0f, 0.0f));
		Matrix4f m4 = Matrix4f::Scale(2.0f);
		//Matrix4f m5 = Matrix4f::Mirror({ 0.0f, 1.0f, 0.0f });

		Vector3 v(2.0f, 3.0f, 5.0f);
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

}
