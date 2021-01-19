#include "../FMT.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix3x3.h"

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

		Matrix3x3 matrix;
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

}
