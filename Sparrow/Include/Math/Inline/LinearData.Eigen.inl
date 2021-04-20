
#include "Core/Config/MathConfig.h"
#if MATH_CONFIG_EIGEN

#include "Math/Detail/LinearData.h"

#include <Eigen/Dense>

using Math::LinearDataType;

//-----None-----
template <typename Derived, typename T, size_t Row, size_t Col>
inline Math::LinearData<Derived, T, Row, Col, LinearDataType::None>::LinearData()
{
}

template <typename Derived, typename T, size_t Row, size_t Col>
inline Math::LinearData<Derived, T, Row, Col, LinearDataType::None>::LinearData(const T &val)
{
}
template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::None>::SetZero()
{
}

template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::None>::SetOne()
{
}
template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::None>::SetValue(const T &val)
{
}

//------Vector2-----

template <typename Derived, typename T> inline Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::LinearData()
{
    auto p = (Eigen::Matrix<T, 2, 1> *)(this);
    p->setZero();
}

template <typename Derived, typename T>
inline Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::LinearData(const T &val)
{
    auto p = (Eigen::Matrix<T, 2, 1> *)(this);
    p->setConstant(val);
}

template <typename Derived, typename T>
inline Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y) : x(x), y(y)
{
}

template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::SetZero()
{
    auto p = (Eigen::Matrix<T, 2, 1> *)(this);
    p->setZero();
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::SetOne()
{
}
template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 2, 1, LinearDataType::Vector>::SetValue(const T &val)
{
}

//------Vector3-----

template <typename Derived, typename T> Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData()
{
    auto p = (Eigen::Matrix<T, 3, 1> *)(this);
    p->setZero();
}

template <typename Derived, typename T>
Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(const T &val)
{
    auto p = (Eigen::Matrix<T, 3, 1> *)(this);
    p->setConstant(val);
}

template <typename Derived, typename T>
Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z)
    : x(x), y(y), z(z)
{
}
template <typename Derived, typename T>
template <typename OtherDerived, typename OtherType, size_t OtherSize>
inline Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(
    const LinearData<OtherDerived, OtherType, OtherSize, 1, LinearDataType::Vector> &val)
{
    for (auto i = min(OtherSize, size_t(3)); i--; *(&x + i) = val.Get(i))
        ;
    if constexpr (OtherSize == size_t(2))
        z = 0;
}

template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetZero()
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetOne()
{
}
template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetValue(const T &val)
{
}

template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::Set(const T &x, const T &y, const T &z)
{
}

//-----Vector4-----

template <typename Derived, typename T> Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData()
{
    auto p = (Eigen::Matrix<T, 4, 1> *)(this);
    p->setZero();
}

template <typename Derived, typename T>
Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData(const T &val)
{
    auto p = (Eigen::Matrix<T, 4, 1> *)(this);
    p->setConstant(val);
}

template <typename Derived, typename T>
Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z, const T &w)
    : x(x), y(y), z(z), w(w)
{
}

template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetZero()
{
    auto p = (Eigen::Matrix<T, 4, 1> *)(this);
    p->setZero();
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetOne()
{
    // auto p = (Eigen::Matrix<T, 4, 1> *)(this);
    // p->setOnes();
}
template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetValue(const T &val)
{
    auto p = (Eigen::Matrix<T, 4, 1> *)(this);
    p->setConstant(val);
}
//-----Matrix-----

template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::Matrix>::SetZero()
{
    auto p = (Eigen::Matrix<T, Row, Col> *)(this);
    p->setZero();
}

template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::Matrix>::SetOne()
{
}
template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived, T, Row, Col, LinearDataType::Matrix>::SetValue(const T &val)
{
    auto p = (Eigen::Matrix<T, Row, Col> *)(this);
    p->setConstant(val);
}

#endif