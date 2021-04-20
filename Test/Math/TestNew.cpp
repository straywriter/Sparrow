
#include "Math/Vector.h"
#include "gtest/gtest.h"

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

}