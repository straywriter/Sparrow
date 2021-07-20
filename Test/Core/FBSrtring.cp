#include "folly/FBString.h"
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>

template <class String> void clause11_21_4_2_i(String &test)
{
    // From initializer_list<char>
    std::initializer_list<typename String::value_type> il = {'h', 'e', 'l', 'l', 'o'};
    String s(il);
    test.swap(s);
}
struct custom_traits : public std::char_traits<char>
{
};

int main()
{

    folly::fbstring s("foo");
    std::cout << s << std::endl;
    std::cout<<sizeof(folly::fbstring);
    // std::string_view sv = s;
    // clause11_21_4_2_i<folly::fbstring>(s);
    // std::cout << s;
    // folly::basic_fbstring<char, custom_traits> s2("bar");
    // std::basic_string_view<char, custom_traits> sv2 = s2;

}