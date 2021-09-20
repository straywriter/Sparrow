#include <Base/Format.h>

#include <gtest/gtest.h>
#include <string_view>



#include "gtest-extra.h"
#include "util.h"
using namespace Sparrow;


TEST(format_test, waht) {

Print("asdfasdf{}",3);
}

// Increment a number in a string.
void increment(char* s) {
  for (int i = static_cast<int>(std::strlen(s)) - 1; i >= 0; --i) {
    if (s[i] != '9') {
      ++s[i];
      break;
    }
    s[i] = '0';
  }
}




TEST(util_test, increment) {
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

//report_system_error

using fmt::runtime;




TEST(format_test, escape) {
  EXPECT_EQ("{", Format("{{"));
  EXPECT_EQ("before {", Format("before {{"));
  EXPECT_EQ("{ after", Format("{{ after"));
  EXPECT_EQ("before { after", Format("before {{ after"));

  EXPECT_EQ("}", Format("}}"));
  EXPECT_EQ("before }", Format("before }}"));
  EXPECT_EQ("} after", Format("}} after"));
  EXPECT_EQ("before } after", Format("before }} after"));

  EXPECT_EQ("{}", Format("{{}}"));
  EXPECT_EQ("{42}", Format("{{{0}}}", 42));
}


// TEST(format_test, unmatched_braces) {
//   EXPECT_THROW_MSG(Format(runtime("{")), FormatError,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("}")), FormatError,
//                    "unmatched '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0{}")), FormatError,
//                    "invalid format string");
// }

TEST(format_test, no_args) { EXPECT_EQ("test", Format("test")); }

TEST(format_test, args_in_different_positions) {
  EXPECT_EQ("42", Format("{0}", 42));
  EXPECT_EQ("before 42", Format("before {0}", 42));
  EXPECT_EQ("42 after", Format("{0} after", 42));
  EXPECT_EQ("before 42 after", Format("before {0} after", 42));
  EXPECT_EQ("answer = 42", Format("{0} = {1}", "answer", 42));
  EXPECT_EQ("42 is the answer", Format("{1} is the {0}", "answer", 42));
  EXPECT_EQ("abracadabra", Format("{0}{1}{0}", "abra", "cad"));
}

// enum:unsigned { 256 = 256 };

// TEST(format_test, arg_errors) {
//   EXPECT_THROW_MSG(Format(runtime("{")), FormatError,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{?}")), FormatError,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0")), FormatError,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0}")), FormatError,
//                    "argument not found");
//   EXPECT_THROW_MSG(Format(runtime("{00}"), 42), FormatError,
//                    "invalid format string");

// //   char format_str[256];
//   char format_str[256];
//   safe_sprintf(format_str, "{%u", INT_MAX);
//   EXPECT_THROW_MSG(Format(runtime(format_str)), FormatError,
//                    "invalid format string");
//   safe_sprintf(format_str, "{%u}", INT_MAX);
//   EXPECT_THROW_MSG(Format(runtime(format_str)), FormatError,
//                    "argument not found");

//   safe_sprintf(format_str, "{%u", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str)), FormatError,
//                    "invalid format string");
//   safe_sprintf(format_str, "{%u}", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str)), FormatError,
//                    "argument not found");
// }

template <int N> struct test_format {
  template <typename... T>
  static std::string format(fmt::string_view fmt, const T&... args) {
    return test_format<N - 1>::format(fmt, N - 1, args...);
  }
};

template <> struct test_format<0> {
  template <typename... T>
  static std::string format(fmt::string_view fmt, const T&... args) {
    return Format(runtime(fmt), args...);
  }
};

// TEST(format_test, many_args) {
//   EXPECT_EQ("19", test_format<20>::format("{19}"));
//   EXPECT_THROW_MSG(test_format<20>::format("{20}"), format_error,
//                    "argument not found");
//   EXPECT_THROW_MSG(test_format<21>::format("{21}"), format_error,
//                    "argument not found");
//   using fmt::detail::max_packed_args;
//   std::string format_str = Format("{{{}}}", max_packed_args + 1);
//   EXPECT_THROW_MSG(test_format<max_packed_args>::format(format_str),
//                    format_error, "argument not found");
// }




// TEST(format_test, named_arg) {
//   EXPECT_EQ("1/a/A", Format("{_1}/{a_}/{A_}", fmt::arg("a_", 'a'),
//                                  fmt::arg("A_", "A"), fmt::arg("_1", 1)));
//   EXPECT_EQ(" -42", Format("{0:{width}}", -42, fmt::arg("width", 4)));
//   EXPECT_EQ("st",
//             Format("{0:.{precision}}", "str", fmt::arg("precision", 2)));
//   EXPECT_EQ("1 2", Format("{} {two}", 1, fmt::arg("two", 2)));
//   EXPECT_EQ("42",
//             Format("{c}", fmt::arg("a", 0), fmt::arg("b", 0),
//                         fmt::arg("c", 42), fmt::arg("d", 0), fmt::arg("e", 0),
//                         fmt::arg("f", 0), fmt::arg("g", 0), fmt::arg("h", 0),
//                         fmt::arg("i", 0), fmt::arg("j", 0), fmt::arg("k", 0),
//                         fmt::arg("l", 0), fmt::arg("m", 0), fmt::arg("n", 0),
//                         fmt::arg("o", 0), fmt::arg("p", 0)));
//   EXPECT_THROW_MSG(Format(runtime("{a}")), format_error,
//                    "argument not found");
//   EXPECT_THROW_MSG(Format(runtime("{a}"), 42), format_error,
//                    "argument not found");
// }

// TEST(format_test, auto_arg_index) {
//   EXPECT_EQ("abc", Format("{}{}{}", 'a', 'b', 'c'));
//   EXPECT_THROW_MSG(Format(runtime("{0}{}"), 'a', 'b'), format_error,
//                    "cannot switch from manual to automatic argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{}{0}"), 'a', 'b'), format_error,
//                    "cannot switch from automatic to manual argument indexing");
//   EXPECT_EQ("1.2", Format("{:.{}}", 1.2345, 2));
//   EXPECT_THROW_MSG(Format(runtime("{0}:.{}"), 1.2345, 2), format_error,
//                    "cannot switch from manual to automatic argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{:.{0}}"), 1.2345, 2), format_error,
//                    "cannot switch from automatic to manual argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{}")), format_error,
//                    "argument not found");
// }

TEST(format_test, empty_specs) { EXPECT_EQ("42", Format("{0:}", 42)); }

TEST(format_test, left_align) {
  EXPECT_EQ("42  ", Format("{0:<4}", 42));
  EXPECT_EQ("42  ", Format("{0:<4o}", 042));
  EXPECT_EQ("42  ", Format("{0:<4x}", 0x42));
  EXPECT_EQ("-42  ", Format("{0:<5}", -42));
  EXPECT_EQ("42   ", Format("{0:<5}", 42u));
  EXPECT_EQ("-42  ", Format("{0:<5}", -42l));
  EXPECT_EQ("42   ", Format("{0:<5}", 42ul));
  EXPECT_EQ("-42  ", Format("{0:<5}", -42ll));
  EXPECT_EQ("42   ", Format("{0:<5}", 42ull));
  EXPECT_EQ("-42  ", Format("{0:<5}", -42.0));
  EXPECT_EQ("-42  ", Format("{0:<5}", -42.0l));
  EXPECT_EQ("c    ", Format("{0:<5}", 'c'));
  EXPECT_EQ("abc  ", Format("{0:<5}", "abc"));
  EXPECT_EQ("0xface  ", Format("{0:<8}", reinterpret_cast<void*>(0xface)));
}

TEST(format_test, right_align) {
  EXPECT_EQ("  42", Format("{0:>4}", 42));
  EXPECT_EQ("  42", Format("{0:>4o}", 042));
  EXPECT_EQ("  42", Format("{0:>4x}", 0x42));
  EXPECT_EQ("  -42", Format("{0:>5}", -42));
  EXPECT_EQ("   42", Format("{0:>5}", 42u));
  EXPECT_EQ("  -42", Format("{0:>5}", -42l));
  EXPECT_EQ("   42", Format("{0:>5}", 42ul));
  EXPECT_EQ("  -42", Format("{0:>5}", -42ll));
  EXPECT_EQ("   42", Format("{0:>5}", 42ull));
  EXPECT_EQ("  -42", Format("{0:>5}", -42.0));
  EXPECT_EQ("  -42", Format("{0:>5}", -42.0l));
  EXPECT_EQ("    c", Format("{0:>5}", 'c'));
  EXPECT_EQ("  abc", Format("{0:>5}", "abc"));
  EXPECT_EQ("  0xface", Format("{0:>8}", reinterpret_cast<void*>(0xface)));
}

TEST(format_test, center_align) {
  EXPECT_EQ(" 42  ", Format("{0:^5}", 42));
  EXPECT_EQ(" 42  ", Format("{0:^5o}", 042));
  EXPECT_EQ(" 42  ", Format("{0:^5x}", 0x42));
  EXPECT_EQ(" -42 ", Format("{0:^5}", -42));
  EXPECT_EQ(" 42  ", Format("{0:^5}", 42u));
  EXPECT_EQ(" -42 ", Format("{0:^5}", -42l));
  EXPECT_EQ(" 42  ", Format("{0:^5}", 42ul));
  EXPECT_EQ(" -42 ", Format("{0:^5}", -42ll));
  EXPECT_EQ(" 42  ", Format("{0:^5}", 42ull));
  EXPECT_EQ(" -42 ", Format("{0:^5}", -42.0));
  EXPECT_EQ(" -42 ", Format("{0:^5}", -42.0l));
  EXPECT_EQ("  c  ", Format("{0:^5}", 'c'));
  EXPECT_EQ(" abc  ", Format("{0:^6}", "abc"));
  EXPECT_EQ(" 0xface ", Format("{0:^8}", reinterpret_cast<void*>(0xface)));
}

TEST(format_test, fill) {
//   EXPECT_THROW_MSG(Format(runtime("{0:{<5}"), 'c'), format_error,
//                    "invalid fill character '{'");
//   EXPECT_THROW_MSG(Format(runtime("{0:{<5}}"), 'c'), format_error,
//                    "invalid fill character '{'");
  EXPECT_EQ("**42", Format("{0:*>4}", 42));
  EXPECT_EQ("**-42", Format("{0:*>5}", -42));
  EXPECT_EQ("***42", Format("{0:*>5}", 42u));
  EXPECT_EQ("**-42", Format("{0:*>5}", -42l));
  EXPECT_EQ("***42", Format("{0:*>5}", 42ul));
  EXPECT_EQ("**-42", Format("{0:*>5}", -42ll));
  EXPECT_EQ("***42", Format("{0:*>5}", 42ull));
  EXPECT_EQ("**-42", Format("{0:*>5}", -42.0));
  EXPECT_EQ("**-42", Format("{0:*>5}", -42.0l));
  EXPECT_EQ("c****", Format("{0:*<5}", 'c'));
  EXPECT_EQ("abc**", Format("{0:*<5}", "abc"));
  EXPECT_EQ("**0xface",
            Format("{0:*>8}", reinterpret_cast<void*>(0xface)));
  EXPECT_EQ("foo=", Format("{:}=", "foo"));
  EXPECT_EQ(std::string("\0\0\0*", 4),
            Format(string_view("{:\0>4}", 6), '*'));
  EXPECT_EQ("жж42", Format("{0:ж>4}", 42));
//   EXPECT_THROW_MSG(Format(runtime("{:\x80\x80\x80\x80\x80>}"), 0),
//                    format_error, "invalid type specifier");
}

TEST(format_test, plus_sign) {
  EXPECT_EQ("+42", Format("{0:+}", 42));
  EXPECT_EQ("-42", Format("{0:+}", -42));
  EXPECT_EQ("+42", Format("{0:+}", 42));
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), 42u), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("+42", Format("{0:+}", 42l));
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), 42ul), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("+42", Format("{0:+}", 42ll));
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), 42ull), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("+42", Format("{0:+}", 42.0));
//   EXPECT_EQ("+42", Format("{0:+}", 42.0l));
//   EXPECT_THROW_MSG(Format(runtime("{0:+"), 'c'), format_error,
//                    "missing '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), 'c'), format_error,
//                    "invalid format specifier for char");
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), "abc"), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{0:+}"), reinterpret_cast<void*>(0x42)),
//                    format_error, "format specifier requires numeric argument");
}

