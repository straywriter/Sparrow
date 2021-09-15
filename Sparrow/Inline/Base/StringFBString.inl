#include "..\..\Include\Base\String.h"
#pragma once

template <typename CharType> inline Sparrow::TString<CharType>::TString() : folly::basic_fbstring<CharType>()
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(const TString<CharType>& str) : folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(const CharType* str) : folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(TString&& str) : folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(const TString& str, size_t start, size_t num)
	: folly::basic_fbstring<CharType>(str, start, num)
{
}
template <typename CharType>
inline Sparrow::TString<CharType>::TString(size_t start, size_t num) : folly::basic_fbstring<CharType>(start, num)
{
}
template <typename CharType>
template <typename Iterator>
inline Sparrow::TString<CharType>::TString(Iterator begin, Iterator end) : folly::basic_fbstring<CharType>(begin, end)
{
}
template <typename CharType>
inline Sparrow::TString<CharType>::TString(const CharType* begin, const CharType* end)
	: folly::basic_fbstring<CharType>(begin, end)
{
}

template <typename CharType> inline Sparrow::TString<CharType>::~TString()
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(const CharType* str, size_t num) : folly::basic_fbstring<CharType>(str, num)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(const std::basic_string<CharType>& str)
	: folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(size_t num, CharType str_char)
	: folly::basic_fbstring<CharType>(num, str_char)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>::TString(std::initializer_list<CharType> init_list)
	: folly::basic_fbstring<CharType>(init_list)
{
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator=(const TString& str)
{
	return folly::basic_fbstring<CharType>::operator=(str);
}

template <typename CharType> inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator=(TString&& str)
{
	return folly::basic_fbstring<CharType>::operator=(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator=(const CharType* str)
{
	return folly::basic_fbstring<CharType>::operator=(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator=(std::initializer_list<CharType> init_list)
{
	return folly::basic_fbstring<CharType>::operator=(init_list);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator=(const std::basic_string<CharType>& str)
{
	return folly::basic_fbstring<CharType>::operator=(str);
}

template <typename CharType> inline CharType* Sparrow::TString<CharType>::begin()
{
	return folly::basic_fbstring<CharType>::begin();
}

template <typename CharType> inline CharType* Sparrow::TString<CharType>::end()
{
	return folly::basic_fbstring<CharType>::end();
}

template <typename CharType> inline const CharType* Sparrow::TString<CharType>::Data() const
{
	return data();
}

template <typename CharType> inline const CharType& Sparrow::TString<CharType>::Front() const
{
	return front();
}

template <typename CharType> inline const CharType& Sparrow::TString<CharType>::Back() const
{
	retrun back();
}

template <typename CharType> inline CharType& Sparrow::TString<CharType>::Front()
{
	return front();
}

template <typename CharType> inline CharType& Sparrow::TString<CharType>::Back()
{
	return back();
}

template <typename CharType> inline void Sparrow::TString<CharType>::PopBack()
{
	return pop_back();
}

template <typename CharType> inline size_t Sparrow::TString<CharType>::Size() const
{
	return size();
}
template <typename CharType> inline size_t Sparrow::TString<CharType>::Length() const
{
	return length();
}
template <typename CharType> inline size_t Sparrow::TString<CharType>::MaxSize() const
{
	return max_size();
}

template <typename CharType> inline void Sparrow::TString<CharType>::Resize(size_t size)
{
	resize(size);
}
template <typename CharType> inline size_t Sparrow::TString<CharType>::Capacity() const
{
	return capacity();
}

template <typename CharType> inline void Sparrow::TString<CharType>::ShrinkToFit()
{
	shrink_to_fit();
}

template <typename CharType> inline void Sparrow::TString<CharType>::Clear()
{
	clear();
}

template <typename CharType> inline bool Sparrow::TString<CharType>::Empty() const
{
	return empty();
}
template <typename CharType> inline const CharType Sparrow::TString<CharType>::At(size_t pos) const
{
	return at(pos);
}

template <typename CharType> inline std::basic_string<CharType> Sparrow::TString<CharType>::ToStdString()
{
	return toStdString();
}

template <typename CharType> inline void Sparrow::TString<CharType>::Reserve(size_t res_arg)
{
	reserve(res_arg);
}

template <typename CharType> inline CharType Sparrow::TString<CharType>::At(size_t pos)
{
	return at(pos);
}

template <typename CharType> inline const CharType Sparrow::TString<CharType>::operator[](size_t pos) const
{
	return folly::basic_fbstring<CharType>::operator[](pos);
}

template <typename CharType> inline CharType Sparrow::TString<CharType>::operator[](size_t pos)
{
	return folly::basic_fbstring<CharType>::operator[](pos);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator+=(const TString& str)
{
	return folly::basic_fbstring<CharType>::operator+=(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator+=(const CharType* str_char)
{
	return folly::basic_fbstring<CharType>::operator[](str_char);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator+=(const CharType str_char)
{
	return folly::basic_fbstring<CharType>::operator[](str_char);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::operator+=(std::initializer_list<CharType> init_list)
{
	return folly::basic_fbstring<CharType>::operator[](init_list);
}

template <typename CharType> inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(const TString& str)
{
	append(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(const TString& str, const size_t start,
	size_t num)
{
	append(str, start, num);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(const CharType* str, size_t num)
{
	append(str, num);
}

template <typename CharType> inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(const CharType* str)
{
	append(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(size_t num, CharType str_char)
{
	append(num, str_char);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Append(std::initializer_list<CharType> init_list)
{
	append(init_list);
}

template <typename CharType> inline void Sparrow::TString<CharType>::PushBack(const CharType str_char)
{
	push_back(str_char);
}

template <typename CharType> inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(const TString& str)
{
	return assign(str);
}

template <typename CharType> inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(TString&& str)
{
	return assign(str);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(const TString& str, const size_t start,
	size_t num)
{
	return assign(str, start, num);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(const CharType* str, size_t num)
{
	return assign(str, num);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(std::initializer_list<CharType> init_list)
{
	return assign(init_list);
}

template <typename CharType>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(size_t num, CharType str_char)
{
	return assign(num, str_char);
}

template <typename CharType>
template <class InputIterator>
inline Sparrow::TString<CharType>& Sparrow::TString<CharType>::Assign(InputIterator first, InputIterator last)
{
	return assign(first, last);
}

template<class CharType>
std::basic_istream<CharType, std::char_traits<CharType>>& getline(
	std::basic_istream<CharType, std::char_traits<CharType>> in_stream, Sparrow::TString<CharType>& str)
{
	return getline(in_stream, str, '\n');
}

template<class CharType>
std::basic_istream<CharType, std::char_traits<CharType>>& getline(
	std::basic_istream<CharType, std::char_traits<CharType>> in_stream, Sparrow::TString<CharType>& str, CharType delim)
{
	return folly::basic_fbstring<CharType>::getline(in_stream, str, delim);
}