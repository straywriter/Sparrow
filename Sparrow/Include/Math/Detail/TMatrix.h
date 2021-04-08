#pragma once

namespace Math
{
	template <typename T, size_t row, size_t col> class TMatrix
	{
	public:
		union {
			T matrix[row][col];
			T data[row * col];
		};

	public:
		TMatrix();
		TMatrix(T val);

	public:
		T Get(size_t index);
		T Get(size_t row, size_t col);

		T* Data();
	};

	template <typename T> struct MatrixType
	{
		using Type = T;
		static const unsigned int row = 0;
		static const unsigned int col = 0;
	};

	template <typename T> struct MatrixType<TMatrix<T, 3, 3>>
	{
		using Type = T;
		static const unsigned int row = 3;
		static const unsigned int col = 3;
	};
	template <typename T> struct MatrixType<TMatrix<T, 4, 3>>
	{
		using Type = T;
		static const unsigned int row = 4;
		static const unsigned int col = 3;
	};
	template <typename T> struct MatrixType<TMatrix<T, 4, 4>>
	{
		MatrixType(TMatrix<T, 4, 4>  val)
		{
		}
		using Type = T;
		static const unsigned int row = 4;
		static const unsigned int col = 4;
	};

	template<typename T>
	unsigned int GetMatrixRowSize(T val)
	{
		return  MatrixType<T>::row;
	};

	template<typename T>
	unsigned int GetMatrixColSize(T val)
	{
		return  MatrixType<T>::col;
	};
} // namespace  Math
