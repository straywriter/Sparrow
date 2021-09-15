

// #include "Core/Log.h"
#include <Base/String.h>
// #include <Inline/Inline.h>


#include "gtest/gtest.h"

#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <list>
#include <random>
#include <sstream>
#include <string>

using namespace std;
using namespace folly;

using namespace Sparrow;
#include <memory>
#include <iostream>

TEST(test, test)
{
	
String a("abcasdfasd\n");

	
String b("abc\n");
auto c(b);
std::cout<<c.Data();
std::cout<<a.ToStdString();
std::cout<<a.At(2);
// a.Reserve(2);
// std::cout<<a.ToStdString();
// std::cout<<a.c_str();

//     std::string ss("abcd");
//     fbstring fs("asdfafs");
//     // randomString<std::string>(&ss);
//     std::cout << fs;
    // clause11_21_4_2_a(ss);
    // clause11_21_4_2_a(fs);
    // auto x = random(0, 9);

    // clause11_21_4_2_a(basic_fbstring<wchar_t>("asdfasdf"));
}



// int main()
// {


//     // OUT_INFO("test log");

//     // LOG_WARN("test log");

// ;
// }