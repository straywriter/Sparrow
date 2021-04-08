// Formatting library for C++ - dynamic argument store tests
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.
#define FMT_HEADER_ONLY
#include "fmt/args.h"
#include "fmt/format.h"

#include <stdint.h>

#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstring>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

#include <sstream>

#include "gmock/gmock.h"
using fmt::format;

using fmt::format_error;



void increment(char* s) {
  for (int i = static_cast<int>(std::strlen(s)) - 1; i >= 0; --i) {
    if (s[i] != '9') {
      ++s[i];
      break;
    }
    s[i] = '0';
  }
}
TEST(UtilTest, Increment) {
  char s[10] = "123";
  increment(s);
  EXPECT_STREQ("124", s);
  s[2] = '8';
  increment(s);
  EXPECT_STREQ("129", s);
  increment(s);
  EXPECT_STREQ("130", s);
  s[1] = s[2] = '9';
  increment(s);
  EXPECT_STREQ("200", s);
}
 