#pragma once

// TVector 2 3 4
namespace Math
{
	template <typename T, size_t size> class TVector
	{
	public:
		union {
			T data[size];
		};

	public:
		TVector();
//		TVector(T val);

	public:
	public:
	};

	template <typename T> class TVector<T, 2>
	{
	public:
		union {
			struct
			{
				T x, y;
			};
			struct
			{
				T u, v;
			};
			T data[2];
		};

	public:
		TVector<T, 2>();
		TVector<T, 2>(const T& x, const T& y);
		TVector<T, 2>(const T& val);

		//    TVector<T, 2>(const TVector &val);
		 //   TVector<T, 2>(TVector<T, 2> &&rval);
		~TVector<T, 2>() = default;

	public:
		// static int Max(const TVector& left, const TVector& right);
		// static int Min(const TVector& left, const TVector& right);
		//	static TVector VectorMax(const TVector& left, const TVector& right);
		// static TVector VectorMin(const TVector& left, const TVector& right);

		static TVector<T, 2> CrossProduct(const TVector& left, const TVector& right);

		static TVector<T, 2> DotProduct(const TVector& left, const TVector& right);

		static TVector<T, 2> Lerp(TVector a, TVector b, float t);

		static TVector<T, 2> Project(TVector vector, TVector onNormal);
		static TVector<T, 2> ProjectOnPlane(TVector vector, TVector planeNormal);

		/*
		static float Angle(TVector from, TVector to);

		static float SignedAngle(TVector from, TVector to, TVector axis);

		static float Distance(TVector a, TVector b);
		*/

		//static TVector ClampMagnitude(TVector vector, float maxLength);

		static TVector MoveTowards(TVector current, TVector target, float maxDistanceDelta);

		static TVector Reflect(TVector inDirection, TVector inNormal);

		static TVector VectorNormalize();

		static TVector VectorAbs();

	public:
		void Set(const T& x, const T& y);

		size_t Size();

		T Lenth();
		T SquaredLenth();
		void Normalize();

		void Cross(const TVector<T, 2>& val);
		void Dot(const TVector<T, 2>& val);

		//bool Equals(const TVector<T, 2>& val);
	//    bool Equals(const TVector<T, 2>& val,int Tolerance);

		TVector<T, 2> Abs();
		T Max();
		T Min();
	};

	template <typename T> class TVector<T, 3>
	{
	public:
		union {
			struct
			{
				T x, y, z;
			};
			T data[3];
		};
	public:
		TVector<T, 3>();
		TVector<T, 3>(const T& val);
		TVector<T, 3>(const T& x, const T& y, const T& z);
		TVector<T, 3>(const T* ptr);

	public:

		static TVector<T, 3> CrossProduct(const TVector& left, const TVector& right);
		static TVector<T, 3> DotProduct(const TVector& left, const TVector& right);

		static TVector<T, 3> Lerp(TVector a, TVector b, float t);

		static TVector<T, 3> Project(TVector vector, TVector onNormal);
		static TVector<T, 3> ProjectOnPlane(TVector vector, TVector planeNormal);

	public:

		void Set(const T& x, const T& y);

		size_t Size();

		T Lenth();
		T SquaredLenth();
		void Normalize();

		void Cross(const TVector<T, 3>& val);
		void Dot(const TVector<T, 3>& val);

		//bool Equals(const TVector<T, 2>& val);
	//    bool Equals(const TVector<T, 2>& val,int Tolerance);

		TVector<T, 3> Abs();
		T Max();
		T Min();

	public:
	};

	template <typename T> class TVector<T, 4>
	{
	public:
		union {
			struct
			{
				T x, y, z, w;
			};
			T data[4];
		};

	public:
		TVector<T, 4>();
		TVector<T, 4>(const T& val);
		TVector<T, 4>(const T& x, const T& y, const T& z,const T&w);
		TVector<T, 4>(const T* ptr);

	public:

		void Set(const T& x, const T& y, const T& z, const T& w);

		size_t Size();

		T Lenth();
		T SquaredLenth();
		void Normalize();

		void Cross(const TVector<T, 4>& val);
		void Dot(const TVector<T, 4>& val);

		//bool Equals(const TVector<T, 2>& val);
	//bool Equals(const TVector<T, 2>& val,int Tolerance);

		TVector<T, 4> Abs();
		T Max();
		T Min();

	public:

		static TVector CrossProduct(const TVector& left, const TVector& right);
		static TVector DotProduct(const TVector& left, const TVector& right);

		static TVector Lerp(TVector a, TVector b, float t);

		static TVector Project(TVector vector, TVector onNormal);
		static TVector ProjectOnPlane(TVector vector, TVector planeNormal);

	public:
	};
} // namespace Math

#include "Math/Detail/VectorOpterator.h"