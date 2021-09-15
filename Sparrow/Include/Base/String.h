
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

  public:
    /**
     *
     *
     * @return * Construct
     */
    TString();

    /** Copy Construct */
    TString(const TString &str);

    /** Move Construct */
    TString(TString &&str);

    /** Copy string from stl string */
    TString(const std::basic_string<CharType> &str);

    TString(const TString &str, size_t pos);

    TString(const CharType *str);

    TString(const CharType *s, size_t n);

    // TString(size_type n, value_type c);

    TString(std::initializer_list<CharType> init_list);

    TString &operator=(const TString &str);

    TString &operator=(TString &&str);

    TString &operator=(const std::basic_string<CharType> &str);

  public:
    std::basic_string<CharType> ToStdString();

    const CharType *Data() const;

    const CharType &Front() const;

    const CharType &Back() const;

    CharType &Front();

    CharType &Back();

    void PopBack();

    CharType Size() const;

    CharType Length() const;

    CharType MaxSize() const;

    void Resize(size_type size);

    void Reserve(size_t res_arg = 0);

    void Clear();

    bool Empty() const;

    const_reference At(size_type n) const;

    CharType At(size_type n);

  public:
    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    TString &Append(const TString &str);

    TString &Append(const TString &str, const size_type pos, size_type n);

    TString &Append(const value_type *s, size_type n);

    TString &Append(const value_type *str_char);

    TString &Append(size_type n, value_type c);

    TString &Append(std::initializer_list<value_type> init_list);

    void PushBack(const value_type str_char);

    TString &Assign(const TString &str);

    TString &Assign(TString &&str);

    // basic_fbstring &assign(const basic_fbstring &str, const size_type pos, size_type n);

    // basic_fbstring &assign(const value_type *s, const size_type n);

    // basic_fbstring &assign(const value_type *s);

    // basic_fbstring &assign(std::initializer_list<value_type> il);

    // basic_fbstring &insert(size_type pos1, const basic_fbstring &str)

  public:
    const_reference operator[](size_type pos) const;

    reference operator[](size_type pos);

    TString &operator=(const value_type *str);

    TString &operator=(value_type str_char);

    TString &operator=(std::initializer_list<value_type> init_list);

    TString &operator+=(const TString &str);
    TString &operator+=(const value_type *str);
    TString &operator+=(const value_type str_char);
    TString &operator+=(std::initializer_list<value_type> init_list);
};

using String = TString<char>;
// class String :protected folly::fbstring
// class String : public folly::fbstring
// {
//   public:
//     String(const char *str) : folly::fbstring(str)
//     {
//     }
// };

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

