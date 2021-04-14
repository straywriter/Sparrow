#pragma once

#include "Core/Config/MathConfig.h"
#include <iostream>
#include <utility>

//------TVector------
template <typename T, size_t size> inline Math::TVector<T, size>::TVector()
{
    for (auto i = size; i--; *this + i - 1 = static_cast<T>(0))
        ;
}

// template <typename T, size_t size> inline Math::TVector<T, size>::TVector(T val)
//{
//    for (auto i = size; i--; this->data[i] = val)
//        ;
//}
//
//------TVector<T,2>------

template <typename T> inline Math::TVector<T, 2>::TVector() : x(0), y(0)
{
}

template <typename T> inline Math::TVector<T, 2>::TVector(const T &x, const T &y) : x(x), y(y)
{
}
template <typename T> inline Math::TVector<T, 2>::TVector(const T &val) : x(val), y(val)
{
}

// template <typename T> inline void Math::TVector<T, 2>::Set(const T& x, const T& y)
// {
// 	this->x = x;
// 	this->y = y;
// }

// template <typename T> inline size_t Math::TVector<T, 2>::Size()
// {
// 	return size_t(2);
// }
// template<typename T>
// inline T Math::TVector<T, 2>::SquaredLenth()
// {
// 	return T();
// }

// template <typename T>
// Math::TVector<T, 2> Math::TVector<T, 2>::CrossProduct(const TVector& left, const TVector& right)
// {
// }

// template <typename T>
// Math::TVector<T, 2> Math::TVector<T, 2>::DotProduct(const TVector& left, const TVector& right)
// {
// }

// template<typename T>
// inline T Math::TVector<T, 2>::Lenth()
// {
// 	return T();
// }
// template<typename T>
// inline void Math::TVector<T, 2>::Normalize()
// {
// }
// template<typename T>
// inline void Math::TVector<T, 2>::Cross(const TVector<T, 2>& val)
// {
// }

// template<typename T>
// inline void Math::TVector<T, 2>::Dot(const TVector<T, 2>& val)
// {
// }

// // template<typename T>
// // inline TVector<T,2> Math::TVector<T, 2>::Abs()
// // {
// // return TVector<T,2>();
// // }
// template<typename T>
// inline T Math::TVector<T, 2>::Max()
// {
// 	return T();
// }

// template<typename T>
// inline T Math::TVector<T, 2>::Min()
// {
// 	return T();
// }

// template<typename T>
// inline TVector<T,2> Math::TVector<T, 2>::Lerp(TVector a, TVector b, float t)
// {
// return TVector<T,2>();
// }

//------TVector<T,3>------
template <typename T> inline Math::TVector<T, 3>::TVector() : x(0), y(0), z(0)
{
}

template <typename T> inline Math::TVector<T, 3>::TVector(const T &v)
{
    this->x = v;
    this->y = v;
    this->z = v;
}

template <typename T> inline Math::TVector<T, 3>::TVector(const T &x, const T &y, const T &z) : x(x), y(y), z(z)
{
}

// template<typename T>
// inline T Math::TVector<T, 3>::Lenth()
// {
// 	return T();
// }

// template <typename T> inline size_t Math::TVector<T, 3>::Size()
// {
// 	return size_t(3);
// }
// template<typename T>
// inline T Math::TVector<T, 3>::SquaredLenth()
// {
// 	return T();
// }

// template <typename T>
// Math::TVector<T, 3> Math::TVector<T, 3>::CrossProduct(const TVector& left, const TVector& right)
// {
// }

// template <typename T>
// Math::TVector<T, 3> Math::TVector<T, 3>::DotProduct(const TVector& left, const TVector& right)
// {
// }

// template<typename T>
// inline void Math::TVector<T, 3>::Normalize()
// {
// }
// template<typename T>
// inline void Math::TVector<T, 3>::Cross(const TVector<T, 3>& val)
// {
// }

// template<typename T>
// inline void Math::TVector<T, 3>::Dot(const TVector<T, 3>& val)
// {
// }

// // template<typename T>
// // inline TVector<T,3> Math::TVector<T, 3>::Abs()
// // {
// // return TVector<T,3>();
// // }
// template<typename T>
// inline T Math::TVector<T, 3>::Max()
// {
// 	return T();
// }

// template<typename T>
// inline T Math::TVector<T, 3>::Min()
// {
// 	return T();
// }

