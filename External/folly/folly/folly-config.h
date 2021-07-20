/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#if !defined(FOLLY_MOBILE)
#if defined(__ANDROID__) || \
    (defined(__APPLE__) &&  \
     (TARGET_IPHONE_SIMULATOR || TARGET_OS_SIMULATOR || TARGET_OS_IPHONE))
#define FOLLY_MOBILE 1
#else
#define FOLLY_MOBILE 0
#endif
#endif // FOLLY_MOBILE

/* #undef FOLLY_HAVE_PTHREAD */
/* #undef FOLLY_HAVE_PTHREAD_ATFORK */

/* #undef FOLLY_HAVE_LIBGFLAGS */
/* #undef FOLLY_UNUSUAL_GFLAGS_NAMESPACE */
/* #undef FOLLY_GFLAGS_NAMESPACE */

/* #undef FOLLY_HAVE_LIBGLOG */

/* #undef FOLLY_USE_JEMALLOC */
/* #undef FOLLY_USE_LIBSTDCPP */

#if __has_include(<features.h>)
#include <features.h>
#endif

/* #undef FOLLY_HAVE_ACCEPT4 */
#define FOLLY_HAVE_GETRANDOM 0
/* #undef FOLLY_HAVE_PREADV */
/* #undef FOLLY_HAVE_PWRITEV */
/* #undef FOLLY_HAVE_CLOCK_GETTIME */
/* #undef FOLLY_HAVE_PIPE2 */
/* #undef FOLLY_HAVE_SENDMMSG */
/* #undef FOLLY_HAVE_RECVMMSG */
/* #undef FOLLY_HAVE_OPENSSL_ASN1_TIME_DIFF */

/* #undef FOLLY_HAVE_IFUNC */
#define FOLLY_HAVE_STD__IS_TRIVIALLY_COPYABLE 1
#define FOLLY_HAVE_UNALIGNED_ACCESS 1
/* #undef FOLLY_HAVE_VLA */
/* #undef FOLLY_HAVE_WEAK_SYMBOLS */
/* #undef FOLLY_HAVE_LINUX_VDSO */
/* #undef FOLLY_HAVE_MALLOC_USABLE_SIZE */
/* #undef FOLLY_HAVE_INT128_T */
/* #undef FOLLY_SUPPLY_MISSING_INT128_TRAITS */
#define FOLLY_HAVE_WCHAR_SUPPORT 1
/* #undef FOLLY_HAVE_EXTRANDOM_SFMT19937 */
/* #undef FOLLY_USE_LIBCPP */
/* #undef HAVE_VSNPRINTF_ERRORS */

/* #undef FOLLY_HAVE_LIBUNWIND */
/* #undef FOLLY_HAVE_DWARF */
/* #undef FOLLY_HAVE_ELF */
/* #undef FOLLY_HAVE_SWAPCONTEXT */
/* #undef FOLLY_HAVE_BACKTRACE */
/* #undef FOLLY_USE_SYMBOLIZER */
#define FOLLY_DEMANGLE_MAX_SYMBOL_SIZE 1024

/* #undef FOLLY_HAVE_SHADOW_LOCAL_WARNINGS */

/* #undef FOLLY_HAVE_LIBLZ4 */
/* #undef FOLLY_HAVE_LIBLZMA */
/* #undef FOLLY_HAVE_LIBSNAPPY */
/* #undef FOLLY_HAVE_LIBZ */
/* #undef FOLLY_HAVE_LIBZSTD */
/* #undef FOLLY_HAVE_LIBBZ2 */

#define FOLLY_LIBRARY_SANITIZE_ADDRESS 0

/* #undef FOLLY_SUPPORT_SHARED_LIBRARY */

#define FOLLY_HAVE_LIBRT 0
