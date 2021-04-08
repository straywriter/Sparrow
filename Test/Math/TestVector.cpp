#include "Math/Math.h"
#include "gtest/gtest.h"

#include <iostream>
// #pragma warning( push )
// #pragma warning( disable : 4201 )

using namespace Math;

#define GTEST_COUT std::cerr << "[ INFO     ] "

TEST(Vector3f, Constructor)
{
	auto d = Vector();

	//    GTEST_COUT << d.x << d.y << d.z<<'\n';
	Vector a(1, 1, 1);
	Vector b(1, 1, 1);
	auto x = a + b;

	//	VectorType<Vector>::Type v = 1;
		//	GTEST_COUT << x.x << x.y << x.z << '\n';
		//	GTEST_COUT << GetVectorSize(x)<<typeid(v).name() << '\n';
	Vector c(2, 2, 2);

	EXPECT_EQ(c, x);
}

TEST(Vector3f, OperatorTest)
{
	// assert(0>1);
	// +
	{
		Vector a(1.1f, 1.2f, 1.3f);
		Vector b(1.1f, 1.2f, 1.3f);

		auto c = a + b;
		auto x = Vector(2.2f, 2.4f, 2.6f);

		//		GTEST_COUT << c.x << c.y << c.z << '\n';

		EXPECT_EQ(c, x);
	}
	// -
	{
		Vector a(1.1f, 1.2f, 1.3f);
		Vector b(1.1f, 1.2f, 1.3f);
		auto x = a - b;
		auto c = Vector();
		EXPECT_EQ(c, x);
	}
	// +=
	{
		Vector a(1.1f, 1.2f, 1.3f);
		Vector b(1.1f, 1.2f, 1.3f);

		auto c = a + b;
		b += a;

		EXPECT_EQ(c, b);
	}
}

TEST(Vector3f, Operator)
{
	//	Matrix a;
	auto a = Matrix();

	//	std::cout << MatrixType<Matrix>::col << MatrixType<Matrix>::row;
//	std::cout << GetMatrixRowSize(a) << GetMatrixColSize(a) << '\n';
}