TEST(format_test, minus_sign) {
  EXPECT_EQ("42", Format("{0:-}", 42));
  EXPECT_EQ("-42", Format("{0:-}", -42));
  EXPECT_EQ("42", Format("{0:-}", 42));
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), 42u), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("42", Format("{0:-}", 42l));
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), 42ul), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("42", Format("{0:-}", 42ll));
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), 42ull), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ("42", Format("{0:-}", 42.0));
//   EXPECT_EQ("42", Format("{0:-}", 42.0l));
//   EXPECT_THROW_MSG(Format(runtime("{0:-"), 'c'), format_error,
//                    "missing '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), 'c'), format_error,
//                    "invalid format specifier for char");
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), "abc"), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{0:-}"), reinterpret_cast<void*>(0x42)),
//                    format_error, "format specifier requires numeric argument");
}

TEST(format_test, space_sign) {
  EXPECT_EQ(" 42", Format("{0: }", 42));
  EXPECT_EQ("-42", Format("{0: }", -42));
  EXPECT_EQ(" 42", Format("{0: }", 42));
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), 42u), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ(" 42", Format("{0: }", 42l));
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), 42ul), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ(" 42", Format("{0: }", 42ll));
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), 42ull), format_error,
//                    "format specifier requires signed argument");
//   EXPECT_EQ(" 42", Format("{0: }", 42.0));
//   EXPECT_EQ(" 42", Format("{0: }", 42.0l));
//   EXPECT_THROW_MSG(Format(runtime("{0: "), 'c'), format_error,
//                    "missing '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), 'c'), format_error,
//                    "invalid format specifier for char");
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), "abc"), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{0: }"), reinterpret_cast<void*>(0x42)),
//                    format_error, "format specifier requires numeric argument");
}

TEST(format_test, hash_flag) {
  EXPECT_EQ("42", Format("{0:#}", 42));
  EXPECT_EQ("-42", Format("{0:#}", -42));
  EXPECT_EQ("0b101010", Format("{0:#b}", 42));
  EXPECT_EQ("0B101010", Format("{0:#B}", 42));
  EXPECT_EQ("-0b101010", Format("{0:#b}", -42));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42));
  EXPECT_EQ("0X42", Format("{0:#X}", 0x42));
  EXPECT_EQ("-0x42", Format("{0:#x}", -0x42));
  EXPECT_EQ("0", Format("{0:#o}", 0));
  EXPECT_EQ("042", Format("{0:#o}", 042));
  EXPECT_EQ("-042", Format("{0:#o}", -042));
  EXPECT_EQ("42", Format("{0:#}", 42u));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42u));
  EXPECT_EQ("042", Format("{0:#o}", 042u));

  EXPECT_EQ("-42", Format("{0:#}", -42l));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42l));
  EXPECT_EQ("-0x42", Format("{0:#x}", -0x42l));
  EXPECT_EQ("042", Format("{0:#o}", 042l));
  EXPECT_EQ("-042", Format("{0:#o}", -042l));
  EXPECT_EQ("42", Format("{0:#}", 42ul));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42ul));
  EXPECT_EQ("042", Format("{0:#o}", 042ul));

  EXPECT_EQ("-42", Format("{0:#}", -42ll));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42ll));
  EXPECT_EQ("-0x42", Format("{0:#x}", -0x42ll));
  EXPECT_EQ("042", Format("{0:#o}", 042ll));
  EXPECT_EQ("-042", Format("{0:#o}", -042ll));
  EXPECT_EQ("42", Format("{0:#}", 42ull));
  EXPECT_EQ("0x42", Format("{0:#x}", 0x42ull));
  EXPECT_EQ("042", Format("{0:#o}", 042ull));

  EXPECT_EQ("-42.0", Format("{0:#}", -42.0));
  EXPECT_EQ("-42.0", Format("{0:#}", -42.0l));
  EXPECT_EQ("4.e+01", Format("{:#.0e}", 42.0));
  EXPECT_EQ("0.", Format("{:#.0f}", 0.01));
  EXPECT_EQ("0.50", Format("{:#.2g}", 0.5));
  EXPECT_EQ("0.", Format("{:#.0f}", 0.5));
//   EXPECT_THROW_MSG(Format(runtime("{0:#"), 'c'), format_error,
//                    "missing '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:#}"), 'c'), format_error,
//                    "invalid format specifier for char");
//   EXPECT_THROW_MSG(Format(runtime("{0:#}"), "abc"), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{0:#}"), reinterpret_cast<void*>(0x42)),
//                    format_error, "format specifier requires numeric argument");
}

