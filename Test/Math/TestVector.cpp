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
#define VECTOR_CONVERT_TEST(VectorType)                                                                                \
    TEST(VectorConvertTest, VectorType)                                                                                \
    {                                                                                                                  \
        VectorConvertTest(VectorType());                                                                               \
    }

VECTOR_CONVERT_TEST(Vector2i)
VECTOR_CONVERT_TEST(Vector3i)
VECTOR_CONVERT_TEST(Vector4i)
VECTOR_CONVERT_TEST(Vector2f)
VECTOR_CONVERT_TEST(Vector3f)
VECTOR_CONVERT_TEST(Vector4f)
VECTOR_CONVERT_TEST(Vector2d)
VECTOR_CONVERT_TEST(Vector3d)
VECTOR_CONVERT_TEST(Vector4d)

template <typename T, size_t Size> void VectorStaticVariableTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;

    VectorType t1;
    t1.SetZero();
    VectorType t2;
    t2.SetOne();
    auto v1 = VectorType::VectorZero;
    auto v2 = VectorType::VectorOne;
    EXPECT_EQ(t1, v1);
    EXPECT_EQ(t2, v2);
#if VECTOR_TEST_OUT
    std::cerr << "[ TVector  ] " << t1 << std::endl;
    std::cerr << "[ Vector   ] " << v1 << std::endl;
    std::cerr << "[ TVector  ] " << t2 << std::endl;
    std::cerr << "[ Vector   ] " << v2 << std::endl;
#endif

    if constexpr (Size == size_t(2))
    {
        VectorType t3;
        t3.x = static_cast<T>(0);
        t3.y = static_cast<T>(1);
        VectorType t4;
        t4.x = static_cast<T>(0);
        t4.y = static_cast<T>(-1);
        VectorType t5;
        t5.x = static_cast<T>(-1);
        t5.y = static_cast<T>(0);
        VectorType t6;
        t6.x = static_cast<T>(1);
        t6.y = static_cast<T>(0);

        auto v3 = VectorType::VectorUp;
        auto v4 = VectorType::VectorDown;
        auto v5 = VectorType::VectorLeft;
        auto v6 = VectorType::VectorRight;

        EXPECT_EQ(t3, v3);
        EXPECT_EQ(t4, v4);
        EXPECT_EQ(t5, v5);
        EXPECT_EQ(t6, v6);
#if VECTOR_TEST_OUT
        std::cerr << "[ TVector  ] " << t3 << std::endl;
        std::cerr << "[ Vector   ] " << v3 << std::endl;
        std::cerr << "[ TVector  ] " << t4 << std::endl;
        std::cerr << "[ Vector   ] " << v4 << std::endl;
        std::cerr << "[ TVector  ] " << t5 << std::endl;
        std::cerr << "[ Vector   ] " << v5 << std::endl;
        std::cerr << "[ TVector  ] " << t6 << std::endl;
        std::cerr << "[ Vector   ] " << v6 << std::endl;
#endif
    }
    else if constexpr (Size == size_t(3))
    {
        VectorType t3;
        t3.x = static_cast<T>(0);
        t3.y = static_cast<T>(1);
        t3.z = static_cast<T>(0);
        VectorType t4;
        t4.x = static_cast<T>(0);
        t4.y = static_cast<T>(-1);
        t4.z = static_cast<T>(0);
        VectorType t5;
        t5.x = static_cast<T>(-1);
        t5.y = static_cast<T>(0);
        t5.z = static_cast<T>(0);
        VectorType t6;
        t6.x = static_cast<T>(1);
        t6.y = static_cast<T>(0);
        t6.z = static_cast<T>(0);
        VectorType t7;
        t7.x = static_cast<T>(0);
        t7.y = static_cast<T>(0);
        t7.z = static_cast<T>(1);
        VectorType t8;
        t8.x = static_cast<T>(0);
        t8.y = static_cast<T>(0);
        t8.z = static_cast<T>(-1);

        auto v3 = VectorType::VectorUp;
        auto v4 = VectorType::VectorDown;
        auto v5 = VectorType::VectorLeft;
        auto v6 = VectorType::VectorRight;
        auto v7 = VectorType::VectorForward;
        auto v8 = VectorType::VectorBack;

        EXPECT_EQ(t3, v3);
        EXPECT_EQ(t4, v4);
        EXPECT_EQ(t5, v5);
        EXPECT_EQ(t6, v6);
        EXPECT_EQ(t7, v7);
        EXPECT_EQ(t8, v8);
#if VECTOR_TEST_OUT
        std::cerr << "[ TVector  ] " << t3 << std::endl;
        std::cerr << "[ Vector   ] " << v3 << std::endl;
        std::cerr << "[ TVector  ] " << t4 << std::endl;
        std::cerr << "[ Vector   ] " << v4 << std::endl;
        std::cerr << "[ TVector  ] " << t5 << std::endl;
        std::cerr << "[ Vector   ] " << v5 << std::endl;
        std::cerr << "[ TVector  ] " << t6 << std::endl;
        std::cerr << "[ Vector   ] " << v6 << std::endl;
        std::cerr << "[ TVector  ] " << t7 << std::endl;
        std::cerr << "[ Vector   ] " << v7 << std::endl;
        std::cerr << "[ TVector  ] " << t8 << std::endl;
        std::cerr << "[ Vector   ] " << v8 << std::endl;
#endif
    }
}

