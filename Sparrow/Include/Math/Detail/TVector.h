#pragma once

#include "Math/Detail/LinearData.h"
#include "Math/Detail/LinearOperator.h"
#include "Math/Detail/VectorBase.h"

#include <utility>

namespace Math
{

template <typename T, size_t Size> class TVector
{
};

template <typename T>
class TVector<T, 2> : public LinearData<TVector<T, 2>, T, 2, 1, Math::LinearDataType::Vector>,
                      public LinearOperator<TVector<T, 2>, T, 2>,
                      public VectorBase<TVector<T, 2>, T, 2>
{
  public:
    TVector() : LinearData()
    {
    }
    TVector(const T &val) : LinearData(val)
    {
    }
    TVector(const T &x, const T &y) : LinearData(x, y)
    {
    }
};

template <typename T>
class TVector<T, 3> : public LinearData<TVector<T, 3>, T, 3, 1, Math::LinearDataType::Vector>,
                      public LinearOperator<TVector<T, 3>, T, 3>,
                      public VectorBase<TVector<T, 3>, T, 3>,
                      public VectorCross<TVector<T, 3>, T>
{
  public:
    TVector() : LinearData()
    {
    }
    TVector(const T &val) : LinearData(val)
    {
    }
    TVector(const T &x, const T &y, const T &z) : LinearData(x, y, z)
    {
    }

    template <typename OtherType, size_t OtherSize>
    TVector(const TVector<OtherType, OtherSize> &val)
        : LinearData(

              // std::forward<const LinearData<TVector<OtherType, OtherSize>, OtherType, OtherSize, 1,
              //                               Math::LinearDataType::Vector> &>(
              //     static_cast<const LinearData<TVector<OtherType, OtherSize>, OtherType, OtherSize, 1,
              //                                  Math::LinearDataType::Vector> &>(val)))
              static_cast<const LinearData<TVector<OtherType, OtherSize>, OtherType, OtherSize, 1,
                                           Math::LinearDataType::Vector> &>(val))
    {
    }
};

template <typename T>
class TVector<T, 4> : public LinearData<TVector<T, 4>, T, 4, 1, Math::LinearDataType::Vector>,
                      public LinearOperator<TVector<T, 4>, T, 4>,
                      public VectorBase<TVector<T, 4>, T, 4>,
                      public VectorCross<TVector<T, 4>, T>
{
  public:
    TVector() : LinearData()
    {
    }
    TVector(const T &val) : LinearData(val)
    {
    }
    TVector(const T &x, const T &y, const T &z, const T &w) : LinearData(x, y, z, w)
    {
    }
};

} // namespace Math

#include "Math/Inline/LinearData.Eigen.inl"
#include "Math/Inline/VectorBase.inl"
#include "Math/Inline/LinearOperator.Eigen.inl"