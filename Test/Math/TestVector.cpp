#include "Math/Math.h"
#include "gtest/gtest.h"

#include <iostream>
// #pragma warning( push )
// #pragma warning( disable : 4201 )

using namespace Math;

#define GTEST_COUT std::cerr << "[ INFO     ] "

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
        Vector2i v3 = {1, 2};
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

        EXPECT_EQ(t2, v9);
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
        Vector3i v6 = Vector3i(4);
        EXPECT_EQ(t3, v6);
    }
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

    // Vector3f
    {
        Vector3f t1;
        t1.x = 0.f;
        t1.y = 0.f;
        t1.z = 0.f;
        Vector3f t2;
        t2.x = 0.1f;
        t2.y = 0.2f;
        t2.z = 0.3f;
        Vector3f t3;
        for (auto i = 3; i--; *(&t3.x + i) = 4.4f)
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

    // Vector2f
    {
        Vector2f t1;
        t1.x = 0.f;
        t1.y = 0.f;
        Vector2f t2;
        t2.x = 1.f;
        t2.y = 2.f;
        Vector2f t3;
        t3.x = 4.4f;
        t3.y = 4.4f;

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

    // Vector4f
    {
        Vector4f t1;
        t1.x = 0.;
        t1.y = 0.;
        t1.z = 0.;
        t1.w = 0.;
        Vector4f t2;
        t2.x = 1.;
        t2.y = 2.f;
        t2.z = 3.f;
        t2.w = 4.f;
        Vector4f t3;
        for (auto i = 4; i--; *(&t3.x + i) = 8.9f)
            ;

        Vector4f v1;
        EXPECT_EQ(t1, v1);
        Vector4f v2(1.f, 2.f, 3.f, 4.f);
        EXPECT_EQ(t2, v2);
        Vector4f v3(8.9f);
        EXPECT_EQ(t3, v3);
        Vector4f v4(v2);
        EXPECT_EQ(t2, v4);
        Vector4f v5 = v2;
        EXPECT_EQ(t2, v5);
        Vector4f v6 = Vector4f();
        EXPECT_EQ(t1, v6);
        Vector4f v7 = Vector4f(8.9f);
        EXPECT_EQ(t3, v7);
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

TEST(Vector, CrossProduct)
{
    // https://www.osgeo.cn/app/s2848
    {
        Vector3f t1(1.f, 3.f, 4.f);
        Vector3f t2(2.f, -5.f, 8.f);
        Vector3f t3(44.f, 0.f, -11.f);

        auto v1 = Vector3f::Cross(t1, t2);
        EXPECT_EQ(t3, v1);
    }
    {{Vector3f t1(1.f, 3.f, 4.f);
    Vector3f t2(2.f, -5.f, 8.f);
    Vector3f t3(44.f, 0.f, -11.f);

    auto v1 = t1.Cross(t2);
    EXPECT_EQ(t3, v1);
}
}
{
    Vector3i t1(4, -2, 1);
    Vector3i t2(1, -1, 3);
    Vector3i t3(-5, -11, -2);
    auto v1 = Vector3i::Cross(t1, t2);
    auto v2 = t1.Cross(t2);
    GTEST_COUT << v1.x << v1.y << v1.z << '\n';
    EXPECT_EQ(t3, v1);
    EXPECT_EQ(t3, v2);
}
{
    Vector3d t1(6., 2, 23.1);
    Vector3d t2(3.6, -8.1, 7.3);
    Vector3d t3(201.71, 39.360000000000014, -55.8);
    auto v1 = Vector3d::Cross(t1, t2);
    auto v2 = a1.Cross(t2);
    GTEST_COUT << v1.x << " " << v1.y << " " << v1.z << '\n';
    EXPECT_EQ(t3, v1);
    EXPECT_EQ(t3, v2);
}
}
TEST(Vector, Get)
{

    // Vector2i Get
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
        Vector2i v3 = {1, 2};
        Vector2i v4 = Vector2i(3);
        Vector2i v5(1, 2);
        Vector2i v6(v3);
        Vector2i v7(Vector2i(1, 2));
        Vector2i v8 = v3;
        Vector2i v9 = Vector2i(1, 2);
        Vector2i v10 = 1;

        EXPECT_EQ(t1.x, v1.Get(0));
        EXPECT_EQ(t1.y, v2.Get(1));
        EXPECT_EQ(t2.y, v3.Get(1));
        EXPECT_EQ(t3.x, v4.Get(0));
        EXPECT_EQ(t2.Get(0), v5.x);
        EXPECT_EQ(t2.Get(0), v6.Get(0));
        EXPECT_EQ(t2.Get(1), v7.Get(1));
        EXPECT_EQ(t2.Get(0), v8.x);
        EXPECT_EQ(t2.Get(0), v9.x);

        EXPECT_EQ(t2.Get(1), v9.y);
    }
     // Vector3f
    {
        Vector3f t1;
        t1.x = 0.f;
        t1.y = 0.f;
        t1.z = 0.f;
        Vector3f t2;
        t2.x = 0.1f;
        t2.y = 0.2f;
        t2.z = 0.3f;
        Vector3f t3;
        for (auto i = 3; i--; *(&t3.x + i) = 4.4f)
            ;

        Vector3f v1;
        EXPECT_EQ(t1.Get(0), v1.x);
        Vector3f v2(0.1f, 0.2f, 0.3f);
        EXPECT_EQ(t2.x, v2.Get(0));
        Vector3f v3(4.4f);
        EXPECT_EQ(t3, v3);
        Vector3f v4(v2);
        EXPECT_EQ(t2, v4);
        Vector3f v5 = Vector3f(4.4f);
        EXPECT_EQ(t3, v5);
        Vector3f v6 = v2;
        EXPECT_EQ(t2, v6);
    }
}




TEST(Vector, Normlise)
{
}