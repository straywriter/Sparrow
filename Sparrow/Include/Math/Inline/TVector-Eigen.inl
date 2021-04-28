

#include "Math/Detail/TVector.h"
#include <Eigen/Dense>

using namespace Math;

//----------Vector2----------
template <typename T> inline Math::TVector<T, 2>::TVector()
{
    static_assert(sizeof(Math::TVector<T, 2>) == sizeof(Eigen::Matrix<T, 2, 1>));
    ((Eigen::Matrix<T, 2, 1> *)(this))->setZero();
}

template <typename T> inline Math::TVector<T, 2>::TVector(const T &val)
{
    static_assert(sizeof(Math::TVector<T, 2>) == sizeof(Eigen::Matrix<T, 2, 1>));
    ((Eigen::Matrix<T, 2, 1> *)(this))->setConstant(static_cast<T>(val));
}

template <typename T> inline Math::TVector<T, 2>::TVector(const T &_x, const T &_y) : x(_x), y(_y)
{
    static_assert(sizeof(Math::TVector<T, 2>) == sizeof(Eigen::Matrix<T, 2, 1>));
}
template <typename T> inline void Math::TVector<T, 2>::SetZero()
{
    ((Eigen::Matrix<T, 2, 1> *)(this))->setZero();
}

template <typename T> inline void Math::TVector<T, 2>::SetOne()
{
    ((Eigen::Matrix<T, 2, 1> *)(this))->setConstant(static_cast<T>(1));
}

template <typename T> inline void Math::TVector<T, 2>::SetValue(const T &val)
{
    ((Eigen::Matrix<T, 2, 1> *)(this))->setConstant(static_cast<T>(val));
}

template <typename T> inline void Math::TVector<T, 2>::Set(const T &_x, const T &_y)
{
    x = _x;
    y = _y;
}

template <typename T> inline void Math::TVector<T, 2>::Normalize()
{
     ((Eigen::Matrix<T, 2, 1> *)(this))->normalize();
}

template <typename T> inline T Math::TVector<T, 2>::Lenth()
{
      return (T)(((Eigen::Matrix<T, 2, 1> *)(this))->norm());
}

template <typename T> inline T Math::TVector<T, 2>::SquaredLenth()
{
       return (T)(((Eigen::Matrix<T, 2, 1> *)(this))->squaredNorm());
}

template <typename T> inline T& Math::TVector<T, 2>::operator[](size_t index)
{
    return const_cast<T&>(Get(index));
}

//----------Vector3----------

template <typename T> inline Math::TVector<T, 3>::TVector()
{
    static_assert(sizeof(Math::TVector<T, 3>) == sizeof(Eigen::Matrix<T, 3, 1>));
    ((Eigen::Matrix<T, 3, 1> *)(this))->setZero();
}

template <typename T> inline Math::TVector<T, 3>::TVector(const T &val)
{
    static_assert(sizeof(Math::TVector<T, 3>) == sizeof(Eigen::Matrix<T, 3, 1>));
    ((Eigen::Matrix<T, 3, 1> *)(this))->setConstant(static_cast<T>(val));
}

template <typename T> inline Math::TVector<T, 3>::TVector(const T &_x, const T &_y, const T &_z) : x(_x), y(_y), z(_z)
{
}
template <typename T> inline void Math::TVector<T, 3>::SetZero()
{
    ((Eigen::Matrix<T, 3, 1> *)(this))->setZero();
}

template <typename T> inline void Math::TVector<T, 3>::SetOne()
{
    ((Eigen::Matrix<T, 3, 1> *)(this))->setConstant(static_cast<T>(1));
}
template <typename T> inline void Math::TVector<T, 3>::SetValue(const T &val)
{
    ((Eigen::Matrix<T, 3, 1> *)(this))->setConstant(static_cast<T>(val));
}
template <typename T> inline void Math::TVector<T, 3>::Set(const T &_x, const T &_y, const T &_z)
{
    x = _x;
    y = _y;
    z = _z;
}

template <typename T> inline void Math::TVector<T, 3>::Normalize()
{
    ((Eigen::Matrix<T, 3, 1> *)(this))->normalize();
}

template <typename T> inline T Math::TVector<T, 3>::Lenth()
{
    return (T)(((Eigen::Matrix<T, 3, 1> *)(this))->norm());
}

template <typename T> inline T Math::TVector<T, 3>::SquaredLenth()
{
    return (T)(((Eigen::Matrix<T, 3, 1> *)(this))->squaredNorm());
}

template <typename T> inline TVector<T, 3> Math::TVector<T, 3>::Cross(const TVector &val)
{
    TVector<T, 3> temp;
    *((Eigen::Matrix<T, 3, 1> *)(&temp)) =
        ((Eigen::Matrix<T, 3, 1> *)(this))->cross(*((Eigen::Matrix<T, 3, 1> *)const_cast<TVector<T, 3> *>(&val)));
    return std::move(temp);
}

