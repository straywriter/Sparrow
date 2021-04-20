

#include "gtest/gtest.h"
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;
 #define ASSERT_APPROX(left, right) 

// NOTE the following workaround was needed on some 32 bits builds to kill extra precision of x87 registers.
// It seems that it os not needed anymore, but let's keep it here, just in case...

template <typename T> EIGEN_DONT_INLINE void kill_extra_precision(T & /* x */)
{
    // This one worked but triggered a warning:
    /* eigen_assert((void*)(&x) != (void*)0); */
    // An alternative could be:
    /* volatile T tmp = x; */
    /* x = tmp; */
}

template <typename BoxType> void alignedbox(const BoxType &_box)
{
    /* this test covers the following files:
       AlignedBox.h
    */
    typedef typename BoxType::Scalar Scalar;
    typedef typename NumTraits<Scalar>::Real RealScalar;
    typedef Matrix<Scalar, BoxType::AmbientDimAtCompileTime, 1> VectorType;

    const Index dim = _box.dim();

    VectorType p0 = VectorType::Random(dim);
    VectorType p1 = VectorType::Random(dim);
    while (p1 == p0)
    {
        p1 = VectorType::Random(dim);
    }
    RealScalar s1 = internal::random<RealScalar>(0, 1);

    BoxType b0(dim);
    BoxType b1(VectorType::Random(dim), VectorType::Random(dim));
    BoxType b2;

    kill_extra_precision(b1);
    kill_extra_precision(p0);
    kill_extra_precision(p1);

    b0.extend(p0);
    b0.extend(p1);
    ASSERT_TRUE(b0.contains(p0 * s1 + (Scalar(1) - s1) * p1));
    ASSERT_TRUE(b0.contains(b0.center()));
    ASSERT_APPROX(b0.center(), (p0 + p1) / Scalar(2));

    (b2 = b0).extend(b1);
    ASSERT_TRUE(b2.contains(b0));
    ASSERT_TRUE(b2.contains(b1));
    ASSERT_APPROX(b2.clamp(b0), b0);

    // intersection
    BoxType box1(VectorType::Random(dim));
    box1.extend(VectorType::Random(dim));
    BoxType box2(VectorType::Random(dim));
    box2.extend(VectorType::Random(dim));

    ASSERT_TRUE(box1.intersects(box2) == !box1.intersection(box2).isEmpty());

    // alignment -- make sure there is no memory alignment assertion
    BoxType *bp0 = new BoxType(dim);
    BoxType *bp1 = new BoxType(dim);
    bp0->extend(*bp1);
    delete bp0;
    delete bp1;

    // sampling
    for (int i = 0; i < 10; ++i)
    {
        VectorType r = b0.sample();
        ASSERT_TRUE(b0.contains(r));
    }
}

template <typename BoxType> void alignedboxCastTests(const BoxType &_box)
{
    // casting
    typedef typename BoxType::Scalar Scalar;
    typedef Matrix<Scalar, BoxType::AmbientDimAtCompileTime, 1> VectorType;

    const Index dim = _box.dim();

    VectorType p0 = VectorType::Random(dim);
    VectorType p1 = VectorType::Random(dim);

    BoxType b0(dim);

    b0.extend(p0);
    b0.extend(p1);

    // const int Dim = BoxType::AmbientDimAtCompileTime;
    // typedef typename GetDifferentType<Scalar>::type OtherScalar;
    // AlignedBox<OtherScalar, Dim> hp1f = b0.template cast<OtherScalar>();
    // ASSERT_APPROX(hp1f.template cast<Scalar>(), b0);
    // AlignedBox<Scalar, Dim> hp1d = b0.template cast<Scalar>();
    // ASSERT_APPROX(hp1d.template cast<Scalar>(), b0);
}

void specificTest1()
{
    Vector2f m;
    m << -1.0f, -2.0f;
    Vector2f M;
    M << 1.0f, 5.0f;

    typedef AlignedBox2f BoxType;
    BoxType box(m, M);

    Vector2f sides = M - m;
    ASSERT_APPROX(sides, box.sizes());
    ASSERT_APPROX(sides[1], box.sizes()[1]);
    ASSERT_APPROX(sides[1], box.sizes().maxCoeff());
    ASSERT_APPROX(sides[0], box.sizes().minCoeff());

    ASSERT_APPROX(14.0f, box.volume());
    ASSERT_APPROX(53.0f, box.diagonal().squaredNorm());
    ASSERT_APPROX(std::sqrt(53.0f), box.diagonal().norm());

    ASSERT_APPROX(m, box.corner(BoxType::BottomLeft));
    ASSERT_APPROX(M, box.corner(BoxType::TopRight));
    Vector2f bottomRight;
    bottomRight << M[0], m[1];
    Vector2f topLeft;
    topLeft << m[0], M[1];
    ASSERT_APPROX(bottomRight, box.corner(BoxType::BottomRight));
    ASSERT_APPROX(topLeft, box.corner(BoxType::TopLeft));
}

void specificTest2()
{
    Vector3i m;
    m << -1, -2, 0;
    Vector3i M;
    M << 1, 5, 3;

    typedef AlignedBox3i BoxType;
    BoxType box(m, M);

    Vector3i sides = M - m;
    ASSERT_APPROX(sides, box.sizes());
    ASSERT_APPROX(sides[1], box.sizes()[1]);
    ASSERT_APPROX(sides[1], box.sizes().maxCoeff());
    ASSERT_APPROX(sides[0], box.sizes().minCoeff());

    ASSERT_APPROX(42, box.volume());
    ASSERT_APPROX(62, box.diagonal().squaredNorm());

    ASSERT_APPROX(m, box.corner(BoxType::BottomLeftFloor));
    ASSERT_APPROX(M, box.corner(BoxType::TopRightCeil));
    Vector3i bottomRightFloor;
    bottomRightFloor << M[0], m[1], m[2];
    Vector3i topLeftFloor;
    topLeftFloor << m[0], M[1], m[2];
    ASSERT_APPROX(bottomRightFloor, box.corner(BoxType::BottomRightFloor));
    ASSERT_APPROX(topLeftFloor, box.corner(BoxType::TopLeftFloor));
}

TEST(Geomotory,AlignedBox)
{
    for (int i = 0; i < 10; i++)
    {
        alignedbox(AlignedBox2f());
        alignedboxCastTests(AlignedBox2f());

        alignedbox(AlignedBox3f());
        alignedboxCastTests(AlignedBox3f());

        alignedbox(AlignedBox4d());
        alignedboxCastTests(AlignedBox4d());

        alignedbox(AlignedBox1d());
        alignedboxCastTests(AlignedBox1d());

        alignedbox(AlignedBox1i());
        alignedbox(AlignedBox2i());
        alignedbox(AlignedBox3i());

        alignedbox(AlignedBox<double, Dynamic>(4));
    }
    specificTest1();
    specificTest2();
}