#include "Math/Math.h"
#include "gtest/gtest.h"
#include <iostream>
#include <math.h>

TEST(VectorConvert, Vector)
{
    Matrix4x4f aa;

    auto xx = Transform::Perspective(1.f, 2, 3, 4);
    auto vv = Transform::Perspective((45.0f), 800.f, 600.f, 0.1f, 100.0f);
    // std::cout<<vv;
    Matrix4x4f cc = Transform::Translate(Vector3f(1, 2, 3));
    // std::cout<<cc;
    // std::cout<<cc;
    Vector3f vecn(1.0f, 0.3f, 0.5f);
    vecn.Normalize();
    Matrix4x4f uu = Transform::Rotate(20.f, vecn);
    std::cout << uu;
    // Matrix3f m;
    // m = AngleAxisf(0.25 * EIGEN_PI, Vector3f::UnitX()) * AngleAxisf(0.5 * M_PI, Vector3f::UnitY()) *
    //     AngleAxisf(0.33 * M_PI, Vector3f::UnitZ());
    // cout << m << endl << "is unitary: " << m.isUnitary() << endl;
}