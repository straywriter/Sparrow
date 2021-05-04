#include "Math/Detail/TVector.h"
#include <Eigen/Dense>
#include <string.h>

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
template <typename T> inline void Math::TVector<T, 2>::SetRandom()
{
    ((Eigen::Matrix<T, 2, 1> *)(this))->setRandom();
}
template <typename T> inline TVector<T, 2> Math::TVector<T, 2>::VectorRandom()
{
    TVector<T, 2> temp;
    temp.SetRandom();
    return std::move(temp);
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

template <typename T> inline T &Math::TVector<T, 2>::operator[](size_t index)
{
    return const_cast<T &>(Get(index));
}

template <typename T> inline T Math::TVector<T, 2>::MaxComponent() const
{
    return ((Eigen::Matrix<T, 2, 1> *)(this))->maxCoeff();
}
template <typename T> inline T Math::TVector<T, 2>::MinComponent() const
{
    return ((Eigen::Matrix<T, 2, 1> *)(this))->minCoeff();
}
template <typename T> inline T Math::TVector<T, 2>::VectorMinComponent(const TVector &val)
{
    return val.MinComponent();
}
template <typename T> inline T Math::TVector<T, 2>::VectorMaxComponent(const TVector &val)
{
    return val.MaxComponent();
}
template <typename T> inline T Math::TVector<T, 2>::Dot(const TVector &val) const
{
    T temp;
    *((&temp)) =
        ((Eigen::Matrix<T, 2, 1> *)(this))->dot(*((Eigen::Matrix<T, 2, 1> *)const_cast<TVector<T, 2> *>(&val)));
    return std::move(temp);
}
template <typename T> inline T Math::TVector<T, 2>::VectorDot(const TVector &a, const TVector &b)
{
    return a.Dot(b);
}

template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorZero = Math::TVector<T, 2>(static_cast<T>(0), static_cast<T>(0));
template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorOne = Math::TVector<T, 2>(static_cast<T>(1), static_cast<T>(1));
template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorUp = Math::TVector<T, 2>(static_cast<T>(0), static_cast<T>(1));
template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorDown = Math::TVector<T, 2>(static_cast<T>(0), static_cast<T>(-1));
template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorLeft = Math::TVector<T, 2>(static_cast<T>(-1), static_cast<T>(0));
template <typename T>
const Math::TVector<T, 2> Math::TVector<T, 2>::VectorRight = Math::TVector<T, 2>(static_cast<T>(1), static_cast<T>(0));

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
template <typename T> inline void Math::TVector<T, 3>::SetRandom()
{
    ((Eigen::Matrix<T, 3, 1> *)(this))->setRandom();
}
template <typename T> inline TVector<T, 3> Math::TVector<T, 3>::VectorRandom()
{
    TVector<T, 3> temp;
    temp.SetRandom();
    return std::move(temp);
}

template <typename T> inline void Math::TVector<T, 3>::Set(const T &_x, const T &_y, const T &_z)
{
    x = _x;
    y = _y;
    z = _z;
}
template <typename T> inline T Math::TVector<T, 3>::Dot(const TVector &val) const
{
    T temp;
    *((&temp)) =
        ((Eigen::Matrix<T, 3, 1> *)(this))->dot(*((Eigen::Matrix<T, 3, 1> *)const_cast<TVector<T, 3> *>(&val)));
    return std::move(temp);
}
template <typename T> inline T Math::TVector<T, 3>::VectorDot(const TVector &a, const TVector &b)
{
    return a.Dot(b);
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

template <typename T> inline T Math::TVector<T, 3>::MaxComponent() const
{
    return (T)((Eigen::Matrix<T, 3, 1> *)(this))->maxCoeff();
}
template <typename T> inline T Math::TVector<T, 3>::MinComponent() const
{
    return (T)((Eigen::Matrix<T, 3, 1> *)(this))->minCoeff();
}
template <typename T> inline T Math::TVector<T, 3>::VectorMinComponent(const TVector &val)
{
    return val.MinComponent();
}
template <typename T> inline T Math::TVector<T, 3>::VectorMaxComponent(const TVector &val)
{
    return val.MaxComponent();
}

template <typename T> inline T &Math::TVector<T, 3>::operator[](size_t index)
{
    return const_cast<T &>(Get(index));
}
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorZero = Math::TVector<T, 3>(static_cast<T>(0), static_cast<T>(0),
                                                                                static_cast<T>(0));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorOne = Math::TVector<T, 3>(static_cast<T>(1), static_cast<T>(1),
                                                                               static_cast<T>(1));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorUp = Math::TVector<T, 3>(static_cast<T>(0), static_cast<T>(1),
                                                                              static_cast<T>(0));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorDown = Math::TVector<T, 3>(static_cast<T>(0), static_cast<T>(-1),
                                                                                static_cast<T>(0));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorLeft = Math::TVector<T, 3>(static_cast<T>(-1), static_cast<T>(0),
                                                                                static_cast<T>(0));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorRight = Math::TVector<T, 3>(static_cast<T>(1), static_cast<T>(0),
                                                                                 static_cast<T>(0));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorForward = Math::TVector<T, 3>(static_cast<T>(0), static_cast<T>(0),
                                                                                   static_cast<T>(1));
template <typename T>
const Math::TVector<T, 3> Math::TVector<T, 3>::VectorBack = Math::TVector<T, 3>(static_cast<T>(0), static_cast<T>(0),
                                                                                static_cast<T>(-1));

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
template <typename T> inline void Math::TVector<T, 4>::SetRandom()
{
    ((Eigen::Matrix<T, 4, 1> *)(this))->setRandom();
}

template <typename T> inline TVector<T, 4> Math::TVector<T, 4>::VectorRandom()
{
    TVector<T, 4> temp;
    temp.SetRandom();
    return std::move(temp);
}
template <typename T> inline T Math::TVector<T, 4>::Dot(const TVector &val) const
{
    T temp;
    *((&temp)) =
        ((Eigen::Matrix<T, 4, 1> *)(this))->dot(*((Eigen::Matrix<T, 4, 1> *)const_cast<TVector<T, 4> *>(&val)));
    return std::move(temp);
}
template <typename T> inline T Math::TVector<T, 4>::VectorDot(const TVector &a, const TVector &b)
{
    return a.Dot(b);
}

template <typename T> inline T Math::TVector<T, 4>::MaxComponent() const
{
    return ((Eigen::Matrix<T, 4, 1> *)(this))->maxCoeff();
}
template <typename T> inline T Math::TVector<T, 4>::MinComponent() const
{
    return ((Eigen::Matrix<T, 4, 1> *)(this))->minCoeff();
}
template <typename T> inline T Math::TVector<T, 4>::VectorMinComponent(const TVector &val)
{
    return val.MinComponent();
}
template <typename T> inline T Math::TVector<T, 4>::VectorMaxComponent(const TVector &val)
{
    return val.MaxComponent();
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
    return (T)(((Eigen::Matrix<T, 4, 1> *)(this))->norm());
}

template <typename T> inline T Math::TVector<T, 4>::SquaredLenth()
{
    return (T)(((Eigen::Matrix<T, 4, 1> *)(this))->squaredNorm());
}

template <typename T> inline T &Math::TVector<T, 4>::operator[](size_t index)
{
    return const_cast<T &>(Get(index));
}

template <typename T>
const Math::TVector<T, 4> Math::TVector<T, 4>::VectorZero = Math::TVector<T, 4>(static_cast<T>(0), static_cast<T>(0),
                                                                                static_cast<T>(0), static_cast<T>(0));
template <typename T>
const Math::TVector<T, 4> Math::TVector<T, 4>::VectorOne = Math::TVector<T, 4>(static_cast<T>(1), static_cast<T>(1),
                                                                               static_cast<T>(1), static_cast<T>(1));

//----------operator----------
namespace Math
{
template <typename T, size_t Size>
TVector<T, Size> operator+(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    TVector<T, Size> temp;
    *((Eigen::Matrix<T, Size, 1> *)(&temp)) = *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) +
                                              *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
    return std::move(temp);
}
template <typename T, size_t Size> TVector<T, Size> operator+(const TVector<T, Size> &left, const T &val)
{
    TVector<T, Size> temp;
    for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) + val)
        ;
    return std::move(temp);
}
template <typename T, size_t Size>
TVector<T, Size> operator-(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    TVector<T, Size> temp;
    *((Eigen::Matrix<T, Size, 1> *)(&temp)) = *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) -
                                              *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
    return std::move(temp);
}
template <typename T, size_t Size> TVector<T, Size> operator-(const TVector<T, Size> &left, const T &val)
{
    TVector<T, Size> temp;
    for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) - val)
        ;
    return std::move(temp);
}
template <typename T, size_t Size>
TVector<T, Size> operator*(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    TVector<T, Size> temp;
     for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) * *(right.GetDataPtr() + (int)i))
        ;
    // *((Eigen::Matrix<T, Size, 1> *)(&temp)) = *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) *
    //                                           *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
    return std::move(temp);
}
template <typename T, size_t Size> TVector<T, Size> operator*(const TVector<T, Size> &left, const T &val)
{
    TVector<T, Size> temp;
    for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) * val)
        ;
    return std::move(temp);
}
template <typename T, size_t Size>
TVector<T, Size> operator/(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    TVector<T, Size> temp;
    for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) / *(right.GetDataPtr() + (int)i))
        ;
    return std::move(temp);
    // *((Eigen::Matrix<T, Size, 1> *)(&temp)) = *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left)))
    // /
    //                                           *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size>
    //                                           *>(&right)));
}
template <typename T, size_t Size> TVector<T, Size> operator/(const TVector<T, Size> &left, const T &val)
{
    TVector<T, Size> temp;
    for (auto i = Size; i--; temp[i] = *(left.GetDataPtr() + (int)i) / val)
        ;
    return std::move(temp);
}
template <typename T, size_t Size> TVector<T, Size> operator-(const TVector<T, Size> &left)
{
    TVector<T, Size> temp;
    ((Eigen::Matrix<T, Size, 1> *)(&temp))->inverse();
    return std::move(temp);
}
template <typename T, size_t Size> void operator+=(TVector<T, Size> &left, const TVector<T, Size> &right)
{

    *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) +=
        *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
}
template <typename T, size_t Size> void operator+=(TVector<T, Size> &left, const T &val)
{
    left = left + val;
}
template <typename T, size_t Size> void operator-=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
    *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) -=
        *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
}
template <typename T, size_t Size> void operator-=(TVector<T, Size> &left, const T &val)
{
    left = left - val;
}
template <typename T, size_t Size> void operator*=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
    *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&left))) *=
        *((Eigen::Matrix<T, Size, 1> *)(const_cast<TVector<T, Size> *>(&right)));
}
template <typename T, size_t Size> void operator*=(TVector<T, Size> &left, const T &val)
{
    left = left * val;
}
template <typename T, size_t Size> void operator/=(TVector<T, Size> &left, const TVector<T, Size> &right)
{
    left = left / right;
}
template <typename T, size_t Size> void operator/=(TVector<T, Size> &left, const T &val)
{
    left = left / val;
}
template <typename T, size_t Size> bool operator==(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return !memcmp(left.GetDataPtr(), right.GetDataPtr(), sizeof(TVector<T, Size>));
}
template <typename T, size_t Size> bool operator!=(const TVector<T, Size> &left, const TVector<T, Size> &right)
{
    return !(left == right);
}
} // namespace Math