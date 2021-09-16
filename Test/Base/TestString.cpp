
#include <Base/String.h>
#include "gtest/gtest.h"
#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <list>
#include <random>
#include <sstream>
#include <string>

using namespace std;

using namespace Sparrow;
#include <memory>
#include <iostream>

using TestString = Sparrow::TString<char>;


template <class String>
void ReBuild(String& test) {
  test.String::~String();
  new (&test) String();
}

TEST(String, Rebuild)
{
    

}


TEST(test, test)
{
	TestString a("abcasdfasd\n");
	TestString b("ccc;ll");
	TestString c(b);
	auto xx = TestString("asdfll");
	std::cout << a+b<<b<<'\n';
	std::cout << *a.begin();
	b.reserve();
	/*

	std::string::size_type st = 1;
	size_t st1 = 1;
	std::cout <<( typeid(st )==typeid(st1));
	String b("abc\n");
	auto c(b);
	std::cout << c.Data();
	std::cout << a.ToStdString();
	std::cout << a.At(2);
	a.Front() = 'x';
	std::cout << *a.begin()<<'\n';

	*/
//	String  sst = a + b;
	//std::cout << sst.Data();
//	String lc = { 'c','b','a'
	//};
	//std::cout << lc;
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