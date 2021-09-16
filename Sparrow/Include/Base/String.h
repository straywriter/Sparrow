
#pragma once
#include <initializer_list>
#include <string>

// if folly
#include <External/folly.h>

namespace Sparrow
{

// template <typename CharType> class TString : protected folly::String<CharType>
// {
//   public:
//     // typedef std::char_traits<CharType> traits_type;
//     // typedef typename traits_type::char_type value_type;
//     // using value_type=folly::basic_fbstring<CharType>::value_type;
//     using StringType = TString<CharType>;

//   public:
//     /** Construct */
//     TString();

//     /** Copy Construct */
//     TString(const TString &str);

//     TString(const TString &str, size_t start, size_t num = size_t(-1));

//     /** Move Construct */
//     TString(TString &&str);

//     /** Copy string from stl string */
//     TString(const std::basic_string<CharType> &str);

//     TString(const CharType *str);

//     TString(const CharType *str, size_t num);

//     TString(size_t start, size_t num);

//     template <typename Iterator> TString(Iterator begin, Iterator end);

//     TString(const CharType *begin, const CharType *end);

//     TString(size_t num, CharType str_char);

//     TString(std::initializer_list<CharType> init_list);

//     ~TString();

//     TString &operator=(const TString &str);

//     TString &operator=(TString &&str);

//     TString &operator=(const std::basic_string<CharType> &str);

//     TString &operator=(const CharType *str);

//     TString &operator=(std::initializer_list<CharType> init_list);

//   public:
//     std::basic_string<CharType> ToStdString();

//     const CharType *Data() const;

//     CharType *Data();

//     const CharType &Front() const;

//     const CharType &Back() const;

//     CharType &Front();

//     CharType &Back();

//     void PopBack();

//     size_t Size() const;

//     size_t Length() const;

//     size_t MaxSize() const;

//     size_t Capacity() const;

//     void Resize(size_t size);

//     void Reserve(size_t res_arg = 0);

//     void ShrinkToFit();

//     void Clear();

//     bool Empty() const;

//     const CharType At(size_t pos) const;

//     CharType At(size_t pos);

//     TString &Append(const TString &str);

//     TString &Append(const TString &str, const size_t start, size_t num);

//     TString &Append(const CharType *str, size_t num);

//     TString &Append(const CharType *str);

//     TString &Append(size_t num, CharType str_char);

//     template <class InputIterator> TString &Append(InputIterator first, InputIterator last);

//     TString &Append(std::initializer_list<CharType> init_list);

//     void PushBack(const CharType str_char);

//     TString &Assign(const TString &str);

//     TString &Assign(TString &&str);

//     TString &Assign(const TString &str, const size_t start, size_t num);

//     TString &Assign(const CharType *str, size_t num);

//     TString &Assign(std::initializer_list<CharType> init_list);

//     TString &Assign(size_t num, CharType str_char);

//     template <class InputIterator> TString &Assign(InputIterator first, InputIterator last);

//     TString &Insert(size_t index, const TString &str);

//     TString &Insert(size_t index, const TString &str, size_t pos, size_t num);

//     TString &Insert(size_t index, const CharType *str, size_t num);

//     TString &Insert(size_t index, const CharType *str);

//     TString &Insert(size_t index, size_t num, CharType str_char);

//     TString &Insert(size_t index, const CharType str_char);

//     TString &Erase(size_t index = size_t(0), size_t num = size_t(-1));

//     CharType *Erase(CharType *pos);

//     CharType *Erase(CharType *first, CharType *lase);

//     TString &Replace(size_t index, size_t num, const TString &str, size_t pos, size_t count);

//     TString &Replace(size_t index, size_t num, const TString &str);

//     TString &Replace(size_t index, size_t num, const CharType *str);

//     size_t Copy(CharType *str, size_t num, size_t pos = size_t(0));

//     void Swap(TString &str);

//     size_t Find(const TString &str, size_t pos = size_t(0)) const;
//     size_t Find(const char *str, size_t pos, size_t num) const;
//     size_t Find(const CharType *str, size_t pos = size_t(0)) const;
//     size_t Find(const CharType str_char, size_t pos = size_t(0)) const;

//     size_t ReverseFind(const TString &str, size_t pos = size_t(-1)) const;
//     size_t ReverseFind(const char *str, size_t pos, size_t num) const;
//     size_t ReverseFind(const CharType *str, size_t pos = size_t(-1)) const;
//     size_t ReverseFind(const CharType str_char, size_t pos = size_t(-1)) const;

//     size_t FindFirst(const TString &str, size_t pos = size_t(0)) const;
//     size_t FindFirst(const char *str, size_t pos, size_t num) const;
//     size_t FindFirst(const CharType *str, size_t pos = size_t(0)) const;
//     size_t FindFirst(const CharType str_char, size_t pos = size_t(0)) const;

//     size_t FindLast(const TString &str, size_t pos = size_t(-1)) const;
//     size_t FindLast(const char *str, size_t pos, size_t num) const;
//     size_t FindLast(const CharType *str, size_t pos = size_t(-1)) const;
//     size_t FindLast(const CharType str_char, size_t pos = size_t(-1)) const;

   
//     size_t FindFirstNot(const TString &str, size_t pos = size_t(-1)) const;
//     size_t FindFirstNot(const char *str, size_t pos, size_t num) const;
//     size_t FindFirstNot(const CharType *str, size_t pos = size_t(-1)) const;
//     size_t FindFirstNot(const CharType str_char, size_t pos = size_t(-1)) const;

//     size_t FindLastNot(const TString &str, size_t pos = size_t(0)) const;
//     size_t FindLastNot(const char *str, size_t pos, size_t num) const;
//     size_t FindLastNot(const CharType *str, size_t pos = size_t(0)) const;
//     size_t FindLastNot(const CharType str_char, size_t pos = size_t(0)) const;

//     TString SubString(size_t start = 0, size_t = size_t(-1)) const &;
//     TString SubString(size_t start = 0, size_t = size_t(-1)) &&;

//     int Compare(const TString &str) const;
//     int Compare(size_t start, size_t num, const TString &str) const;
//     int Compare(size_t start, size_t num, const CharType *str) const;
//     int Compare(size_t start, size_t num, const CharType *str, size_t count) const;
//     int Compare(size_t start, size_t num, const TString &str, size_t pos, size_t count) const;
//     int Compare(const CharType *str) const;

//   public:
//     CharType *begin();

//     CharType *end();

//   public:
//     const CharType operator[](size_t pos) const;

//     CharType operator[](size_t pos);

//     TString &operator+=(const TString &str);

//     TString &operator+=(const CharType *str_char);

//     TString &operator+=(const CharType str_char);

//     TString &operator+=(std::initializer_list<CharType> init_list);

//    //friend TString operator +(const TString & left_str,const TString &right_str);

//   public:
//     friend std::basic_istream<CharType, std::char_traits<CharType>> &getline(
//         std::basic_istream<CharType, std::char_traits<CharType>> in_stream, TString &str);

//     friend std::basic_istream<CharType, std::char_traits<CharType>> &getline(
//         std::basic_istream<CharType, std::char_traits<CharType>> in_stream, TString &str, CharType delim);
// };



/**
 * This is the basic_string replacement. For conformity,
 * basic_fbstring takes the same template parameters, plus the last
 * one which is the core.
 */
template <
    typename CharType,
    class TraitType = std::char_traits<CharType>,
    class Allocator = std::allocator<CharType>,
    class Storage = folly::fbstring_core<CharType>>
class TString {

