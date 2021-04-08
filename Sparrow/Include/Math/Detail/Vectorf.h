// #pragma once

// #include "Math/Detail/TVector.h"

// using namespace Math;

// template <> class TVector<float, 3>
// {
// public:
// 	union {
// 		struct
// 		{
// 			float x, y, z;
// 		};
// 		float data[3];
// 	};

// 	TVector();
// 	TVector(float val);
// 	TVector(float x, float y, float z);
// 	TVector(float* p);

// 	// public static
// public:

// 	static TVector CrossProduct(const TVector& left, const TVector& right);
// 	static TVector DotProduct(const TVector& left, const TVector& right);

// 	static TVector Lerp(TVector a, TVector b, float t);

// 	static TVector Project(TVector vector, TVector onNormal);
// 	static TVector ProjectOnPlane(TVector vector, TVector planeNormal);

// 	/*
// 	static float Angle(TVector from, TVector to);

// 	static float SignedAngle(TVector from, TVector to, TVector axis);

// 	static float Distance(TVector a, TVector b);

// 	static TVector ClampMagnitude(TVector vector, float maxLength);

// 	static TVector MoveTowards(TVector current, TVector target, float maxDistanceDelta);

// 	static TVector Reflect(TVector inDirection, TVector inNormal);

// 	static TVector VectorNormalize();

// 	static TVector Abs();
// 	*/

// public:
// 	void Set(float x, float y, float z);

// 	void Cross(const TVector& value);
// 	void Dot(const TVector& value);

// 	bool IsNormalized();
// 	bool IsZero();
// 	// bool Equals(TVector other);

// 	void Normalize();
// 	void Normalize(float tolerance);
// 	float SquaredLength();
// 	float Lenth();

// 	float ComponentMin(const TVector& other) const;
// 	float ComponentMax(const TVector& other) const;

// 	float operator[](size_t index);
// };

// template <> class TVector<float, 2>
// {
// public:
// 	union {
// 		struct
// 		{
// 			float x, y;
// 		};
// 		struct
// 		{
// 			float u, v;
// 		};
// 		float data[2];
// 	};

// public:
// 	TVector();
// 	TVector(float val);
// 	TVector(float x, float y);
// 	TVector(float* p);

// public:

// 	static TVector CrossProduct(const TVector& left, const TVector& right);
// 	static TVector DotProduct(const TVector& left, const TVector& right);

// 	static TVector Lerp(TVector a, TVector b, float t);

// 	static TVector Project(TVector vector, TVector onNormal);
// 	static TVector ProjectOnPlane(TVector vector, TVector planeNormal);

// public:

// 	void Set(float x, float y, float z);

// 	void Cross(const TVector& value);
// 	void Dot(const TVector& value);

// 	bool IsNormalized();
// 	bool IsZero();
// 	// bool Equals(TVector other);

// 	void Normalize();
// 	void Normalize(float tolerance);
// 	float SquaredLength();
// 	float Lenth();

// 	float ComponentMin(const TVector& other) const;
// 	float ComponentMax(const TVector& other) const;

// 	float operator[](size_t index);
// };

// template <> class TVector<float, 4>
// {
// public:
// 	union {
// 		struct
// 		{
// 			float x, y, z, w;
// 		};
// 		float data[4];
// 	};

// public:
// 	TVector();
// 	TVector(float val);
// 	TVector(float x, float y, float z, float w);
// 	TVector(const float* p);

// public:

// 	static TVector CrossProduct(const TVector& left, const TVector& right);
// 	static TVector DotProduct(const TVector& left, const TVector& right);

// 	static TVector Lerp(TVector a, TVector b, float t);

// 	static TVector Project(TVector vector, TVector onNormal);
// 	static TVector ProjectOnPlane(TVector vector, TVector planeNormal);

// public:

// 	void Set(float x, float y, float z,float w);

// 	void Cross(const TVector& value);
// 	void Dot(const TVector& value);

// 	bool IsNormalized();
// 	bool IsZero();
// 	// bool Equals(TVector other);

// 	void Normalize();
// 	void Normalize(float tolerance);
// 	float SquaredLength();
// 	float Lenth();

// 	float ComponentMin(const TVector& other) const;
// 	float ComponentMax(const TVector& other) const;

// 	float operator[](size_t index);
// };
