
// #include "Base/Macros.h"
// #include "Base/TypeTraits.h"
#include "gtest/gtest.h"
#include <iostream>
#include <type_traits>


template <int N> struct marshall
{
    char buf[N];
};

template <typename T> void TestMarshall(const T values[], int num_values)
{
    for (int i = 0; i < num_values; ++i)
    {
        T t0 = values[i];
        marshall<sizeof(T)> m0 = bit_cast<marshall<sizeof(T)>>(t0);
        T t1 = bit_cast<T>(m0);
        marshall<sizeof(T)> m1 = bit_cast<marshall<sizeof(T)>>(t1);
        ASSERT_EQ(0, memcmp(&t0, &t1, sizeof(T)));
        ASSERT_EQ(0, memcmp(&m0, &m1, sizeof(T)));
    }
}

template <typename T, typename I> void TestIntegral(const T values[], int num_values)
{
    for (int i = 0; i < num_values; ++i)
    {
        T t0 = values[i];
        I i0 = bit_cast<I>(t0);
        T t1 = bit_cast<T>(i0);
        I i1 = bit_cast<I>(t1);
        ASSERT_EQ(0, memcmp(&t0, &t1, sizeof(T)));
        ASSERT_EQ(i0, i1);
    }
}

TEST(asdf, asdf)
{
    using aaT = void;
    std::cout << __cplusplus;
    auto a = 3;
    // std::cout<<_MSC_VER;
    std::cout << _MSVC_LANG;
    ASSERT_EQ(IsVoid<void>::value, true);
    ASSERT_EQ(std::is_void<void>::value, true);
}

TEST(BitCast, Bool)
{
    static const bool bool_list[] = {false, true};
    TestMarshall<bool>(bool_list, ARRAY_SIZE(bool_list));
}

TEST(BitCast, Int32)
{
    static const int32_t int_list[] = {0, 1, 100, 2147483647, -1, -100, -2147483647, -2147483647 - 1};
    TestMarshall<int32_t>(int_list, ARRAY_SIZE(int_list));
}

TEST(BitCast, Int64)
{
    static const int64_t int64_list[] = {0, 1, 1LL << 40, -1, -(1LL << 40)};
    TestMarshall<int64_t>(int64_list, ARRAY_SIZE(int64_list));
}

TEST(BitCast, Uint64)
{
    static const uint64_t uint64_list[] = {0, 1, 1LLU << 40, 1LLU << 63};
    TestMarshall<uint64_t>(uint64_list, ARRAY_SIZE(uint64_list));
}

TEST(BitCast, Float)
{
    static const float float_list[] = {0.0f,  1.0f,   -1.0f,  10.0f,    -10.0f,  1e10f,
                                       1e20f, 1e-10f, 1e-20f, 2.71828f, 3.14159f};
    TestMarshall<float>(float_list, ARRAY_SIZE(float_list));
    TestIntegral<float, int>(float_list, ARRAY_SIZE(float_list));
    TestIntegral<float, unsigned>(float_list, ARRAY_SIZE(float_list));
}

TEST(BitCast, Double)
{
    static const double double_list[] = {0.0,
                                         1.0,
                                         -1.0,
                                         10.0,
                                         -10.0,
                                         1e10,
                                         1e100,
                                         1e-10,
                                         1e-100,
                                         2.718281828459045,
                                         3.141592653589793238462643383279502884197169399375105820974944};
    TestMarshall<double>(double_list, ARRAY_SIZE(double_list));
    TestIntegral<double, int64_t>(double_list, ARRAY_SIZE(double_list));
    TestIntegral<double, uint64_t>(double_list, ARRAY_SIZE(double_list));
}