TEST(format_test, zero_flag) {
  EXPECT_EQ("42", Format("{0:0}", 42));
  EXPECT_EQ("-0042", Format("{0:05}", -42));
  EXPECT_EQ("00042", Format("{0:05}", 42u));
  EXPECT_EQ("-0042", Format("{0:05}", -42l));
  EXPECT_EQ("00042", Format("{0:05}", 42ul));
  EXPECT_EQ("-0042", Format("{0:05}", -42ll));
  EXPECT_EQ("00042", Format("{0:05}", 42ull));
  EXPECT_EQ("-000042", Format("{0:07}", -42.0));
  EXPECT_EQ("-000042", Format("{0:07}", -42.0l));
//   EXPECT_THROW_MSG(Format(runtime("{0:0"), 'c'), format_error,
//                    "missing '}' in format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:05}"), 'c'), format_error,
//                    "invalid format specifier for char");
//   EXPECT_THROW_MSG(Format(runtime("{0:05}"), "abc"), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(
//       Format(runtime("{0:05}"), reinterpret_cast<void*>(0x42)),
//       format_error, "format specifier requires numeric argument");
}


TEST(format_test, width) {
  char format_str[256];
  safe_sprintf(format_str, "{0:%u", UINT_MAX);
  increment(format_str + 3);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");
//   size_t size = std::strlen(format_str);
//   format_str[size] = '}';
//   format_str[size + 1] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");

//   safe_sprintf(format_str, "{0:%u", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");
//   safe_sprintf(format_str, "{0:%u}", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
                //    "number is too big");
  EXPECT_EQ(" -42", Format("{0:4}", -42));
  EXPECT_EQ("   42", Format("{0:5}", 42u));
  EXPECT_EQ("   -42", Format("{0:6}", -42l));
  EXPECT_EQ("     42", Format("{0:7}", 42ul));
  EXPECT_EQ("   -42", Format("{0:6}", -42ll));
  EXPECT_EQ("     42", Format("{0:7}", 42ull));
  EXPECT_EQ("   -1.23", Format("{0:8}", -1.23));
  EXPECT_EQ("    -1.23", Format("{0:9}", -1.23l));
  EXPECT_EQ("    0xcafe",
            Format("{0:10}", reinterpret_cast<void*>(0xcafe)));
  EXPECT_EQ("x          ", Format("{0:11}", 'x'));
  EXPECT_EQ("str         ", Format("{0:12}", "str"));
  EXPECT_EQ(Format("{:*^6}", "🤡"), "**🤡**");
  EXPECT_EQ(Format("{:*^8}", "你好"), "**你好**");
  EXPECT_EQ(Format("{:#6}", 42.0), "  42.0");
  EXPECT_EQ(Format("{:6c}", static_cast<int>('x')), "x     ");
  EXPECT_EQ(Format("{:>06.0f}", 0.00884311), "000000");
}

TEST(format_test, runtime_width) {
  char format_str[256];
  safe_sprintf(format_str, "{0:{%u", UINT_MAX);
  increment(format_str + 4);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "invalid format string");
//   size_t size = std::strlen(format_str);
//   format_str[size] = '}';
//   format_str[size + 1] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "argument not found");
//   format_str[size + 1] = '}';
//   format_str[size + 2] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "argument not found");

//   EXPECT_THROW_MSG(Format(runtime("{0:{"), 0), format_error,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:{}"), 0), format_error,
//                    "cannot switch from manual to automatic argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{0:{?}}"), 0), format_error,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0), format_error,
//                    "argument not found");

//   EXPECT_THROW_MSG(Format(runtime("{0:{0:}}"), 0), format_error,
//                    "invalid format string");

//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, -1), format_error,
//                    "negative width");
//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, (INT_MAX + 1u)),
//                    format_error, "number is too big");
//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, -1l), format_error,
//                    "negative width");
//   if (fmt::detail::const_check(sizeof(long) > sizeof(int))) {
//     long value = INT_MAX;
//     EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, (value + 1)),
//                      format_error, "number is too big");
//   }
//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, (INT_MAX + 1ul)),
//                    format_error, "number is too big");

//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, '0'), format_error,
//                    "width is not integer");
//   EXPECT_THROW_MSG(Format(runtime("{0:{1}}"), 0, 0.0), format_error,
//                    "width is not integer");

  EXPECT_EQ(" -42", Format("{0:{1}}", -42, 4));
  EXPECT_EQ("   42", Format("{0:{1}}", 42u, 5));
  EXPECT_EQ("   -42", Format("{0:{1}}", -42l, 6));
  EXPECT_EQ("     42", Format("{0:{1}}", 42ul, 7));
  EXPECT_EQ("   -42", Format("{0:{1}}", -42ll, 6));
  EXPECT_EQ("     42", Format("{0:{1}}", 42ull, 7));
  EXPECT_EQ("   -1.23", Format("{0:{1}}", -1.23, 8));
  EXPECT_EQ("    -1.23", Format("{0:{1}}", -1.23l, 9));
  EXPECT_EQ("    0xcafe",
            Format("{0:{1}}", reinterpret_cast<void*>(0xcafe), 10));
  EXPECT_EQ("x          ", Format("{0:{1}}", 'x', 11));
  EXPECT_EQ("str         ", Format("{0:{1}}", "str", 12));
}

TEST(format_test, precision) {
  char format_str[256];
  safe_sprintf(format_str, "{0:.%u", UINT_MAX);
  increment(format_str + 4);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");
//   size_t size = std::strlen(format_str);
//   format_str[size] = '}';
//   format_str[size + 1] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");

//   safe_sprintf(format_str, "{0:.%u", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");
//   safe_sprintf(format_str, "{0:.%u}", INT_MAX + 1u);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "number is too big");

//   EXPECT_THROW_MSG(Format(runtime("{0:."), 0), format_error,
//                    "missing precision specifier");
//   EXPECT_THROW_MSG(Format(runtime("{0:.}"), 0), format_error,
//                    "missing precision specifier");

//   EXPECT_THROW_MSG(Format(runtime("{0:.2"), 0), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42u), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42u), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42l), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42l), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42ul), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42ul), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42ll), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42ll), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2}"), 42ull), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.2f}"), 42ull), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:3.0}"), 'x'), format_error,
//                    "precision not allowed for this argument type");
  EXPECT_EQ("1.2", Format("{0:.2}", 1.2345));
  EXPECT_EQ("1.2", Format("{0:.2}", 1.2345l));
  EXPECT_EQ("1.2e+56", Format("{:.2}", 1.234e56));
  EXPECT_EQ("1.1", Format("{0:.3}", 1.1));
  EXPECT_EQ("1e+00", Format("{:.0e}", 1.0L));
  EXPECT_EQ("  0.0e+00", Format("{:9.1e}", 0.0));
  EXPECT_EQ(
      Format("{:.494}", 4.9406564584124654E-324),
      "4.9406564584124654417656879286822137236505980261432476442558568250067550"
      "727020875186529983636163599237979656469544571773092665671035593979639877"
      "479601078187812630071319031140452784581716784898210368871863605699873072"
      "305000638740915356498438731247339727316961514003171538539807412623856559"
      "117102665855668676818703956031062493194527159149245532930545654440112748"
      "012970999954193198940908041656332452475714786901472678015935523861155013"
      "480352649347201937902681071074917033322268447533357208324319361e-324");

  std::string outputs[] = {
      "-0X1.41FE3FFE71C9E000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000P+127",
      "-0XA.0FF1FFF38E4F0000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000P+124"};
  EXPECT_THAT(outputs,
              testing::Contains(Format("{:.838A}", -2.14001164E+38)));

  EXPECT_EQ("123.", Format("{:#.0f}", 123.0));
  EXPECT_EQ("1.23", Format("{:.02f}", 1.234));
  EXPECT_EQ("0.001", Format("{:.1g}", 0.001));
  EXPECT_EQ("1019666400", Format("{}", 1019666432.0f));
  EXPECT_EQ("1e+01", Format("{:.0e}", 9.5));
  EXPECT_EQ("1.0e-34", Format("{:.1e}", 1e-34));

//   EXPECT_THROW_MSG(
//       Format(runtime("{0:.2}"), reinterpret_cast<void*>(0xcafe)),
//       format_error, "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(
//       Format(runtime("{0:.2f}"), reinterpret_cast<void*>(0xcafe)),
//       format_error, "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(
//       Format(runtime("{:.{}e}"), 42.0, fmt::detail::max_value<int>()),
//       format_error, "number is too big");

  EXPECT_EQ("st", Format("{0:.2}", "str"));
}

TEST(format_test, runtime_precision) {
  char format_str[256];
  safe_sprintf(format_str, "{0:.{%u", UINT_MAX);
  increment(format_str + 5);
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "invalid format string");
//   size_t size = std::strlen(format_str);
//   format_str[size] = '}';
//   format_str[size + 1] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "argument not found");
//   format_str[size + 1] = '}';
//   format_str[size + 2] = 0;
//   EXPECT_THROW_MSG(Format(runtime(format_str), 0), format_error,
//                    "argument not found");

//   EXPECT_THROW_MSG(Format(runtime("{0:.{"), 0), format_error,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{}"), 0), format_error,
//                    "cannot switch from manual to automatic argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{?}}"), 0), format_error,
//                    "invalid format string");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}"), 0, 0), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0), format_error,
//                    "argument not found");