  static_assert(
      std::is_same<Allocator, std::allocator<CharType>>::value,
      "fbstring ignores custom allocators");

  template <typename Ex, typename... Args>
  FOLLY_ALWAYS_INLINE static void enforce(bool condition, Args&&... args) {
    if (!condition) {
      folly::throw_exception<Ex>(static_cast<Args&&>(args)...);
    }
  }

  bool isSane() const {
    return begin() <= end() && empty() == (size() == 0) &&
        empty() == (begin() == end()) && size() <= max_size() &&
        capacity() <= max_size() && size() <= capacity() &&
        begin()[size()] == '\0';
  }

  struct Invariant {
    Invariant& operator=(const Invariant&) = delete;
    explicit Invariant(const TString& s) noexcept : s_(s) {
      assert(s_.isSane());
    }
    ~Invariant() noexcept { assert(s_.isSane()); }

   private:
    const TString& s_;
  };

 public:
  // types
  typedef TraitType traits_type;
  typedef typename traits_type::char_type value_type;
  typedef Allocator allocator_type;
  typedef typename std::allocator_traits<Allocator>::size_type size_type;
  typedef typename std::allocator_traits<Allocator>::difference_type difference_type;

  typedef typename std::allocator_traits<Allocator>::value_type& reference;
  typedef typename std::allocator_traits<Allocator>::value_type const& const_reference;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;

  //typedef CharType* iterator;
  using iterator = CharType*;
  typedef const CharType* const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static constexpr size_type npos = size_type(-1);
  typedef std::true_type IsRelocatable;

 private:
  static void procrustes(size_type& n, size_type nmax);

  static size_type traitsLength(const value_type* s);

 public:
  // C++11 21.4.2 construct/copy/destroy

  // Note: while the following two constructors can be (and previously were)
  // collapsed into one constructor written this way:
  //
  //   explicit basic_fbstring(const A& a = A()) noexcept { }
  //
  // This can cause Clang (at least version 3.7) to fail with the error:
  //   "chosen constructor is explicit in copy-initialization ...
  //   in implicit initialization of field '(x)' with omitted initializer"
  //
  // if used in a struct which is default-initialized.  Hence the split into
  // these two separate constructors.

  TString() noexcept : TString(Allocator()) {}

  explicit TString(const Allocator&) noexcept {}

  TString(const TString& str) : store_(str.store_) {}

  // Move constructor
  TString(TString&& goner) noexcept
      : store_(std::move(goner.store_)) {}

  // This is defined for compatibility with std::string
  template <typename A2>
  /* implicit */ TString(const std::basic_string<CharType, TraitType, A2>& str)
      : store_(str.data(), str.size()) {}

  TString(
      const TString& str,
      size_type pos,
      size_type n = npos,
      const Allocator& /* a */ = Allocator()) {
    assign(str, pos, n);
  }

  FOLLY_NOINLINE
  /* implicit */ TString(const value_type* s, const Allocator& /*a*/ = Allocator())
      : store_(s, traitsLength(s)) {}

  FOLLY_NOINLINE
  TString(const value_type* s, size_type n, const Allocator& /*a*/ = Allocator())
      : store_(s, n) {}

  FOLLY_NOINLINE
  TString(size_type n, value_type c, const Allocator& /*a*/ = Allocator()) {
    auto const pData = store_.expandNoinit(n);
    fbstring_detail::podFill(pData, pData + n, c);
  }

  template <class InIt>
  FOLLY_NOINLINE TString(
      InIt begin,
      InIt end,
      typename std::enable_if<
          !std::is_same<InIt, value_type*>::value,
          const Allocator>::type& /*a*/
      = Allocator()) {
    assign(begin, end);
  }

  // Specialization for const char*, const char*
  FOLLY_NOINLINE
  TString(const value_type* b, const value_type* e, const Allocator& /*a*/ = Allocator())
      : store_(b, size_type(e - b)) {}

  // Nonstandard constructor
  TString(
      value_type* s, size_type n, size_type c, folly::AcquireMallocatedString a)
      : store_(s, n, c, a) {}

  // Construction from initialization list
  FOLLY_NOINLINE
  TString(std::initializer_list<value_type> il) {
    assign(il.begin(), il.end());
  }

  ~TString() noexcept {}

  TString& operator=(const TString& lhs);

  // Move assignment
  TString& operator=(TString&& goner) noexcept;

  // Compatibility with std::string
  template <typename A2>
  TString& operator=(const std::basic_string<CharType, TraitType, A2>& rhs) {
    return assign(rhs.data(), rhs.size());
  }

  // Compatibility with std::string
  std::basic_string<CharType, TraitType, Allocator> toStdString() const {
    return std::basic_string<CharType, TraitType, Allocator>(data(), size());
  }

  TString& operator=(const value_type* s) { return assign(s); }

  TString& operator=(value_type c);

  // This actually goes directly against the C++ spec, but the
  // value_type overload is dangerous, so we're explicitly deleting
  // any overloads of operator= that could implicitly convert to
  // value_type.
  // Note that we do need to explicitly specify the template types because
  // otherwise MSVC 2017 will aggressively pre-resolve value_type to
  // traits_type::char_type, which won't compare as equal when determining
  // which overload the implementation is referring to.
  template <typename TP>
  typename std::enable_if<
      std::is_convertible<
          TP,
          typename TString<CharType, TraitType, Allocator, Storage>::value_type>::value &&
          !std::is_same<
              typename std::decay<TP>::type,
              typename TString<CharType, TraitType, Allocator, Storage>::value_type>::value,
      TString<CharType, TraitType, Allocator, Storage>&>::type
  operator=(TP c) = delete;

