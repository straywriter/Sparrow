
#pragma once
#include <initializer_list>
#include <string>

// if folly
#include <External/folly.h>

namespace Sparrow
{

template <typename CharType> class TString : protected folly::basic_fbstring<CharType>
{
  public:
    // typedef std::char_traits<CharType> traits_type;
    // typedef typename traits_type::char_type value_type;
    // using value_type=folly::basic_fbstring<CharType>::value_type;
    using StringType = TString<CharType>;

  public:
    /** Construct */
    TString();

    /** Copy Construct */
    TString(const TString &str);

    TString(const TString &str, size_t start, size_t num = size_t(-1));

    /** Move Construct */
    TString(TString &&str);

    /** Copy string from stl string */
    TString(const std::basic_string<CharType> &str);

    TString(const CharType *str);

    TString(const CharType *str, size_t num);

    TString(size_t start, size_t num);

    template <typename Iterator> TString(Iterator begin, Iterator end);

    TString(const CharType *begin, const CharType *end);

    TString(size_t num, CharType str_char);

    TString(std::initializer_list<CharType> init_list);

    ~TString();

    TString &operator=(const TString &str);

    TString &operator=(TString &&str);

    TString &operator=(const std::basic_string<CharType> &str);

    TString &operator=(const CharType *str);

    TString &operator=(std::initializer_list<CharType> init_list);

  public:
    std::basic_string<CharType> ToStdString();

    const CharType *Data() const;

    CharType *Data();

    const CharType &Front() const;

    const CharType &Back() const;

    CharType &Front();

    CharType &Back();

    void PopBack();

    size_t Size() const;

    size_t Length() const;

    size_t MaxSize() const;

    size_t Capacity() const;

    void Resize(size_t size);

    void Reserve(size_t res_arg = 0);

    void ShrinkToFit();

    void Clear();

    bool Empty() const;

    const CharType At(size_t pos) const;

    CharType At(size_t pos);

    TString &Append(const TString &str);

    TString &Append(const TString &str, const size_t start, size_t num);

    TString &Append(const CharType *str, size_t num);

    TString &Append(const CharType *str);

    TString &Append(size_t num, CharType str_char);

    template <class InputIterator> TString &Append(InputIterator first, InputIterator last);

    TString &Append(std::initializer_list<CharType> init_list);

    void PushBack(const CharType str_char);

    TString &Assign(const TString &str);

    TString &Assign(TString &&str);

    TString &Assign(const TString &str, const size_t start, size_t num);

    TString &Assign(const CharType *str, size_t num);

    TString &Assign(std::initializer_list<CharType> init_list);

    TString &Assign(size_t num, CharType str_char);

    template <class InputIterator> TString &Assign(InputIterator first, InputIterator last);

    TString &Insert(size_t index, const TString &str);

    TString &Insert(size_t index, const TString &str, size_t pos, size_t num);

    TString &Insert(size_t index, const CharType *str, size_t num);

    TString &Insert(size_t index, const CharType *str);

    TString &Insert(size_t index, size_t num, CharType str_char);

    TString &Insert(size_t index, const CharType str_char);

    TString &Erase(size_t index = size_t(0), size_t num = size_t(-1));

    CharType *Erase(CharType *pos);

    CharType *Erase(CharType *first, CharType *lase);

    TString &Replace(size_t index, size_t num, const TString &str, size_t pos, size_t count);

    TString &Replace(size_t index, size_t num, const TString &str);

    TString &Replace(size_t index, size_t num, const CharType *str);

    size_t Copy(CharType *str, size_t num, size_t pos = size_t(0));

    void Swap(TString &str);

    size_t Find(const TString &str, size_t pos = size_t(0)) const;
    size_t Find(const char *str, size_t pos, size_t num) const;
    size_t Find(const CharType *str, size_t pos = size_t(0)) const;
    size_t Find(const CharType str_char, size_t pos = size_t(0)) const;

    size_t ReverseFind(const TString &str, size_t pos = size_t(-1)) const;
    size_t ReverseFind(const char *str, size_t pos, size_t num) const;
    size_t ReverseFind(const CharType *str, size_t pos = size_t(-1)) const;
    size_t ReverseFind(const CharType str_char, size_t pos = size_t(-1)) const;

    size_t FindFirst(const TString &str, size_t pos = size_t(0)) const;
    size_t FindFirst(const char *str, size_t pos, size_t num) const;
    size_t FindFirst(const CharType *str, size_t pos = size_t(0)) const;
    size_t FindFirst(const CharType str_char, size_t pos = size_t(0)) const;

    size_t FindLast(const TString &str, size_t pos = size_t(-1)) const;
    size_t FindLast(const char *str, size_t pos, size_t num) const;
    size_t FindLast(const CharType *str, size_t pos = size_t(-1)) const;
    size_t FindLast(const CharType str_char, size_t pos = size_t(-1)) const;

   
    size_t FindFirstNot(const TString &str, size_t pos = size_t(-1)) const;
    size_t FindFirstNot(const char *str, size_t pos, size_t num) const;
    size_t FindFirstNot(const CharType *str, size_t pos = size_t(-1)) const;
    size_t FindFirstNot(const CharType str_char, size_t pos = size_t(-1)) const;

    size_t FindLastNot(const TString &str, size_t pos = size_t(0)) const;
    size_t FindLastNot(const char *str, size_t pos, size_t num) const;
    size_t FindLastNot(const CharType *str, size_t pos = size_t(0)) const;
    size_t FindLastNot(const CharType str_char, size_t pos = size_t(0)) const;

    TString SubString(size_t start = 0, size_t = size_t(-1)) const &;
    TString SubString(size_t start = 0, size_t = size_t(-1)) &&;

    int Compare(const TString &str) const;
    int Compare(size_t start, size_t num, const TString &str) const;
    int Compare(size_t start, size_t num, const CharType *str) const;
    int Compare(size_t start, size_t num, const CharType *str, size_t count) const;
    int Compare(size_t start, size_t num, const TString &str, size_t pos, size_t count) const;
    int Compare(const CharType *str) const;

  public:
    CharType *begin();

    CharType *end();

  public:
    const CharType operator[](size_t pos) const;

    CharType operator[](size_t pos);

    TString &operator+=(const TString &str);

    TString &operator+=(const CharType *str_char);

    TString &operator+=(const CharType str_char);

    TString &operator+=(std::initializer_list<CharType> init_list);

   //friend TString operator +(const TString & left_str,const TString &right_str);

  public:
    friend std::basic_istream<CharType, std::char_traits<CharType>> &getline(
        std::basic_istream<CharType, std::char_traits<CharType>> in_stream, TString &str);

    friend std::basic_istream<CharType, std::char_traits<CharType>> &getline(
        std::basic_istream<CharType, std::char_traits<CharType>> in_stream, TString &str, CharType delim);
};

using String = TString<char>;


// template <typename CharType>
// Sparrow::TString<CharType> operator+(const Sparrow::TString<CharType>& left_str,
//     const Sparrow::TString<CharType>& right_str);


  // TString operator +(const TString & left_str,const TString &right_str);


} // namespace Sparrow

#include <Base/StringFBString.inl>

/* my string function
Append
contains 包含
count
fill
insert
isEmpty
isUpper
ToDouble
ToInt

*/
