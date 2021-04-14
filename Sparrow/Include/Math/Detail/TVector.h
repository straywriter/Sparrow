#pragma once

// TVector 2 3 4
namespace Math
{
template <typename T, size_t size> class TVector
{
  public:
    TVector();

  public:
};

template <typename T> class TVector<T, 2>
{
  public:
    T x, y;

  public:
    TVector<T, 2>();
    TVector<T, 2>(const T &x, const T &y);
    TVector<T, 2>(const T &val);

    //    TVector<T, 2>(const TVector &val);
    //   TVector<T, 2>(TVector<T, 2> &&rval);
    ~TVector<T, 2>() = default;
};

template <typename T> class TVector<T, 3>
{
  public:
    T x, y, z;

  public:
    TVector<T, 3>();
    TVector<T, 3>(const T &val);
    TVector<T, 3>(const T &x, const T &y, const T &z);
    TVector<T, 3>(const T *ptr);

  public:
    template <typename T, size_t size>
    static TVector<T, 3> CrossProduct(const TVector<T, 3> &left, const TVector<T, 3> &right);
};

template <typename T> class TVector<T, 4>
{
  public:
    T x, y, z, w;

  public:
    TVector<T, 4>();
    TVector<T, 4>(const T &val);
    TVector<T, 4>(const T &x, const T &y, const T &z, const T &w);
    TVector<T, 4>(const T *ptr);

    //   public:
    //     void Set(const T &x, const T &y, const T &z, const T &w);

    //     size_t Size();

    //     T Lenth();
    //     T SquaredLenth();
    //     void Normalize();

    //     void Cross(const TVector<T, 4> &val);
    //     void Dot(const TVector<T, 4> &val);

    //     // bool Equals(const TVector<T, 2>& val);
    //     // bool Equals(const TVector<T, 2>& val,int Tolerance);

    //     TVector<T, 4> Abs();
    //     T Max();
    //     T Min();

    //   public:
    //     static TVector CrossProduct(const TVector &left, const TVector &right);
    //     static TVector DotProduct(const TVector &left, const TVector &right);

    //     static TVector Lerp(TVector a, TVector b, float t);

    //     static TVector Project(TVector vector, TVector onNormal);
    //     static TVector ProjectOnPlane(TVector vector, TVector planeNormal);

    //   public:
};

namespace Vector
{

template <typename T, size_t size>
TVector<T, size> DotProduct(const TVector<T, size> &left, const TVector<T, size> &right);

template <typename T, size_t size> TVector<T, size> Nomalize(const TVector<T, size> &vec);

template <typename T, size_t size> TVector<T, size> Equal(const TVector<T, size> &vec);

} // namespace Vector

} // namespace Math

#include "Math/Detail/VectorOpterator.h"