#pragma once

#include <Base/String.h>
#include <External/folly.h>

template <typename T> inline Sparrow::TString<T>::TString() : folly::basic_fbstring<T>()
{
}

// template<typename T>
// inline Sparrow::TString<T>::TString(const TString<T> & str): folly::basic_fbstring<T>(str)
// {
// }

template <typename T> inline Sparrow::TString<T>::TString(const value_type *str) : folly::basic_fbstring<T>(str)
{
}

template <typename T> inline const T *Sparrow::TString<T>::Data() const
{
    return data();
}

template <typename T> inline std::basic_string<T> Sparrow::TString<T>::ToStdString()
{
    return toStdString();
}

template <typename T> inline void Sparrow::TString<T>::Reserve(size_type res_arg)
{
    reserve(res_arg);
}

