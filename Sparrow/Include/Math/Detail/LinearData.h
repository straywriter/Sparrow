#pragma once
#include <algorithm>
using namespace std;

namespace Math
{

enum class LinearDataType
{
    None,
    Vector,
    Color,
    Matrix,
    Point
};
template <typename Derived, typename T, size_t Row, size_t Col, LinearDataType> class LinearData
{
  public:
    inline T *GetDataPtr()
    {
        return nullptr;
    }
};

template <typename Derived, typename T, size_t Row, size_t Col>
class LinearData<Derived, T, Row, Col, LinearDataType::None>
{
  private:
    T data[Row * Col];

  public:
    LinearData();
    LinearData(const T &val);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);

    inline size_t GetSize()
    {
        return Col * Row;
    }

    inline T *GetDataPtr()
    {
        return data;
    }
};

template <typename Derived, typename T, size_t Row, size_t Col>
class LinearData<Derived, T, Row, Col, LinearDataType::Matrix>
{
  private:
    T matrix[Row][Col];

  public:
    LinearData();
    LinearData(const T &val);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);

    inline const T &Get(size_t index) const
    {
        return *(GetDataPtr() + index);
    }

    inline size_t GetRowSize()
    {
        return Row;
    }

    inline size_t GetColSize()
    {
        return Col;
    }

    inline size_t GetSize()
    {
        return Col * Row;
    }

    inline const T *GetDataPtr()const
    {
        return data;
    }

  private:
    inline T *data()
    {
        return matrix;
    }
};

template <typename Derived, typename T> class LinearData<Derived, T, 3, 1, LinearDataType::Vector>
{
  public:
    T x, y, z;

  public:
    LinearData();

    LinearData(const T &val);

    LinearData(const T &x, const T &y, const T &z);

    template <typename OtherDerived, typename OtherType, size_t OtherSize>
    LinearData(const LinearData<OtherDerived, OtherType, OtherSize, 1, LinearDataType::Vector> &val);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z);

    const T &Get(size_t index) const
    {
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize()const
    {
        return size_t(3);
    }

    inline const  T *GetDataPtr() const
    {
        return &x;
    }
};

template <typename Derived, typename T> class LinearData<Derived, T, 2, 1, LinearDataType::Vector>
{
  public:
    T x, y;

  public:
    LinearData();
    LinearData(const T &val);
    LinearData(const T &x, const T &y);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);

    void Set(const T &x, const T &y);

    const T &Get(size_t index) const
    {
        return *(GetDataPtr() + index);
    }

    inline size_t GetSize()
    {
        return 2;
    }
    inline const T *GetDataPtr() const
    {
        return &x;
    }
};

template <typename Derived, typename T> class LinearData<Derived, T, 4, 1, LinearDataType::Vector>
{
  public:
    T x, y, z, w;

  public:
    LinearData();
    LinearData(const T &val);
    LinearData(const T &x, const T &y, const T &z, const T &w);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);
    void Set(const T &x, const T &y, const T &z, const T &w);

    inline size_t GetSize()
    {
        return 4;
    }
    inline const T *GetDataPtr()const
    {
        return &x;
    }
};
} // namespace Math