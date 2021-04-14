
#include "Core/Common/ArrayRef.h"
#include "gtest/gtest.h"
#include <array>
#include <iostream>
#include <vector>
#include <string>


TEST(ArrayRefConstructorTest, Default)
{
    auto test = ArrayRef<int>();
    EXPECT_EQ(test.empty(), true);
    EXPECT_EQ(test.size(), 0);
    EXPECT_EQ(test.data(), nullptr);
}

TEST(ArrayRefConstructorTest, DataSize)
{

    int testData[6]{1, 2, 3, 4, 5, 6};
    auto test = ArrayRef<int>(testData, 6);
    auto test1 = ArrayRef<int>(testData, 6);

    EXPECT_EQ(test.empty(), false);
    EXPECT_EQ(test.size(), 6);
    EXPECT_EQ(*test.begin(), 1);
    EXPECT_EQ(test.back(), 6);
    EXPECT_EQ(test.equals(test1), true);
}

TEST(ArrayRefConstructorTest, BeginEnd)
{
    char testData[4]{'a', 'b', 'c', 'd'};
    auto test = MakeArrayRef(testData, testData + 4);
    auto test1 = ArrayRef<char>(testData);
    EXPECT_EQ(test.empty(), false);
    EXPECT_EQ(test.size(), 4);
    EXPECT_EQ(test.front(), 'a');
    EXPECT_EQ(test.back(), 'd');
    EXPECT_EQ(test == test1, true);
    EXPECT_EQ(test != test1, false);
}

TEST(ArrayRefConstructorTest, STDArray)
{
    auto testData = std::array<float, 5>{1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    auto test = ArrayRef<float>(testData);
    auto test1 = MakeArrayRef(testData);

    EXPECT_EQ(test.empty(), false);
    EXPECT_EQ(test.size(), 5);
    EXPECT_EQ(test.front(), 1.1f);
    EXPECT_EQ(test.back(), 5.5f);
    EXPECT_EQ(test[3], 3.3f);
    EXPECT_EQ(test == test1, true);
    EXPECT_EQ(test != test1, false);
}

TEST(ArrayRefConstructorTest, STDVector)
{
    auto testData = std::vector<int>{9, 8, 6, 3, 5, 4};
    auto test = ArrayRef<int>(testData);
    auto test1 = MakeArrayRef(testData);
}

TEST(ArrayRefConstructorTest, CArray)
{
    char testData[4]{'a', 'b', 'c', 'd'};
    auto test = ArrayRef<char>(testData);
    auto test1 = MakeArrayRef(testData);
}

TEST(ArrayRefDataChange, Example1)
{
    auto testData = std::vector<int>{9, 8, 6, 3, 5, 4};
    auto test = MakeArrayRef(testData);

    EXPECT_EQ(test.front(), 9);
    test.front() = 1;
    EXPECT_EQ(test.front(), 1);
    EXPECT_EQ(test.back(), 4);
    test.back() = 10;
    EXPECT_EQ(test.back(), 10);
    *test.data() = 99;
    EXPECT_EQ(test.front(), 99);
    test[1] = 88;
    EXPECT_EQ(test.front(), 88);
}

TEST(ArrayRefSlice, Example)
{
    auto testData = std::vector<int>{9, 8, 6, 3, 5, 4};
    auto test = MakeArrayRef(testData);

    EXPECT_EQ(test.front(), 9);
    EXPECT_EQ(test.back(), 4);
    auto test1 = test.slice(2, 5);
    EXPECT_EQ(test1.front(), 8);
    EXPECT_EQ(test1.back(), 5);
}

TEST(ArrayMakeVector, Example)
{
    auto testData = std::vector<int>{9, 8, 6, 3, 5, 4};
    auto test = MakeArrayRef(testData);

    auto test1 = test.make_vector();

    EXPECT_EQ(test1.front(), 9);
    EXPECT_EQ(test1.back(), 4);

    EXPECT_EQ(test.data() == testData.data(), true);
}

TEST(ArrayMakeCopy, Example)
{
    auto testData = std::vector<int>{9, 8, 6, 3, 5, 4};
    auto test = MakeArrayRef(testData);
    auto a = std::allocator<int>();
    auto test1 = test.copy(a);

    EXPECT_EQ(test1.front() == test.front(), true);
    EXPECT_EQ(test1.back() == test.back(), true);
    EXPECT_EQ(test1 == test, true);
    EXPECT_EQ(test1.data() == test.data(), false);
    auto v = test1.make_vector();

    EXPECT_EQ(testData.data() != v.data(), true);
}