#define VECTOR_STATICVARIABLE_TEST(VectorType)                                                                         \
    TEST(VectorStaticVariableTest, VectorType)                                                                         \
    {                                                                                                                  \
        VectorStaticVariableTest(VectorType());                                                                        \
    }

VECTOR_STATICVARIABLE_TEST(Vector2i)
VECTOR_STATICVARIABLE_TEST(Vector3i)
VECTOR_STATICVARIABLE_TEST(Vector4i)
VECTOR_STATICVARIABLE_TEST(Vector2f)
VECTOR_STATICVARIABLE_TEST(Vector3f)
VECTOR_STATICVARIABLE_TEST(Vector4f)
VECTOR_STATICVARIABLE_TEST(Vector2d)
VECTOR_STATICVARIABLE_TEST(Vector3d)
VECTOR_STATICVARIABLE_TEST(Vector4d)

template <typename T, size_t Size> void VectorRandomTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;

    VectorType t1;
    t1.SetRandom();
    VectorType t2;
    t2.SetRandom();
    auto t3 = VectorType::VectorRandom();
    auto t4 = VectorType::VectorRandom();
    std::cerr << "[ Random   ] " << t1 << std::endl;
    std::cerr << "[ Random   ] " << t2 << std::endl;
    std::cerr << "[ Random   ] " << t3 << std::endl;
    std::cerr << "[ Random   ] " << t4 << std::endl;
}
#define VECTOR_RANDOM_TEST(VectorType) \
    TEST(VectorRandomTest, VectorType) \
    { \
        VectorRandomTest(VectorType()); \
    }

VECTOR_RANDOM_TEST(Vector2i)
VECTOR_RANDOM_TEST(Vector3i)
VECTOR_RANDOM_TEST(Vector4i)
VECTOR_RANDOM_TEST(Vector2f)
VECTOR_RANDOM_TEST(Vector3f)
VECTOR_RANDOM_TEST(Vector4f)
VECTOR_RANDOM_TEST(Vector2d)
VECTOR_RANDOM_TEST(Vector3d)
VECTOR_RANDOM_TEST(Vector4d)

template <typename T, size_t Size> void VectorMinMaxTest(TVector<T, Size> vec)
{
    vec.SetZero();
    using VectorType = TVector<T, Size>;

    VectorType t1;
    t1.SetValue(static_cast<T>(6));
    VectorType t2;
    t2.SetValue(static_cast<T>(8));
    VectorType v1 = VectorType::VectorMax(t1, t1);
    VectorType v2 = VectorType::VectorMin(t1, t1);
    VectorType v3 = t1.Max(t2);
    VectorType v4 = t1.Max(t2);

    EXPECT_EQ(t1, v2);
    EXPECT_EQ(t2, v1);
    EXPECT_EQ(t1, v4);
    EXPECT_EQ(t2, v3);
#if VECTOR_TEST_OUT
    std::cerr << "[ Vector   ] " << t1 << std::endl;
    std::cerr << "[ Vector   ] " << t2 << std::endl;
    std::cerr << "[ Vector   ] " << v1 << std::endl;
    std::cerr << "[ Vector   ] " << v2 << std::endl;
    std::cerr << "[ Vector   ] " << v3 << std::endl;
    std::cerr << "[ Vector   ] " << v4 << std::endl;
#endif
}

