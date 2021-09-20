
#include <Base/String.h>
#include "gtest/gtest.h"
#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <list>
#include <random>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
using namespace std;

// using namespace Sparrow;
#include <memory>
#include <iostream>

using TestString     = Sparrow::TString<char>;
using TestWideString = Sparrow::TString<wchar_t>;

template <class String>
void ReBuild(String &test)
{
  test.String::~String();

  new (&test) String();
}

TEST(String, Rebuild)
{
  std::string    std_string("std string");
  TestString     sparrow_string("sparrow string ");
  TestWideString sparrow_wide_string(L"sparrow wide string");

  // std::cout<<std_string<<'\n';
  // std::cout<<sparrow_string<<'\n';
  // std::cout<<sparrow_wide_string<<'\n';

  ReBuild(std_string);
  ReBuild(sparrow_string);
  ReBuild(sparrow_wide_string);

  EXPECT_EQ(std_string.data(), std::string());
  EXPECT_EQ(sparrow_string.Data(), TestString());
  EXPECT_EQ(sparrow_wide_string.Data(), TestWideString());

  std::cout << std_string << '\n';
  std::cout << sparrow_string << '\n';
}
/*

TEST(String, Getline)
{
  const char *s1 = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras accumsan \n\
elit ut urna consectetur in sagittis mi auctor. Nulla facilisi. In nec \n\
dolor leo, vitae imperdiet neque. Donec ut erat mauris, a faucibus \n\
elit. Integer consectetur gravida augue, sit amet mattis mauris auctor \n\
sed. Morbi congue libero eu nunc sodales adipiscing. In lectus nunc, \n\
vulputate a fringilla at, venenatis quis justo. Proin eu velit \n\
nibh. Maecenas vitae tellus eros. Pellentesque habitant morbi \n\
tristique senectus et netus et malesuada fames ac turpis \n\
egestas. Vivamus faucibus feugiat consequat. Donec fermentum neque sit \n\
amet ligula suscipit porta. Phasellus facilisis felis in purus luctus \n\
quis posuere leo tempor. Nam nunc purus, luctus a pharetra ut, \n\
placerat at dui. Donec imperdiet, diam quis convallis pulvinar, dui \n\
est commodo lorem, ut tincidunt diam nibh et nibh. Maecenas nec velit \n\
massa, ut accumsan magna. Donec imperdiet tempor nisi et \n\
laoreet. Phasellus lectus quam, ultricies ut tincidunt in, dignissim \n\
id eros. Mauris vulputate tortor nec neque pellentesque sagittis quis \n\
sed nisl. In diam lacus, lobortis ut posuere nec, ornare id quam.";

  vector<TestString> v;
  boost::split(v, s1, boost::is_any_of("\n"));
  // std::cout << v;
  {
    //  istringstream input(s1);
    // TestString     line();
    // for (auto i: v)
    // {
    //  std::cout << i<<'\n';
    // Sparrow::getline(input, line);
    //   getline()
    //  EXPECT_TRUE(!getline(input, line).fail());
    // EXPECT_EQ(line, i);
    //}
  }
}
*/

TEST(String, MoveConstructor)
{
  // Move constructor. Make sure we allocate a large string, so the
  // small string optimization doesn't kick in.
  auto       size = 200;
  TestString s(size, 'a');
  TestString test = std::move(s);
  EXPECT_TRUE(s.Empty());
  EXPECT_EQ(size, test.Size());
}

TEST(String, MoveAssign)
{
  // Move constructor. Make sure we allocate a large string, so the
  // small string optimization doesn't kick in.
  auto       size = 400;
  TestString s(size, 'a');
  TestString test;
  test = std::move(s);
  EXPECT_TRUE(s.Empty());
  EXPECT_EQ(size, test.Size());
}

TEST(String, testMoveOperatorPlusLhs)
{
  // Make sure we allocate a large string, so the
  // small string optimization doesn't kick in.
  auto       size1 = 500;
  auto       size2 = 60;
  TestString s1(size1, 'a');
  TestString s2(size2, 'b');
  TestString test;
  test = std::move(s1) + s2;
  EXPECT_TRUE(s1.Empty());
  EXPECT_EQ(size1 + size2, test.Size());
}

TEST(String, testMoveOperatorPlusRhs)
{
  // Make sure we allocate a large string, so the
  // small string optimization doesn't kick in.
  auto       size1 = 90;
  auto       size2 = 70;
  TestString s1(size1, 'a');
  TestString s2(size2, 'b');
  TestString test;
  test = s1 + std::move(s2);
  EXPECT_EQ(size1 + size2, test.Size());
}

TEST(String, findWithNpos)
{
  TestString fbstr("localhost:80");
  EXPECT_EQ(TestString::npos, fbstr.Find(":", TestString::npos));
}

