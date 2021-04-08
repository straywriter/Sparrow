#pragma once
#include "Math/Vector.h"

namespace Math
{

template <typename T, size_t size> class Ray
{
  public:
    TVector<T, size> origin, direction;

  public:
};

} // namespace Math