#define VECTOR_MINMAX_TEST(VectorType)                                                                                 \
    TEST(VectorMinMaxTest, VectorType)                                                                                 \
    {                                                                                                                  \
        VectorMinMaxTest(VectorType());                                                                                \
    }

VECTOR_MINMAX_TEST(Vector2i)
VECTOR_MINMAX_TEST(Vector3i)
VECTOR_MINMAX_TEST(Vector4i)
VECTOR_MINMAX_TEST(Vector2f)
VECTOR_MINMAX_TEST(Vector3f)
VECTOR_MINMAX_TEST(Vector4f)
VECTOR_MINMAX_TEST(Vector2d)
VECTOR_MINMAX_TEST(Vector3d)
VECTOR_MINMAX_TEST(Vector4d)

// template <typename T, size_t Size> void VectorDotTest(TVector<T, Size> vec)
// {
//     vec.SetZero();
//     using VectorType = TVector<T, Size>;

//     VectorType t1;
//     for (auto i = Size; i--; t1[i] = (T)i + static_cast<T>(1))
//         ;
//     VectorType t2;
//     for (auto i = Size; i--; t2[i] = (T)i + static_cast<T>(1))
//         ;
//     T t3 = static_cast<T>(0);
//     for (auto i = Size; i--; t3 += t1[i] * t2[i])
//         ;
//     auto v1 = t1.Dot(t2);
//     auto v2 = VectorType::VectorDot(t1, t1);

//     EXPECT_EQ(t3, v1);
//     EXPECT_EQ(t3, v2);
// #if VECTOR_TEST_OUT
//     std::cerr << "[ Vector   ] " << t1 << std::endl;
//     std::cerr << "[ Vector   ] " << t2 << std::endl;
//     std::cerr << "[ VectorDot] " << t3 << std::endl;
//     std::cerr << "[ Vector   ] " << v1 << std::endl;
//     std::cerr << "[ Vector   ] " << v2 << std::endl;
// #endif
// }

// #define VECTOR_DOT_TEST(VectorType)                                                                                    \
//     TEST(VectorDotTest, VectorType)                                                                                    \
//     {                                                                                                                  \
//         VectorDotTest(VectorType());                                                                                   \
//     }

// VECTOR_DOT_TEST(Vector2i)
// VECTOR_DOT_TEST(Vector3i)
// VECTOR_DOT_TEST(Vector4i)
// VECTOR_DOT_TEST(Vector2f)
// VECTOR_DOT_TEST(Vector3f)
// VECTOR_DOT_TEST(Vector4f)
// VECTOR_DOT_TEST(Vector2d)
// VECTOR_DOT_TEST(Vector3d)
// VECTOR_DOT_TEST(Vector4d)

// template <typename T, size_t Size> void VectorOperatorTest(TVector<T, Size> vec)
// {
//     vec.SetZero();
//     using VectorType = TVector<T, Size>;
//     {
//         VectorType t1;
//         t1.SetValue(static_cast<T>(1));
//         VectorType t2;
//         t2.SetValue(static_cast<T>(2));
//         VectorType t3;
//         t3.SetValue(static_cast<T>(3));

//         auto v1 = t1 + t2;
//         auto v2 = t2 + static_cast<T>(1);