//   EXPECT_THROW_MSG(Format(runtime("{0:.{0:}}"), 0), format_error,
//                    "invalid format string");

//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, -1), format_error,
//                    "negative precision");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, (INT_MAX + 1u)),
//                    format_error, "number is too big");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, -1l), format_error,
//                    "negative precision");
//   if (fmt::detail::const_check(sizeof(long) > sizeof(int))) {
//     long value = INT_MAX;
//     EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, (value + 1)),
//                      format_error, "number is too big");
//   }
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, (INT_MAX + 1ul)),
//                    format_error, "number is too big");

//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, '0'), format_error,
//                    "precision is not integer");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 0, 0.0), format_error,
//                    "precision is not integer");

//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42u, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42u, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42l, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42l, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42ul, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42ul, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42ll, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42ll, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}}"), 42ull, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:.{1}f}"), 42ull, 2), format_error,
//                    "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(Format(runtime("{0:3.{1}}"), 'x', 0), format_error,
//                    "precision not allowed for this argument type");
  EXPECT_EQ("1.2", Format("{0:.{1}}", 1.2345, 2));
  EXPECT_EQ("1.2", Format("{1:.{0}}", 2, 1.2345l));

//   EXPECT_THROW_MSG(
//       Format(runtime("{0:.{1}}"), reinterpret_cast<void*>(0xcafe), 2),
//       format_error, "precision not allowed for this argument type");
//   EXPECT_THROW_MSG(
//       Format(runtime("{0:.{1}f}"), reinterpret_cast<void*>(0xcafe), 2),
//       format_error, "precision not allowed for this argument type");

  EXPECT_EQ("st", Format("{0:.{1}}", "str", 2));
}

TEST(format_test, format_bool) {
  EXPECT_EQ("true", Format("{}", true));
  EXPECT_EQ("false", Format("{}", false));
  EXPECT_EQ("1", Format("{:d}", true));
  EXPECT_EQ("true ", Format("{:5}", true));
  EXPECT_EQ("true", Format("{:s}", true));
  EXPECT_EQ("false", Format("{:s}", false));
  EXPECT_EQ("false ", Format("{:6s}", false));
}

TEST(format_test, format_short) {
  short s = 42;
  EXPECT_EQ("42", Format("{0:d}", s));
  unsigned short us = 42;
  EXPECT_EQ("42", Format("{0:d}", us));
}

template <typename T>
void check_unknown_types(const T& value, const char* types, const char*) {
  char format_str[256];
  const char* special = ".0123456789L}";
  for (int i = CHAR_MIN; i <= CHAR_MAX; ++i) {
    char c = static_cast<char>(i);
    if (std::strchr(types, c) || std::strchr(special, c) || !c) continue;
    safe_sprintf(format_str, "{0:10%c}", c);
    const char* message = "invalid type specifier";
    EXPECT_THROW_MSG(Format(runtime(format_str), value), format_error,
                     message)
        << format_str << " " << message;
  }
}

TEST(format_test, format_int) {
//   EXPECT_THROW_MSG(Format(runtime("{0:v"), 42), format_error,
//                    "invalid type specifier");
  check_unknown_types(42, "bBdoxXnLc", "integer");
  EXPECT_EQ("x", Format("{:c}", static_cast<int>('x')));
}

TEST(format_test, format_bin) {
  EXPECT_EQ("0", Format("{0:b}", 0));
  EXPECT_EQ("101010", Format("{0:b}", 42));
  EXPECT_EQ("101010", Format("{0:b}", 42u));
  EXPECT_EQ("-101010", Format("{0:b}", -42));
  EXPECT_EQ("11000000111001", Format("{0:b}", 12345));
  EXPECT_EQ("10010001101000101011001111000", Format("{0:b}", 0x12345678));
  EXPECT_EQ("10010000101010111100110111101111",
            Format("{0:b}", 0x90ABCDEF));
  EXPECT_EQ("11111111111111111111111111111111",
            Format("{0:b}", fmt::detail::max_value<uint32_t>()));
}

#if FMT_USE_INT128
constexpr auto int128_max = static_cast<__int128_t>(
    (static_cast<__uint128_t>(1) << ((__SIZEOF_INT128__ * CHAR_BIT) - 1)) - 1);
constexpr auto int128_min = -int128_max - 1;

constexpr auto uint128_max = ~static_cast<__uint128_t>(0);
#endif

TEST(format_test, format_dec) {
  EXPECT_EQ("0", Format("{0}", 0));
  EXPECT_EQ("42", Format("{0}", 42));
  EXPECT_EQ("42", Format("{0:d}", 42));
  EXPECT_EQ("42", Format("{0}", 42u));
  EXPECT_EQ("-42", Format("{0}", -42));
  EXPECT_EQ("12345", Format("{0}", 12345));
  EXPECT_EQ("67890", Format("{0}", 67890));
#if FMT_USE_INT128
  EXPECT_EQ("0", Format("{0}", static_cast<__int128_t>(0)));
  EXPECT_EQ("0", Format("{0}", static_cast<__uint128_t>(0)));
  EXPECT_EQ("9223372036854775808",
            Format("{0}", static_cast<__int128_t>(INT64_MAX) + 1));
  EXPECT_EQ("-9223372036854775809",
            Format("{0}", static_cast<__int128_t>(INT64_MIN) - 1));
  EXPECT_EQ("18446744073709551616",
            Format("{0}", static_cast<__int128_t>(UINT64_MAX) + 1));
  EXPECT_EQ("170141183460469231731687303715884105727",
            Format("{0}", int128_max));
  EXPECT_EQ("-170141183460469231731687303715884105728",
            Format("{0}", int128_min));
  EXPECT_EQ("340282366920938463463374607431768211455",
            Format("{0}", uint128_max));
#endif

  char buffer[256];
  safe_sprintf(buffer, "%d", INT_MIN);
  EXPECT_EQ(buffer, Format("{0}", INT_MIN));
  safe_sprintf(buffer, "%d", INT_MAX);
  EXPECT_EQ(buffer, Format("{0}", INT_MAX));
  safe_sprintf(buffer, "%u", UINT_MAX);
  EXPECT_EQ(buffer, Format("{0}", UINT_MAX));
  safe_sprintf(buffer, "%ld", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, Format("{0}", LONG_MIN));
  safe_sprintf(buffer, "%ld", LONG_MAX);
  EXPECT_EQ(buffer, Format("{0}", LONG_MAX));
  safe_sprintf(buffer, "%lu", ULONG_MAX);
  EXPECT_EQ(buffer, Format("{0}", ULONG_MAX));
}

TEST(format_test, format_hex) {
  EXPECT_EQ("0", Format("{0:x}", 0));
  EXPECT_EQ("42", Format("{0:x}", 0x42));
  EXPECT_EQ("42", Format("{0:x}", 0x42u));
  EXPECT_EQ("-42", Format("{0:x}", -0x42));
  EXPECT_EQ("12345678", Format("{0:x}", 0x12345678));
  EXPECT_EQ("90abcdef", Format("{0:x}", 0x90abcdef));
  EXPECT_EQ("12345678", Format("{0:X}", 0x12345678));
  EXPECT_EQ("90ABCDEF", Format("{0:X}", 0x90ABCDEF));
#if FMT_USE_INT128
  EXPECT_EQ("0", Format("{0:x}", static_cast<__int128_t>(0)));
  EXPECT_EQ("0", Format("{0:x}", static_cast<__uint128_t>(0)));
  EXPECT_EQ("8000000000000000",
            Format("{0:x}", static_cast<__int128_t>(INT64_MAX) + 1));
  EXPECT_EQ("-8000000000000001",
            Format("{0:x}", static_cast<__int128_t>(INT64_MIN) - 1));
  EXPECT_EQ("10000000000000000",
            Format("{0:x}", static_cast<__int128_t>(UINT64_MAX) + 1));
  EXPECT_EQ("7fffffffffffffffffffffffffffffff",
            Format("{0:x}", int128_max));
  EXPECT_EQ("-80000000000000000000000000000000",
            Format("{0:x}", int128_min));
  EXPECT_EQ("ffffffffffffffffffffffffffffffff",
            Format("{0:x}", uint128_max));
#endif

  char buffer[256];
  safe_sprintf(buffer, "-%x", 0 - static_cast<unsigned>(INT_MIN));
  EXPECT_EQ(buffer, Format("{0:x}", INT_MIN));
  safe_sprintf(buffer, "%x", INT_MAX);
  EXPECT_EQ(buffer, Format("{0:x}", INT_MAX));
  safe_sprintf(buffer, "%x", UINT_MAX);
  EXPECT_EQ(buffer, Format("{0:x}", UINT_MAX));
  safe_sprintf(buffer, "-%lx", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, Format("{0:x}", LONG_MIN));
  safe_sprintf(buffer, "%lx", LONG_MAX);
  EXPECT_EQ(buffer, Format("{0:x}", LONG_MAX));
  safe_sprintf(buffer, "%lx", ULONG_MAX);
  EXPECT_EQ(buffer, Format("{0:x}", ULONG_MAX));
}

