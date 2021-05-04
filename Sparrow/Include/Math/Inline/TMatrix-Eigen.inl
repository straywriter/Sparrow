#include <Eigen/Dense>

template<typename T, size_t Row, size_t Col>
inline Math::TMatrix<T, Row, Col>::TMatrix()
{
    //  static_assert(sizeof(Math::TVector<T, 3>) == sizeof(Eigen::Matrix<T, 3, 1>));
    ((Eigen::Matrix<T, Row, Col> *)(this))->setZero();
}

template<typename T, size_t Row, size_t Col>
inline Math::TMatrix<T, Row, Col>::TMatrix(const T & val)
{
}

template<typename T, size_t Row, size_t Col>
inline void Math::TMatrix<T, Row, Col>::SetZero()
{
}

template<typename T, size_t Row, size_t Col>
inline void Math::TMatrix<T, Row, Col>::SetOne()
{
}
template<typename T, size_t Row, size_t Col>
inline void Math::TMatrix<T, Row, Col>::SetValue(const T & val)
{
}

template<typename T, size_t Row, size_t Col>
inline void Math::TMatrix<T, Row, Col>::SetRandom()
{
}

template<typename T, size_t Row, size_t Col>
inline TMatrix<T,Row,Col> Math::TMatrix<T, Row, Col>::MatrixRandom()
{
return TMatrix();
}

template<typename T, size_t Row, size_t Col>
inline T & Math::TMatrix<T, Row, Col>::operator[](size_t index)
{
// TODO: 在此处插入 return 语句
}

template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator+(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return TMatrix<T,Row,Col>();
}

template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator+(const TMatrix<T,Row,Col>& left, const T & val)
{
return TMatrix<T,Row,Col>();
}

template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator-(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return TMatrix<T,Row,Col>();
}

template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator-(const TMatrix<T,Row,Col>& left, const T & val)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator*(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator*(const TMatrix<T,Row,Col>& left, const T & val)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator/(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator/(const TMatrix<T,Row,Col>& left, const T & val)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
TMatrix<T,Row,Col> Math::operator-(const TMatrix<T,Row,Col>& left)
{
return TMatrix<T,Row,Col>();
}
template<typename T, size_t Row, size_t Col>
void Math::operator+=(TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator+=(TMatrix<T,Row,Col>& left, const T & val)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator-=(TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator-=(TMatrix<T,Row,Col>& left, const T & val)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator*=(TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator*=(TMatrix<T,Row,Col>& left, const T & val)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator/=(TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
}
template<typename T, size_t Row, size_t Col>
void Math::operator/=(TMatrix<T,Row,Col>& left, const T & val)
{
}
template<typename T, size_t Row, size_t Col>
bool Math::operator==(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return false;
}
template<typename T, size_t Row, size_t Col>
bool Math::operator!=(const TMatrix<T,Row,Col>& left, const TMatrix<T,Row,Col>& right)
{
return false;
}