//         EXPECT_EQ(t3, v2);
//         EXPECT_EQ(t3, v1);
// #if VECTOR_TEST_OUT
//         std::cerr << "[ Vector+   ] " << t1 << std::endl;
//         std::cerr << "[ Vector+   ] " << t2 << std::endl;
//         std::cerr << "[ Vector+   ] " << t3 << std::endl;
//         std::cerr << "[ Vector+   ] " << v1 << std::endl;
//         std::cerr << "[ Vector+   ] " << v2 << std::endl;
// #endif
//     }
//     {
//         VectorType t1;
//         t1.SetValue(static_cast<T>(1));
//         VectorType t2;
//         t2.SetValue(static_cast<T>(2));
//         VectorType t3;
//         t3.SetValue(static_cast<T>(3));

//         auto v1 = t2 - t1;
//         auto v2 = t3 - static_cast<T>(1);

//         EXPECT_EQ(t2, v2);
//         EXPECT_EQ(t1, v1);
// #if VECTOR_TEST_OUT
//         std::cerr << "[ Vector-   ] " << t1 << std::endl;
//         std::cerr << "[ Vector-   ] " << t2 << std::endl;
//         std::cerr << "[ Vector-   ] " << t3 << std::endl;
//         std::cerr << "[ Vector-   ] " << v1 << std::endl;
//         std::cerr << "[ Vector-   ] " << v2 << std::endl;
// #endif
//     }
//     {
//         VectorType t1;
//         t1.SetValue(static_cast<T>(2));
//         VectorType t2;
//         t2.SetValue(static_cast<T>(2));
//         VectorType t3;
//         t3.SetValue(static_cast<T>(4));

//         auto v1 = t2 * t1;
//         auto v2 = t1 * static_cast<T>(2);

//         EXPECT_EQ(t3, v2);
//         EXPECT_EQ(t3, v1);
// #if VECTOR_TEST_OUT
//         std::cerr << "[ Vector*   ] " << t1 << std::endl;
//         std::cerr << "[ Vector*   ] " << t2 << std::endl;
//         std::cerr << "[ Vector*   ] " << t3 << std::endl;
//         std::cerr << "[ Vector*   ] " << v1 << std::endl;
//         std::cerr << "[ Vector*   ] " << v2 << std::endl;
// #endif
//     }
//     {
//         VectorType t1;
//         t1.SetValue(static_cast<T>(2));
//         VectorType t2;
//         t2.SetValue(static_cast<T>(3));
//         VectorType t3;
//         t3.SetValue(static_cast<T>(6));

//         auto v1 = t3 / t2;
//         auto v2 = t1 / static_cast<T>(2);

//         EXPECT_EQ(t2, v2);
//         EXPECT_EQ(t1, v1);
// #if VECTOR_TEST_OUT
//         std::cerr << "[ Vector/   ] " << t1 << std::endl;
//         std::cerr << "[ Vector/   ] " << t2 << std::endl;
//         std::cerr << "[ Vector/   ] " << t3 << std::endl;
//         std::cerr << "[ Vector/   ] " << v1 << std::endl;
//         std::cerr << "[ Vector/   ] " << v2 << std::endl;
// #endif
//     }
// }
// #define VECTOR_OPERATOR_TEST(VectorType) \
//     TEST(VectorOperatorTest, VectorType) \
//     { \
//         VectorOperatorTest(VectorType()); \
//     }

// VECTOR_OPERATOR_TEST(Vector2i)
// VECTOR_OPERATOR_TEST(Vector3i)
// VECTOR_OPERATOR_TEST(Vector4i)
// VECTOR_OPERATOR_TEST(Vector2f)
// VECTOR_OPERATOR_TEST(Vector3f)
// VECTOR_OPERATOR_TEST(Vector4f)
// VECTOR_OPERATOR_TEST(Vector2d)
// VECTOR_OPERATOR_TEST(Vector3d)
// VECTOR_OPERATOR_TEST(Vector4d)

// template <typename T, size_t Size> void VectorReflectTest(TVector<T, Size> vec)
// {
// }
// template <typename T, size_t Size> void VectorAngleTest(TVector<T, Size> vec)
// {
// }

// template <typename T, size_t Size> void VectorEqualsTest(TVector<T, Size> vec)
// {
// }

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
    Vector2f ccc{2.f, 3.f};
    std::cout << '\n' << ccc;
    auto yy = Vector2d::VectorZero;
    std::cout << '\n' << yy;
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