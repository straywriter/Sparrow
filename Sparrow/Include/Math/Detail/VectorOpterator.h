#pragma once
#include "Core/Config.h"

namespace Math
{
template <typename T, size_t size>
TVector<T, size> operator+(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> TVector<T, size> operator+(const TVector<T, size> &left, const T &val);

template <typename T, size_t size>
TVector<T, size> operator-(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> TVector<T, size> operator-(const TVector<T, size> &left, const T &val);

template <typename T, size_t size>
TVector<T, size> operator*(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> TVector<T, size> operator*(const TVector<T, size> &left, const T &val);

template <typename T, size_t size>
TVector<T, size> operator/(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> TVector<T, size> operator/(const TVector<T, size> &left, const T &val);

template <typename T, size_t size> TVector<T, size> operator-(const TVector<T, size> &left);

template <typename T, size_t size> void operator+=(TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> void operator+=(TVector<T, size> &left, const T &val);

template <typename T, size_t size> void operator-=(TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> void operator-=(TVector<T, size> &left, const T &val);

template <typename T, size_t size> void operator*=(TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> void operator*=(TVector<T, size> &left, const T &val);

template <typename T, size_t size> void operator/=(TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> void operator/=(TVector<T, size> &left, const T &val);

template <typename T, size_t size> bool operator==(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> bool operator!=(const TVector<T, size> &left, const TVector<T, size> &right);

} // namespace Math