  TString& operator=(std::initializer_list<value_type> il) {
    return assign(il.begin(), il.end());
  }

#if FOLLY_HAS_STRING_VIEW
  operator std::basic_string_view<value_type, traits_type>() const noexcept {
    return {data(), size()};
  }
#endif

  // C++11 21.4.3 iterators:
  CharType* begin();
 // iterator begin() { return store_.mutableData(); }

  //const_iterator begin() const;
  const_iterator begin() const { return store_.data(); }

  const_iterator cbegin() const { return begin(); }

  iterator end() { return store_.mutableData() + store_.size(); }

  const_iterator end() const { return store_.data() + store_.size(); }

  const_iterator cend() const { return end(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator crbegin() const { return rbegin(); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crend() const { return rend(); }

  // Added by C++11
  // C++11 21.4.5, element access:
  const value_type& front() const { return *begin(); }
  const value_type& back() const {
    assert(!empty());
    // Should be begin()[size() - 1], but that branches twice
    return *(end() - 1);
  }
  value_type& front() { return *begin(); }
  value_type& back() {
    assert(!empty());
    // Should be begin()[size() - 1], but that branches twice
    return *(end() - 1);
  }
  void pop_back() {
    assert(!empty());
    store_.shrink(1);
  }

  // C++11 21.4.4 capacity:
  size_type size() const { return store_.size(); }

  size_type length() const { return size(); }

  size_type max_size() const { return std::numeric_limits<size_type>::max(); }

  void resize(size_type n, value_type c = value_type());

  size_type capacity() const { return store_.capacity(); }

  void reserve(size_type res_arg = 0) {
    enforce<std::length_error>(res_arg <= max_size(), "");
    store_.reserve(res_arg);
  }

  void shrink_to_fit() {
    // Shrink only if slack memory is sufficiently large
    if (capacity() < size() * 3 / 2) {
      return;
    }
    TString(cbegin(), cend()).swap(*this);
  }

  void clear() { resize(0); }

  bool empty() const { return size() == 0; }

  // C++11 21.4.5 element access:
  const_reference operator[](size_type pos) const { return *(begin() + pos); }

  reference operator[](size_type pos) { return *(begin() + pos); }

  const_reference at(size_type n) const {
    enforce<std::out_of_range>(n < size(), "");
    return (*this)[n];
  }

  reference at(size_type n) {
    enforce<std::out_of_range>(n < size(), "");
    return (*this)[n];
  }

  // C++11 21.4.6 modifiers:
  TString& operator+=(const TString& str) { return append(str); }

  TString& operator+=(const value_type* s) { return append(s); }

  TString& operator+=(const value_type c) {
    push_back(c);
    return *this;
  }

  TString& operator+=(std::initializer_list<value_type> il) {
    append(il);
    return *this;
  }

  TString& append(const TString& str);

  TString& append(
      const TString& str, const size_type pos, size_type n);

  TString& append(const value_type* s, size_type n);

  TString& append(const value_type* s) {
    return append(s, traitsLength(s));
  }

  TString& append(size_type n, value_type c);

  template <class InputIterator>
  TString& append(InputIterator first, InputIterator last) {
    insert(end(), first, last);
    return *this;
  }

  TString& append(std::initializer_list<value_type> il) {
    return append(il.begin(), il.end());
  }

  void push_back(const value_type c) { // primitive
    store_.push_back(c);
  }

  TString& assign(const TString& str) {
    if (&str == this) {
      return *this;
    }
    return assign(str.data(), str.size());
  }

  TString& assign(TString&& str) {
    return *this = std::move(str);
  }

  TString& assign(
      const TString& str, const size_type pos, size_type n);

  TString& assign(const value_type* s, const size_type n);

  TString& assign(const value_type* s) {
    return assign(s, traitsLength(s));
  }

  TString& assign(std::initializer_list<value_type> il) {
    return assign(il.begin(), il.end());
  }

  template <class ItOrLength, class ItOrChar>
  TString& assign(ItOrLength first_or_n, ItOrChar last_or_c) {
    return replace(begin(), end(), first_or_n, last_or_c);
  }

  TString& insert(size_type pos1, const TString& str) {
    return insert(pos1, str.data(), str.size());
  }

  TString& insert(
      size_type pos1, const TString& str, size_type pos2, size_type n) {
    enforce<std::out_of_range>(pos2 <= str.length(), "");
    procrustes(n, str.length() - pos2);
    return insert(pos1, str.data() + pos2, n);
  }

  TString& insert(size_type pos, const value_type* s, size_type n) {
    enforce<std::out_of_range>(pos <= length(), "");
    insert(begin() + pos, s, s + n);
    return *this;
  }

  TString& insert(size_type pos, const value_type* s) {
    return insert(pos, s, traitsLength(s));
  }

  TString& insert(size_type pos, size_type n, value_type c) {
    enforce<std::out_of_range>(pos <= length(), "");
    insert(begin() + pos, n, c);
    return *this;
  }

  iterator insert(const_iterator p, const value_type c) {
    const size_type pos = p - cbegin();
    insert(p, 1, c);
    return begin() + pos;
  }

 private:
  typedef std::basic_istream<value_type, traits_type> istream_type;
  istream_type& getlineImpl(istream_type& is, value_type delim);

 public:
  friend inline istream_type& getline(
      istream_type& is, TString& str, value_type delim) {
    return str.getlineImpl(is, delim);
  }

  friend inline istream_type& getline(istream_type& is, TString& str) {
    return getline(is, str, '\n');
  }

 private:
  iterator insertImplDiscr(
      const_iterator i, size_type n, value_type c, std::true_type);

  template <class InputIter>
  iterator insertImplDiscr(
      const_iterator i, InputIter b, InputIter e, std::false_type);

  template <class FwdIterator>
  iterator insertImpl(
      const_iterator i,
      FwdIterator s1,
      FwdIterator s2,
      std::forward_iterator_tag);

  template <class InputIterator>
  iterator insertImpl(
      const_iterator i,
      InputIterator b,
      InputIterator e,
      std::input_iterator_tag);

 public:
  template <class ItOrLength, class ItOrChar>
  iterator insert(const_iterator p, ItOrLength first_or_n, ItOrChar last_or_c) {
    using Sel = bool_constant<std::numeric_limits<ItOrLength>::is_specialized>;
    return insertImplDiscr(p, first_or_n, last_or_c, Sel());
  }

  iterator insert(const_iterator p, std::initializer_list<value_type> il) {
    return insert(p, il.begin(), il.end());
  }

  TString& erase(size_type pos = 0, size_type n = npos) {
    Invariant checker(*this);

    enforce<std::out_of_range>(pos <= length(), "");
    procrustes(n, length() - pos);
    std::copy(begin() + pos + n, end(), begin() + pos);
    resize(length() - n);
    return *this;
  }

  iterator erase(iterator position) {
    const size_type pos(position - begin());
    enforce<std::out_of_range>(pos <= size(), "");
    erase(pos, 1);
    return begin() + pos;
  }

  iterator erase(iterator first, iterator last) {
    const size_type pos(first - begin());
    erase(pos, last - first);
    return begin() + pos;
  }

  // Replaces at most n1 chars of *this, starting with pos1 with the
  // content of str
  TString& replace(
      size_type pos1, size_type n1, const TString& str) {
    return replace(pos1, n1, str.data(), str.size());
  }

  // Replaces at most n1 chars of *this, starting with pos1,
  // with at most n2 chars of str starting with pos2
  TString& replace(
      size_type pos1,
      size_type n1,
      const TString& str,
      size_type pos2,
      size_type n2) {
    enforce<std::out_of_range>(pos2 <= str.length(), "");
    return replace(
        pos1, n1, str.data() + pos2, std::min(n2, str.size() - pos2));
  }

  // Replaces at most n1 chars of *this, starting with pos, with chars from s
  TString& replace(size_type pos, size_type n1, const value_type* s) {
    return replace(pos, n1, s, traitsLength(s));
  }

  // Replaces at most n1 chars of *this, starting with pos, with n2
  // occurrences of c
  //
  // consolidated with
  //
  // Replaces at most n1 chars of *this, starting with pos, with at
  // most n2 chars of str.  str must have at least n2 chars.
  template <class StrOrLength, class NumOrChar>
  TString& replace(
      size_type pos, size_type n1, StrOrLength s_or_n2, NumOrChar n_or_c) {
    Invariant checker(*this);

    enforce<std::out_of_range>(pos <= size(), "");
    procrustes(n1, length() - pos);
    const iterator b = begin() + pos;
    return replace(b, b + n1, s_or_n2, n_or_c);
  }

  TString& replace(iterator i1, iterator i2, const TString& str) {
    return replace(i1, i2, str.data(), str.length());
  }

  TString& replace(iterator i1, iterator i2, const value_type* s) {
    return replace(i1, i2, s, traitsLength(s));
  }

 private:
  TString& replaceImplDiscr(
      iterator i1,
      iterator i2,
      const value_type* s,
      size_type n,
      std::integral_constant<int, 2>);

  TString& replaceImplDiscr(
      iterator i1,
      iterator i2,
      size_type n2,
      value_type c,
      std::integral_constant<int, 1>);

  template <class InputIter>
  TString& replaceImplDiscr(
      iterator i1,
      iterator i2,
      InputIter b,
      InputIter e,
      std::integral_constant<int, 0>);

 private:
  template <class FwdIterator>
  bool replaceAliased(
      iterator /* i1 */,
      iterator /* i2 */,
      FwdIterator /* s1 */,
      FwdIterator /* s2 */,
      std::false_type) {
    return false;
  }

  template <class FwdIterator>
  bool replaceAliased(
      iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::true_type);

  template <class FwdIterator>
  void replaceImpl(
      iterator i1,
      iterator i2,
      FwdIterator s1,
      FwdIterator s2,
      std::forward_iterator_tag);

  template <class InputIterator>
  void replaceImpl(
      iterator i1,
      iterator i2,
      InputIterator b,
      InputIterator e,
      std::input_iterator_tag);

 public:
  template <class T1, class T2>
  TString& replace(
      iterator i1, iterator i2, T1 first_or_n_or_s, T2 last_or_c_or_n) {
    constexpr bool num1 = std::numeric_limits<T1>::is_specialized,
                   num2 = std::numeric_limits<T2>::is_specialized;
    using Sel =
        std::integral_constant<int, num1 ? (num2 ? 1 : -1) : (num2 ? 2 : 0)>;
    return replaceImplDiscr(i1, i2, first_or_n_or_s, last_or_c_or_n, Sel());
  }

  size_type copy(value_type* s, size_type n, size_type pos = 0) const {
    enforce<std::out_of_range>(pos <= size(), "");
    procrustes(n, size() - pos);

    if (n != 0) {
      fbstring_detail::podCopy(data() + pos, data() + pos + n, s);
    }
    return n;
  }

  void swap(TString& rhs) { store_.swap(rhs.store_); }

  const value_type* c_str() const { return store_.c_str(); }

  const value_type* data() const { return c_str(); }

  value_type* data() { return store_.data(); }

  allocator_type get_allocator() const { return allocator_type(); }

  size_type find(const TString& str, size_type pos = 0) const {
    return find(str.data(), pos, str.length());
  }

  size_type find(
      const value_type* needle, size_type pos, size_type nsize) const;

  size_type find(const value_type* s, size_type pos = 0) const {
    return find(s, pos, traitsLength(s));
  }

  size_type find(value_type c, size_type pos = 0) const {
    return find(&c, pos, 1);
  }

  size_type rfind(const TString& str, size_type pos = npos) const {
    return rfind(str.data(), pos, str.length());
  }

  size_type rfind(const value_type* s, size_type pos, size_type n) const;

  size_type rfind(const value_type* s, size_type pos = npos) const {
    return rfind(s, pos, traitsLength(s));
  }

  size_type rfind(value_type c, size_type pos = npos) const {
    return rfind(&c, pos, 1);
  }

  size_type find_first_of(const TString& str, size_type pos = 0) const {
    return find_first_of(str.data(), pos, str.length());
  }

  size_type find_first_of(
      const value_type* s, size_type pos, size_type n) const;

  size_type find_first_of(const value_type* s, size_type pos = 0) const {
    return find_first_of(s, pos, traitsLength(s));
  }

  size_type find_first_of(value_type c, size_type pos = 0) const {
    return find_first_of(&c, pos, 1);
  }

  size_type find_last_of(
      const TString& str, size_type pos = npos) const {
    return find_last_of(str.data(), pos, str.length());
  }

  size_type find_last_of(const value_type* s, size_type pos, size_type n) const;

  size_type find_last_of(const value_type* s, size_type pos = npos) const {
    return find_last_of(s, pos, traitsLength(s));
  }

  size_type find_last_of(value_type c, size_type pos = npos) const {
    return find_last_of(&c, pos, 1);
  }

  size_type find_first_not_of(
      const TString& str, size_type pos = 0) const {
    return find_first_not_of(str.data(), pos, str.size());
  }

  size_type find_first_not_of(
      const value_type* s, size_type pos, size_type n) const;

  size_type find_first_not_of(const value_type* s, size_type pos = 0) const {
    return find_first_not_of(s, pos, traitsLength(s));
  }

  size_type find_first_not_of(value_type c, size_type pos = 0) const {
    return find_first_not_of(&c, pos, 1);
  }

  size_type find_last_not_of(
      const TString& str, size_type pos = npos) const {
    return find_last_not_of(str.data(), pos, str.length());
  }

  size_type find_last_not_of(
      const value_type* s, size_type pos, size_type n) const;

  size_type find_last_not_of(const value_type* s, size_type pos = npos) const {
    return find_last_not_of(s, pos, traitsLength(s));
  }

  size_type find_last_not_of(value_type c, size_type pos = npos) const {
    return find_last_not_of(&c, pos, 1);
  }

  TString substr(size_type pos = 0, size_type n = npos) const& {
    enforce<std::out_of_range>(pos <= size(), "");
    return TString(data() + pos, std::min(n, size() - pos));
  }

  TString substr(size_type pos = 0, size_type n = npos) && {
    enforce<std::out_of_range>(pos <= size(), "");
    erase(0, pos);
    if (n < size()) {
      resize(n);
    }
    return std::move(*this);
  }

  int compare(const TString& str) const {
    // FIX due to Goncalo N M de Carvalho July 18, 2005
    return compare(0, size(), str);
  }

  int compare(size_type pos1, size_type n1, const TString& str) const {
    return compare(pos1, n1, str.data(), str.size());
  }

  int compare(size_type pos1, size_type n1, const value_type* s) const {
    return compare(pos1, n1, s, traitsLength(s));
  }

  int compare(
      size_type pos1, size_type n1, const value_type* s, size_type n2) const {
    enforce<std::out_of_range>(pos1 <= size(), "");
    procrustes(n1, size() - pos1);
    // The line below fixed by Jean-Francois Bastien, 04-23-2007. Thanks!
    const int r = traits_type::compare(pos1 + data(), s, std::min(n1, n2));
    return r != 0 ? r : n1 > n2 ? 1 : n1 < n2 ? -1 : 0;
  }

  int compare(
      size_type pos1,
      size_type n1,
      const TString& str,
      size_type pos2,
      size_type n2) const {
    enforce<std::out_of_range>(pos2 <= str.size(), "");
    return compare(
        pos1, n1, str.data() + pos2, std::min(n2, str.size() - pos2));
  }

  // Code from Jean-Francois Bastien (03/26/2007)
  int compare(const value_type* s) const {
    // Could forward to compare(0, size(), s, traitsLength(s))
    // but that does two extra checks
    const size_type n1(size()), n2(traitsLength(s));
    const int r = traits_type::compare(data(), s, std::min(n1, n2));
    return r != 0 ? r : n1 > n2 ? 1 : n1 < n2 ? -1 : 0;
  }

 private:
  // Data
  Storage store_;
};



template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::operator=(
    const TString& lhs) {
  Invariant checker(*this);

  if (FOLLY_UNLIKELY(&lhs == this)) {
    return *this;
  }

  return assign(lhs.data(), lhs.size());
}

// Move assignment
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::operator=(
    TString&& goner) noexcept {
  if (FOLLY_UNLIKELY(&goner == this)) {
    // Compatibility with std::basic_string<>,
    // C++11 21.4.2 [string.cons] / 23 requires self-move-assignment support.
    return *this;
  }
  // No need of this anymore
  this->~basic_fbstring();
  // Move the goner into this
  new (&store_) S(std::move(goner.store_));
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::operator=(
    value_type c) {
  Invariant checker(*this);

  if (empty()) {
    store_.expandNoinit(1);
  } else if (store_.isShared()) {
    basic_fbstring(1, c).swap(*this);
    return *this;
  } else {
    store_.shrink(size() - 1);
  }
  front() = c;
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline void TString<CharType, TraitType, Allocator, S>::resize(
    const size_type n, const value_type c /*= value_type()*/) {
  Invariant checker(*this);

  auto size = this->size();
  if (n <= size) {
    store_.shrink(size - n);
  } else {
    auto const delta = n - size;
    auto pData = store_.expandNoinit(delta);
    fbstring_detail::podFill(pData, pData + delta, c);
  }
  assert(this->size() == n);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::append(
    const TString& str) {
#ifndef NDEBUG
  auto desiredSize = size() + str.size();
#endif
  append(str.data(), str.size());
  assert(size() == desiredSize);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::append(
    const TString& str, const size_type pos, size_type n) {
  const size_type sz = str.size();
  enforce<std::out_of_range>(pos <= sz, "");
  procrustes(n, sz - pos);
  return append(str.data() + pos, n);
}

template <typename CharType, class TraitType, class Allocator, class S>
FOLLY_NOINLINE TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::append(
    const value_type* s, size_type n) {
  Invariant checker(*this);

  if (FOLLY_UNLIKELY(!n)) {
    // Unlikely but must be done
    return *this;
  }
  auto const oldSize = size();
  auto const oldData = data();
  auto pData = store_.expandNoinit(n, /* expGrowth = */ true);

  // Check for aliasing (rare). We could use "<=" here but in theory
  // those do not work for pointers unless the pointers point to
  // elements in the same array. For that reason we use
  // std::less_equal, which is guaranteed to offer a total order
  // over pointers. See discussion at http://goo.gl/Cy2ya for more
  // info.
  std::less_equal<const value_type*> le;
  if (FOLLY_UNLIKELY(le(oldData, s) && !le(oldData + oldSize, s))) {
    assert(le(s + n, oldData + oldSize));
    // expandNoinit() could have moved the storage, restore the source.
    s = data() + (s - oldData);
    folly::fbstring_detail::podMove(s, s + n, pData);
  } else {
   folly:: fbstring_detail::podCopy(s, s + n, pData);
  }

  assert(size() == oldSize + n);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::append(
    size_type n, value_type c) {
  Invariant checker(*this);
  auto pData = store_.expandNoinit(n, /* expGrowth = */ true);
  fbstring_detail::podFill(pData, pData + n, c);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::assign(
    const TString& str, const size_type pos, size_type n) {
  const size_type sz = str.size();
  enforce<std::out_of_range>(pos <= sz, "");
  procrustes(n, sz - pos);
  return assign(str.data() + pos, n);
}

template <typename CharType, class TraitType, class Allocator, class S>
FOLLY_NOINLINE TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::assign(
    const value_type* s, const size_type n) {
  Invariant checker(*this);

  if (n == 0) {
    resize(0);
  } else if (size() >= n) {
    // s can alias this, we need to use podMove.
    fbstring_detail::podMove(s, s + n, store_.mutableData());
    store_.shrink(size() - n);
    assert(size() == n);
  } else {
    // If n is larger than size(), s cannot alias this string's
    // storage.
    resize(0);
    // Do not use exponential growth here: assign() should be tight,
    // to mirror the behavior of the equivalent constructor.
    fbstring_detail::podCopy(s, s + n, store_.expandNoinit(n));
  }

  assert(size() == n);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::istream_type&
TString<CharType, TraitType, Allocator, S>::getlineImpl(istream_type& is, value_type delim) {
  Invariant checker(*this);

  clear();
  size_t size = 0;
  while (true) {
    size_t avail = capacity() - size;
    // fbstring has 1 byte extra capacity for the null terminator,
    // and getline null-terminates the read string.
    is.getline(store_.expandNoinit(avail), avail + 1, delim);
    size += is.gcount();

    if (is.bad() || is.eof() || !is.fail()) {
      // Done by either failure, end of file, or normal read.
      if (!is.bad() && !is.eof()) {
        --size; // gcount() also accounts for the delimiter.
      }
      resize(size);
      break;
    }

    assert(size == this->size());
    assert(size == capacity());
    // Start at minimum allocation 63 + terminator = 64.
    reserve(std::max<size_t>(63, 3 * size / 2));
    // Clear the error so we can continue reading.
    is.clear();
  }
  return is;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::find(
    const value_type* needle,
    const size_type pos,
    const size_type nsize) const {
  auto const size = this->size();
  // nsize + pos can overflow (eg pos == npos), guard against that by checking
  // that nsize + pos does not wrap around.
  if (nsize + pos > size || nsize + pos < pos) {
    return npos;
  }

  if (nsize == 0) {
    return pos;
  }
  // Don't use std::search, use a Boyer-Moore-like trick by comparing
  // the last characters first
  auto const haystack = data();
  auto const nsize_1 = nsize - 1;
  auto const lastNeedle = needle[nsize_1];

  // Boyer-Moore skip value for the last char in the needle. Zero is
  // not a valid value; skip will be computed the first time it's
  // needed.
  size_type skip = 0;

  const CharType* i = haystack + pos;
  auto iEnd = haystack + size - nsize_1;

  while (i < iEnd) {
    // Boyer-Moore: match the last element in the needle
    while (i[nsize_1] != lastNeedle) {
      if (++i == iEnd) {
        // not found
        return npos;
      }
    }
    // Here we know that the last char matches
    // Continue in pedestrian mode
    for (size_t j = 0;;) {
      assert(j < nsize);
      if (i[j] != needle[j]) {
        // Not found, we can skip
        // Compute the skip value lazily
        if (skip == 0) {
          skip = 1;
          while (skip <= nsize_1 && needle[nsize_1 - skip] != lastNeedle) {
            ++skip;
          }
        }
        i += skip;
        break;
      }
      // Check if done searching
      if (++j == nsize) {
        // Yay
        return i - haystack;
      }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::iterator
TString<CharType, TraitType, Allocator, S>::insertImplDiscr(
    const_iterator i, size_type n, value_type c, std::true_type) {
  Invariant checker(*this);

  assert(i >= cbegin() && i <= cend());
  const size_type pos = i - cbegin();

  auto oldSize = size();
  store_.expandNoinit(n, /* expGrowth = */ true);
  auto b = begin();
  fbstring_detail::podMove(b + pos, b + oldSize, b + pos + n);
  fbstring_detail::podFill(b + pos, b + pos + n, c);

  return b + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIter>
inline typename TString<CharType, TraitType, Allocator, S>::iterator
TString<CharType, TraitType, Allocator, S>::insertImplDiscr(
    const_iterator i, InputIter b, InputIter e, std::false_type) {
  return insertImpl(
      i, b, e, typename std::iterator_traits<InputIter>::iterator_category());
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline typename TString<CharType, TraitType, Allocator, S>::iterator
TString<CharType, TraitType, Allocator, S>::insertImpl(
    const_iterator i,
    FwdIterator s1,
    FwdIterator s2,
    std::forward_iterator_tag) {
  Invariant checker(*this);

  assert(i >= cbegin() && i <= cend());
  const size_type pos = i - cbegin();
  auto n = std::distance(s1, s2);
  assert(n >= 0);

  auto oldSize = size();
  store_.expandNoinit(n, /* expGrowth = */ true);
  auto b = begin();
  fbstring_detail::podMove(b + pos, b + oldSize, b + pos + n);
  std::copy(s1, s2, b + pos);

  return b + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIterator>
inline typename TString<CharType, TraitType, Allocator, S>::iterator
TString<CharType, TraitType, Allocator, S>::insertImpl(
    const_iterator i,
    InputIterator b,
    InputIterator e,
    std::input_iterator_tag) {
  const auto pos = i - cbegin();
  TString temp(cbegin(), i);
  for (; b != e; ++b) {
    temp.push_back(*b);
  }
  temp.append(i, cend());
  swap(temp);
  return begin() + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1,
    iterator i2,
    const value_type* s,
    size_type n,
    std::integral_constant<int, 2>) {
  assert(i1 <= i2);
  assert(begin() <= i1 && i1 <= end());
  assert(begin() <= i2 && i2 <= end());
  return replace(i1, i2, s, s + n);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1,
    iterator i2,
    size_type n2,
    value_type c,
    std::integral_constant<int, 1>) {
  const size_type n1 = i2 - i1;
  if (n1 > n2) {
    std::fill(i1, i1 + n2, c);
    erase(i1 + n2, i2);
  } else {
    std::fill(i1, i2, c);
    insert(i2, n2 - n1, c);
  }
  assert(isSane());
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIter>
inline TString<CharType, TraitType, Allocator, S>& TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1,
    iterator i2,
    InputIter b,
    InputIter e,
    std::integral_constant<int, 0>) {
  using Cat = typename std::iterator_traits<InputIter>::iterator_category;
  replaceImpl(i1, i2, b, e, Cat());
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline bool TString<CharType, TraitType, Allocator, S>::replaceAliased(
    iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::true_type) {
  std::less_equal<const value_type*> le{};
  const bool aliased = le(&*begin(), &*s1) && le(&*s1, &*end());
  if (!aliased) {
    return false;
  }
  // Aliased replace, copy to new string
  TString temp;
  temp.reserve(size() - (i2 - i1) + std::distance(s1, s2));
  temp.append(begin(), i1).append(s1, s2).append(i2, end());
  swap(temp);
  return true;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline void TString<CharType, TraitType, Allocator, S>::replaceImpl(
    iterator i1,
    iterator i2,
    FwdIterator s1,
    FwdIterator s2,
    std::forward_iterator_tag) {
  Invariant checker(*this);

  // Handle aliased replace
  using Sel = bool_constant<
      std::is_same<FwdIterator, iterator>::value ||
      std::is_same<FwdIterator, const_iterator>::value>;
  if (replaceAliased(i1, i2, s1, s2, Sel())) {
    return;
  }

  auto const n1 = i2 - i1;
  assert(n1 >= 0);
  auto const n2 = std::distance(s1, s2);
  assert(n2 >= 0);

  if (n1 > n2) {
    // shrinks
    std::copy(s1, s2, i1);
    erase(i1 + n2, i2);
  } else {
    // grows
    s1 = fbstring_detail::copy_n(s1, n1, i1).first;
    insert(i2, s1, s2);
  }
  assert(isSane());
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIterator>
inline void TString<CharType, TraitType, Allocator, S>::replaceImpl(
    iterator i1,
    iterator i2,
    InputIterator b,
    InputIterator e,
    std::input_iterator_tag) {
  TString temp(begin(), i1);
  temp.append(b, e).append(i2, end());
  swap(temp);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::rfind(
    const value_type* s, size_type pos, size_type n) const {
  if (n > length()) {
    return npos;
  }
  pos = std::min(pos, length() - n);
  if (n == 0) {
    return pos;
  }

  const_iterator i(begin() + pos);
  for (;; --i) {
    if (traits_type::eq(*i, *s) && traits_type::compare(&*i, s, n) == 0) {
      return i - begin();
    }
    if (i == begin()) {
      break;
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::find_first_of(
    const value_type* s, size_type pos, size_type n) const {
  if (pos > length() || n == 0) {
    return npos;
  }
  const_iterator i(begin() + pos), finish(end());
  for (; i != finish; ++i) {
    if (traits_type::find(s, n, *i) != nullptr) {
      return i - begin();
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::find_last_of(
    const value_type* s, size_type pos, size_type n) const {
  if (!empty() && n > 0) {
    pos = std::min(pos, length() - 1);
    const_iterator i(begin() + pos);
    for (;; --i) {
      if (traits_type::find(s, n, *i) != nullptr) {
        return i - begin();
      }
      if (i == begin()) {
        break;
      }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::find_first_not_of(
    const value_type* s, size_type pos, size_type n) const {
  if (pos < length()) {
    const_iterator i(begin() + pos), finish(end());
    for (; i != finish; ++i) {
      if (traits_type::find(s, n, *i) == nullptr) {
        return i - begin();
      }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::find_last_not_of(
    const value_type* s, size_type pos, size_type n) const {
  if (!this->empty()) {
    pos = std::min(pos, size() - 1);
    const_iterator i(begin() + pos);
    for (;; --i) {
      if (traits_type::find(s, n, *i) == nullptr) {
        return i - begin();
      }
      if (i == begin()) {
        break;
      }
    }
  }
  return npos;
}

// non-member functions
// C++11 21.4.8.1/1
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  TString<CharType, TraitType, Allocator, S> result;
  result.reserve(lhs.size() + rhs.size());
  result.append(lhs).append(rhs);
  return result;
}

// C++11 21.4.8.1/2
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    TString<CharType, TraitType, Allocator, S>&& lhs, const TString<CharType, TraitType, Allocator, S>& rhs) {
  return std::move(lhs.append(rhs));
}

// C++11 21.4.8.1/3
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const TString<CharType, TraitType, Allocator, S>& lhs, TString<CharType, TraitType, Allocator, S>&& rhs) {
  if (rhs.capacity() >= lhs.size() + rhs.size()) {
    // Good, at least we don't need to reallocate
    return std::move(rhs.insert(0, lhs));
  }
  // Meh, no go. Forward to operator+(const&, const&).
  auto const& rhsC = rhs;
  return lhs + rhsC;
}

// C++11 21.4.8.1/4
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    TString<CharType, TraitType, Allocator, S>&& lhs, TString<CharType, TraitType, Allocator, S>&& rhs) {
  return std::move(lhs.append(rhs));
}

// C++11 21.4.8.1/5
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const CharType* lhs, const TString<CharType, TraitType, Allocator, S>& rhs) {
  //
  TString<CharType, TraitType, Allocator, S> result;
  const auto len = TString<CharType, TraitType, Allocator, S>::traits_type::length(lhs);
  result.reserve(len + rhs.size());
  result.append(lhs, len).append(rhs);
  return result;
}

// C++11 21.4.8.1/6
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const CharType* lhs, TString<CharType, TraitType, Allocator, S>&& rhs) {
  //
  const auto len = TString<CharType, TraitType, Allocator, S>::traits_type::length(lhs);
  if (rhs.capacity() >= len + rhs.size()) {
    // Good, at least we don't need to reallocate
    rhs.insert(rhs.begin(), lhs, lhs + len);
    return std::move(rhs);
  }
  // Meh, no go. Do it by hand since we have len already.
  TString<CharType, TraitType, Allocator, S> result;
  result.reserve(len + rhs.size());
  result.append(lhs, len).append(rhs);
  return result;
}

// C++11 21.4.8.1/7
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    CharType lhs, const TString<CharType, TraitType, Allocator, S>& rhs) {
  TString<CharType, TraitType, Allocator, S> result;
  result.reserve(1 + rhs.size());
  result.push_back(lhs);
  result.append(rhs);
  return result;
}

// C++11 21.4.8.1/8
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    CharType lhs, TString<CharType, TraitType, Allocator, S>&& rhs) {
  //
  if (rhs.capacity() > rhs.size()) {
    // Good, at least we don't need to reallocate
    rhs.insert(rhs.begin(), lhs);
    return std::move(rhs);
  }
  // Meh, no go. Forward to operator+(E, const&).
  auto const& rhsC = rhs;
  return lhs + rhsC;
}

// C++11 21.4.8.1/9
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const TString<CharType, TraitType, Allocator, S>& lhs, const CharType* rhs) {
  typedef typename TString<CharType, TraitType, Allocator, S>::size_type size_type;
  typedef typename TString<CharType, TraitType, Allocator, S>::traits_type traits_type;

  TString<CharType, TraitType, Allocator, S> result;
  const size_type len = traits_type::length(rhs);
  result.reserve(lhs.size() + len);
  result.append(lhs).append(rhs, len);
  return result;
}

// C++11 21.4.8.1/10
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    TString<CharType, TraitType, Allocator, S>&& lhs, const CharType* rhs) {
  //
  return std::move(lhs += rhs);
}

// C++11 21.4.8.1/11
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    const TString<CharType, TraitType, Allocator, S>& lhs, CharType rhs) {
  TString<CharType, TraitType, Allocator, S> result;
  result.reserve(lhs.size() + 1);
  result.append(lhs);
  result.push_back(rhs);
  return result;
}

// C++11 21.4.8.1/12
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(
    TString<CharType, TraitType, Allocator, S>&& lhs, CharType rhs) {
  //
  return std::move(lhs += rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs == lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return lhs.compare(rhs) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return lhs.compare(rhs) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return lhs.compare(rhs) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs.compare(lhs) > 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const typename TString<CharType, TraitType, Allocator, S>::value_type* rhs) {
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(
    const typename TString<CharType, TraitType, Allocator, S>::value_type* lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs < rhs);
}

// C++11 21.4.8.8
template <typename CharType, class TraitType, class Allocator, class S>
void swap(TString<CharType, TraitType, Allocator, S>& lhs, TString<CharType, TraitType, Allocator, S>& rhs) {
  lhs.swap(rhs);
}

// TODO: make this faster.
template <typename CharType, class TraitType, class Allocator, class S>
inline std::basic_istream<
    typename TString<CharType, TraitType, Allocator, S>::value_type,
    typename TString<CharType, TraitType, Allocator, S>::traits_type>&
operator>>(
    std::basic_istream<
        typename TString<CharType, TraitType, Allocator, S>::value_type,
        typename TString<CharType, TraitType, Allocator, S>::traits_type>& is,
    TString<CharType, TraitType, Allocator, S>& str) {
  typedef std::basic_istream<
      typename TString<CharType, TraitType, Allocator, S>::value_type,
      typename TString<CharType, TraitType, Allocator, S>::traits_type>
      _istream_type;
  typename _istream_type::sentry sentry(is);
  size_t extracted = 0;
  typename _istream_type::iostate err = _istream_type::goodbit;
  if (sentry) {
    auto n = is.width();
    if (n <= 0) {
      n = str.max_size();
    }
    str.erase();
    for (auto got = is.rdbuf()->sgetc(); extracted != size_t(n); ++extracted) {
      if (got == TraitType::eof()) {
        err |= _istream_type::eofbit;
        is.width(0);
        break;
      }
      if (isspace(got)) {
        break;
      }
      str.push_back(got);
      got = is.rdbuf()->snextc();
    }
  }
  if (!extracted) {
    err |= _istream_type::failbit;
  }
  if (err) {
    is.setstate(err);
  }
  return is;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline std::basic_ostream<
    typename TString<CharType, TraitType, Allocator, S>::value_type,
    typename TString<CharType, TraitType, Allocator, S>::traits_type>&
operator<<(
    std::basic_ostream<
        typename TString<CharType, TraitType, Allocator, S>::value_type,
        typename TString<CharType, TraitType, Allocator, S>::traits_type>& os,
    const TString<CharType, TraitType, Allocator, S>& str) {
#if _LIBCPP_VERSION
  typedef std::basic_ostream<
      typename TString<CharType, TraitType, Allocator, S>::value_type,
      typename TString<CharType, TraitType, Allocator, S>::traits_type>
      _ostream_type;
  typename _ostream_type::sentry _s(os);
  if (_s) {
    typedef std::ostreambuf_iterator<
        typename TString<CharType, TraitType, Allocator, S>::value_type,
        typename TString<CharType, TraitType, Allocator, S>::traits_type>
        _Ip;
    size_t __len = str.size();
    bool __left =
        (os.flags() & _ostream_type::adjustfield) == _ostream_type::left;
    if (__pad_and_output(
            _Ip(os),
            str.data(),
            __left ? str.data() + __len : str.data(),
            str.data() + __len,
            os,
            os.fill())
            .failed()) {
      os.setstate(_ostream_type::badbit | _ostream_type::failbit);
    }
  }
#elif defined(_MSC_VER)
  typedef decltype(os.precision()) streamsize;
  // MSVC doesn't define __ostream_insert
  os.write(str.data(), static_cast<streamsize>(str.size()));
#else
  std::__ostream_insert(os, str.data(), str.size());
#endif
  return os;
}

template <typename E1, class TraitType, class Allocator, class S>
constexpr typename TString<E1, TraitType, Allocator, S>::size_type
    TString<E1, TraitType, Allocator, S>::npos;

// basic_string compatibility routines

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator==(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator==(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs == lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator!=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator!=(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) > 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs > lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return !(lhs > rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>=(
    const TString<CharType, TraitType, Allocator, S>& lhs,
    const std::basic_string<CharType, TraitType, A2>& rhs) {
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<=(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs > rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>=(
    const std::basic_string<CharType, TraitType, A2>& lhs,
    const TString<CharType, TraitType, Allocator, S>& rhs) {
  return !(lhs < rhs);
}

typedef TString<char> fbstring;

// fbstring is relocatable
//template <class TraitType, class R, class Allocator, class S>
//FOLLY_ASSUME_RELOCATABLE(String<TraitType, R, Allocator, S>);

// Compatibility function, to make sure toStdString(s) can be called
// to convert a std::string or fbstring variable s into type std::string
// with very little overhead if s was already std::string
inline std::string toStdString(const folly::fbstring& s) {
  return std::string(s.data(), s.size());
}

inline const std::string& toStdString(const std::string& s) {
  return s;
}

// If called with a temporary, the compiler will select this overload instead
// of the above, so we don't return a (lvalue) reference to a temporary.
inline std::string&& toStdString(std::string&& s) {
  return std::move(s);
}


using TestString = TString<char>;


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
