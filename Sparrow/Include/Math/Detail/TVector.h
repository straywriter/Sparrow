#pragma once

#include "Math/Detail/LinearData.h"

#include "Math/Detail/VectorBase.h"
#include <assert.h>
#include <utility>

namespace Math
{
template <typename T, size_t Size> class TVector
{
};

template <typename T> class TVector<T, 2>
{
  public:
    T x, y;

  public:
    TVector();

    TVector(const T &val);

    TVector(const T &x, const T &y);

    void SetZero();
    void SetZero(size_t index);
    void SetOne();
    void SetOne(size_t index);
    void SetValue(const T &val);
    void Set(const T &x, const T &y);

  public:
    void Normalize();

    T Lenth();

    T SquaredLenth();

  public:
    const T &Get(size_t index) const
    {
        assert(index < size_t(2));
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize() const
    {
        return size_t(3);
    }

    inline const T *GetDataPtr() const
    {
        return &x;
    }
    inline friend std::ostream &operator<<(std::ostream &out, const TVector &val)
    {
        for (auto i = 0; i < 2; i++)
            out << *(const_cast<T *>(val.GetDataPtr()) + i) << ' ';
        return out;
    }

  public:
    T& operator[](size_t index);
};

template <typename T> class TVector<T, 3>
{
  public:
    T x, y, z;

  public:
    TVector();

    TVector(const T &val);

    TVector(const T &x, const T &y, const T &z);

    void SetZero();
    void SetZero(size_t index);
    void SetOne();
    void SetOne(size_t index);
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z);

  public:
    void Normalize();

    T Lenth();

    T SquaredLenth();

    TVector Cross(const TVector &val);

    static TVector Cross(const TVector &left, const TVector &right);

  public:
    const T &Get(size_t index) const
    {
        assert(index < size_t(3));
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize() const
    {
        return size_t(3);
    }

    inline const T *GetDataPtr() const
    {
        return &x;
    }
    inline friend std::ostream &operator<<(std::ostream &out, const TVector &val)
    {
        for (auto i = 0; i < 3; i++)
            out << *(const_cast<T *>(val.GetDataPtr()) + i) << ' ';
        return out;
    }

  public:
    T& operator[](size_t index);
};

template <typename T> class TVector<T, 4>

{
  public:
    T x, y, z, w;

  public:
    TVector();

    TVector(const T &val);

    TVector(const T &x, const T &y, const T &z, const T &w);

    void SetZero();
    void SetZero(size_t index);
    void SetOne();
    void SetOne(size_t index);
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z, const T &w);

  public:
    void Normalize();

    T Lenth();

    T SquaredLenth();

  public:
    inline const T &Get(size_t index) const
    {
        assert(index < size_t(4));
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize() const
    {
        return size_t(3);
    }

    inline const T *GetDataPtr() const
    {
        return &x;
    }
    inline friend std::ostream &operator<<(std::ostream &out, const TVector &val)
    {
        for (auto i = 0; i < 4; i++)
            out << *(const_cast<T *>(val.GetDataPtr()) + i) << ' ';
        return out;
    }

  public:
    T& operator[](size_t index);
};

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

// #include "Math/Inline/LinearData.Eigen.inl"
// #include "Math/Inline/VectorBase.inl"
#include "Math/Inline/TVector-Eigen.inl"