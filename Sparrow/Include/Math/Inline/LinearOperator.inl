#include "Math/Detail/LinearOperator.h"

using namespace Math;

template<typename Derived, typename T, size_t Size>
inline T LinearOperator<Derived, T, Size>::operator[](size_t index)
{
return T();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator+(const Derived & val) const
{
return Derived();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator+(T val) const
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator-(T val) const
{
return Derived();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator-(T val) const
{
return Derived();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator*(const Derived & val) const
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator*(T scale) const
{
return Derived();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator/(const Derived & val) const
{
return Derived();
}   

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator/(T scale)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator-() const
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator+=(T val)
{
return Derived();
}

template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator+=(T val)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator-=(const Derived & val)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator-=(T val)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator*=(const Derived & val)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator*=(T scale)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator/=(const Derived & val)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline Derived LinearOperator<Derived, T, Size>::operator/=(T scale)
{
return Derived();
}
template<typename Derived, typename T, size_t Size>
inline bool LinearOperator<Derived, T, Size>::operator==(const Derived & val) const
{
return false;
}

template<typename Derived, typename T, size_t Size>
inline bool LinearOperator<Derived, T, Size>::operator!=(const Derived & val) const
{
return false;
}