TEST(format_test, format_oct) {
  EXPECT_EQ("0", Format("{0:o}", 0));
  EXPECT_EQ("42", Format("{0:o}", 042));
  EXPECT_EQ("42", Format("{0:o}", 042u));
  EXPECT_EQ("-42", Format("{0:o}", -042));
  EXPECT_EQ("12345670", Format("{0:o}", 012345670));
#if FMT_USE_INT128
  EXPECT_EQ("0", Format("{0:o}", static_cast<__int128_t>(0)));
  EXPECT_EQ("0", Format("{0:o}", static_cast<__uint128_t>(0)));
  EXPECT_EQ("1000000000000000000000",
            Format("{0:o}", static_cast<__int128_t>(INT64_MAX) + 1));
  EXPECT_EQ("-1000000000000000000001",
            Format("{0:o}", static_cast<__int128_t>(INT64_MIN) - 1));
  EXPECT_EQ("2000000000000000000000",
            Format("{0:o}", static_cast<__int128_t>(UINT64_MAX) + 1));
  EXPECT_EQ("1777777777777777777777777777777777777777777",
            Format("{0:o}", int128_max));
  EXPECT_EQ("-2000000000000000000000000000000000000000000",
            Format("{0:o}", int128_min));
  EXPECT_EQ("3777777777777777777777777777777777777777777",
            Format("{0:o}", uint128_max));
#endif

  char buffer[256];
  safe_sprintf(buffer, "-%o", 0 - static_cast<unsigned>(INT_MIN));
  EXPECT_EQ(buffer, Format("{0:o}", INT_MIN));
  safe_sprintf(buffer, "%o", INT_MAX);
  EXPECT_EQ(buffer, Format("{0:o}", INT_MAX));
  safe_sprintf(buffer, "%o", UINT_MAX);
  EXPECT_EQ(buffer, Format("{0:o}", UINT_MAX));
  safe_sprintf(buffer, "-%lo", 0 - static_cast<unsigned long>(LONG_MIN));
  EXPECT_EQ(buffer, Format("{0:o}", LONG_MIN));
  safe_sprintf(buffer, "%lo", LONG_MAX);
  EXPECT_EQ(buffer, Format("{0:o}", LONG_MAX));
  safe_sprintf(buffer, "%lo", ULONG_MAX);
  EXPECT_EQ(buffer, Format("{0:o}", ULONG_MAX));
}

TEST(format_test, format_int_locale) {
  EXPECT_EQ("1234", Format("{:L}", 1234));
}

TEST(format_test, format_float) {
  EXPECT_EQ("0", Format("{}", 0.0f));
  EXPECT_EQ("392.500000", Format("{0:f}", 392.5f));
}

TEST(format_test, format_double) {
  EXPECT_EQ("0", Format("{}", 0.0));
  check_unknown_types(1.2, "eEfFgGaAnL%", "double");
  EXPECT_EQ("0", Format("{:}", 0.0));
  EXPECT_EQ("0.000000", Format("{:f}", 0.0));
  EXPECT_EQ("0", Format("{:g}", 0.0));
  EXPECT_EQ("392.65", Format("{:}", 392.65));
  EXPECT_EQ("392.65", Format("{:g}", 392.65));
  EXPECT_EQ("392.65", Format("{:G}", 392.65));
  EXPECT_EQ("4.9014e+06", Format("{:g}", 4.9014e6));
  EXPECT_EQ("392.650000", Format("{:f}", 392.65));
  EXPECT_EQ("392.650000", Format("{:F}", 392.65));
  EXPECT_EQ("42", Format("{:L}", 42.0));
  EXPECT_EQ("    0x1.0cccccccccccdp+2", Format("{:24a}", 4.2));
  EXPECT_EQ("0x1.0cccccccccccdp+2    ", Format("{:<24a}", 4.2));
  char buffer[256];
  safe_sprintf(buffer, "%e", 392.65);
  EXPECT_EQ(buffer, Format("{0:e}", 392.65));
  safe_sprintf(buffer, "%E", 392.65);
  EXPECT_EQ(buffer, Format("{0:E}", 392.65));
  EXPECT_EQ("+0000392.6", Format("{0:+010.4g}", 392.65));
  safe_sprintf(buffer, "%a", -42.0);
  EXPECT_EQ(buffer, Format("{:a}", -42.0));
  safe_sprintf(buffer, "%A", -42.0);
  EXPECT_EQ(buffer, Format("{:A}", -42.0));
  EXPECT_EQ("9223372036854775808.000000",
            Format("{:f}", 9223372036854775807.0));
}

TEST(format_test, precision_rounding) {
  EXPECT_EQ("0", Format("{:.0f}", 0.0));
  EXPECT_EQ("0", Format("{:.0f}", 0.01));
  EXPECT_EQ("0", Format("{:.0f}", 0.1));
  EXPECT_EQ("0.000", Format("{:.3f}", 0.00049));
  EXPECT_EQ("0.001", Format("{:.3f}", 0.0005));
  EXPECT_EQ("0.001", Format("{:.3f}", 0.00149));
  EXPECT_EQ("0.002", Format("{:.3f}", 0.0015));
  EXPECT_EQ("1.000", Format("{:.3f}", 0.9999));
  EXPECT_EQ("0.00123", Format("{:.3}", 0.00123));
  EXPECT_EQ("0.1", Format("{:.16g}", 0.1));
  EXPECT_EQ("1", Format("{:.0}", 1.0));
  EXPECT_EQ("225.51575035152063720",
            Format("{:.17f}", 225.51575035152064));
  EXPECT_EQ("-761519619559038.2", Format("{:.1f}", -761519619559038.2));
  EXPECT_EQ("1.9156918820264798e-56",
            Format("{}", 1.9156918820264798e-56));
  EXPECT_EQ("0.0000", Format("{:.4f}", 7.2809479766055470e-15));

  // Trigger a rounding error in Grisu by a specially chosen number.
  EXPECT_EQ("3788512123356.985352", Format("{:f}", 3788512123356.985352));
}

TEST(format_test, prettify_float) {
  EXPECT_EQ("0.0001", Format("{}", 1e-4));
  EXPECT_EQ("1e-05", Format("{}", 1e-5));
  EXPECT_EQ("1000000000000000", Format("{}", 1e15));
  EXPECT_EQ("1e+16", Format("{}", 1e16));
  EXPECT_EQ("9.999e-05", Format("{}", 9.999e-5));
  EXPECT_EQ("10000000000", Format("{}", 1e10));
  EXPECT_EQ("100000000000", Format("{}", 1e11));
  EXPECT_EQ("12340000000", Format("{}", 1234e7));
  EXPECT_EQ("12.34", Format("{}", 1234e-2));
  EXPECT_EQ("0.001234", Format("{}", 1234e-6));
  EXPECT_EQ("0.1", Format("{}", 0.1f));
  EXPECT_EQ("0.10000000149011612", Format("{}", double(0.1f)));
  EXPECT_EQ("1.3563156e-19", Format("{}", 1.35631564e-19f));
}

TEST(format_test, format_nan) {
  double nan = std::numeric_limits<double>::quiet_NaN();
  EXPECT_EQ("nan", Format("{}", nan));
  EXPECT_EQ("+nan", Format("{:+}", nan));
  EXPECT_EQ("  +nan", Format("{:+06}", nan));
  EXPECT_EQ("+nan  ", Format("{:<+06}", nan));
  EXPECT_EQ(" +nan ", Format("{:^+06}", nan));
  EXPECT_EQ("  +nan", Format("{:>+06}", nan));
  if (std::signbit(-nan)) {
    EXPECT_EQ("-nan", Format("{}", -nan));
    EXPECT_EQ("  -nan", Format("{:+06}", -nan));
  } else {
    fmt::print("Warning: compiler doesn't handle negative NaN correctly");
  }
  EXPECT_EQ(" nan", Format("{: }", nan));
  EXPECT_EQ("NAN", Format("{:F}", nan));
  EXPECT_EQ("nan    ", Format("{:<7}", nan));
  EXPECT_EQ("  nan  ", Format("{:^7}", nan));
  EXPECT_EQ("    nan", Format("{:>7}", nan));
}

