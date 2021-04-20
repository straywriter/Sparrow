#include "Math/Detail/VectorBase.h"

//-----VectorBase------

template <typename Derived, typename T, size_t Size> inline void VectorBase<Derived, T, Size>::Normalize()
{
}

template <typename Derived, typename T, size_t Size> inline T VectorBase<Derived, T, Size>::Lenth()
{
    return T();
}
template <typename Derived, typename T, size_t Size> inline T VectorBase<Derived, T, Size>::SquaredLenth()
{
    return T();
}

//-----VectorCross-----

template <typename Derived, typename T> inline Derived VectorCross<Derived, T>::Cross(const Derived &val)
{
    Derived temp;
    temp.x = data()->y * val.z - val.y * data()->z;
    temp.y = data()->z * val.x - data()->x * val.z;
    temp.z = data()->x * val.y - data()->y * val.x;
    return std::move(temp);
}
template <typename Derived, typename T>
inline Derived VectorCross<Derived, T>::Cross(const Derived &left, const Derived &right)
{
    Derived temp;
    temp.x = left.y * right.z - right.y * left.z;
    temp.y = left.z * right.x - left.x * right.z;
    temp.z = left.x * right.y - left.y * right.x;
    return std::move(temp);
}