/* string synopsis
namespace std
{
    template <class stateT> class fpos
    {
      private:
        stateT st;

      public:
        fpos(streamoff = streamoff());
        operator streamoff() const;
        stateT state() const;
        void state(stateT);
        fpos &operator+=(streamoff);
        fpos operator+(streamoff) const;
        fpos &operator-=(streamoff);
        fpos operator-(streamoff) const;
    };
    template <class stateT> streamoff operator-(const fpos<stateT> &x, const fpos<stateT> &y);
    template <class stateT> bool operator==(const fpos<stateT> &x, const fpos<stateT> &y);
    template <class stateT> bool operator!=(const fpos<stateT> &x, const fpos<stateT> &y);
    template <class charT> struct char_traits
    {
        typedef charT char_type;
        typedef... int_type;
        typedef streamoff off_type;
        typedef streampos pos_type;
        typedef mbstate_t state_type;
        static void assign(char_type &c1, const char_type &c2) noexcept;
        static constexpr bool eq(char_type c1, char_type c2) noexcept;
        static constexpr bool lt(char_type c1, char_type c2) noexcept;
        static int compare(const char_type *s1, const char_type *s2, size_t n);
        static size_t length(const char_type *s);
        static const char_type *find(const char_type *s, size_t n, const char_type &a);
        static char_type *move(char_type *s1, const char_type *s2, size_t n);
        static char_type *copy(char_type *s1, const char_type *s2, size_t n);
        static char_type *assign(char_type *s, size_t n, char_type a);
        static constexpr int_type not_eof(int_type c) noexcept;
        static constexpr char_type to_char_type(int_type c) noexcept;
        static constexpr int_type to_int_type(char_type c) noexcept;
        static constexpr bool eq_int_type(int_type c1, int_type c2) noexcept;
        static constexpr int_type eof() noexcept;
    };

    template <> struct char_traits<char>;
    template <> struct char_traits<wchar_t>;
    template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT>> class basic_string
    {
      public:
        // types:
        typedef traits traits_type;
        typedef typename traits_type::char_type value_type;
        typedef Allocator allocator_type;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef implementation - defined iterator;
        typedef implementation - defined const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        static const size_type npos = -1;
        basic_string() noexcept(is_nothrow_default_constructible<allocator_type>::value);
        explicit basic_string(const allocator_type &a);
        basic_string(const basic_string &str);
        basic_string(basic_string &&str) noexcept(is_nothrow_move_constructible<allocator_type>::value);
        basic_string(const basic_string &str, size_type pos, const allocator_type &a = allocator_type());
        basic_string(const basic_string &str, size_type pos, size_type n, const Allocator &a = Allocator());
        template <class T>
        basic_string(const T &t, size_type pos, size_type n, const Allocator &a = Allocator()); // C++17
        template <class T> explicit basic_string(const T &t, const Allocator &a = Allocator()); // C++17
        basic_string(const value_type *s, const allocator_type &a = allocator_type());
        basic_string(const value_type *s, size_type n, const allocator_type &a = allocator_type());
        basic_string(size_type n, value_type c, const allocator_type &a = allocator_type());
        template <class InputIterator>
        basic_string(InputIterator begin, InputIterator end, const allocator_type &a = allocator_type());
        basic_string(initializer_list<value_type>, const Allocator & = Allocator());
        basic_string(const basic_string &, const Allocator &);
        basic_string(basic_string &&, const Allocator &);
        ~basic_string();
        operator basic_string_view<charT, traits>() const noexcept;
        basic_string &operator=(const basic_string &str);
        template <class T> basic_string &operator=(const T &t); // C++17
        basic_string &operator=(basic_string &&str) noexcept(
            allocator_type::propagate_on_container_move_assignment::value ||
            allocator_type::is_always_equal::value); // C++17
        basic_string &operator=(const value_type *s);
        basic_string &operator=(value_type c);
        basic_string &operator=(initializer_list<value_type>);
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;
        size_type size() const noexcept;
        size_type length() const noexcept;
        size_type max_size() const noexcept;
        size_type capacity() const noexcept;
        void resize(size_type n, value_type c);
        void resize(size_type n);
        void reserve(size_type res_arg = 0);
        void shrink_to_fit();
        void clear() noexcept;
        bool empty() const noexcept;
        const_reference operator[](size_type pos) const;
        reference operator[](size_type pos);
        const_reference at(size_type n) const;
        reference at(size_type n);
        basic_string &operator+=(const basic_string &str);
        template <class T> basic_string &operator+=(const T &t); // C++17
        basic_string &operator+=(const value_type *s);
        basic_string &operator+=(value_type c);
        basic_string &operator+=(initializer_list<value_type>);
        basic_string &append(const basic_string &str);
        template <class T> basic_string &append(const T &t);                                    // C++17
        basic_string &append(const basic_string &str, size_type pos, size_type n = npos);       // C++14
        template <class T> basic_string &append(const T &t, size_type pos, size_type n = npos); // C++17
        basic_string &append(const value_type *s, size_type n);
        basic_string &append(const value_type *s);
        basic_string &append(size_type n, value_type c);
        template <class InputIterator> basic_string &append(InputIterator first, InputIterator last);
        basic_string &append(initializer_list<value_type>);
        void push_back(value_type c);
        void pop_back();
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        basic_string &assign(const basic_string &str);
        template <class T> basic_string &assign(const T &t); // C++17
        basic_string &assign(basic_string &&str);
        basic_string &assign(const basic_string &str, size_type pos, size_type n = npos);       // C++14
        template <class T> basic_string &assign(const T &t, size_type pos, size_type n = npos); // C++17
        basic_string &assign(const value_type *s, size_type n);
        basic_string &assign(const value_type *s);
        basic_string &assign(size_type n, value_type c);
        template <class InputIterator> basic_string &assign(InputIterator first, InputIterator last);
        basic_string &assign(initializer_list<value_type>);
        basic_string &insert(size_type pos1, const basic_string &str);
        template <class T> basic_string &insert(size_type pos1, const T &t);
        basic_string &insert(size_type pos1, const basic_string &str, size_type pos2, size_type n);
        template <class T> basic_string &insert(size_type pos1, const T &t, size_type pos2, size_type n); // C++17
        basic_string &insert(size_type pos, const value_type *s, size_type n = npos);                     // C++14
        basic_string &insert(size_type pos, const value_type *s);
        basic_string &insert(size_type pos, size_type n, value_type c);
        iterator insert(const_iterator p, value_type c);
        iterator insert(const_iterator p, size_type n, value_type c);
        template <class InputIterator> iterator insert(const_iterator p, InputIterator first, InputIterator last);
        iterator insert(const_iterator p, initializer_list<value_type>);
        basic_string &erase(size_type pos = 0, size_type n = npos);
        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);
        basic_string &replace(size_type pos1, size_type n1, const basic_string &str);
        template <class T> basic_string &replace(size_type pos1, size_type n1, const T &t); // C++17
        basic_string &replace(size_type pos1, size_type n1, const basic_string &str, size_type pos2,
                              size_type n2 = npos); // C++14
        template <class T>
        basic_string &replace(size_type pos1, size_type n1, const T &t, size_type pos2, size_type n); // C++17
        basic_string &replace(size_type pos, size_type n1, const value_type *s, size_type n2);
        basic_string &replace(size_type pos, size_type n1, const value_type *s);
        basic_string &replace(size_type pos, size_type n1, size_type n2, value_type c);
        basic_string &replace(const_iterator i1, const_iterator i2, const basic_string &str);
        template <class T> basic_string &replace(const_iterator i1, const_iterator i2, const T &t); // C++17
        basic_string &replace(const_iterator i1, const_iterator i2, const value_type *s, size_type n);
        basic_string &replace(const_iterator i1, const_iterator i2, const value_type *s);
        basic_string &replace(const_iterator i1, const_iterator i2, size_type n, value_type c);
        template <class InputIterator>
        basic_string &replace(const_iterator i1, const_iterator i2, InputIterator j1, InputIterator j2);
        basic_string &replace(const_iterator i1, const_iterator i2, initializer_list<value_type>);
        size_type copy(value_type *s, size_type n, size_type pos = 0) const;
        basic_string substr(size_type pos = 0, size_type n = npos) const;
        void swap(basic_string &str) noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                                              allocator_traits<allocator_type>::is_always_equal::value); // C++17
        const value_type *c_str() const noexcept;
        const value_type *data() const noexcept;
        value_type *data() noexcept; // C++17
        allocator_type get_allocator() const noexcept;
        size_type find(const basic_string &str, size_type pos = 0) const noexcept;
        template <class T> size_type find(const T &t, size_type pos = 0) const; // C++17
        size_type find(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type find(const value_type *s, size_type pos = 0) const noexcept;
        size_type find(value_type c, size_type pos = 0) const noexcept;
        size_type rfind(const basic_string &str, size_type pos = npos) const noexcept;
        template <class T> size_type rfind(const T &t, size_type pos = npos) const; // C++17
        size_type rfind(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type rfind(const value_type *s, size_type pos = npos) const noexcept;
        size_type rfind(value_type c, size_type pos = npos) const noexcept;
        size_type find_first_of(const basic_string &str, size_type pos = 0) const noexcept;
        template <class T> size_type find_first_of(const T &t, size_type pos = 0) const; // C++17
        size_type find_first_of(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type find_first_of(const value_type *s, size_type pos = 0) const noexcept;
        size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
        size_type find_last_of(const basic_string &str, size_type pos = npos) const noexcept;
        template <class T> size_type find_last_of(const T &t, size_type pos = npos) const noexcept; // C++17
        size_type find_last_of(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type find_last_of(const value_type *s, size_type pos = npos) const noexcept;
        size_type find_last_of(value_type c, size_type pos = npos) const noexcept;
        size_type find_first_not_of(const basic_string &str, size_type pos = 0) const noexcept;
        template <class T> size_type find_first_not_of(const T &t, size_type pos = 0) const; // C++17
        size_type find_first_not_of(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type find_first_not_of(const value_type *s, size_type pos = 0) const noexcept;
        size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
        size_type find_last_not_of(const basic_string &str, size_type pos = npos) const noexcept;
        template <class T> size_type find_last_not_of(const T &t, size_type pos = npos) const; // C++17
        size_type find_last_not_of(const value_type *s, size_type pos, size_type n) const noexcept;
        size_type find_last_not_of(const value_type *s, size_type pos = npos) const noexcept;
        size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;
        int compare(const basic_string &str) const noexcept;
        template <class T> int compare(const T &t) const noexcept; // C++17
        int compare(size_type pos1, size_type n1, const basic_string &str) const;
        template <class T> int compare(size_type pos1, size_type n1, const T &t) const; // C++17
        int compare(size_type pos1, size_type n1, const basic_string &str, size_type pos2,
                    size_type n2 = npos) const; // C++14
        template <class T>
        int compare(size_type pos1, size_type n1, const T &t, size_type pos2, size_type n2 = npos) const; // C++17
        int compare(const value_type *s) const noexcept;
        int compare(size_type pos1, size_type n1, const value_type *s) const;
        int compare(size_type pos1, size_type n1, const value_type *s, size_type n2) const;
        bool starts_with(basic_string_view<charT, traits> sv) const noexcept; // C++2a
        bool starts_with(charT c) const noexcept;                             // C++2a
        bool starts_with(const charT *s) const;                               // C++2a
        bool ends_with(basic_string_view<charT, traits> sv) const noexcept;   // C++2a
        bool ends_with(charT c) const noexcept;                               // C++2a
        bool ends_with(const charT *s) const;                                 // C++2a
        bool __invariants() const;
    };
    template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
    basic_string(InputIterator, InputIterator, Allocator = Allocator())
        -> basic_string<typename iterator_traits<InputIterator>::value_type,
                        char_traits<typename iterator_traits<InputIterator>::value_type>,
                        Allocator>; // C++17
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> operator+(const basic_string<charT, traits, Allocator> &lhs,
                                                     const basic_string<charT, traits, Allocator> &rhs);
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> operator+(const charT *lhs,
                                                     const basic_string<charT, traits, Allocator> &rhs);
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> operator+(charT lhs, const basic_string<charT, traits, Allocator> &rhs);
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> operator+(const basic_string<charT, traits, Allocator> &lhs,
                                                     const charT *rhs);
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> operator+(const basic_string<charT, traits, Allocator> &lhs, charT rhs);
    template <class charT, class traits, class Allocator>
    bool operator==(const basic_string<charT, traits, Allocator> &lhs,
                    const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator==(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator==(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator!=(const basic_string<charT, traits, Allocator> &lhs,
                    const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator!=(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator!=(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<(const basic_string<charT, traits, Allocator> &lhs,
                   const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>(const basic_string<charT, traits, Allocator> &lhs,
                   const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<=(const basic_string<charT, traits, Allocator> &lhs,
                    const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<=(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator<=(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>=(const basic_string<charT, traits, Allocator> &lhs,
                    const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>=(const basic_string<charT, traits, Allocator> &lhs, const charT *rhs) noexcept;
    template <class charT, class traits, class Allocator>
    bool operator>=(const charT *lhs, const basic_string<charT, traits, Allocator> &rhs) noexcept;
    template <class charT, class traits, class Allocator>
    void swap(basic_string<charT, traits, Allocator> & lhs,
              basic_string<charT, traits, Allocator> & rhs) noexcept(noexcept(lhs.swap(rhs)));
    template <class charT, class traits, class Allocator>
    basic_istream<charT, traits> &operator>>(basic_istream<charT, traits> &is,
                                             basic_string<charT, traits, Allocator> &str);
    template <class charT, class traits, class Allocator>
    basic_ostream<charT, traits> &operator<<(basic_ostream<charT, traits> &os,
                                             const basic_string<charT, traits, Allocator> &str);
    template <class charT, class traits, class Allocator>
    basic_istream<charT, traits> &getline(basic_istream<charT, traits> & is,
                                          basic_string<charT, traits, Allocator> & str, charT delim);
    template <class charT, class traits, class Allocator>
    basic_istream<charT, traits> &getline(basic_istream<charT, traits> & is,
                                          basic_string<charT, traits, Allocator> & str);
    template <class charT, class traits, class Allocator, class U>
    void erase(basic_string<charT, traits, Allocator> & c, const U &value); // C++20
    template <class charT, class traits, class Allocator, class Predicate>
    void erase_if(basic_string<charT, traits, Allocator> & c, Predicate pred); // C++20
    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;
    int stoi(const string &str, size_t *idx = 0, int base = 10);
    long stol(const string &str, size_t *idx = 0, int base = 10);
    unsigned long stoul(const string &str, size_t *idx = 0, int base = 10);
    long long stoll(const string &str, size_t *idx = 0, int base = 10);
    unsigned long long stoull(const string &str, size_t *idx = 0, int base = 10);
    float stof(const string &str, size_t *idx = 0);
    double stod(const string &str, size_t *idx = 0);
    long double stold(const string &str, size_t *idx = 0);
    string to_string(int val);
    string to_string(unsigned val);
    string to_string(long val);
    string to_string(unsigned long val);
    string to_string(long long val);
    string to_string(unsigned long long val);
    string to_string(float val);
    string to_string(double val);
    string to_string(long double val);
    int stoi(const wstring &str, size_t *idx = 0, int base = 10);
    long stol(const wstring &str, size_t *idx = 0, int base = 10);
    unsigned long stoul(const wstring &str, size_t *idx = 0, int base = 10);
    long long stoll(const wstring &str, size_t *idx = 0, int base = 10);
    unsigned long long stoull(const wstring &str, size_t *idx = 0, int base = 10);
    float stof(const wstring &str, size_t *idx = 0);
    double stod(const wstring &str, size_t *idx = 0);
    long double stold(const wstring &str, size_t *idx = 0);
    wstring to_wstring(int val);
    wstring to_wstring(unsigned val);
    wstring to_wstring(long val);
    wstring to_wstring(unsigned long val);
    wstring to_wstring(long long val);
    wstring to_wstring(unsigned long long val);
    wstring to_wstring(float val);
    wstring to_wstring(double val);
    wstring to_wstring(long double val);
    template <> struct hash<string>;
    template <> struct hash<u16string>;
    template <> struct hash<u32string>;
    template <> struct hash<wstring>;
    basic_string<char> operator"" s(const char *str, size_t len);         // C++14
    basic_string<wchar_t> operator"" s(const wchar_t *str, size_t len);   // C++14
    basic_string<char16_t> operator"" s(const char16_t *str, size_t len); // C++14
    basic_string<char32_t> operator"" s(const char32_t *str, size_t len); // C++14
}
*/