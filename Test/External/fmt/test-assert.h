// Formatting library for C++ - test version of FMT_ASSERT
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_TEST_ASSERT_H_
#define FMT_TEST_ASSERT_H_

#include <stdexcept>

void throw_assertion_failure(const char* message);
#define FMT_ASSERT(condition, message) \
  if (!(condition)) throw_assertion_failure(message);

#include "gtest-extra.h"
#include "gtest/gtest.h"

class assertion_failure : public std::logic_error {
 public:
  explicit assertion_failure(const char* message) : std::logic_error(message) {}

 private:
  virtual void avoid_weak_vtable();
};

void assertion_failure::avoid_weak_vtable() {}

// We use a separate function (rather than throw directly from FMT_ASSERT) to
// avoid GCC's -Wterminate warning when FMT_ASSERT is used in a destructor.
inline void throw_assertion_failure(const char* message) {
  throw assertion_failure(message);
}

// Expects an assertion failure.
#define EXPECT_ASSERT(stmt, message) \
  FMT_TEST_THROW_(stmt, assertion_failure, message, GTEST_NONFATAL_FAILURE_)



#include <cstdlib>

#include "gtest/gtest.h"

#ifdef _WIN32
#  include <windows.h>
#endif

#ifdef _MSC_VER
#  include <crtdbg.h>
#else
#  define _CrtSetReportFile(a, b)
#  define _CrtSetReportMode(a, b)
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
  // Don't display any error dialogs. This also suppresses message boxes
  // on assertion failures in MinGW where _set_error_mode/CrtSetReportMode
  // doesn't help.
  SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
               SEM_NOOPENFILEERRORBOX);
#endif
  // Disable message boxes on assertion failures.
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  try {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  } catch (...) {
    // Catch all exceptions to make Coverity happy.
  }
  return EXIT_FAILURE;
}


#endif  // FMT_TEST_ASSERT_H_
