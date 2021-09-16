
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
using namespace folly;

template <typename CharType,
          class TraitType = std::char_traits<CharType>,
          class Allocator = std::allocator<CharType>,
          class Storage   = folly::fbstring_core<CharType>>
class TString
{
private:
  // Data
  Storage storage;

public:
  // types
  typedef TraitType                                                  traits_type;
  typedef typename traits_type::char_type                            value_type;
  typedef Allocator                                                  allocator_type;
  typedef typename std::allocator_traits<Allocator>::size_type       size_type;
  typedef typename std::allocator_traits<Allocator>::difference_type difference_type;

  typedef typename std::allocator_traits<Allocator>::value_type &      reference;
  typedef typename std::allocator_traits<Allocator>::value_type const &const_reference;
  typedef typename std::allocator_traits<Allocator>::pointer           pointer;
  typedef typename std::allocator_traits<Allocator>::const_pointer     const_pointer;

  // typedef CharType* iterator;
  using iterator = CharType *;
  typedef const CharType *                      const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static constexpr size_type npos = size_type(-1);
  typedef std::true_type     IsRelocatable;

private:
  static void procrustes(size_type &n, size_type nmax);

  static size_type traitsLength(const value_type *s);

public:
  TString() noexcept;

  explicit TString(const Allocator &) noexcept;

  TString(const TString &str);

  // Move constructor
  TString(TString &&goner) noexcept;

  // This is defined for compatibility with std::string
  template <typename A2>
  TString(const std::basic_string<CharType, TraitType, A2> &str) : storage(str.data(), str.size())
  {}

  TString(const TString &str, size_type pos, size_type n = npos, const Allocator &  = Allocator());

  FOLLY_NOINLINE
  TString(const value_type *s, const Allocator & /*a*/ = Allocator());

  FOLLY_NOINLINE
  TString(const value_type *s, size_type n, const Allocator & /*a*/ = Allocator());

  FOLLY_NOINLINE
  TString(size_type n, value_type c, const Allocator & /*a*/ = Allocator());

  template <class InIt>
  FOLLY_NOINLINE TString(
      InIt begin,
      InIt end,
      typename std::enable_if<!std::is_same<InIt, value_type *>::value, const Allocator>::type & /*a*/
      = Allocator())
  {
    Assign(begin, end);
  }

  // Specialization for const char*, const char*
  FOLLY_NOINLINE
  TString(const value_type *b, const value_type *e, const Allocator & /*a*/ = Allocator()) :
      storage(b, size_type(e - b))
  {}

  // Nonstandard constructor
  TString(value_type *s, size_type n, size_type c, folly::AcquireMallocatedString a) : storage(s, n, c, a) {}

  // Construction from initialization list
  FOLLY_NOINLINE
  TString(std::initializer_list<value_type> il) { Assign(il.begin(), il.end()); }

  ~TString() noexcept {}

  TString &operator=(const TString &lhs);

  // Move assignment
  TString &operator=(TString &&goner) noexcept;

  // Compatibility with std::string
  template <typename A2>
  TString &operator=(const std::basic_string<CharType, TraitType, A2> &rhs)
  {
    return Assign(rhs.data(), rhs.size());
  }

  // Compatibility with std::string
  std::basic_string<CharType, TraitType, Allocator> ToStdString() const
  {
    return std::basic_string<CharType, TraitType, Allocator>(Data(), Size());
  }

  TString &operator=(const value_type *s) { return Assign(s); }

  TString &operator=(value_type c);

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
      std::is_convertible<TP, typename TString<CharType, TraitType, Allocator, Storage>::value_type>::value
          && !std::is_same<typename std::decay<TP>::type,
                           typename TString<CharType, TraitType, Allocator, Storage>::value_type>::value,
      TString<CharType, TraitType, Allocator, Storage> &>::type
  operator=(TP c)
      = delete;

  TString &operator=(std::initializer_list<value_type> il) { return Assign(il.begin(), il.end()); }

#if FOLLY_HAS_STRING_VIEW
  operator std::basic_string_view<value_type, traits_type>() const noexcept { return {Data(), Size()}; }
#endif
public:
    iterator Begin() { return storage.mutableData(); }

