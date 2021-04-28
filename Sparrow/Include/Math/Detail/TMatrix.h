#pragma once
#include "Math/Detail/TVector.h"
#include <iostream>

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
    void SetOne();
    void SetValue(const T &val);
    void SetRandom();

  public:
    //伴随矩阵

    //矩阵的逆
  public:
    static TMatrix MatrixRandom();

  public:
  public:
    const T &Get(size_t index) const
    {
        assert(index < Row * Col);
        return *(GetDataPtr() + index);
    }

    inline size_t GetRowSize() const
    {
        return Row;
    }
    inline size_t GetColSize() const
    {
        return Col;
    }

    inline size_t GetSize() const
    {
        return Row * Col;
    }

    inline const T *GetDataPtr() const
    {
        return &matrix;
    }

  public:
    inline T &operator[](size_t index);

    template <typename OtherType, size_t OtherRow, size_t OtherCol>
    inline operator TMatrix<OtherType, OtherRow, OtherCol>()
    {
        // if constexpr (OtherRow <= Row && OtherCol <= Col)
        // {
        //     TMatrix<OtherType, OtherRow, OtherCol> temp;
        //     for (auto i = Row; i--;)
        //         for (auto j = Col; j--)
        //             temp.matrix[i][j] = (OtherType)this->matrix[i][j];
        //     return temp;
        // }
        // else if constexpr(OtherRow>Row||OtherCol>Col)
        // {

        // }
    }
};
template <typename T, size_t Row, size_t Col>
std::ostream &operator<<(std::ostream &out, const TMatrix<T, Row, Col> &val);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator+(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator+(const TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator-(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator-(const TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator*(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator*(const TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator/(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col>
TMatrix<T, Row, Col> operator/(const TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col> TMatrix<T, Row, Col> operator-(const TMatrix<T, Row, Col> &left);

template <typename T, size_t Row, size_t Col>
void operator+=(TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col> void operator+=(TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
void operator-=(TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col> void operator-=(TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
void operator*=(TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col> void operator*=(TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
void operator/=(TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col> void operator/=(TMatrix<T, Row, Col> &left, const T &val);

template <typename T, size_t Row, size_t Col>
bool operator==(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

template <typename T, size_t Row, size_t Col>
bool operator!=(const TMatrix<T, Row, Col> &left, const TMatrix<T, Row, Col> &right);

} // namespace  Math
