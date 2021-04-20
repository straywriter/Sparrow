
#include "Math/Detail/LinearData.h"

#include <Eigen/Dense>

using Math::LinearDataType;

// template <typename T, size_t Row, size_t Col>
// inline Math::LinearData<T, Row, Col, LinearDataType::Vector>::LinearData()
// {
// }

// template <typename T, size_t Row, size_t Col> inline Math::LinearData<T, Row, Col, LinearDataType::Vector>::LinearData()
// {
// }

// template <typename T, size_t Row, size_t Col>
// inline Math::LinearData<T, Row, Col, LinearDataType::Vector>::LinearData(const T &val)
// {
// }

template<typename T, size_t Row, size_t Col >
inline Math::LinearData<T, Row, Col, LinearDataType::Vector >::LinearData()
{
}

//------Vector2-----

// template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector>::LinearData()
// {
// }

template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector>::LinearData(const T &val)
{
}

template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y)
{
}

//------Vector3-----

// template <typename T> Math::LinearData<T, 3, 1, LinearDataType::Vector>::LinearData()
// {
//     auto p = (Eigen::Matrix<T, 3, 1> *)(this);
//     p->setZero();
// }

template <typename T> Math::LinearData<T, 3, 1, LinearDataType::Vector>::LinearData(const T &val)
{
    auto p = (Eigen::Matrix<T, 3, 1> *)(this);
    p->setConstant(val);
}

template <typename T> Math::LinearData<T, 3, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z)
{
}

//-----Vector4-----

// template <typename T> Math::LinearData<T, 4, 1, LinearDataType::Vector>::LinearData()
// {
// }

template <typename T> Math::LinearData<T, 4, 1, LinearDataType::Vector>::LinearData(const T &val)
{
}

template <typename T>
Math::LinearData<T, 4, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z, const T &w)
{
}
