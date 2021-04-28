#include "Math/Math.h"
#include "gtest/gtest.h"
#include <Eigen/Dense>
#include <iostream>
#include <math.h>
// #pragma warning( push )
// #pragma warning( disable : 4201 )

using namespace Math;

#define VECTOR_TEST_OUT 1

#define GTEST_COUT std::cerr << "[ INFO     ] "

template <typename T, size_t Size> void VectorConstructTest(TVector<T, Size> vec)
{
    vec.SetZero();

    using VectorType = TVector<T, Size>;
    VectorType t1;
    for (auto i = Size; i--; *(const_cast<T *>(t1.GetDataPtr()) + (int)i) = static_cast<T>(0))
        ;
    // std::cerr << "[ VecT1    ] " << t1<<std::endl;
    VectorType t2;
    for (auto i = Size; i--; *(&t2.x + (int)i) = static_cast<T>(0))
        ;
    VectorType t3;
    for (auto i = Size; i--; *(&t3.x + (int)i) = static_cast<T>(1))
        ;
    VectorType t4;
    for (auto i = Size; i--; *(&t4.x + (int)i) = (T)i + static_cast<T>(1))
        ;

#if VECTOR_TEST_OUT
    std::cerr << "[ VecTest1 ] " << t1 << std::endl;
    std::cerr << "[ VecTest2 ] " << t2 << std::endl;
    std::cerr << "[ VecTest3 ] " << t3 << std::endl;
    std::cerr << "[ VecTest4 ] " << t4 << std::endl;
#endif

    // default constructor
    VectorType v10;
    EXPECT_EQ(t1, v10);
    VectorType v11 = VectorType();
    EXPECT_EQ(t1, v11);
    VectorType v12(v11);
    EXPECT_EQ(t1, v12);
    VectorType v13 = v10;
    EXPECT_EQ(t1, v13);

    // defalut zero
    VectorType v20 = VectorType(static_cast<T>(0));
    EXPECT_EQ(t2, v20);
    VectorType v21(static_cast<T>(0));
    EXPECT_EQ(t2, v21);
    VectorType v22(VectorType(static_cast<T>(0)));
    EXPECT_EQ(t2, v22);
    VectorType v23(v21);
    EXPECT_EQ(t2, v23);
    VectorType v24;
    v24.SetZero();
    EXPECT_EQ(t2, v24);

    // defalut one
    VectorType v30 = VectorType(static_cast<T>(1));
    EXPECT_EQ(t3, v30);
    VectorType v31(static_cast<T>(1));
    EXPECT_EQ(t3, v31);
    VectorType v32(VectorType(static_cast<T>(1)));
    EXPECT_EQ(t3, v32);
    VectorType v33(v31);
    EXPECT_EQ(t3, v33);
    VectorType v34;
    v34.SetOne();
    EXPECT_EQ(t3, v34);

    if constexpr (Size == size_t(2))
    {
        VectorType v40(static_cast<T>(1), static_cast<T>(2));
        EXPECT_EQ(t4, v40);
        VectorType v41(VectorType(static_cast<T>(1), static_cast<T>(2)));
        EXPECT_EQ(t4, v41);
        VectorType v42 = VectorType(static_cast<T>(1), static_cast<T>(2));
        EXPECT_EQ(t4, v42);
        VectorType v43 = v40;
        EXPECT_EQ(t4, v43);
        VectorType v44(v40);
        EXPECT_EQ(t4, v44);
        VectorType v45;
        v45.Set(static_cast<T>(1), static_cast<T>(2));
        EXPECT_EQ(t4, v45);
    }
    if constexpr (Size == size_t(3))
    {
        VectorType v40(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3));
        EXPECT_EQ(t4, v40);
        VectorType v41(VectorType(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3)));
        EXPECT_EQ(t4, v41);
        VectorType v42 = VectorType(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3));
        EXPECT_EQ(t4, v42);
        VectorType v43 = v40;
        EXPECT_EQ(t4, v43);
        VectorType v44(v40);
        EXPECT_EQ(t4, v44);
        VectorType v45;
        v45.Set(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3));
        EXPECT_EQ(t4, v45);
    }
    if constexpr (Size == size_t(4))
    {
        VectorType v40(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(4));
        EXPECT_EQ(t4, v40);
        VectorType v41(VectorType(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(4)));
        EXPECT_EQ(t4, v41);
        VectorType v42 = VectorType(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(4));
        EXPECT_EQ(t4, v42);
        VectorType v43 = v40;
        EXPECT_EQ(t4, v43);
        VectorType v44(v40);
        EXPECT_EQ(t4, v44);
        VectorType v45;
        v45.Set(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(4));
        EXPECT_EQ(t4, v45);
    }
}