    iterator End() { return storage.mutableData() + storage.size(); }
    public:
    iterator begin() { return storage.mutableData(); }

  // C++11 21.4.3 iterators:

  // const_iterator begin() const;
  const_iterator begin() const { return storage.data(); }



  iterator end() { return storage.mutableData() + storage.size(); }

  const_iterator end() const { return storage.data() + storage.size(); }

  private:
  const_iterator cbegin() const { return begin(); }

  const_iterator cend() const { return end(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  const_reverse_iterator crbegin() const { return rbegin(); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  const_reverse_iterator crend() const { return rend(); }

  public:
  // Added by C++11
  // C++11 21.4.5, element access:
  const value_type &Front() const { return *begin(); }

  const value_type &Back() const
  {
    assert(!Empty());
    // Should be begin()[size() - 1], but that branches twice
    return *(end() - 1);
  }
  value_type &Front() { return *begin(); }
  value_type &Back()
  {
    assert(!Empty());
    // Should be begin()[size() - 1], but that branches twice
    return *(end() - 1);
  }
  void PopBack()
  {
    assert(!Empty());
    storage.shrink(1);
  }

  // C++11 21.4.4 capacity:
  size_type Size() const { return storage.size(); }

  size_type Length() const { return Size(); }

  size_type MaxSize() const { return std::numeric_limits<size_type>::max(); }

  void Resize(size_type n, value_type c = value_type());

  size_type Capacity() const { return storage.capacity(); }

  void Reserve(size_type res_arg = 0)
  {
    enforce<std::length_error>(res_arg <= MaxSize(), "");
    storage.reserve(res_arg);
  }

  void ShrinkToFit()
  {
    // Shrink only if slack memory is sufficiently large
    if (Capacity() < Size() * 3 / 2) { return; }
    TString(cbegin(), cend()).Swap(*this);
  }

  void Clear() { Resize(0); }

  bool Empty() const { return Size() == 0; }

  // C++11 21.4.5 element access:
  const_reference operator[](size_type pos) const { return *(begin() + pos); }

  reference operator[](size_type pos) { return *(begin() + pos); }

  const_reference At(size_type n) const
  {
    enforce<std::out_of_range>(n < Size(), "");
    return (*this)[n];
  }

  reference At(size_type n)
  {
    enforce<std::out_of_range>(n < Size(), "");
    return (*this)[n];
  }

  // C++11 21.4.6 modifiers:
  TString &operator+=(const TString &str) { return Append(str); }

  TString &operator+=(const value_type *s) { return Append(s); }

  TString &operator+=(const value_type c)
  {
    PushBack(c);
    return *this;
  }

  TString &operator+=(std::initializer_list<value_type> il)
  {
    Append(il);
    return *this;
  }

  TString &Append(const TString &str);

  TString &Append(const TString &str, const size_type pos, size_type n);

  TString &Append(const value_type *s, size_type n);

  TString &Append(const value_type *s) { return Append(s, traitsLength(s)); }

  TString &Append(size_type n, value_type c);

  template <class InputIterator>
  TString &Append(InputIterator first, InputIterator last)
  {
    Insert(end(), first, last);
    return *this;
  }

  TString &Append(std::initializer_list<value_type> il) { return Append(il.begin(), il.end()); }

  void PushBack(const value_type c)
  { // primitive
    storage.push_back(c);
  }

  TString &Assign(const TString &str)
  {
    if (&str == this) { return *this; }
    return Assign(str.Data(), str.Size());
  }

  TString &Assign(TString &&str) { return *this = std::move(str); }

  TString &Assign(const TString &str, const size_type pos, size_type n);

  TString &Assign(const value_type *s, const size_type n);

  TString &Assign(const value_type *s) { return Assign(s, traitsLength(s)); }

  TString &Assign(std::initializer_list<value_type> il) { return Assign(il.begin(), il.end()); }

  template <class ItOrLength, class ItOrChar>
  TString &Assign(ItOrLength first_or_n, ItOrChar last_or_c)
  {
    return Replace(begin(), end(), first_or_n, last_or_c);
  }

  TString &Insert(size_type pos1, const TString &str) { return Insert(pos1, str.Data(), str.Size()); }

  TString &Insert(size_type pos1, const TString &str, size_type pos2, size_type n)
  {
    enforce<std::out_of_range>(pos2 <= str.Length(), "");
    procrustes(n, str.Length() - pos2);
    return Insert(pos1, str.Data() + pos2, n);
  }

  TString &Insert(size_type pos, const value_type *s, size_type n)
  {
    enforce<std::out_of_range>(pos <= Length(), "");
    Insert(begin() + pos, s, s + n);
    return *this;
  }

  TString &Insert(size_type pos, const value_type *s) { return Insert(pos, s, traitsLength(s)); }

  TString &Insert(size_type pos, size_type n, value_type c)
  {
    enforce<std::out_of_range>(pos <= Length(), "");
    Insert(begin() + pos, n, c);
    return *this;
  }

  iterator Insert(const_iterator p, const value_type c)
  {
    const size_type pos = p - cbegin();
    Insert(p, 1, c);
    return begin() + pos;
  }

private:
  typedef std::basic_istream<value_type, traits_type> istream_type;
  istream_type &                                      GetlLineImpl(istream_type &is, value_type delim);

public:
  friend inline istream_type &getline(istream_type &is, TString &str, value_type delim)
  {
    return str.GetlLineImpl(is, delim);
  }

  friend inline istream_type &getline(istream_type &is, TString &str) { return getline(is, str, '\n'); }

private:
  iterator InsertImplDiscr(const_iterator i, size_type n, value_type c, std::true_type);

  template <class InputIter>
  iterator InsertImplDiscr(const_iterator i, InputIter b, InputIter e, std::false_type);

  template <class FwdIterator>
  iterator InsertImpl(const_iterator i, FwdIterator s1, FwdIterator s2, std::forward_iterator_tag);

  template <class InputIterator>
  iterator InsertImpl(const_iterator i, InputIterator b, InputIterator e, std::input_iterator_tag);

public:
  template <class ItOrLength, class ItOrChar>
  iterator Insert(const_iterator p, ItOrLength first_or_n, ItOrChar last_or_c)
  {
    using Sel = bool_constant<std::numeric_limits<ItOrLength>::is_specialized>;
    return InsertImplDiscr(p, first_or_n, last_or_c, Sel());
  }

  iterator Insert(const_iterator p, std::initializer_list<value_type> il) { return Insert(p, il.begin(), il.end()); }

  TString &Erase(size_type pos = 0, size_type n = npos)
  {
    Invariant checker(*this);

    enforce<std::out_of_range>(pos <= Length(), "");
    procrustes(n, Length() - pos);
    std::copy(begin() + pos + n, end(), begin() + pos);
    Resize(Length() - n);
    return *this;
  }

  iterator Erase(iterator position)
  {
    const size_type pos(position - begin());
    enforce<std::out_of_range>(pos <= Size(), "");
    Erase(pos, 1);
    return begin() + pos;
  }

  iterator Erase(iterator first, iterator last)
  {
    const size_type pos(first - begin());
    Erase(pos, last - first);
    return begin() + pos;
  }

  // Replaces at most n1 chars of *this, starting with pos1 with the
  // content of str
  TString &Replace(size_type pos1, size_type n1, const TString &str)
  {
    return Replace(pos1, n1, str.Data(), str.Size());
  }

  // Replaces at most n1 chars of *this, starting with pos1,
  // with at most n2 chars of str starting with pos2
  TString &Replace(size_type pos1, size_type n1, const TString &str, size_type pos2, size_type n2)
  {
    enforce<std::out_of_range>(pos2 <= str.Length(), "");
    return Replace(pos1, n1, str.Data() + pos2, std::min(n2, str.Size() - pos2));
  }

  // Replaces at most n1 chars of *this, starting with pos, with chars from s
  TString &Replace(size_type pos, size_type n1, const value_type *s) { return Replace(pos, n1, s, traitsLength(s)); }

  // Replaces at most n1 chars of *this, starting with pos, with n2
  // occurrences of c
  //
  // consolidated with
  //
  // Replaces at most n1 chars of *this, starting with pos, with at
  // most n2 chars of str.  str must have at least n2 chars.
  template <class StrOrLength, class NumOrChar>
  TString &Replace(size_type pos, size_type n1, StrOrLength s_or_n2, NumOrChar n_or_c)
  {
    Invariant checker(*this);

    enforce<std::out_of_range>(pos <= Size(), "");
    procrustes(n1, Length() - pos);
    const iterator b = begin() + pos;
    return Replace(b, b + n1, s_or_n2, n_or_c);
  }

  TString &Replace(iterator i1, iterator i2, const TString &str) { return Replace(i1, i2, str.Data(), str.Length()); }

  TString &Replace(iterator i1, iterator i2, const value_type *s) { return Replace(i1, i2, s, traitsLength(s)); }

private:
  TString &replaceImplDiscr(iterator i1, iterator i2, const value_type *s, size_type n, std::integral_constant<int, 2>);

  TString &replaceImplDiscr(iterator i1, iterator i2, size_type n2, value_type c, std::integral_constant<int, 1>);

  template <class InputIter>
  TString &replaceImplDiscr(iterator i1, iterator i2, InputIter b, InputIter e, std::integral_constant<int, 0>);

private:
  template <class FwdIterator>
  bool ReplaceAliased(iterator /* i1 */, iterator /* i2 */, FwdIterator /* s1 */, FwdIterator /* s2 */, std::false_type)
  {
    return false;
  }

  template <class FwdIterator>
  bool ReplaceAliased(iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::true_type);

  template <class FwdIterator>
  void ReplaceImpl(iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::forward_iterator_tag);

  template <class InputIterator>
  void ReplaceImpl(iterator i1, iterator i2, InputIterator b, InputIterator e, std::input_iterator_tag);

public:
  template <class T1, class T2>
  TString &Replace(iterator i1, iterator i2, T1 first_or_n_or_s, T2 last_or_c_or_n)
  {
    constexpr bool num1 = std::numeric_limits<T1>::is_specialized, num2 = std::numeric_limits<T2>::is_specialized;
    using Sel = std::integral_constant<int, num1 ? (num2 ? 1 : -1) : (num2 ? 2 : 0)>;
    return replaceImplDiscr(i1, i2, first_or_n_or_s, last_or_c_or_n, Sel());
  }

  size_type Copy(value_type *s, size_type n, size_type pos = 0) const
  {
    enforce<std::out_of_range>(pos <= Size(), "");
    procrustes(n, Size() - pos);

    if (n != 0) { fbstring_detail::podCopy(Data() + pos, Data() + pos + n, s); }
    return n;
  }

  void Swap(TString &rhs) { storage.swap(rhs.storage); }

  private:
  const value_type *c_str() const { return storage.c_str(); }

  public:
  const value_type *Data() const { return c_str(); }

  value_type *Data() { return storage.data(); }

  allocator_type get_allocator() const { return allocator_type(); }

  size_type find(const TString &str, size_type pos = 0) const { return find(str.Data(), pos, str.Length()); }

  size_type find(const value_type *needle, size_type pos, size_type nsize) const;

  size_type find(const value_type *s, size_type pos = 0) const { return find(s, pos, traitsLength(s)); }

  size_type find(value_type c, size_type pos = 0) const { return find(&c, pos, 1); }

  size_type rfind(const TString &str, size_type pos = npos) const { return rfind(str.Data(), pos, str.Length()); }

  size_type rfind(const value_type *s, size_type pos, size_type n) const;

  size_type rfind(const value_type *s, size_type pos = npos) const { return rfind(s, pos, traitsLength(s)); }

  size_type rfind(value_type c, size_type pos = npos) const { return rfind(&c, pos, 1); }

  size_type find_first_of(const TString &str, size_type pos = 0) const
  {
    return find_first_of(str.Data(), pos, str.Length());
  }

  size_type find_first_of(const value_type *s, size_type pos, size_type n) const;

  size_type find_first_of(const value_type *s, size_type pos = 0) const
  {
    return find_first_of(s, pos, traitsLength(s));
  }

  size_type find_first_of(value_type c, size_type pos = 0) const { return find_first_of(&c, pos, 1); }

  size_type find_last_of(const TString &str, size_type pos = npos) const
  {
    return find_last_of(str.Data(), pos, str.Length());
  }

  size_type find_last_of(const value_type *s, size_type pos, size_type n) const;

  size_type find_last_of(const value_type *s, size_type pos = npos) const
  {
    return find_last_of(s, pos, traitsLength(s));
  }

  size_type find_last_of(value_type c, size_type pos = npos) const { return find_last_of(&c, pos, 1); }

  size_type find_first_not_of(const TString &str, size_type pos = 0) const
  {
    return find_first_not_of(str.Data(), pos, str.Size());
  }

  size_type find_first_not_of(const value_type *s, size_type pos, size_type n) const;

  size_type find_first_not_of(const value_type *s, size_type pos = 0) const
  {
    return find_first_not_of(s, pos, traitsLength(s));
  }

  size_type find_first_not_of(value_type c, size_type pos = 0) const { return find_first_not_of(&c, pos, 1); }

  size_type find_last_not_of(const TString &str, size_type pos = npos) const
  {
    return find_last_not_of(str.Data(), pos, str.Length());
  }

  size_type find_last_not_of(const value_type *s, size_type pos, size_type n) const;

  size_type find_last_not_of(const value_type *s, size_type pos = npos) const
  {
    return find_last_not_of(s, pos, traitsLength(s));
  }

  size_type find_last_not_of(value_type c, size_type pos = npos) const { return find_last_not_of(&c, pos, 1); }

  TString substr(size_type pos = 0, size_type n = npos) const &
  {
    enforce<std::out_of_range>(pos <= Size(), "");
    return TString(Data() + pos, std::min(n, Size() - pos));
  }

  TString substr(size_type pos = 0, size_type n = npos) &&
  {
    enforce<std::out_of_range>(pos <= Size(), "");
    Erase(0, pos);
    if (n < Size()) { Resize(n); }
    return std::move(*this);
  }

  int compare(const TString &str) const
  {
    // FIX due to Goncalo N M de Carvalho July 18, 2005
    return compare(0, Size(), str);
  }

  int compare(size_type pos1, size_type n1, const TString &str) const
  {
    return compare(pos1, n1, str.Data(), str.Size());
  }

  int compare(size_type pos1, size_type n1, const value_type *s) const { return compare(pos1, n1, s, traitsLength(s)); }

  int compare(size_type pos1, size_type n1, const value_type *s, size_type n2) const
  {
    enforce<std::out_of_range>(pos1 <= Size(), "");
    procrustes(n1, Size() - pos1);
    // The line below fixed by Jean-Francois Bastien, 04-23-2007. Thanks!
    const int r = traits_type::compare(pos1 + Data(), s, std::min(n1, n2));
    return r != 0 ? r : n1 > n2 ? 1 : n1 < n2 ? -1 : 0;
  }

  int compare(size_type pos1, size_type n1, const TString &str, size_type pos2, size_type n2) const
  {
    enforce<std::out_of_range>(pos2 <= str.Size(), "");
    return compare(pos1, n1, str.Data() + pos2, std::min(n2, str.Size() - pos2));
  }

  // Code from Jean-Francois Bastien (03/26/2007)
  int compare(const value_type *s) const
  {
    // Could forward to compare(0, size(), s, traitsLength(s))
    // but that does two extra checks
    const size_type n1(Size()), n2(traitsLength(s));
    const int       r = traits_type::compare(Data(), s, std::min(n1, n2));
    return r != 0 ? r : n1 > n2 ? 1 : n1 < n2 ? -1 : 0;
  }

private:
  static_assert(std::is_same<Allocator, std::allocator<CharType>>::value, "string ignores custom allocators");

  template <typename Ex, typename... Args>
  FOLLY_ALWAYS_INLINE static void enforce(bool condition, Args &&...args)
  {
    if (!condition) { folly::throw_exception<Ex>(static_cast<Args &&>(args)...); }
  }

  bool isSane() const
  {
    return begin() <= end() && Empty() == (Size() == 0) && Empty() == (begin() == end()) && Size() <= MaxSize()
           && Capacity() <= MaxSize() && Size() <= Capacity() && begin()[Size()] == '\0';
  }

  struct Invariant
  {
    Invariant &operator=(const Invariant &) = delete;
    explicit Invariant(const TString &s) noexcept : s_(s) { assert(s_.isSane()); }
    ~Invariant() noexcept { assert(s_.isSane()); }

  private:
    const TString &s_;
  };
};

using String = TString<char>;

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
