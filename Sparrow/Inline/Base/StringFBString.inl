#pragma once


template <typename CharType> inline Sparrow::TString<CharType>::TString() : folly::basic_fbstring<CharType>()
{
}

template<typename CharType>
inline Sparrow::TString<CharType>::TString(const TString<CharType> & str): folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType> inline Sparrow::TString<CharType>::TString(const CharType *str) : folly::basic_fbstring<CharType>(str)
{
}

template <typename CharType> inline const CharType *Sparrow::TString<CharType>::Data() const
{
    return data();
}

template <typename CharType> inline std::basic_string<CharType> Sparrow::TString<CharType>::ToStdString()
{
    return toStdString();
}

template <typename CharType> inline void Sparrow::TString<CharType>::Reserve(size_t res_arg)
{
    reserve(res_arg);
}

template<typename CharType>
inline CharType Sparrow::TString<CharType>::At(size_type n)
{
    return at(n);
}
