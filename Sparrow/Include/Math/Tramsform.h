#pragma once
#include "Math/Matrix.h"
#include "Math/Vector.h"

#include "Math/Detail/TMatrix.h"
#include "Math/Detail/TVector.h"
#include <Eigen/Dense>

namespace Transform
{

// template<typename T>
Matrix4x4f Translate(const Vector3f &val);
Matrix4x4f Translate(const Vector3f &val)
{
    static_assert(sizeof(Eigen::Transform<float, 3, Eigen::Affine>) == sizeof(Matrix4x4f));
    Matrix4x4f temp;
    new (&temp) Eigen::Transform<float, 3, Eigen::Affine>(Eigen::Translation3f(val.x, val.y, val.z));

    // Eigen::Affine3f transform(Eigen::Translation3f(val.x, val.y, val.z));
    // (*((Eigen::Matrix<float, 4, 4> *)(&temp))) = transform.matrix();

    return std::move(temp);
}

Matrix4x4f Rotate(float x, float y, float z);
Matrix4x4f Rotate(float angle, Vector3f axis)
{
    static_assert(sizeof(Eigen::Transform<float, 3, Eigen::Affine>) == sizeof(Matrix4x4f));
    Matrix4x4f temp;
    new (&temp) Eigen::Transform<float, 3, Eigen::Affine>(
        Eigen::AngleAxisf(angle * static_cast<float>(0.01745329251994329576923690768489),
                          *((Eigen::Matrix<float, 3, 1> *)const_cast<TVector<float, 3> *>(&axis))));
    return std::move(temp);
}

// Scale

// Rotate  x y z

// LookAt

// Perspective

Matrix4x4f Perspective(float x, float y, float far, float near);
Matrix4x4f Perspective(float x, float y, float far, float near)
{
    float range = far - near;
    Matrix4x4f temp;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(0, 0) = x;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(1, 1) = y;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(2, 2) = -(near + far) / range;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(2, 3) = (-2 * near * far) / range;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(3, 2) = -1;
    return std::move(temp);
}

Matrix4x4f Perspective(float fovy, float x, float y, float far, float near)
{
    float const tanHalfFovy = tan(fovy / static_cast<float>(2));
    float aspect = x / y;
    float range = far - near;
    assert(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));
    Matrix4x4f temp;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(0, 0) = static_cast<float>(1) / (aspect * tanHalfFovy);
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(1, 1) = static_cast<float>(1) / (tanHalfFovy);
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(2, 2) = (near + far) / range;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(2, 3) = (2 * near * far) / range;
    (*((Eigen::Matrix<float, 4, 4> *)(&temp)))(3, 2) = -1;
    return std::move(temp);
}

// Ortho

} // namespace Transform