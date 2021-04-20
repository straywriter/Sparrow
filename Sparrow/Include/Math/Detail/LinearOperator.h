#pragma once

#include "Math/Detail/LinearData.h"

template <typename Derived, typename T, size_t Size> class LinearOperator
{
  private:
  private:
  const Derived *data() const
    {
        return static_cast<const Derived *>(this);
    }

  public:

  T operator[](size_t index);

  Derived operator+(const Derived &val) const;
  Derived operator+(T val) const;
  Derived operator-(const Derived &val) const;
  Derived operator-(T val) const;
  Derived operator*(const Derived &val) const;
  Derived operator*(T scale) const;
  Derived operator/(const Derived &val) const;
  Derived operator/(T scale);

  Derived operator-() const;

  Derived operator+=(const Derived &val);
  Derived operator+=(T val);
  Derived operator-=(const Derived &val);
  Derived operator-=(T val);
  Derived operator*=(const Derived &val);
  Derived operator*=(T scale);
  Derived operator/=(const Derived &val);
  Derived operator/=(T scale);

  bool operator==(const Derived &val) const;
  bool operator!=(const Derived &val) const;

};