TEST(Vector, Constructor)
{
	//

	// Vector2i
	{
		Vector2i t1;
		t1.x = 0;
		t1.y = 0;
		Vector2i t2;
		t2.x = 1;
		t2.y = 2;
		Vector2i t3;
		t3.x = 3;
		t3.y = 3;

		Vector2i v1;
		Vector2i v2 = Vector2i();
		Vector2i v3 = { 1, 2 };
		Vector2i v4 = Vector2i(3);
		Vector2i v5(1, 2);
		Vector2i v6(v3);
		Vector2i v7(Vector2i(1, 2));
		Vector2i v8 = v3;
		Vector2i v9 = Vector2i(1, 2);
		Vector2i v10 = 1;

		EXPECT_EQ(t1, v1);
		EXPECT_EQ(t1, v2);
		EXPECT_EQ(t2, v3);
		EXPECT_EQ(t3, v4);
		EXPECT_EQ(t2, v5);
		EXPECT_EQ(t2, v6);
		EXPECT_EQ(t2, v7);
		EXPECT_EQ(t2, v8);
		EXPECT_EQ(t2, v9);

		EXPECT_EQ(t3, v9);
	}
	// Vector3i
	{
		Vector3i t1;
		t1.x = 0;
		t1.y = 0;
		t1.z = 0;
		Vector3i t2;
		t2.x = 1;
		t2.y = 2;
		t2.z = 3;
		Vector3i t3;
		t3.x = 4;
		t3.y = 4;
		t3.z = 4;

		Vector3i v1;
		EXPECT_EQ(v1, t1);
		Vector3i v2(1, 2, 3);
		EXPECT_EQ(t2, v2);
		Vector3i v3(4, 4, 4);
		EXPECT_EQ(t3, v3);
		Vector3i v4 = v1;
		EXPECT_EQ(t1, v4);
		Vector3i v5 = Vector3i(1, 2, 3);
		EXPECT_EQ(t2, v5);
		Vector3i v6 = Vector3i(5);
		EXPECT_EQ(t3, v6);
	}
	//Vector4i
	{
		Vector4i t1;
		t1.x = 0; t1.y = 0; t1.z = 0; t1.w = 0;
		Vector4i t2;
		t2.x = 1; t2.y = 2; t2.z = 3; t2.w = 4;
		Vector4i t3;
		for (auto i = 4; i--; t3.data[i] = 5)
			;

		Vector4i v1;
		EXPECT_EQ(t1, v1);
		Vector4i v2(1, 2, 3, 4);
		EXPECT_EQ(t2, v2);
		Vector4i v3(5);
		EXPECT_EQ(t3, v3);

		Vector4i v4 = v2;
		EXPECT_EQ(t2, v4);
		Vector4i v5 = Vector4i();
		EXPECT_EQ(t1, v5);
		Vector4i v6 = Vector4i(5);
		EXPECT_EQ(t3, v6);
	}

	//Vector3f
	{
		Vector3f t1;
		t1.x = 0.f; t1.y = 0.f; t1.z = 0.f;
		Vector3f t2;
		t2.x = 0.1f; t2.y = 0.2f; t2.z = 0.3f;
		Vector3f t3;
		for (auto i = 3; i--; t3.data[i] = 4.4f)
			;

		Vector3f v1;
		EXPECT_EQ(t1, v1);
		Vector3f v2(0.1f, 0.2f, 0.3f);
		EXPECT_EQ(t2, v2);
		Vector3f v3(4.4f);
		EXPECT_EQ(t3, v3);
		Vector3f v4(v2);
		EXPECT_EQ(t2, v4);
		Vector3f v5 = Vector3f(4.4f);
		EXPECT_EQ(t3, v5);
		Vector3f v6 = v2;
		EXPECT_EQ(t2, v6);
	}

	//Vector2f
	{
		Vector2f t1;
		t1.x = 0.f; t1.y = 0.f;
		Vector2f t2;
		t2.x = 1.f; t2.y = 2.f;
		Vector2f t3;
		t3.x = 4.4f; t3.y = 4.4f;

		Vector2f v1;
		EXPECT_EQ(t1, v1);
		Vector2f v2(1.f, 2.f);
		EXPECT_EQ(t2, v2);
		Vector2f v3(4.4f);
		EXPECT_EQ(t3, v3);
		Vector2f v4 = v2;
		EXPECT_EQ(v4, t2);
		Vector2f v5 = Vector2f();
		EXPECT_EQ(t1, v5);
	}

	//Vector4f
	{
		Vector4f t1;
		t1.x = 0.; t1.y = 0.; t1.z = 0.; t1.w = 0.;
		Vector4f t2;
		t2.x = 1.; t2.y = 2.f; t2.z = 3.f; t2.w = 4.f;
		Vector4f t3;
		for (auto i = 4; i--; t3.data[i] = 8.9f);

		Vector4f v1;
		EXPECT_EQ(t1, v1);
		Vector4f v2(1.f, 2.f, 3.f, 4.f);
		EXPECT_EQ(t2, v2);
		Vector4f  v3(8.9f);
		EXPECT_EQ(t3, v3);
		Vector4f v4(v2);
		EXPECT_EQ(t2, v4);
		Vector4f v5 = v2;
		EXPECT_EQ(t2, v5);
		Vector4f v6 = Vector4f();
		EXPECT_EQ(t1, v6);
		Vector4f v7 = Vector4f(8.9f);
		EXPECT_EQ(t2, v7);

	}

	//Vector2d
	{
		Vector2d t1;
		t1.x = 0.; t1.y = 0.; 
		Vector2d t2;
		t2.x = 1.1; t2.y = 2.2;
		Vector2d t3;
		t3.x = 8.8; t3.y = 8.8;

		Vector2d v1;
		EXPECT_EQ(t1, v1);
		Vector2d v2(1.1, 2.2);
		EXPECT_EQ(t2, v2);
		Vector2d v3(8.8);
		EXPECT_EQ(t3, v3);
		Vector2d v4 = v2;
		EXPECT_EQ(t2, v4);
		Vector2d v5 = Vector2d();
		EXPECT_EQ(t1, v5);
		Vector2d v6 = Vector2d(8.8);
		EXPECT_EQ(t3, v6);
	}

	//Vector3d
	{
		Vector3d t1;
		t1.x = 0.; t1.y = 0.; t1.z = 0.;
		Vector3d t2;
		t2.x = 1.1; t2.y = 2.2; t2.z = 3.3;
		Vector3d t3;
		for (auto i = 3; i--; t3.data[i] = 8.9);


		Vector3d v1;
		EXPECT_EQ(t1, v1);
		Vector3d v2(1.1, 2.2, 3.3);
		EXPECT_EQ(t2, v2);
		Vector3d v3(8.9);
		EXPECT_EQ(t3, v3);
		Vector3d v4 = t1;
		EXPECT_EQ(t1, v4);
		Vector3d v5 = Vector3d();
		EXPECT_EQ(t1, v5);
		Vector3d v6 = Vector3d(8.9);
		EXPECT_EQ(t3, v6);

	}
	//Vector4d
	{
		
		Vector4d t1;
		t1.x = 0.f; t1.y = 0.f; t1.z = 0.f; t1.w = 0.f;
		Vector4d t2;
		t2.x = 1.f; t2.y = 2.f; t2.z = 3.f; t2.w = 4.f;
		Vector4d t3;
		for (auto i = 4; i--; t3.data[i] = 8.9f);

		Vector4d v1;
		EXPECT_EQ(t1, v1);
		Vector4d v2(1.f, 2.f, 3.f, 4.f);
		EXPECT_EQ(t2, v2);
		Vector4d  v3(8.9f);
		EXPECT_EQ(t3, v3);
		Vector4d v4(v2);
		EXPECT_EQ(t2, v4);
		Vector4d v5 = v2;
		EXPECT_EQ(t2, v5);
		Vector4d v6 = Vector4d();
		EXPECT_EQ(t1, v6);
		Vector4d v7 = Vector4d(8.9f);
		EXPECT_EQ(t2, v7);

	}


}