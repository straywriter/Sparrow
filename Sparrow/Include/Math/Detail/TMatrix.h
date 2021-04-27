#pragma once
#include "Math/Detail/TVector.h"

namespace Math
{

template <typename T, size_t Row, size_t Col> class TMatrix
{
  public:
    T matrix[Row][Col];

  public:
    TMatrix();

    TMatrix(const T &val);

    void SetZero();
    void SetZero(size_t index);
    void SetZero(size_t row, size_t col);
    void SetOne();
    void SetOne(size_t index);
    void SetOne(size_t row, size_t col);
    void SetValue(const T &val);

    // template<typename VectorType,size_t Size>
    // void SetVector(TVector<T,)

  public:



  public:
  
 

  public:
    const T &Get(size_t index) const
    {
        assert(index < Row * Col);
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize() const
    {
        return size_t(3);
    }

    inline const T *GetDataPtr() const
    {
        return &matrix;
    }
    inline friend std::ostream &operator<<(std::ostream &out, const TVector &val)
    {
        // for (auto i = 0; i < 2; i++)
        //     out << *(const_cast<T *>(val.GetDataPtr()) + i) << ' ';
        // return out;
    }

  public:
    inline T &operator[](size_t index);


}

} // namespace  Math
