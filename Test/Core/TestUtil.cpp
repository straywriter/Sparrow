
#include "Core/Util/UtilMacro.h"

#include "gtest/gtest.h"
#include <iostream>


TEST(ArraySizeHelperTest, Example)
{
    char s[7];
    int a[9];
    float f[5];



std::cout<<__cplusplus;

    EXPECT_EQ(sizeof(ArraySizeHelper(f)), 5);
    EXPECT_EQ(sizeof(ArraySizeHelper(s)), 7);
    EXPECT_EQ(sizeof(ArraySizeHelper(a)), 9);
    EXPECT_EQ(ARRAY_SIZE(s), 7);
    EXPECT_EQ(ARRAY_SIZE(a), 9);
    EXPECT_EQ(ARRAY_SIZE(f), 5);
}