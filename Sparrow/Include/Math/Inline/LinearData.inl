

#include "Math/Detail/LinearData.h"

using Math::LinearDataType;

template <typename T, size_t Row, size_t Col, LinearDataType> Math::LinearData<T, Row, Col>::LinearData()
{
}

template <typename T, size_t Row, size_t Col, Math::LinearDataType> void Math::LinearData<T, Row, Col>::SetZero()
{
}

template <typename T, size_t Row, size_t Col, Math::LinearDataType> void Math::LinearData<T, Row, Col>::SetOne()
{
}

template <typename T, size_t Row, size_t Col, Math::LinearDataType>
void Math::LinearData<T, Row, Col>::SetValue(const T &val)
{
}

//------Vector2-----

template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::LinearData()
{
}

template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::LinearData(const T &val)
{
}

template <typename T> Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::LinearData(const T &x, const T &y)
{
}

template <typename T> void Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::SetZero()
{
}

template <typename T> void Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::SetOne()
{
}

template <typename T> void Math::LinearData<T, 2, 1, LinearDataType::Vector><T>::SetValue(const T &val)
{
}

//------Vector3-----

template <typename T> Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::LinearData()
{
}

template <typename T> Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::LinearData(const T &val)
{
}

template <typename T>
Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::LinearData(const T &x, const T &y, const T &z)
{
}

template <typename T> void Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::SetZero()
{
}

template <typename T> void Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::SetOne()
{
}

template <typename T> void Math::LinearData<T, 3, 1, LinearDataType::Vector><T>::SetValue(const T &val)
{
}

//-----Vector4-----

template <typename T> Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::LinearData()
{
}

template <typename T> Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::LinearData(const T &val)
{
}

template <typename T>
Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::LinearData(const T &x, const T &y, const T &z, const T &w)
{
}

template <typename T> void Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::SetZero()
{
}

template <typename T> void Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::SetOne()
{
}

template <typename T> void Math::LinearData<T, 4, 1, LinearDataType::Vector><T>::SetValue(const T &val)
{
}