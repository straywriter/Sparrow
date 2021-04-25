#include "Math/Math.h"
#include "gtest/gtest.h"
#include <Eigen/Dense>
#include <iostream>
#include <math.h>
// #pragma warning( push )
// #pragma warning( disable : 4201 )

using namespace Math;

#define VECTOR_TEST_OUT 0

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

        // t1[0] = static_cast<T>(3);

        VectorType v10 = t4;
        for (auto i = Size; i--; v10.SetOne(i))
            ;
        EXPECT_EQ(t3, v10);
        VectorType v11 = t4;
        for (auto i = Size; i--; v11.SetZero(i))
            ;
        EXPECT_EQ(t2, v11);;
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
   std:: cout << t1;
    float t2=static_cast<T>(0);
    for (auto i = Size; i--; t2 += ((float)i + static_cast<float>(1))* ((float)i + static_cast<float>(1)))
        ;

    EXPECT_EQ(t1.SquaredLenth(), t2);
     std:: cout<<" " <<t2;
    auto vv = std::sqrt(t2);
   std:: cout<<" " <<t2;
    EXPECT_EQ(t1.Lenth(), (T)vv);
}
#define VECTOR_LENTH_TEST(VectorType)                                                                                 \
    TEST(VectorLenthTest, VectorType)                                                                                 \
    {                                                                                                                  \
        VectorLenthTest(VectorType());                                                                                \
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
   std:: cout << t1;
    T t2;
    for (auto i = Size; i--; *(&t2.x + (int)i) = ((T)i + static_cast<T>(1)))
        ;
    for (auto i = Size; i--; t2[i] = t2[i] / t2.Lenth())
        ;

 std::   cout << t1;
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