#pragma once
#include <Eigen/Dense>
// #include "Eigen/Dense"
// using namespace Eigen;
using namespace std;

namespace Math
{

enum class LinearDataType
{
    Vector,
    Color
};

template <typename T, size_t Row, size_t Col, LinearDataType> class LinearData
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

template <typename T> class LinearData<T, 3, 1, LinearDataType::Vector>
{
  public:
    T x, y, z;

  public:
    LinearData();

    LinearData(const T &val);

    LinearData(const T &x, const T &y, const T &z);

    void SetZero();
    void SetOne();
    void SetValue(const T &val);

    inline size_t GetSize()
    {
        return 3;
    }

    inline T *GetDataPtr()
    {
        return &x;
    }
};

template <typename T> class LinearData<T, 2, 1, LinearDataType::Vector>
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

    inline size_t GetSize()
    {
        return 2;
    }
    inline T *GetDataPtr()
    {
        return &x;
    }
};

template <typename T> class LinearData<T, 4, 1, LinearDataType::Vector>
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

    inline size_t GetSize()
    {
        return 4;
    }
    inline T *GetDataPtr()
    {
        return &x;
    }
};
} // namespace Math