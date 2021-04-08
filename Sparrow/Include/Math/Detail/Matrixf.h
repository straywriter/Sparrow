#pragma once

#include "Math/Detail/TMatrix.h"
#include "Math/Vector.h"
namespace Math
{
	template <> class TMatrix<float, 4, 4>
	{
	public:
		union {
			float matrix[4][4];
			float data[16];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};

	public:

		TMatrix();
		TMatrix(float val);
		TMatrix(Vector4f v1, Vector4f v2, Vector4f v3, Vector4f v4);

	public:
		float Get(size_t index);
		float Get(size_t row, size_t col);

		float* Data();

	};
} // namespace Math