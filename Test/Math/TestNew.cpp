#include "Math/Vector.h"
#include "gtest/gtest.h"
#include "Eigen/Dense"
#include <iostream>

using namespace Math;
TEST(Vec, DotProduct)
{
	Vector3f aa;
	Vector3f bb;
	// auto cc = aa.GetDataPtr();
	aa.x = 4;
	aa.y = -2;
	aa.z = 1;
	bb.x = 1;
	bb.y = -1;
	bb.z = 3;

	// *cc = 5;
	auto s = aa.GetSize();
	// auto dd = aa + bb;

	auto tt = aa.Cross(bb);
	std::cout << s << tt.x << tt.y << tt.z;
	auto rr = Vector3f(3);

	std::cout << '\n' << rr.x << rr.y << rr.z;
	Vector2f v2;
	v2.x = 1;
	v2.y = 2;
	v2.SetZero();

	Vector3f v3(v2);
	std::cout << '\n' << v3.x << v3.y << v3.z;

	Vector3f v4;
	std::cout << '\n' << v4.x << v4.y << v4.z;

	v4.Set(1, 2, 3);
	std::cout << '\n' << v4.x << v4.y << v4.z;

	v4.SetZero();
	std::cout << '\n' << v4.x << v4.y << v4.z;

	v4.SetOne();
	std::cout << '\n' << v4.x << v4.y << v4.z;

	v4.Set(1, 2, 3);
	auto p = (Eigen::Matrix<float, 3, 1> *)(&v4);
	p->setOnes();
	//p->matrix()
	Eigen::Matrix<float, 3, 1> vv1(0,0,0);

	std::cout<<'\n' << vv1 ;

	Vector2f vv2;
	std::cout<<'\n' << vv2.x << vv2.y;


//  auto p = (Eigen::Matrix<T, 2, 1> *)(&vv2);
//     new (p) Eigen::Matrix<T, 2, 1>(x, y);

}


TEST(Vector, Constructor)
{
	
	// Vector4i
	{
		Vector4i t1;
		t1.x = 0;
		t1.y = 0;
		t1.z = 0;
		t1.w = 0;
		Vector4i t2;
		t2.x = 1;
		t2.y = 2;
		t2.z = 3;
		t2.w = 4;
		Vector4i t3;
		for (auto i = 4; i--; *(&t3.x + i) = 5)
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

// Vector2d
	{
		Vector2d t1;
		t1.x = 0.;
		t1.y = 0.;
		Vector2d t2;
		t2.x = 1.1;
		t2.y = 2.2;
		Vector2d t3;
		t3.x = 8.8;
		t3.y = 8.8;

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

	// Vector3d
	{
		Vector3d t1;
		t1.x = 0.;
		t1.y = 0.;
		t1.z = 0.;
		Vector3d t2;
		t2.x = 1.1;
		t2.y = 2.2;
		t2.z = 3.3;
		Vector3d t3;
		for (auto i = 3; i--; *(&t3.x + i) = 8.9)
			;

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
	// Vector4d
	{
		Vector4d t1;
		t1.x = 0.f;
		t1.y = 0.f;
		t1.z = 0.f;
		t1.w = 0.f;
		Vector4d t2;
		t2.x = 1.f;
		t2.y = 2.f;
		t2.z = 3.f;
		t2.w = 4.f;
		Vector4d t3;
		for (auto i = 4; i--; *(&t3.x + i) = 8.9f)
			;

		Vector4d v1;
		EXPECT_EQ(t1, v1);
		Vector4d v2(1.f, 2.f, 3.f, 4.f);
		EXPECT_EQ(t2, v2);
		Vector4d v3(8.9f);
		EXPECT_EQ(t3, v3);
		Vector4d v4(v2);
		EXPECT_EQ(t2, v4);
		Vector4d v5 = v2;
		EXPECT_EQ(t2, v5);
		Vector4d v6 = Vector4d();
		EXPECT_EQ(t1, v6);
		Vector4d v7 = Vector4d(8.9f);
		EXPECT_EQ(t3, v7);
	}

}
