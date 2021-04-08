#include "Math/Matrix.h"

using namespace Math;

template<typename T, size_t row, size_t col>
Math::TMatrix<T, row, col>::TMatrix()
{
	for (auto i = row * col; i--; this->data[i]=0)
		;
}

template<typename T, size_t row, size_t col>
Math::TMatrix<T, row, col>::TMatrix(T val)
{
	for (auto i = row * col; i--; this.data[i]=val)
		;
}

Math::TMatrix<float, 4, 4>::TMatrix()
{
	for (auto i =16; i--; this->data[i]=0)
		;
}