// template<typename T>
// inline TVector<T,3> Math::TVector<T, 3>::Lerp(TVector a, TVector b, float t)
// {
// return TVector<T,3>();
// }

//------TVector4------
template <typename T>
inline Math::TVector<T, 4>::TVector()
    : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0))
{
}

template <typename T> inline Math::TVector<T, 4>::TVector(const T &val) : x(val), y(val), z(val), w(val)
{
}

template <typename T>
inline Math::TVector<T, 4>::TVector(const T &x, const T &y, const T &z, const T &w) : x(x), y(y), z(z), w(w)
{
}

// template <typename T> inline size_t Math::TVector<T, 4>::Size()
// {
// 	return size_t(4);
// }
// template<typename T>
// inline T Math::TVector<T, 4>::SquaredLenth()
// {
// 	return T();
// }

// template <typename T>
// Math::TVector<T, 4> Math::TVector<T, 4>::CrossProduct(const TVector& left, const TVector& right)
// {
// }

// template <typename T>
// Math::TVector<T, 4> Math::TVector<T, 4>::DotProduct(const TVector& left, const TVector& right)
// {
// }

// template<typename T>
// inline T Math::TVector<T, 4>::Lenth()
// {
// 	return T();
// }
// template<typename T>
// inline void Math::TVector<T, 4>::Normalize()
// {
// }
// template<typename T>
// inline void Math::TVector<T, 4>::Cross(const TVector<T, 4>& val)
// {
// }

// template<typename T>
// inline void Math::TVector<T, 4>::Dot(const TVector<T, 4>& val)
// {
// }

// // template<typename T>
// // inline TVector<T,4> Math::TVector<T, 4>::Abs()
// // {
// // return TVector<T,4>();
// // }
// template<typename T>
// inline T Math::TVector<T, 4>::Max()
// {
// 	return T();
// }

// template<typename T>
// inline T Math::TVector<T, 4>::Min()
// {
// 	return T();
// }

// template<typename T>
// inline TVector<T,4> Math::TVector<T, 4>::Lerp(TVector a, TVector b, float t)
// {
// return TVector<T,4>();
// }

//---------------vector3f---------

// using namespace Math;

// inline TVector<float, 3>::TVector()
// {
//     for (auto i = 3; i--; this->data[i] = 0.f)
//         ;
// }
// inline TVector<float, 3>::TVector(float val)
// {
//     for (auto i = 3; i--; this->data[i] = val)
//         ;
// }
// inline TVector<float, 3>::TVector(float x, float y, float z)
// {
//     this->x = x;
//     this->y = y;
//     this->z = z;
// }
// inline TVector<float, 3>::TVector(float *p)
// {
//     for (auto i = 3; i--; this->data[i] = *(p + i))
//         ;
// }
// inline void TVector<float, 3>::Set(float x, float y, float z)
// {
//     this->x = x;
//     this->y = y;
//     this->z = z;
// }

// inline float TVector<float, 3>::operator[](size_t index)
// {
//     // assert(index < 3);
//     return this->data[index];
// }
// inline TVector<float, 3> TVector<float, 3>::CrossProduct(const TVector &left, const TVector &right)
// {
//     TVector<float, 3> temp;

//     temp.x = left.y * right.z - right.y * left.z;
//     temp.y = left.z * right.x - left.x * right.z;
//     temp.z = left.x * right.y - left.y * right.x;

//     return temp;
// }
// inline TVector<float, 3> TVector<float, 3>::DotProduct(const TVector &left, const TVector &right)
// {
//     TVector<float, 3> temp;
//     for (auto i = 3; i--; temp.data[i] = left.data[i] * right.data[i])
//         ;
//     return temp;
// }

template <typename T, size_t size> TVector<T, 2> CrossProduct(const TVector<T, 2> &left, const TVector<T, 2> &right)
{
    TVector<T, 2> temp;
  
    return std::move(temp);
}
template <typename T, size_t size> TVector<T, 3> CrossProduct(const TVector<T, 3> &left, const TVector<T, 3> &right)
{
    TVector<T, 3> temp;
    temp.x = left.y * right.z - right.y * left.z;
    temp.y = left.z * right.x - left.x * right.z;
    temp.z = left.x * right.y - left.y * right.x;
    return std::move(temp);
}