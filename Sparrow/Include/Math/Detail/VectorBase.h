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

    // Equal
        //     static TVector Lerp(TVector a, TVector b, float t);
    //     static TVector Project(TVector vector, TVector onNormal);
    //     static TVector ProjectOnPlane(TVector vector, TVector planeNormal);
    //     TVector<T, 4> Abs();
    //     T Max();
    //     T Min()
  public:
    static Derived Lerp(const Derived &a, const Derived &b, float t);

};

template <typename Derived, typename T> class VectorCross
{
  private:
  inline  const Derived *data() const
    {
        return static_cast<const Derived *>(this);
    }

  public:
    Derived Cross(const Derived &val);
   

    static Derived Cross(const Derived &left, const Derived &right);
    
};
