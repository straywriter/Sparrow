
#include "Core/Config/MathConfig.h"
#if MATH_CONFIG_BASE

#include "Math/Detail/LinearData.h"


using Math::LinearDataType;

//-----None-----
template <typename Derived, typename T, size_t Row, size_t Col> inline Math::LinearData<Derived,T, Row, Col, LinearDataType::None>::LinearData()
{
}

template <typename Derived, typename T, size_t Row, size_t Col>
inline Math::LinearData<Derived,T, Row, Col, LinearDataType::None>::LinearData(const T &val)
{
}
template <typename Derived, typename T, size_t Row, size_t Col> inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::None>::SetZero()
{
}

template <typename Derived, typename T, size_t Row, size_t Col> inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::None>::SetOne()
{
}
template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::None>::SetValue(const T &val)
{
}

//------Vector2-----

template <typename Derived, typename T> inline Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::LinearData()
{
}

template <typename Derived, typename T> inline Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::LinearData(const T &val)
{
}

template <typename Derived, typename T> inline Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y)
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::SetZero()
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::SetOne()
{
}
template <typename Derived, typename T> inline void Math::LinearData<Derived,T, 2, 1, LinearDataType::Vector>::SetValue(const T &val)
{
}

//------Vector3-----

template <typename Derived, typename T> Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData()
{
    
}

template <typename Derived, typename T> Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(const T &val)
{
  
}

template <typename Derived, typename T> Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z)
{
}

template<typename Derived, typename T>
template<typename OtherDerived, typename OtherType, size_t OtherSize>
inline Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::LinearData(LinearData<OtherDerived,OtherType,OtherSize,1,LinearDataType::Vector>& val)
{
        for ( auto i = min(OtherSize, size_t(3)) ; i--; *(this->GetDataPtr() + i) = *(val.GetDataPtr() + i))
            ;
        if (OtherSize == 2)
            z = 0;
}


template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetZero()
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetOne()
{
}
template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::SetValue(const T &val)
{
}

template <typename Derived, typename T>
inline void Math::LinearData<Derived, T, 3, 1, LinearDataType::Vector>::Set(const T &x, const T &y, const T &z)
{
}

//-----Vector4-----

template <typename Derived, typename T> Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData()
{
}

template <typename Derived, typename T> Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData(const T &val)
{
}

template <typename Derived, typename T>
Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::LinearData(const T &x, const T &y, const T &z, const T &w)
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetZero()
{
}

template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetOne()
{
}
template <typename Derived, typename T> inline void Math::LinearData<Derived, T, 4, 1, LinearDataType::Vector>::SetValue(const T &val)
{
}
//-----Matrix-----

template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::Matrix>::SetZero()
{
}

template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::Matrix>::SetOne()
{
}
template <typename Derived, typename T, size_t Row, size_t Col>
inline void Math::LinearData<Derived,T, Row, Col, LinearDataType::Matrix>::SetValue(const T &val)
{
}

#endif