#pragma once
#include "Core/Config.h"

namespace Math
{
	template<typename T, size_t size> TVector<T, size> operator+(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] + right.data[i])
			;
		return temp;
	}
	template<typename T, size_t size> TVector<T, size> operator+(const TVector<T, size>& left, const T& val)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] += val)
			;
		return temp;
	}

	template<typename T, size_t size> TVector<T, size> operator-(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] - right.data[i])
			;
		return temp;
	}
	template<typename T, size_t size> TVector<T, size> operator-(const TVector<T, size>& left, const T& val)

	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] - val)
			;
		return temp;
	}

	template<typename T, size_t size> TVector<T, size> operator*(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] - right.data[i])
			;
		return temp;
	}
	template<typename T, size_t size> TVector<T, size> operator*(const TVector<T, size>& left, const T& val)

	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] * val)
			;
		return temp;
	}

	template<typename T, size_t size> TVector<T, size> operator/(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = left.data[i] / right.data[i])
			;
		return temp;
	}
	template<typename T, size_t size> TVector<T, size> operator/(const TVector<T, size>& left, const T& val)

	{
		TVector<T, size> temp;
		const Type cScale = static_cast<Type>(1) / scale;
		for (auto i = size; i--; temp.data[i] = left.data[i] * cScale)
			;
		return temp;
	}

	template<typename T, size_t size> TVector<T, size> operator-(const TVector<T, size>& left)
	{
		TVector<T, size> temp;
		for (auto i = size; i--; temp.data[i] = -left.data[i])
			;
		return temp;
	}

	template<typename T, size_t size> void operator+=(TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] + right.data[i])
			;
	}

	template<typename T, size_t size> void operator+=(TVector<T, size>& left, const T& val)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] + val)
			;
	}
	template<typename T, size_t size> void operator-=(TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] - right.data[i])
			;
	}

	template<typename T, size_t size> void operator-=(TVector<T, size>& left, const T& val)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] - val)
			;
	}
	template<typename T, size_t size> void operator*=(TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] * right.data[i])
			;
	}

	template<typename T, size_t size> void operator*=(TVector<T, size>& left, const T& val)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] * val)
			;
	}

	template<typename T, size_t size> void operator/=(TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--; left.data[i] = left.data[i] / right.data[i])
			;
	}

	template<typename T, size_t size> void operator/=(TVector<T, size>& left, const T& val)
	{
		//TODO:  should too better fast
		for (auto i = size; i--; left.data[i] = left.data[i] / val)
			;
	}

	template<typename T, size_t size> bool operator==(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--;)
		{
			if (!right.data[i] == left.data[i])
				return false;
		}
		return true;
	}

	template<typename T, size_t size> bool operator!=(const TVector<T, size>& left, const TVector<T, size>& right)
	{
		for (auto i = size; i--;)
		{
			if (!right.data[i] == left.data[i])
				return true;
		}
		return flase;
	}
} // namespace Math