#define VECTOR_CONSTRUCTOR_TEST(VectorType)                                                                            \
    TEST(VectorConstructor, VectorType)                                                                                \
    {                                                                                                                  \
        VectorConstructTest(VectorType());                                                                             \
    }
VECTOR_CONSTRUCTOR_TEST(Vector2i)
VECTOR_CONSTRUCTOR_TEST(Vector3i)
VECTOR_CONSTRUCTOR_TEST(Vector4i)
VECTOR_CONSTRUCTOR_TEST(Vector2f)
VECTOR_CONSTRUCTOR_TEST(Vector3f)
VECTOR_CONSTRUCTOR_TEST(Vector4f)
VECTOR_CONSTRUCTOR_TEST(Vector2d)
VECTOR_CONSTRUCTOR_TEST(Vector3d)
VECTOR_CONSTRUCTOR_TEST(Vector4d)

//----------
TEST(VectorCrossProduct, Vector3f)
{
    // https://www.osgeo.cn/app/s2848
    {
        Vector3f t1(1.f, 3.f, 4.f);
        Vector3f t2(2.f, -5.f, 8.f);
        Vector3f t3(44.f, 0.f, -11.f);

        auto v1 = Vector3f::Cross(t1, t2);
        EXPECT_EQ(t3, v1);
    }
    {
        Vector3f t1(1.f, 3.f, 4.f);
        Vector3f t2(2.f, -5.f, 8.f);
        Vector3f t3(44.f, 0.f, -11.f);

        auto v1 = t1.Cross(t2);
        EXPECT_EQ(t3, v1);
    }
    {
        Vector3i t1(4, -2, 1);
        Vector3i t2(1, -1, 3);
        Vector3i t3(-5, -11, -2);
        auto v1 = Vector3i::Cross(t1, t2);
        auto v2 = t1.Cross(t2);

        // GTEST_COUT << v1.x << v1.y << v1.z << '\n';
        // GTEST_COUT << t3.x << t3.y << t3.z << '\n';
        EXPECT_EQ(t3, v1);
        EXPECT_EQ(t3, v2);
    }
    {
        Vector3d t1(6., 2, 23.1);
        Vector3d t2(3.6, -8.1, 7.3);
        Vector3d t3(201.71, 39.360000000000014, -55.8);
        auto v1 = Vector3d::Cross(t1, t2);
        auto v2 = t1.Cross(t2);
        // GTEST_COUT << v1.x << " " << v1.y << " " << v1.z << '\n';
        EXPECT_EQ(t3, v1);
        EXPECT_EQ(t3, v2);
    }
}

template <typename T, size_t Size> void VectorGetSetTest(TVector<T, Size> vec)
{
    {
        using VectorType = TVector<T, Size>;
        VectorType t1;
        VectorType t2(static_cast<T>(1));
        VectorType t3(static_cast<T>(0));

        vec.SetZero();
        EXPECT_EQ(t1, vec);
        EXPECT_EQ(t3, vec);
        vec.SetOne();
        EXPECT_EQ(t2, vec);
        vec.SetValue(static_cast<T>(4));
    }
    // Get
    {
        using VectorType = TVector<T, Size>;
        VectorType t1;
        for (auto i = Size; i--; *(const_cast<T *>(t1.GetDataPtr()) + (int)i) = static_cast<T>(0))
            ;
        // std::cerr << "[ VecT1    ] " << t1<<std::endl;
        VectorType t2;
        for (auto i = Size; i--; *(&t2.x + (int)i) = static_cast<T>(0))
            ;
        VectorType t3;
        for (auto i = Size; i--; *(&t3.x + (int)i) = static_cast<T>(1))
            ;
        VectorType t4;
        for (auto i = Size; i--; *(&t4.x + (int)i) = (T)i + static_cast<T>(1))
            ;
        for (auto i = Size; i--;)
        {
            EXPECT_EQ(t1.Get(i), static_cast<T>(0));
        }
        for (auto i = Size; i--;)
        {
            EXPECT_EQ(t2.Get(i), static_cast<T>(0));
        }
        for (auto i = Size; i--;)
        {
            EXPECT_EQ(t3.Get(i), static_cast<T>(1));
        }
        for (auto i = Size; i--;)
        {
            EXPECT_EQ(t4.Get(i), static_cast<T>(i + 1));
        }
    }
    // Set
    {
    }
}
#define VECTOR_GETSET_TEST(VectorType)                                                                                 \
    TEST(VectorGetSetTest, VectorType)                                                                                 \
    {                                                                                                                  \
        VectorGetSetTest(VectorType());                                                                                \
    }
