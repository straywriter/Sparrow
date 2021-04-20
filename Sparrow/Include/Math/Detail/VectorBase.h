#pragma once
#include <stdint.h>

template <typename Derived, typename T, size_t Size> class VectorBase
{
  private:
    inline const T *data() const
    {
        return static_cast<const Derived *>(this);
    }

  public:
  

    void Normalize();

    T Lenth();
    T SquaredLenth();

  public:
    static Derived Lerp(const Derived &a, const Derived &b, float t);

};

template <typename Derived, typename T> class VectorCross
{
  private:
    const Derived *data() const
    {
        return static_cast<const Derived *>(this);
    }

  public:
    Derived Cross(const Derived &val)
    {
        Derived temp;
        temp.x = data()->y * val.z - val.y * data()->z;
        temp.y = data()->z * val.x - data()->x * val.z;
        temp.z = data()->x * val.y - data()->y * val.x;
        return std::move(temp);
    }

    static Derived Cross(const Derived &left, const Derived &right)
    {
        Derived temp;
        temp.x = left.y * right.z - right.y * left.z;
        temp.y = left.z * right.x - left.x * right.z;
        temp.z = left.x * right.y - left.y * right.x;
        return std::move(temp);
    }
};