template <typename T> inline TVector<T, 3> Math::TVector<T, 3>::Cross(const TVector &left, const TVector &right)
{
    TVector<T, 3> temp;
    *((Eigen::Matrix<T, 3, 1> *)(&temp)) =
        ((Eigen::Matrix<T, 3, 1> *)(const_cast<TVector<T, 3> *>(&left)))
            ->cross(*((Eigen::Matrix<T, 3, 1> *)const_cast<TVector<T, 3> *>(&right)));
    return std::move(temp);
}

template <typename T> inline T& Math::TVector<T, 3>::operator[](size_t index)
{
      return const_cast<T&>(Get(index));
}

//----------Vector4----------

template <typename T> inline Math::TVector<T, 4>::TVector()
{
    static_assert(sizeof(Math::TVector<T, 4>) == sizeof(Eigen::Matrix<T, 4, 1>));
    ((Eigen::Matrix<T, 4, 1> *)(this))->setZero();
}

template <typename T> inline Math::TVector<T, 4>::TVector(const T &val)
{
    static_assert(sizeof(Math::TVector<T, 4>) == sizeof(Eigen::Matrix<T, 4, 1>));
    ((Eigen::Matrix<T, 4, 1> *)(this))->setConstant(static_cast<T>(val));
}

template <typename T>
inline Math::TVector<T, 4>::TVector(const T &_x, const T &_y, const T &_z, const T &_w) : x(_x), y(_y), z(_z), w(_w)
{
}
template <typename T> inline void Math::TVector<T, 4>::SetZero()
{
    ((Eigen::Matrix<T, 4, 1> *)(this))->setZero();
}

template <typename T> inline void Math::TVector<T, 4>::SetOne()
{
    ((Eigen::Matrix<T, 4, 1> *)(this))->setConstant(static_cast<T>(1));
}

template <typename T> inline void Math::TVector<T, 4>::SetValue(const T &val)
{
    ((Eigen::Matrix<T, 4, 1> *)(this))->setConstant(static_cast<T>(val));
}
template <typename T> inline void Math::TVector<T, 4>::Set(const T &_x, const T &_y, const T &_z, const T &_w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

template <typename T> inline void Math::TVector<T, 4>::Normalize()
{
     ((Eigen::Matrix<T, 4, 1> *)(this))->normalize();
}

template <typename T> inline T Math::TVector<T, 4>::Lenth()
{
       return (T)(((Eigen::Matrix<T, 4 ,1> *)(this))->norm());
}

template <typename T> inline T Math::TVector<T, 4>::SquaredLenth()
{
       return (T)(((Eigen::Matrix<T, 4, 1> *)(this))->squaredNorm());
}

template <typename T> inline T &Math::TVector<T, 4>::operator[](size_t index)
{
      return const_cast<T&>(Get(index));
}

//----------operator----------
namespace Math
{

template <typename T, size_t Size>
TVector<T, Size> Math::operator+(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> TVector<T, Size> Math::operator+(const TVector<T, Size> &left, const T &val)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size>
TVector<T, Size> Math::operator-(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> TVector<T, Size> Math::operator-(const TVector<T, Size> &left, const T &val)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size>
TVector<T, Size> Math::operator*(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> TVector<T, Size> Math::operator*(const TVector<T, Size> &left, const T &val)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size>
TVector<T, Size> Math::operator/(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> TVector<T, Size> Math::operator/(const TVector<T, Size> &left, const T &val)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> TVector<T, Size> Math::operator-(const TVector<T, Size> &left)
{
    return TVector<T, Size>();
}
template <typename T, size_t Size> void Math::operator+=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
}
template <typename T, size_t Size> void Math::operator+=(TVector<T, Size> &left, const T &val)
{
}
template <typename T, size_t Size> void Math::operator-=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
}
template <typename T, size_t Size> void Math::operator-=(TVector<T, Size> &left, const T &val)
{
}
template <typename T, size_t Size> void Math::operator*=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
}
template <typename T, size_t Size> void Math::operator*=(TVector<T, Size> &left, const T &val)
{
}
template <typename T, size_t Size> void Math::operator/=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
}
template <typename T, size_t Size> void Math::operator/=(TVector<T, Size> &left, const T &val)
{
}
template <typename T, size_t Size> bool Math::operator==(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    // return(*((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) )==
    //     (*((Eigen::Matrix<T, Size, 1> *)const_cast<TVector<T, Size> *>(&right)));
    for (auto i = Size; i--;)
    {
        if (!(*(&right.x + i) == *(&left.x + i)))
            return false;
    }
    return true;
}
template <typename T, size_t Size> bool Math::operator!=(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    for (auto i = Size; i--;)
    {
        if (!(*(&right.x + i) == *(&left.x + i)))
            return true;
    }
    return false;
}
} // namespace Math