/*
TEST(String, testHash) {
  TestString a;
  TestString b;
  a.push_back(0);
  a.push_back(1);
  b.push_back(0);
  b.push_back(2);
  //std::hash<TestString> hashfunc;
 // EXPECT_NE(hashfunc(a), hashfunc(b));
}
*/

TEST(String, testFrontBack)
{
  TestString str("hello");
  EXPECT_EQ(str.Front(), 'h');
  EXPECT_EQ(str.Back(), 'o');
  str.Front() = 'H';
  EXPECT_EQ(str.Front(), 'H');
  str.Back() = 'O';
  EXPECT_EQ(str.Back(), 'O');
  EXPECT_EQ(str, "HellO");
}

TEST(String, noexcept)
{
  EXPECT_TRUE(noexcept(TestString()));
  TestString x;
  EXPECT_FALSE(noexcept(TestString(x)));
  EXPECT_TRUE(noexcept(TestString(std::move(x))));
  TestString y;
  EXPECT_FALSE(noexcept(y = x));
  EXPECT_TRUE(noexcept(y = std::move(x)));
}

TEST(String, iomanip)
{
  stringstream ss;
  TestString   fbstr("Hello");

  ss << setw(6) << fbstr;
  EXPECT_EQ(ss.str(), " Hello");
  ss.str("");

  ss << left << setw(6) << fbstr;
  EXPECT_EQ(ss.str(), "Hello ");
  ss.str("");

  ss << right << setw(6) << fbstr;
  EXPECT_EQ(ss.str(), " Hello");
  ss.str("");

  ss << setw(4) << fbstr;
  EXPECT_EQ(ss.str(), "Hello");
  ss.str("");

  ss << setfill('^') << setw(6) << fbstr;
  EXPECT_EQ(ss.str(), "^Hello");
  ss.str("");
}

TEST(String, rvalueIterators)
{
  // you cannot take &* of a move-iterator, so use that for testing
  TestString s = "base";
  TestString r = "hello";
  r.Replace(r.begin(), r.end(), make_move_iterator(s.begin()), make_move_iterator(s.end()));
  EXPECT_EQ("base", r);

  // The following test is probably not required by the standard.
  // i.e. this could be in the realm of undefined behavior.
  TestString b   = "123abcXYZ";
  auto       ait = b.begin() + 3;
  auto       Xit = b.begin() + 6;
  b.Replace(ait, b.end(), b.begin(), Xit);
  EXPECT_EQ("123123abc", b); // if things go wrong, you'd get "123123123"
}

TEST(String, moveTerminator)
{
  // The source of a move must remain in a valid state
  TestString s(100, 'x'); // too big to be in-situ
  TestString k;
  k = std::move(s);

  EXPECT_EQ(0, s.Size());
  EXPECT_EQ('\0', *s.Data());
}

namespace
{
/*
 * t8968589: Clang 3.7 refused to compile w/ certain constructors (specifically
 * those that were "explicit" and had a defaulted parameter, if they were used
 * in structs which were default-initialized).  Exercise these just to ensure
 * they compile.
 *
 * In diff D2632953 the old constructor:
 *   explicit basic_fbstring(const A& a = A()) noexcept;
 *
 * was split into these two, as a workaround:
 *   basic_fbstring() noexcept;
 *   explicit basic_fbstring(const A& a) noexcept;
 */

struct TestStructDefaultAllocator
{
  TestString stringMember;
};

std::atomic<size_t> allocatorConstructedCount(0);
struct TestStructStringAllocator : std::allocator<char>
{
  TestStructStringAllocator() { ++allocatorConstructedCount; }
};

} // namespace

TEST(FBStringCtorTest, DefaultInitStructDefaultAlloc)
{
  TestStructDefaultAllocator t1 {};
  EXPECT_TRUE(t1.stringMember.Empty());
}

TEST(FBStringCtorTest, NullZeroConstruction)
{
  char *     p = nullptr;
  int        n = 0;
  TestString f(p, n);
  EXPECT_EQ(f.Size(), 0);
}