TEST(format_test, format_infinity) {
  double inf = std::numeric_limits<double>::infinity();
  EXPECT_EQ("inf", Format("{}", inf));
  EXPECT_EQ("+inf", Format("{:+}", inf));
  EXPECT_EQ("-inf", Format("{}", -inf));
  EXPECT_EQ("  +inf", Format("{:+06}", inf));
  EXPECT_EQ("  -inf", Format("{:+06}", -inf));
  EXPECT_EQ("+inf  ", Format("{:<+06}", inf));
  EXPECT_EQ(" +inf ", Format("{:^+06}", inf));
  EXPECT_EQ("  +inf", Format("{:>+06}", inf));
  EXPECT_EQ(" inf", Format("{: }", inf));
  EXPECT_EQ("INF", Format("{:F}", inf));
  EXPECT_EQ("inf    ", Format("{:<7}", inf));
  EXPECT_EQ("  inf  ", Format("{:^7}", inf));
  EXPECT_EQ("    inf", Format("{:>7}", inf));
}

TEST(format_test, format_long_double) {
  EXPECT_EQ("0", Format("{0:}", 0.0l));
  EXPECT_EQ("0.000000", Format("{0:f}", 0.0l));
  EXPECT_EQ("392.65", Format("{0:}", 392.65l));
  EXPECT_EQ("392.65", Format("{0:g}", 392.65l));
  EXPECT_EQ("392.65", Format("{0:G}", 392.65l));
  EXPECT_EQ("392.650000", Format("{0:f}", 392.65l));
  EXPECT_EQ("392.650000", Format("{0:F}", 392.65l));
  char buffer[256];
  safe_sprintf(buffer, "%Le", 392.65l);
  EXPECT_EQ(buffer, Format("{0:e}", 392.65l));
  EXPECT_EQ("+0000392.6", Format("{0:+010.4g}", 392.64l));
  safe_sprintf(buffer, "%La", 3.31l);
  EXPECT_EQ(buffer, Format("{:a}", 3.31l));
}

TEST(format_test, format_char) {
  const char types[] = "cbBdoxX";
  check_unknown_types('a', types, "char");
  EXPECT_EQ("a", Format("{0}", 'a'));
  EXPECT_EQ("z", Format("{0:c}", 'z'));
  int n = 'x';
  for (const char* type = types + 1; *type; ++type) {
    std::string format_str = Format("{{:{}}}", *type);
    EXPECT_EQ(Format(runtime(format_str), n),
              Format(runtime(format_str), 'x'))
        << format_str;
  }
  EXPECT_EQ(Format("{:02X}", n), Format("{:02X}", 'x'));
}

TEST(format_test, format_volatile_char) {
  volatile char c = 'x';
  EXPECT_EQ("x", Format("{}", c));
}

TEST(format_test, format_unsigned_char) {
  EXPECT_EQ("42", Format("{}", static_cast<unsigned char>(42)));
  EXPECT_EQ("42", Format("{}", static_cast<uint8_t>(42)));
}

TEST(format_test, format_cstring) {
  check_unknown_types("test", "sp", "string");
  EXPECT_EQ("test", Format("{0}", "test"));
  EXPECT_EQ("test", Format("{0:s}", "test"));
  char nonconst[] = "nonconst";
  EXPECT_EQ("nonconst", Format("{0}", nonconst));
//   EXPECT_THROW_MSG(
//       Format(runtime("{0}"), static_cast<const char*>(nullptr)),
//       format_error, "string pointer is null");
}

TEST(format_test, format_schar_string) {
  signed char str[] = "test";
  EXPECT_EQ("test", Format("{0:s}", str));
  const signed char* const_str = str;
  EXPECT_EQ("test", Format("{0:s}", const_str));
}

TEST(format_test, format_uchar_string) {
  unsigned char str[] = "test";
  EXPECT_EQ("test", Format("{0:s}", str));
  const unsigned char* const_str = str;
  EXPECT_EQ("test", Format("{0:s}", const_str));
  unsigned char* ptr = str;
  EXPECT_EQ("test", Format("{0:s}", ptr));
}

void function_pointer_test(int, double, std::string) {}

TEST(format_test, format_pointer) {
  check_unknown_types(reinterpret_cast<void*>(0x1234), "p", "pointer");
  EXPECT_EQ("0x0", Format("{0}", static_cast<void*>(nullptr)));
  EXPECT_EQ("0x1234", Format("{0}", reinterpret_cast<void*>(0x1234)));
  EXPECT_EQ("0x1234", Format("{0:p}", reinterpret_cast<void*>(0x1234)));
  EXPECT_EQ("0x" + std::string(sizeof(void*) * CHAR_BIT / 4, 'f'),
            Format("{0}", reinterpret_cast<void*>(~uintptr_t())));
  EXPECT_EQ("0x1234",
            Format("{}", fmt::ptr(reinterpret_cast<int*>(0x1234))));
  std::unique_ptr<int> up(new int(1));
  EXPECT_EQ(Format("{}", fmt::ptr(up.get())),
            Format("{}", fmt::ptr(up)));
  std::shared_ptr<int> sp(new int(1));
  EXPECT_EQ(Format("{}", fmt::ptr(sp.get())),
            Format("{}", fmt::ptr(sp)));
  EXPECT_EQ(Format("{}", fmt::detail::bit_cast<const void*>(
                                  &function_pointer_test)),
            Format("{}", fmt::ptr(function_pointer_test)));
  EXPECT_EQ("0x0", Format("{}", nullptr));
}

TEST(format_test, format_string) {
  EXPECT_EQ("test", Format("{0}", std::string("test")));
//   EXPECT_THROW(Format(fmt::runtime("{:x}"), std::string("test")),
//                fmt::format_error);
}

TEST(format_test, format_string_view) {
  EXPECT_EQ("test", Format("{}", string_view("test")));
  EXPECT_EQ("", Format("{}", string_view()));
}


#ifdef FMT_USE_STRING_VIEW
struct string_viewable {};

FMT_BEGIN_NAMESPACE
template <> struct formatter<string_viewable> : formatter<std::string_view> {
  auto format(string_viewable, format_context& ctx) -> decltype(ctx.out()) {
    return formatter<std::string_view>::format("foo", ctx);
  }
};
FMT_END_NAMESPACE

TEST(format_test, format_std_string_view) {
  EXPECT_EQ("test", Format("{}", std::string_view("test")));
  EXPECT_EQ("foo", Format("{}", string_viewable()));
}

struct explicitly_convertible_to_std_string_view {
  explicit operator std::string_view() const { return "foo"; }
};

template <>
struct fmt::formatter<explicitly_convertible_to_std_string_view>
    : formatter<std::string_view> {
  auto format(explicitly_convertible_to_std_string_view v, format_context& ctx)
      -> decltype(ctx.out()) {
    return format_to(ctx.out(), "'{}'", std::string_view(v));
  }
};

TEST(format_test, format_explicitly_convertible_to_std_string_view) {
  EXPECT_EQ("'foo'",
            Format("{}", explicitly_convertible_to_std_string_view()));
}
#endif

struct converible_to_anything {
  template <typename T> operator T() const { return T(); }
};

FMT_BEGIN_NAMESPACE
template <> struct formatter<converible_to_anything> {
  FMT_CONSTEXPR auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  auto format(converible_to_anything, format_context& ctx)
      -> decltype(ctx.out()) {
    return format_to(ctx.out(), "foo");
  }
};
FMT_END_NAMESPACE

TEST(format_test, format_convertible_to_anything) {
  EXPECT_EQ("foo", Format("{}", converible_to_anything()));
}

class Answer {};

FMT_BEGIN_NAMESPACE
template <> struct formatter<date> {
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin();
    if (it != ctx.end() && *it == 'd') ++it;
    return it;
  }

  auto format(const date& d, format_context& ctx) -> decltype(ctx.out()) {
    format_to(ctx.out(), "{}-{}-{}", d.year(), d.month(), d.day());
    return ctx.out();
  }
};

template <> struct formatter<Answer> : formatter<int> {
  template <typename FormatContext>
  auto format(Answer, FormatContext& ctx) -> decltype(ctx.out()) {
    return formatter<int>::format(42, ctx);
  }
};
FMT_END_NAMESPACE

TEST(format_test, format_custom) {
//   EXPECT_THROW_MSG(Format(runtime("{:s}"), date(2012, 12, 9)),
//                    format_error, "unknown format specifier");
  EXPECT_EQ("42", Format("{0}", Answer()));
  EXPECT_EQ("0042", Format("{:04}", Answer()));
}

TEST(format_test, format_to_custom) {
  char buf[10] = {};
  auto end =
      &*FormatTo(fmt::detail::make_checked(buf, 10), "{}", Answer());
  EXPECT_EQ(end, buf + 2);
  EXPECT_STREQ(buf, "42");
}

