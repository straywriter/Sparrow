#pragma once

#include "Math/Detail/LinearData.h"
#include "Math/Detail/LinearOperator.h"
#include "Math/Detail/VectorBase.h"
namespace Math
{

template <typename T>
class TVector3 : public LinearData<T, 3, 1, Math::LinearDataType::Vector>,
                 public LinearOperator<TVector3<T>, T, 3>,
                 public VectorBase<TVector3<T>, T, 3>,
                 public VectorCross<TVector3<T>, T>
{
  public:
    TVector3() : LinearData()
    {
    }
    TVector3(const T &val) : LinearData(val)
    {
    }
    TVector3(const T &x, const T &y, const T &z) : LinearData(x, y, z)
    {
    }
};

} // namespace Math

#include "Math/Inline/LinearData-Eigen.inl"