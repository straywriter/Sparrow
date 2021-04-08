
#pragma once

// class utils

// namespace
#define NAMESPACE_BEGIN(name)                                                                                          \
    namespace name                                                                                                     \
    {
#define NAMESPACE_END() }

// array
#define ARRAY_SIZE(c_array) (sizeof(ArraySizeHelper(c_array)))

// ArraySizeHelper
template <typename T, size_t N> auto ArraySizeHelper(const T (&array)[N]) -> char (&)[N];
