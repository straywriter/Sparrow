#pragma once
#include "Math/Detail/TVector.h"
#include <assert.h>
#include <iostream>
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
    void SetOne();
    void SetValue(const T &val);
    void Set(const T &x, const T &y);
    void SetRandom();

  public:
    void Normalize();

    T Lenth();

    T SquaredLenth();
    T Dot(const TVector &val);
    float Angle(const TVector &val);
    TVector Reflect(const TVector &);
    bool Equals(const TVector &val);
    TVector<T, 2> Max(const T &val);
    TVector<T, 2> Min(const T &val);

  public:
    static TVector Reflect(const TVector &a, const TVector &b);
    static float VectorAngle(const TVector &a, const TVector &b);
    static TVector VectorLerp(const TVector &a, const TVector &b);
    T static VectorDot(const TVector &a, const TVector &b);
    static TVector VectorMax(const TVector &a, const TVector &b);
    static TVector VectorMin(const TVector &a, const TVector &b);
    static TVector VectorRandom();
    static TVector VectorNormalize(const TVector &val);
    static TVector VectorInverse(const TVector &val);

    const static TVector VectorZero;
    const static TVector VectorOne;
    const static TVector VectorUp;
    const static TVector VectorDown;
    const static TVector VectorLeft;
    const static TVector VectorRight;

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

  public:
    T &operator[](size_t index);

    template <typename T, size_t Size> inline operator TVector<T, Size>() const
    {
        if constexpr (Size == size_t(2))
            return std::move(TVector<T, Size>((const T)x, (const T)y));
        else if constexpr (Size == size_t(3))
            return std::move(TVector<T, Size>((const T)x, (const T)y, static_cast<T>(0)));
        else if constexpr (Size == size_t(4))
            return std::move(TVector<T, Size>((const T)x, (const T)y, static_cast<T>(0), static_cast<T>(0)));
    }
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
    void SetOne();
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z);
    void SetRandom();

  public:
    void Normalize();
    T Lenth();
    T SquaredLenth();
    TVector Cross(const TVector &val);

    static TVector Cross(const TVector &left, const TVector &right);

    T Dot(const TVector &val);
    T Angle(const TVector &val);
    TVector Reflect(const TVector &);
    bool Equals(const TVector &val);

  public:
    static TVector Reflect(const TVector &a, const TVector &b);
    static float VectorAngle(const TVector &a, const TVector &b);
    static TVector VectorLerp(const TVector &a, const TVector &b);
    T static VectorDot(const TVector &a, const TVector &b);
    static TVector VectorMax(const TVector &a, const TVector &b);
    static TVector VectorMin(const TVector &a, const TVector &b);
    static TVector VectorRandom();
    static TVector VectorNormalize(const TVector &val);

    const static TVector VectorZero;
    const static TVector VectorOne;
    const static TVector VectorUp;
    const static TVector VectorDown;
    const static TVector VectorLeft;
    const static TVector VectorRight;
    const static TVector VectorForward;
    const static TVector VectorBack;

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

  public:
    T &operator[](size_t index);
    template <typename T, size_t Size> inline operator TVector<T, Size>() const
    {
        // static_assert(Size != size_t(3));
        if constexpr (Size == size_t(2))
            return std::move(TVector<T, Size>((T)x, (T)y));
        else if constexpr (Size == size_t(3))
            return std::move(TVector<T, Size>((T)x, (T)y, (T)z));
        else if constexpr (Size == size_t(4))
            return std::move(TVector<T, Size>((T)x, (T)y, (T)z, static_cast<T>(0)));
    }
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
    void SetOne();
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z, const T &w);
    void SetRandom();

  public:
    void Normalize();

    T Lenth();

    T SquaredLenth();
    T Dot(const TVector &val);
    T Angle(const TVector &val);
    TVector Reflect(const TVector &);
    bool Equals(const TVector &val);

  public:
    static TVector Reflect(const TVector &a, const TVector &b);
    static float VectorAngle(const TVector &a, const TVector &b);
    static TVector VectorLerp(const TVector &a, const TVector &b);
    T static VectorDot(const TVector &a, const TVector &b);
    static TVector VectorMax(const TVector &a, const TVector &b);
    static TVector VectorMin(const TVector &a, const TVector &b);
    static TVector VectorRandom();
    static TVector VectorNormalize(const TVector &val);

    const static TVector VectorZero;
    const static TVector VectorOne;

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

  public:
    T &operator[](size_t index);

    template <typename T, size_t Size> inline operator TVector<T, Size>() const
    {
        if constexpr (Size == size_t(2))
            return std::move(TVector<T, Size>((T)x, (T)y));
        else if constexpr (Size == size_t(3))
            return std::move(TVector<T, Size>((T)x, (T)y, (T)z));
        else if constexpr (Size == size_t(4))
            return std::move(TVector<T, Size>((T)x, (T)y, (T)z, T(w)));
    }
};

template <typename T, size_t Size> std::ostream &operator<<(std::ostream &out, const TVector<T, Size> &val)
{
    for (auto i = 0; i < Size; i++)
        out << *(const_cast<T *>(val.GetDataPtr()) + i) << ' ';
    return out;
}

template <typename T, size_t Size>
TVector<T, Size> operator+(const TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> TVector<T, Size> operator+(const TVector<T, Size> &left, const T &val);

template <typename T, size_t Size>
TVector<T, Size> operator-(const TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> TVector<T, Size> operator-(const TVector<T, Size> &left, const T &val);

template <typename T, size_t Size>
TVector<T, Size> operator*(const TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> TVector<T, Size> operator*(const TVector<T, Size> &left, const T &val);

template <typename T, size_t Size>
TVector<T, Size> operator/(const TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> TVector<T, Size> operator/(const TVector<T, Size> &left, const T &val);

template <typename T, size_t Size> TVector<T, Size> operator-(const TVector<T, Size> &left);

template <typename T, size_t Size> void operator+=(TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> void operator+=(TVector<T, Size> &left, const T &val);

template <typename T, size_t Size> void operator-=(TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> void operator-=(TVector<T, Size> &left, const T &val);

template <typename T, size_t Size> void operator*=(TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> void operator*=(TVector<T, Size> &left, const T &val);

template <typename T, size_t Size> void operator/=(TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> void operator/=(TVector<T, Size> &left, const T &val);

template <typename T, size_t Size> bool operator==(const TVector<T, Size> &left, const TVector<T, Size> &right);

template <typename T, size_t Size> bool operator!=(const TVector<T, Size> &left, const TVector<T, Size> &right);
} // namespace Math

#include "Math/Inline/TVector-Eigen.inl"