TEST(format_test, format_string_from_speed_test) {
  EXPECT_EQ("1.2340000000:0042:+3.13:str:0x3e8:X:%",
            Format("{0:0.10f}:{1:04}:{2:+g}:{3}:{4}:{5}:%", 1.234, 42,
                        3.13, "str", reinterpret_cast<void*>(1000), 'X'));
}

TEST(format_test, format_examples) {
  std::string message = Format("The answer is {}", 42);
  EXPECT_EQ("The answer is 42", message);

  EXPECT_EQ("42", Format("{}", 42));

  memory_buffer out;
  format_to(std::back_inserter(out), "The answer is {}.", 42);
  EXPECT_EQ("The answer is 42.", to_string(out));

  const char* filename = "nonexistent";
  FILE* ftest = safe_fopen(filename, "r");
  if (ftest) fclose(ftest);
  int error_code = errno;
  EXPECT_TRUE(ftest == nullptr);
  EXPECT_SYSTEM_ERROR(
      {
        FILE* f = safe_fopen(filename, "r");
        if (!f)
          throw fmt::system_error(errno, "Cannot open file '{}'", filename);
        fclose(f);
      },
      error_code, "Cannot open file 'nonexistent'");

  EXPECT_EQ("First, thou shalt count to three",
            Format("First, thou shalt count to {0}", "three"));
  EXPECT_EQ("Bring me a shrubbery", Format("Bring me a {}", "shrubbery"));
  EXPECT_EQ("From 1 to 3", Format("From {} to {}", 1, 3));

  char buffer[256];
  safe_sprintf(buffer, "%03.2f", -1.2);
  EXPECT_EQ(buffer, Format("{:03.2f}", -1.2));

  EXPECT_EQ("a, b, c", Format("{0}, {1}, {2}", 'a', 'b', 'c'));
  EXPECT_EQ("a, b, c", Format("{}, {}, {}", 'a', 'b', 'c'));
  EXPECT_EQ("c, b, a", Format("{2}, {1}, {0}", 'a', 'b', 'c'));
  EXPECT_EQ("abracadabra", Format("{0}{1}{0}", "abra", "cad"));

  EXPECT_EQ("left aligned                  ",
            Format("{:<30}", "left aligned"));
  EXPECT_EQ("                 right aligned",
            Format("{:>30}", "right aligned"));
  EXPECT_EQ("           centered           ",
            Format("{:^30}", "centered"));
  EXPECT_EQ("***********centered***********",
            Format("{:*^30}", "centered"));

  EXPECT_EQ("+3.140000; -3.140000", Format("{:+f}; {:+f}", 3.14, -3.14));
  EXPECT_EQ(" 3.140000; -3.140000", Format("{: f}; {: f}", 3.14, -3.14));
  EXPECT_EQ("3.140000; -3.140000", Format("{:-f}; {:-f}", 3.14, -3.14));

  EXPECT_EQ("int: 42;  hex: 2a;  oct: 52",
            Format("int: {0:d};  hex: {0:x};  oct: {0:o}", 42));
  EXPECT_EQ("int: 42;  hex: 0x2a;  oct: 052",
            Format("int: {0:d};  hex: {0:#x};  oct: {0:#o}", 42));

  EXPECT_EQ("The answer is 42", Format("The answer is {}", 42));
//   EXPECT_THROW_MSG(Format(runtime("The answer is {:d}"), "forty-two"),
//                    format_error, "invalid type specifier");

//   EXPECT_WRITE(
//       stdout, fmt::print("{}", std::numeric_limits<double>::infinity()), "inf");
}

TEST(format_test, print) {
//   EXPECT_WRITE(stdout, fmt::print("Don't {}!", "panic"), "Don't panic!");
//   EXPECT_WRITE(stderr, fmt::print(stderr, "Don't {}!", "panic"),
//                "Don't panic!");
}

TEST(format_test, variadic) {
  EXPECT_EQ("abc1", Format("{}c{}", "ab", 1));
}

TEST(format_test, dynamic) {
  using ctx = fmt::format_context;
  auto args = std::vector<fmt::basic_format_arg<ctx>>();
  args.emplace_back(fmt::detail::make_arg<ctx>(42));
  args.emplace_back(fmt::detail::make_arg<ctx>("abc1"));
  args.emplace_back(fmt::detail::make_arg<ctx>(1.5f));

  std::string result = fmt::vformat(
      "{} and {} and {}",
      fmt::format_args(args.data(), static_cast<int>(args.size())));

  EXPECT_EQ("42 and abc1 and 1.5", result);
}

TEST(format_test, bytes) {
  auto s = Format("{:10}", fmt::bytes("ёжик"));
  EXPECT_EQ("ёжик  ", s);
  EXPECT_EQ(10, s.size());
}

TEST(format_test, group_digits_view) {
  EXPECT_EQ(Format("{}", fmt::group_digits(10000000)), "10,000,000");
  EXPECT_EQ(Format("{:8}", fmt::group_digits(1000)), "   1,000");
}

enum test_enum { foo, bar };

TEST(format_test, join) {
  using fmt::join;
  int v1[3] = {1, 2, 3};
  auto v2 = std::vector<float>();
  v2.push_back(1.2f);
  v2.push_back(3.4f);
  void* v3[2] = {&v1[0], &v1[1]};

  EXPECT_EQ("(1, 2, 3)", Format("({})", join(v1, v1 + 3, ", ")));
  EXPECT_EQ("(1)", Format("({})", join(v1, v1 + 1, ", ")));
  EXPECT_EQ("()", Format("({})", join(v1, v1, ", ")));
  EXPECT_EQ("(001, 002, 003)", Format("({:03})", join(v1, v1 + 3, ", ")));
  EXPECT_EQ("(+01.20, +03.40)",
            Format("({:+06.2f})", join(v2.begin(), v2.end(), ", ")));

  EXPECT_EQ("1, 2, 3", Format("{0:{1}}", join(v1, v1 + 3, ", "), 1));

  EXPECT_EQ(Format("{}, {}", v3[0], v3[1]),
            Format("{}", join(v3, v3 + 2, ", ")));

  EXPECT_EQ("(1, 2, 3)", Format("({})", join(v1, ", ")));
  EXPECT_EQ("(+01.20, +03.40)", Format("({:+06.2f})", join(v2, ", ")));

  auto v4 = std::vector<test_enum>{foo, bar, foo};
  EXPECT_EQ("0 1 0", Format("{}", join(v4, " ")));
}

#ifdef __cpp_lib_byte
TEST(format_test, join_bytes) {
  auto v = std::vector<std::byte>{std::byte(1), std::byte(2), std::byte(3)};
  EXPECT_EQ("1, 2, 3", Format("{}", fmt::join(v, ", ")));
}
#endif

std::string vformat_message(int id, const char* format, fmt::format_args args) {
  auto buffer = fmt::memory_buffer();
  format_to(fmt::appender(buffer), "[{}] ", id);
  vformat_to(fmt::appender(buffer), format, args);
  return to_string(buffer);
}

template <typename... Args>
std::string format_message(int id, const char* format, const Args&... args) {
  auto va = fmt::make_format_args(args...);
  return vformat_message(id, format, va);
}

TEST(format_test, format_message_example) {
  EXPECT_EQ("[42] something happened",
            format_message(42, "{} happened", "something"));
}

template <typename... Args>
void print_error(const char* file, int line, const char* format,
                 const Args&... args) {
  fmt::print("{}: {}: ", file, line);
  fmt::print(format, args...);
}

TEST(format_test, unpacked_args) {
  EXPECT_EQ("0123456789abcdefg",
            Format("{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}", 0, 1, 2, 3, 4, 5,
                        6, 7, 8, 9, 'a', 'b', 'c', 'd', 'e', 'f', 'g'));
}

struct string_like {};
fmt::string_view to_string_view(string_like) { return "foo"; }

constexpr char with_null[3] = {'{', '}', '\0'};
constexpr char no_null[2] = {'{', '}'};
static FMT_CONSTEXPR_DECL const char static_with_null[3] = {'{', '}', '\0'};
static FMT_CONSTEXPR_DECL const char static_no_null[2] = {'{', '}'};

TEST(format_test, compile_time_string) {
  EXPECT_EQ("foo", Format(FMT_STRING("foo")));
  EXPECT_EQ("42", Format(FMT_STRING("{}"), 42));
  EXPECT_EQ("foo", Format(FMT_STRING("{}"), string_like()));

#if FMT_USE_NONTYPE_TEMPLATE_PARAMETERS
  using namespace fmt::literals;
  EXPECT_EQ("foobar", Format(FMT_STRING("{foo}{bar}"), "bar"_a = "bar",
                                  "foo"_a = "foo"));
  EXPECT_EQ("", Format(FMT_STRING("")));
  EXPECT_EQ("", Format(FMT_STRING(""), "arg"_a = 42));
#endif

  (void)static_with_null;
  (void)static_no_null;
#ifndef _MSC_VER
  EXPECT_EQ("42", Format(FMT_STRING(static_with_null), 42));
  EXPECT_EQ("42", Format(FMT_STRING(static_no_null), 42));
#endif

  (void)with_null;
  (void)no_null;
#if __cplusplus >= 201703L
  EXPECT_EQ("42", Format(FMT_STRING(with_null), 42));
  EXPECT_EQ("42", Format(FMT_STRING(no_null), 42));
#endif
#if defined(FMT_USE_STRING_VIEW) && __cplusplus >= 201703L
  EXPECT_EQ("42", Format(FMT_STRING(std::string_view("{}")), 42));
#endif
}