VECTOR_GETSET_TEST(Vector2i)
VECTOR_GETSET_TEST(Vector3i)
VECTOR_GETSET_TEST(Vector4i)
VECTOR_GETSET_TEST(Vector2f)
VECTOR_GETSET_TEST(Vector3f)
VECTOR_GETSET_TEST(Vector4f)
VECTOR_GETSET_TEST(Vector2d)
VECTOR_GETSET_TEST(Vector3d)
VECTOR_GETSET_TEST(Vector4d)

template <typename T, size_t Size> void VectorLenthTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;
    VectorType t1;
    for (auto i = Size; i--; t1[i] = (T)i + static_cast<T>(1))
        ;
    std::cout << t1;
    T t2 = static_cast<T>(0);
    for (auto i = Size; i--; t2 += ((T)i + static_cast<T>(1)) * ((T)i + static_cast<T>(1)))
        ;

    EXPECT_EQ(t1.SquaredLenth(), t2);
    auto vv = std::sqrt(t2);
    EXPECT_EQ(t1.Lenth(), (T)vv);

#if VECTOR_TEST_OUT
    std::cerr << "[ VecTest  ] " << t1 << std::endl;
    std::cerr << "[ VecTest2 ] " << t2 << std::endl;
    std::cerr << "[ Vec      ] " << vv << std::endl;
#endif
}
#define VECTOR_LENTH_TEST(VectorType)                                                                                  \
    TEST(VectorLenthTest, VectorType)                                                                                  \
    {                                                                                                                  \
        VectorLenthTest(VectorType());                                                                                 \
    }
VECTOR_LENTH_TEST(Vector2i)
VECTOR_LENTH_TEST(Vector3i)
VECTOR_LENTH_TEST(Vector4i)
VECTOR_LENTH_TEST(Vector2f)
VECTOR_LENTH_TEST(Vector3f)
VECTOR_LENTH_TEST(Vector4f)
VECTOR_LENTH_TEST(Vector2d)
VECTOR_LENTH_TEST(Vector3d)
VECTOR_LENTH_TEST(Vector4d)

template <typename T, size_t Size> void VectorNormalizeTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;

    VectorType t1;
    for (auto i = Size; i--; *(&t1.x + (int)i) = (T)i + static_cast<T>(1))
        ;
    VectorType t2;
    for (auto i = Size; i--; *(&t2.x + (int)i) = ((T)i + static_cast<T>(1)))
        ;

    auto lenth = t2.Lenth();

    for (auto i = Size; i--; t2[i] = t2[i] / lenth)
        ;
    auto v1 = t1;
    v1.Normalize();
    EXPECT_EQ(v1, t2);

#if VECTOR_TEST_OUT
    std::cerr << "[ VecTest1 ] " << t1 << std::endl;
    std::cerr << "[ VecLenth ] " << lenth << std::endl;
    std::cerr << "[ VecTest2 ] " << t2 << std::endl;
    std::cerr << "[ Vec      ] " << v1 << std::endl;
#endif
}
#define VECTOR_NORMALIZE_TEST(VectorType)                                                                              \
    TEST(VectorNormalizeTest, VectorType)                                                                              \
    {                                                                                                                  \
        VectorNormalizeTest(VectorType());                                                                             \
    }
VECTOR_NORMALIZE_TEST(Vector2i)
VECTOR_NORMALIZE_TEST(Vector3i)
VECTOR_NORMALIZE_TEST(Vector4i)
VECTOR_NORMALIZE_TEST(Vector2f)
VECTOR_NORMALIZE_TEST(Vector3f)
VECTOR_NORMALIZE_TEST(Vector4f)
VECTOR_NORMALIZE_TEST(Vector2d)
VECTOR_NORMALIZE_TEST(Vector3d)
VECTOR_NORMALIZE_TEST(Vector4d)

template <typename T, size_t Size> void VectorConvertTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;

    if constexpr (Size == 2)
    {
    }
    VectorType t;
    for (auto i = Size; i--; t[i] = (T)i + static_cast<T>(1))
        ;