TEST(FBString, compareToStdString)
{
  // using folly::TestString;
  using namespace std::string_literals;
  auto       stdA = "a"s;
  auto       stdB = "b"s;
  TestString fbA("a");
  TestString fbB("b");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

TEST(U16FBString, compareToStdU16String)
{
  // using Sprrow::TString;
  using namespace std::string_literals;
  auto                       stdA = u"a"s;
  auto                       stdB = u"b"s;
  Sparrow::TString<char16_t> fbA(u"a");
  Sparrow::TString<char16_t> fbB(u"b");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

TEST(U32FBString, compareToStdU32String)
{
  // using Sprrow::TString;
  using namespace std::string_literals;
  auto                       stdA = U"a"s;
  auto                       stdB = U"b"s;
  Sparrow::TString<char32_t> fbA(U"a");
  Sparrow::TString<char32_t> fbB(U"b");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

TEST(WFBString, compareToStdWString)
{
  using folly::basic_fbstring;
  using namespace std::string_literals;
  auto                      stdA = L"a"s;
  auto                      stdB = L"b"s;
  Sparrow::TString<wchar_t> fbA(L"a");
  Sparrow::TString<wchar_t> fbB(L"b");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

// Same again, but with a more challenging input - a common prefix and different
// lengths.

TEST(FBString, compareToStdStringLong)
{
  // using folly::TestString;
  using namespace std::string_literals;
  auto       stdA = "1234567890a"s;
  auto       stdB = "1234567890ab"s;
  TestString fbA("1234567890a");
  TestString fbB("1234567890ab");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

TEST(U16FBString, compareToStdU16StringLong)
{
  //  using folly::basic_fbstring;
  using namespace std::string_literals;
  auto                       stdA = u"1234567890a"s;
  auto                       stdB = u"1234567890ab"s;
  Sparrow::TString<char16_t> fbA(u"1234567890a");
  Sparrow::TString<char16_t> fbB(u"1234567890ab");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

#if FOLLY_HAVE_WCHAR_SUPPORT
TEST(U32FBString, compareToStdU32StringLong)
{
  // using folly::basic_fbstring;
  using namespace std::string_literals;
  auto                       stdA = U"1234567890a"s;
  auto                       stdB = U"1234567890ab"s;
  Sparrow::TString<char32_t> fbA(U"1234567890a");
  Sparrow::TString<char32_t> fbB(U"1234567890ab");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}

TEST(WFBString, compareToStdWStringLong)
{
  // using folly::basic_fbstring;
  using namespace std::string_literals;
  auto                      stdA = L"1234567890a"s;
  auto                      stdB = L"1234567890ab"s;
  Sparrow::TString<wchar_t> fbA(L"1234567890a");
  Sparrow::TString<wchar_t> fbB(L"1234567890ab");
  EXPECT_TRUE(stdA == fbA);
  EXPECT_TRUE(fbB == stdB);
  EXPECT_TRUE(stdA != fbB);
  EXPECT_TRUE(fbA != stdB);
  EXPECT_TRUE(stdA < fbB);
  EXPECT_TRUE(fbA < stdB);
  EXPECT_TRUE(stdB > fbA);
  EXPECT_TRUE(fbB > stdA);
  EXPECT_TRUE(stdA <= fbB);
  EXPECT_TRUE(fbA <= stdB);
  EXPECT_TRUE(stdA <= fbA);
  EXPECT_TRUE(fbA <= stdA);
  EXPECT_TRUE(stdB >= fbA);
  EXPECT_TRUE(fbB >= stdA);
  EXPECT_TRUE(stdB >= fbB);
  EXPECT_TRUE(fbB >= stdB);
}
#endif

#if FOLLY_HAS_STRING_VIEW
struct custom_traits : public std::char_traits<char>
{};

TEST(FBString, convertToStringView)
{
  TestString s("foo");
  std::string_view  sv = s;
  EXPECT_EQ(sv, "foo");
  Sparrow::TString<char, custom_traits> s2("bar");
  Sparrow::TString<char, custom_traits> sv2 = s2;
  EXPECT_EQ(sv2, "bar");
}
#endif

TEST(test, test)
{
  TestString a("abcasdfasd\n");
  TestString b("ccc;ll");
  TestString c(b);
  auto       xx = TestString("asdfll");
  std::cout << a + b << b << '\n';
  std::cout << *a.begin();
  b.Reserve();
}

#define FOR_EACH_RANGE(v, init, size) for (auto v = init; v < size; v++)

// static const auto seed=std::random_device();
static const int seed = 25;
using RandomT         = std::mt19937;
static RandomT      rng(seed);
static const size_t maxString     = 100;
static const bool   avoidAliasing = true;

template <class Integral1, class Integral2>
Integral2 random(Integral1 low, Integral2 up)
{
  std::uniform_int_distribution<> range(low, up);
  return (Integral2) range(rng);
}

#include <algorithm>
#include <ctime>

void randomstdString(std::string *toFill)
{
  assert(toFill);

  using namespace std;
  string        possible_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  random_device rd;
  mt19937       engine(rd());
  std::uniform_int_distribution<int> disti(1, 700);
  uniform_int_distribution<size_t>   dist(0, possible_characters.size() - 1);
  int                                max_length = disti(engine);
  string                             ret        = "";
  for (int i = 0; i < max_length; i++)
  {
    auto random_index = dist(engine); // get index between 0 and possible_characters.size()-1
    ret += possible_characters[random_index];
  }
  *toFill = ret;
}
// #include <unistd.h>
// no runtime random
template <class String>
void randomString(String *toFill, unsigned int maxSize = 1000)
{
  assert(toFill);

  //* static random
  int len = random(0, maxSize);
  toFill->resize(len);
  auto randchar = []() -> char
  {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };

  std::generate_n((*toFill).begin(), len, randchar);
}

template <class String, class Integral>
void Num2String(String &str, Integral n)
{
  // std::string tmp = folly::to<std::string>(n);
  // str = String(tmp.begin(), tmp.end());
}

std::list<char> RandomList(unsigned int maxSize)
{
  std::list<char>           lst(random(0u, maxSize));
  std::list<char>::iterator i = lst.begin();
  for (; i != lst.end(); ++i)
  {
    *i = random('a', 'z');
  }
  return lst;
}

////////////////////////////////////////////////////////////////////////////////
// Tests begin here
////////////////////////////////////////////////////////////////////////////////
template <class String>
void clause11_21_4_2_a(String &test)
{
  test.String::~String();
  new (&test) String();
}
// TEST(test, test)
// {
//     std::string ss("abcd");
//     TestString fs("asdfafs");
//     randomString<std::string>(&ss);
//     std::cout << ss;
//     clause11_21_4_2_a(ss);
//     clause11_21_4_2_a(fs);
//     auto x = random(0, 9);
//     x++;
//     // clause11_21_4_2_a(basic_fbstring<wchar_t>("asdfasdf"));
// }

template <class String>
void clause11_21_4_2_b(String &test)
{
  String test2(test);
  assert(test2 == test);
}

#define TEST_CLAUSE(x)                                                                                                 \
  TEST(FBString, x)                                                                                                    \
  {                                                                                                                    \
    EXPECT_TRUE(1) << "Starting with seed: " << seed;                                                                  \
    std::string                r;                                                                                      \
    TestString                 c;                                                                                      \
    std::wstring               wr;                                                                                     \
    Sparrow ::TString<wchar_t> wc;                                                                                     \
    int                        count = 0;                                                                              \
                                                                                                                       \
    auto l = [&](const char *const clause,                                                                             \
                 void (*f_string)(std::string &),                                                                      \
                 void (*f_fbstring)(TestString &),                                                                     \
                 void (*f_wfbstring)(Sparrow ::TString<wchar_t> &))                                                    \
    {                                                                                                                  \
      do                                                                                                               \
      {                                                                                                                \
        if (true) {}                                                                                                   \
        else                                                                                                           \
        {                                                                                                              \
          EXPECT_TRUE(1) << "Testing clause " << clause;                                                               \
        }                                                                                                              \
        randomString(&r);                                                                                              \
        c                 = r;                                                                                         \
        TestString testself = r;                                                                                         \
        EXPECT_EQ(testself, r);                                                                                        \
                                                                                                                       \
        wr = std::wstring(r.begin(), r.end());                                                                         \
        wc = Sparrow::TString<wchar_t>(wr.c_str());                                                               \
                                                                                                                       \
        auto localSeed = seed + count;                                                                                 \
        rng            = RandomT(localSeed);                                                                           \
        f_string(r);                                                                                                   \
        rng = RandomT(localSeed);                                                                                      \
        f_fbstring(c);                                                                                                 \
        EXPECT_EQ(r, c) << "Lengths: " << r.size() << " vs. " << c.Size() << "\nReference: '" << r << "'"              \
                        << "\nActual:    '" << c.Data()[0] << "'";                                                     \
                                                                                                                       \
        rng = RandomT(localSeed);                                                                                      \
        f_wfbstring(wc);                                                                                               \
        auto  wret = wcslen(wc.Data());                                                                                \
        auto mbv  = std::vector<char>(wret + 1);                                                                       \
        auto mb   = mbv.data();                                                                                        \
        auto  ret  = wcstombs(mb, wc.Data(), wret + 1);                                                                \
        if (ret == wret) { mb[wret] = '\0'; }                                                                          \
        const char *mc = c.Data();                                                                                    \
        std::string one(mb);                                                                                           \
        std::string two(mc);                                                                                           \
        EXPECT_EQ(one, two);                                                                                           \
      }                                                                                                                \
      while (++count % 1 != 0);                                                                                        \
    };                                                                                                                 \
    l(#x, clause11_##x<std::string>, clause11_##x<TestString>, clause11_##x<Sparrow::TString<wchar_t>>);     \
  }

TEST_CLAUSE(21_4_2_a);
TEST_CLAUSE(21_4_2_b);