TEST(format_test, custom_format_compile_time_string) {
  EXPECT_EQ("42", Format(FMT_STRING("{}"), Answer()));
  auto answer = Answer();
  EXPECT_EQ("42", Format(FMT_STRING("{}"), answer));
  char buf[10] = {};
  FormatTo(buf, FMT_STRING("{}"), answer);
  const Answer const_answer = Answer();
  EXPECT_EQ("42", Format(FMT_STRING("{}"), const_answer));
}

#if FMT_USE_USER_DEFINED_LITERALS
// Passing user-defined literals directly to EXPECT_EQ causes problems
// with macro argument stringification (#) on some versions of GCC.
// Workaround: Assing the UDL result to a variable before the macro.

using namespace fmt::literals;

TEST(format_test, format_udl) {
  EXPECT_EQ("{}c{}"_format("ab", 1), Format("{}c{}", "ab", 1));
  EXPECT_EQ("foo"_format(), "foo");
  EXPECT_EQ("{0:10}"_format(42), "        42");
  EXPECT_EQ("{}"_format(date(2015, 10, 21)), "2015-10-21");
}

TEST(format_test, named_arg_udl) {
  auto udl_a = Format("{first}{second}{first}{third}", "first"_a = "abra",
                           "second"_a = "cad", "third"_a = 99);
  EXPECT_EQ(
      Format("{first}{second}{first}{third}", fmt::arg("first", "abra"),
                  fmt::arg("second", "cad"), fmt::arg("third", 99)),
      udl_a);
}
#endif  // FMT_USE_USER_DEFINED_LITERALS

TEST(format_test, enum) { EXPECT_EQ("0", Format("{}", foo)); }

TEST(format_test, formatter_not_specialized) {
  static_assert(!fmt::has_formatter<fmt::formatter<test_enum>,
                                    fmt::format_context>::value,
                "");
}

#if FMT_HAS_FEATURE(cxx_strong_enums)
enum big_enum : unsigned long long { big_enum_value = 5000000000ULL };

TEST(format_test, strong_enum) {
  EXPECT_EQ("5000000000", Format("{}", big_enum_value));
}
#endif

TEST(format_test, non_null_terminated_format_string) {
  EXPECT_EQ("42", Format(string_view("{}foo", 2), 42));
}

struct variant {
  enum { int_type, string_type } type;
  explicit variant(int) : type(int_type) {}
  explicit variant(const char*) : type(string_type) {}
};

FMT_BEGIN_NAMESPACE
template <> struct formatter<variant> : dynamic_formatter<> {
  auto format(variant value, format_context& ctx) -> decltype(ctx.out()) {
    if (value.type == variant::int_type)
      return dynamic_formatter<>::format(42, ctx);
    return dynamic_formatter<>::format("foo", ctx);
  }
};
FMT_END_NAMESPACE

TEST(format_test, dynamic_formatter) {
  auto num = variant(42);
  auto str = variant("foo");
  EXPECT_EQ("42", Format("{:d}", num));
  EXPECT_EQ("foo", Format("{:s}", str));
  EXPECT_EQ(" 42 foo ", Format("{:{}} {:{}}", num, 3, str, 4));
//   EXPECT_THROW_MSG(Format(runtime("{0:{}}"), num), format_error,
//                    "cannot switch from manual to automatic argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{:{0}}"), num), format_error,
//                    "cannot switch from automatic to manual argument indexing");
//   EXPECT_THROW_MSG(Format(runtime("{:+}"), str), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{:-}"), str), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{: }"), str), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{:#}"), str), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{:0}"), str), format_error,
//                    "format specifier requires numeric argument");
//   EXPECT_THROW_MSG(Format(runtime("{:.2}"), num), format_error,
//                    "precision not allowed for this argument type");
}

namespace adl_test {
namespace fmt {
namespace detail {
struct foo {};
template <typename, typename OutputIt> void write(OutputIt, foo) = delete;
}  // namespace detail
}  // namespace fmt
}  // namespace adl_test

FMT_BEGIN_NAMESPACE
template <>
struct formatter<adl_test::fmt::detail::foo> : formatter<std::string> {
  template <typename FormatContext>
  auto format(adl_test::fmt::detail::foo, FormatContext& ctx)
      -> decltype(ctx.out()) {
    return formatter<std::string>::format("foo", ctx);
  }
};
FMT_END_NAMESPACE

TEST(format_test, to_string) {
  EXPECT_EQ("42", fmt::to_string(42));
  EXPECT_EQ("0x1234", fmt::to_string(reinterpret_cast<void*>(0x1234)));
  EXPECT_EQ("foo", fmt::to_string(adl_test::fmt::detail::foo()));

  enum test_enum2 : unsigned char { test_value };
  EXPECT_EQ("0", fmt::to_string(test_value));
}

TEST(format_test, output_iterators) {
  std::list<char> out;
  FormatTo(std::back_inserter(out), "{}", 42);
  EXPECT_EQ("42", std::string(out.begin(), out.end()));
  std::stringstream s;
  FormatTo(std::ostream_iterator<char>(s), "{}", 42);
  EXPECT_EQ("42", s.str());
}

TEST(format_test, formatted_size) {
  EXPECT_EQ(2u, fmt::formatted_size("{}", 42));
}

TEST(format_test, format_to_no_args) {
  std::string s;
  FormatTo(std::back_inserter(s), "test");
  EXPECT_EQ("test", s);
}

TEST(format_test, format_to) {
  std::string s;
  FormatTo(std::back_inserter(s), "part{0}", 1);
  EXPECT_EQ("part1", s);
  FormatTo(std::back_inserter(s), "part{0}", 2);
  EXPECT_EQ("part1part2", s);
}

TEST(format_test, format_to_memory_buffer) {
  auto buf = fmt::basic_memory_buffer<char, 100>();
  FormatTo(fmt::appender(buf), "{}", "foo");
  EXPECT_EQ("foo", to_string(buf));
}

TEST(format_test, format_to_vector) {
  std::vector<char> v;
  FormatTo(std::back_inserter(v), "{}", "foo");
  EXPECT_EQ(string_view(v.data(), v.size()), "foo");
}

struct nongrowing_container {
  using value_type = char;
  void push_back(char) { throw std::runtime_error("can't take it any more"); }
};

TEST(format_test, format_to_propagates_exceptions) {
  auto c = nongrowing_container();
  EXPECT_THROW(FormatTo(std::back_inserter(c), "{}", 42),
               std::runtime_error);
}

TEST(format_test, format_to_n) {
  char buffer[4];
  buffer[3] = 'x';
  auto result = FormatToN(buffer, 3, "{}", 12345);
  EXPECT_EQ(5u, result.size);
  EXPECT_EQ(buffer + 3, result.out);
  EXPECT_EQ("123x", fmt::string_view(buffer, 4));

  result = FormatToN(buffer, 3, "{:s}", "foobar");
  EXPECT_EQ(6u, result.size);
  EXPECT_EQ(buffer + 3, result.out);
  EXPECT_EQ("foox", fmt::string_view(buffer, 4));

  buffer[0] = 'x';
  buffer[1] = 'x';
  buffer[2] = 'x';
  result = FormatToN(buffer, 3, "{}", 'A');
  EXPECT_EQ(1u, result.size);
  EXPECT_EQ(buffer + 1, result.out);
  EXPECT_EQ("Axxx", fmt::string_view(buffer, 4));

  result = FormatToN(buffer, 3, "{}{} ", 'B', 'C');
  EXPECT_EQ(3u, result.size);
  EXPECT_EQ(buffer + 3, result.out);
  EXPECT_EQ("BC x", fmt::string_view(buffer, 4));

  result = FormatToN(buffer, 4, "{}", "ABCDE");
  EXPECT_EQ(5u, result.size);
  EXPECT_EQ("ABCD", fmt::string_view(buffer, 4));

  buffer[3] = 'x';
  result = FormatToN(buffer, 3, "{}", std::string(1000, '*'));
  EXPECT_EQ(1000u, result.size);
  EXPECT_EQ("***x", fmt::string_view(buffer, 4));
}