#define VECTOR_INIT(size, type, var)                                                                                   \
    if constexpr (Size > size_t(size))                                                                                 \
    {                                                                                                                  \
        for (auto i = size; i--; var[i] = (type)i + static_cast<type>(1))                                              \
            ;                                                                                                          \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        for (auto i = 0; i < Size; ++i)                                                                                \
        {                                                                                                              \
            var[i] = (type)i + static_cast<type>(1);                                                                   \
        }                                                                                                              \
        for (auto i = Size; i < size; ++i)                                                                             \
        {                                                                                                              \
            var[i] = static_cast<type>(0);                                                                             \
        }                                                                                                              \
    }

    Vector2i t1;
    VECTOR_INIT(2, int, t1)
    Vector3i t2;
    VECTOR_INIT(3, int, t2)
    Vector4i t3;
    VECTOR_INIT(4, int, t3)
    Vector2f t4;
    VECTOR_INIT(2, float, t4)
    Vector3f t5;
    VECTOR_INIT(3, float, t5)
    Vector4f t6;
    VECTOR_INIT(4, float, t6)
    Vector2d t7;
    VECTOR_INIT(2, double, t7)
    Vector3d t8;
    VECTOR_INIT(3, double, t8)
    Vector4d t9;
    VECTOR_INIT(4, double, t9)

    Vector2i v1 = t;
    Vector3i v2 = t;
    Vector4i v3 = t;
    Vector2f v4 = t;
    Vector3f v5 = t;
    Vector4f v6 = t;
    Vector2d v7 = t;
    Vector3d v8 = t;
    Vector4d v9 = t;
    EXPECT_EQ(t1, v1);
    EXPECT_EQ(t2, v2);
    EXPECT_EQ(t3, v3);
    EXPECT_EQ(t4, v4);
    EXPECT_EQ(t5, v5);
    EXPECT_EQ(t6, v6);
    EXPECT_EQ(t7, v7);
    EXPECT_EQ(t8, v8);
    EXPECT_EQ(t9, v9);

#if VECTOR_TEST_OUT
    std::cerr << "[ VecTest  ] " << t << std::endl;
    std::cerr << "[ TVector1i] " << t1 << std::endl;
    std::cerr << "[ Vector2i ] " << v1 << std::endl;
    std::cerr << "[ TVector3i] " << t2 << std::endl;
    std::cerr << "[ Vector3i ] " << v2 << std::endl;
    std::cerr << "[ TVector4i] " << t3 << std::endl;
    std::cerr << "[ Vector4i ] " << v3 << std::endl;
    std::cerr << "[ TVector2f] " << t4 << std::endl;
    std::cerr << "[ Vector2f ] " << v4 << std::endl;
    std::cerr << "[ TVector3f] " << t5 << std::endl;
    std::cerr << "[ Vector3f ] " << v5 << std::endl;
    std::cerr << "[ TVector4f] " << t6 << std::endl;
    std::cerr << "[ Vector4f ] " << v6 << std::endl;
    std::cerr << "[ TVector2d] " << t7 << std::endl;
    std::cerr << "[ Vector2d ] " << v7 << std::endl;
    std::cerr << "[ TVector3d] " << t8 << std::endl;
    std::cerr << "[ Vector3d ] " << v8 << std::endl;
    std::cerr << "[ TVector4d] " << t9 << std::endl;
    std::cerr << "[ Vector4d ] " << v9 << std::endl;
#endif
}

#define VECTORCONVERTTEST(VectorType)                                                                                  \
    TEST(VectorConvertTest, VectorType)                                                                                \
    {                                                                                                                  \
        VectorConvertTest(VectorType());                                                                               \
    }

VECTORCONVERTTEST(Vector2i)
VECTORCONVERTTEST(Vector3i)
VECTORCONVERTTEST(Vector4i)
VECTORCONVERTTEST(Vector2f)
VECTORCONVERTTEST(Vector3f)
VECTORCONVERTTEST(Vector4f)
VECTORCONVERTTEST(Vector2d)
VECTORCONVERTTEST(Vector3d)
VECTORCONVERTTEST(Vector4d)

template <typename T, size_t Size> void VectorStaticVariableTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorRandomTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorMinMaxTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorDotTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorReflectTest(TVector<T, Size> vec)
{

}
template <typename T, size_t Size> void VectorAngleTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorEqualsTest(TVector<T, Size> vec)
{

}

template <typename T, size_t Size> void VectorOperatorTest(TVector<T, Size> vec)
{

}



TEST(VectorConvert, Vector)
{
    Vector3f V(1.f, 2.f, 3.f);
    std::cout << V;
    Vector2f f = V;
    std::cout << f;
    Vector3f a = V;
    Vector2i i2 = V;

    Vector3f f3 = i2;
    // Vector3d d3=f3;
    auto xx = V;

    Vector3i i3;
    Vector4f f4 = i3;
Vector2f ccc{2.f,3.f};
    std::cout<<'\n' << ccc;
    auto yy = Vector2d::VectorZero;
    std::cout<<'\n' << yy;

}

// TEST(Vector, Normlise)
// {
//     struct T
//     {
//         double x, y;
//     };
//     struct T1
//     {
//         float x, y;
//     };

//     // std::cerr << "[ INFO     ] "<<sizeof(Vector2d);
//     // std::cerr << "[ INFO  T  ] "<<sizeof(T);
//     // std::cerr << "[ INFO  T1  ] "<<sizeof(Vector2f);
//     // std::cerr << "[ INFO  T  ] "<<sizeof(T1);

//     // std::cerr << "[ INFO     ] "<<sizeof(Eigen::Vector2d);

//     // std::cerr << "[ INFO     ] "<<sizeof(Eigen::Matrix<double, 2, 1>);
// }