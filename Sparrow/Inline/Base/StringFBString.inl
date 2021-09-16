#include "..\..\Include\Base\String.h"
// #include
//#include <Base/String.h>



template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(const TString &str) : storage(str.storage)
{}

template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(const value_type *s,
                                                                   const Allocator & /*a*/ /*= Allocator()*/) :
    storage(s, traitsLength(s))
{}

template <typename CharType,
          class TraitType, //*= std::char_traits<CharType>*/
          class Allocator, //*= std::allocator<CharType>*/
          class Storage>   //*= folly::fbstring_core<CharType>*/
Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(TString &&goner) noexcept :
    storage(std::move(goner.storage))
{}

template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(const TString &str,
                                                                   size_type      pos,
                                                                   size_type      n /*= npos*/,
                                                                   const Allocator & /* a */ /*= Allocator()*/)
{
  Assign(str, pos, n);
}

template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(const value_type *s,
                                                                   size_type         n,
                                                                   const Allocator & /*a*/ /*= Allocator()*/) :
    storage(s, n)
{}

template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
FOLLY_NOINLINE Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(
    size_type n, value_type c, const Allocator & /*a*/ /*= Allocator()*/)
{
  auto const pData = storage.expandNoinit(n);
  fbstring_detail::podFill(pData, pData + n, c);
}

template <typename CharType,
          class TraitType /*= std::char_traits<CharType>*/,
          class Allocator /*= std::allocator<CharType>*/,
          class Storage /*= folly::fbstring_core<CharType>*/>
void Sparrow::TString<CharType, TraitType, Allocator, Storage>::procrustes(size_type &n, size_type nmax)
{
  if (n > nmax) { n = nmax; }
}

// template <typename CharType,
//           class TraitType /*= std::char_traits<CharType>*/,
//           class Allocator /*= std::allocator<CharType>*/,
//           class Storage /*= folly::fbstring_core<CharType>*/>
// Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString() noexcept : TString(Allocator())
// {}

template <typename CharType, class TraitType, class Allocator, class Storage>
inline Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString() noexcept : TString(Allocator())
{}

template <typename CharType, class TraitType, class Allocator, class Storage>
inline Sparrow::TString<CharType, TraitType, Allocator, Storage>::TString(const Allocator &) noexcept
{}

namespace Sparrow
{

template <typename CharType, class TraitType, class Allocator, class S>
FOLLY_NOINLINE typename TString<CharType, TraitType, Allocator, S>::size_type
TString<CharType, TraitType, Allocator, S>::traitsLength(const value_type *s)
{
  return s ? traits_type::length(s)
           : (folly::throw_exception<std::logic_error>("basic_fbstring: null pointer initializer not valid"), 0);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::operator=(
    const TString &lhs)
{
  Invariant checker(*this);

  if (FOLLY_UNLIKELY(&lhs == this)) { return *this; }

  return Assign(lhs.data(), lhs.size());
}

// Move assignment
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::operator=(
    TString &&goner) noexcept
{
  if (FOLLY_UNLIKELY(&goner == this))
  {
    // Compatibility with std::basic_string<>,
    // C++11 21.4.2 [string.cons] / 23 requires self-move-assignment support.
    return *this;
  }
  // No need of this anymore
  this->~TString();
  // Move the goner into this
  new (&storage) S(std::move(goner.storage));
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::operator=(value_type c)
{
  Invariant checker(*this);

  if (Empty()) { storage.expandNoinit(1); }
  else if (storage.isShared())
  {
    basic_fbstring(1, c).swap(*this);
    return *this;
  }
  else
  {
    storage.shrink(Size() - 1);
  }
  Front() = c;
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline void TString<CharType, TraitType, Allocator, S>::Resize(const size_type n, const value_type c /*= value_type()*/)
{
  Invariant checker(*this);

  auto size = this->Size();
  if (n <= size) { storage.shrink(size - n); }
  else
  {
    auto const delta = n - size;
    auto       pData = storage.expandNoinit(delta);
    folly::fbstring_detail::podFill(pData, pData + delta, c);
  }
  assert(this->Size() == n);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Append(
    const TString &str)
{
#ifndef NDEBUG
  auto desiredSize = Size() + str.Size();
#endif
  Append(str.Data(), str.Size());
  assert(Size() == desiredSize);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Append(
    const TString &str, const size_type pos, size_type n)
{
  const size_type sz = str.size();
  enforce<std::out_of_range>(pos <= sz, "");
  procrustes(n, sz - pos);
  return Append(str.data() + pos, n);
}

template <typename CharType, class TraitType, class Allocator, class S>
FOLLY_NOINLINE TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Append(
    const value_type *s, size_type n)
{
  Invariant checker(*this);

  if (FOLLY_UNLIKELY(!n))
  {
    // Unlikely but must be done
    return *this;
  }
  auto const oldSize = Size();
  auto const oldData = Data();
  auto       pData   = storage.expandNoinit(n, /* expGrowth = */ true);

  // Check for aliasing (rare). We could use "<=" here but in theory
  // those do not work for pointers unless the pointers point to
  // elements in the same array. For that reason we use
  // std::less_equal, which is guaranteed to offer a total order
  // over pointers. See discussion at http://goo.gl/Cy2ya for more
  // info.
  std::less_equal<const value_type *> le;
  if (FOLLY_UNLIKELY(le(oldData, s) && !le(oldData + oldSize, s)))
  {
    assert(le(s + n, oldData + oldSize));
    // expandNoinit() could have moved the storage, restore the source.
    s = Data() + (s - oldData);
    folly::fbstring_detail::podMove(s, s + n, pData);
  }
  else
  {
    folly::fbstring_detail::podCopy(s, s + n, pData);
  }

  assert(Size() == oldSize + n);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Append(size_type  n,
                                                                                                      value_type c)
{
  Invariant checker(*this);
  auto      pData = storage.expandNoinit(n, /* expGrowth = */ true);
  fbstring_detail::podFill(pData, pData + n, c);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Assign(
    const TString &str, const size_type pos, size_type n)
{
  const size_type sz = str.size();
  enforce<std::out_of_range>(pos <= sz, "");
  procrustes(n, sz - pos);
  return Assign(str.data() + pos, n);
}

template <typename CharType, class TraitType, class Allocator, class S>
FOLLY_NOINLINE TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::Assign(
    const value_type *s, const size_type n)
{
  Invariant checker(*this);

  if (n == 0) { Resize(0); }
  else if (Size() >= n)
  {
    // s can alias this, we need to use podMove.
    fbstring_detail::podMove(s, s + n, storage.mutableData());
    storage.shrink(Size() - n);
    assert(Size() == n);
  }
  else
  {
    // If n is larger than size(), s cannot alias this string's
    // storage.
    Resize(0);
    // Do not use exponential growth here: assign() should be tight,
    // to mirror the behavior of the equivalent constructor.
    fbstring_detail::podCopy(s, s + n, storage.expandNoinit(n));
  }

  assert(Size() == n);
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::istream_type &TString<CharType, TraitType, Allocator, S>::
    GetlLineImpl(istream_type &is, value_type delim)
{
  Invariant checker(*this);

  Clear();
  size_t size = 0;
  while (true)
  {
    size_t avail = Capacity() - size;
    // fbstring has 1 byte extra capacity for the null terminator,
    // and getline null-terminates the read string.
    is.getline(storage.expandNoinit(avail), avail + 1, delim);
    size += is.gcount();

    if (is.bad() || is.eof() || !is.fail())
    {
      // Done by either failure, end of file, or normal read.
      if (!is.bad() && !is.eof())
      {
        --size; // gcount() also accounts for the delimiter.
      }
      Resize(size);
      break;
    }

    assert(size == this->Size());
    assert(size == Capacity());
    // Start at minimum allocation 63 + terminator = 64.
    Reserve(std::max<size_t>(63, 3 * size / 2));
    // Clear the error so we can continue reading.
    is.clear();
  }
  return is;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::find(
    const value_type *needle, const size_type pos, const size_type nsize) const
{
  auto const size = this->Size();
  // nsize + pos can overflow (eg pos == npos), guard against that by checking
  // that nsize + pos does not wrap around.
  if (nsize + pos > size || nsize + pos < pos) { return npos; }

  if (nsize == 0) { return pos; }
  // Don't use std::search, use a Boyer-Moore-like trick by comparing
  // the last characters first
  auto const haystack   = Data();
  auto const nsize_1    = nsize - 1;
  auto const lastNeedle = needle[nsize_1];

  // Boyer-Moore skip value for the last char in the needle. Zero is
  // not a valid value; skip will be computed the first time it's
  // needed.
  size_type skip = 0;

  const CharType *i    = haystack + pos;
  auto            iEnd = haystack + size - nsize_1;

  while (i < iEnd)
  {
    // Boyer-Moore: match the last element in the needle
    while (i[nsize_1] != lastNeedle)
    {
      if (++i == iEnd)
      {
        // not found
        return npos;
      }
    }
    // Here we know that the last char matches
    // Continue in pedestrian mode
    for (size_t j = 0;;)
    {
      assert(j < nsize);
      if (i[j] != needle[j])
      {
        // Not found, we can skip
        // Compute the skip value lazily
        if (skip == 0)
        {
          skip = 1;
          while (skip <= nsize_1 && needle[nsize_1 - skip] != lastNeedle)
          {
            ++skip;
          }
        }
        i += skip;
        break;
      }
      // Check if done searching
      if (++j == nsize)
      {
        // Yay
        return i - haystack;
      }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::iterator TString<CharType, TraitType, Allocator, S>::
    InsertImplDiscr(const_iterator i, size_type n, value_type c, std::true_type)
{
  Invariant checker(*this);

  assert(i >= cbegin() && i <= cend());
  const size_type pos = i - cbegin();

  auto oldSize = Size();
  storage.expandNoinit(n, /* expGrowth = */ true);
  auto b = begin();
  fbstring_detail::podMove(b + pos, b + oldSize, b + pos + n);
  fbstring_detail::podFill(b + pos, b + pos + n, c);

  return b + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIter>
inline typename TString<CharType, TraitType, Allocator, S>::iterator TString<CharType, TraitType, Allocator, S>::
    InsertImplDiscr(const_iterator i, InputIter b, InputIter e, std::false_type)
{
  return InsertImpl(i, b, e, typename std::iterator_traits<InputIter>::iterator_category());
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline typename TString<CharType, TraitType, Allocator, S>::iterator TString<CharType, TraitType, Allocator, S>::
    InsertImpl(const_iterator i, FwdIterator s1, FwdIterator s2, std::forward_iterator_tag)
{
  Invariant checker(*this);

  assert(i >= cbegin() && i <= cend());
  const size_type pos = i - cbegin();
  auto            n   = std::distance(s1, s2);
  assert(n >= 0);

  auto oldSize = Size();
  storage.expandNoinit(n, /* expGrowth = */ true);
  auto b = begin();
  folly::fbstring_detail::podMove(b + pos, b + oldSize, b + pos + n);
  std::copy(s1, s2, b + pos);

  return b + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIterator>
inline typename TString<CharType, TraitType, Allocator, S>::iterator TString<CharType, TraitType, Allocator, S>::
    InsertImpl(const_iterator i, InputIterator b, InputIterator e, std::input_iterator_tag)
{
  const auto pos = i - cbegin();
  TString    temp(cbegin(), i);
  for (; b != e; ++b)
  {
    temp.push_back(*b);
  }
  temp.Append(i, cend());
  swap(temp);
  return begin() + pos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1, iterator i2, const value_type *s, size_type n, std::integral_constant<int, 2>)
{
  assert(i1 <= i2);
  assert(begin() <= i1 && i1 <= end());
  assert(begin() <= i2 && i2 <= end());
  return Replace(i1, i2, s, s + n);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1, iterator i2, size_type n2, value_type c, std::integral_constant<int, 1>)
{
  const size_type n1 = i2 - i1;
  if (n1 > n2)
  {
    std::fill(i1, i1 + n2, c);
    Erase(i1 + n2, i2);
  }
  else
  {
    std::fill(i1, i2, c);
    Insert(i2, n2 - n1, c);
  }
  assert(isSane());
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIter>
inline TString<CharType, TraitType, Allocator, S> &TString<CharType, TraitType, Allocator, S>::replaceImplDiscr(
    iterator i1, iterator i2, InputIter b, InputIter e, std::integral_constant<int, 0>)
{
  using Cat = typename std::iterator_traits<InputIter>::iterator_category;
  ReplaceImpl(i1, i2, b, e, Cat());
  return *this;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline bool TString<CharType, TraitType, Allocator, S>::ReplaceAliased(
    iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::true_type)
{
  std::less_equal<const value_type *> le {};
  const bool                          aliased = le(&*begin(), &*s1) && le(&*s1, &*end());
  if (!aliased) { return false; }
  // Aliased replace, copy to new string
  TString temp;
  temp.Reserve(Size() - (i2 - i1) + std::distance(s1, s2));
  temp.Append(begin(), i1).Append(s1, s2).Append(i2, end());
  Swap(temp);
  return true;
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class FwdIterator>
inline void TString<CharType, TraitType, Allocator, S>::ReplaceImpl(
    iterator i1, iterator i2, FwdIterator s1, FwdIterator s2, std::forward_iterator_tag)
{
  Invariant checker(*this);

  // Handle aliased replace
  using Sel
      = bool_constant<std::is_same<FwdIterator, iterator>::value || std::is_same<FwdIterator, const_iterator>::value>;
  if (ReplaceAliased(i1, i2, s1, s2, Sel())) { return; }

  auto const n1 = i2 - i1;
  assert(n1 >= 0);
  auto const n2 = std::distance(s1, s2);
  assert(n2 >= 0);

  if (n1 > n2)
  {
    // shrinks
    std::copy(s1, s2, i1);
    Erase(i1 + n2, i2);
  }
  else
  {
    // grows
    s1 = folly::fbstring_detail::copy_n(s1, n1, i1).first;
    Insert(i2, s1, s2);
  }
  assert(isSane());
}

template <typename CharType, class TraitType, class Allocator, class S>
template <class InputIterator>
inline void TString<CharType, TraitType, Allocator, S>::ReplaceImpl(
    iterator i1, iterator i2, InputIterator b, InputIterator e, std::input_iterator_tag)
{
  TString temp(begin(), i1);
  temp.Append(b, e).Append(i2, end());
  swap(temp);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::rfind(
    const value_type *s, size_type pos, size_type n) const
{
  if (n > Length()) { return npos; }
  pos = std::min(pos, Length() - n);
  if (n == 0) { return pos; }

  const_iterator i(begin() + pos);
  for (;; --i)
  {
    if (traits_type::eq(*i, *s) && traits_type::compare(&*i, s, n) == 0) { return i - begin(); }
    if (i == begin()) { break; }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::
    find_first_of(const value_type *s, size_type pos, size_type n) const
{
  if (pos > Length() || n == 0) { return npos; }
  const_iterator i(begin() + pos), finish(end());
  for (; i != finish; ++i)
  {
    if (traits_type::find(s, n, *i) != nullptr) { return i - begin(); }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::
    find_last_of(const value_type *s, size_type pos, size_type n) const
{
  if (!Empty() && n > 0)
  {
    pos = std::min(pos, Length() - 1);
    const_iterator i(begin() + pos);
    for (;; --i)
    {
      if (traits_type::find(s, n, *i) != nullptr) { return i - begin(); }
      if (i == begin()) { break; }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::
    find_first_not_of(const value_type *s, size_type pos, size_type n) const
{
  if (pos < Length())
  {
    const_iterator i(begin() + pos), finish(end());
    for (; i != finish; ++i)
    {
      if (traits_type::find(s, n, *i) == nullptr) { return i - begin(); }
    }
  }
  return npos;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline typename TString<CharType, TraitType, Allocator, S>::size_type TString<CharType, TraitType, Allocator, S>::
    find_last_not_of(const value_type *s, size_type pos, size_type n) const
{
  if (!this->Empty())
  {
    pos = std::min(pos, Size() - 1);
    const_iterator i(begin() + pos);
    for (;; --i)
    {
      if (traits_type::find(s, n, *i) == nullptr) { return i - begin(); }
      if (i == begin()) { break; }
    }
  }
  return npos;
}

// non-member functions
// C++11 21.4.8.1/1
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const TString<CharType, TraitType, Allocator, S> &lhs,
                                                            const TString<CharType, TraitType, Allocator, S> &rhs)
{
  TString<CharType, TraitType, Allocator, S> result;
  result.Reserve(lhs.Size() + rhs.Size());
  result.Append(lhs).Append(rhs);
  return result;
}

// C++11 21.4.8.1/2
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(TString<CharType, TraitType, Allocator, S> &&     lhs,
                                                            const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return std::move(lhs.Append(rhs));
}

// C++11 21.4.8.1/3
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const TString<CharType, TraitType, Allocator, S> &lhs,
                                                            TString<CharType, TraitType, Allocator, S> &&     rhs)
{
  if (rhs.Capacity() >= lhs.Size() + rhs.Size())
  {
    // Good, at least we don't need to reallocate
    return std::move(rhs.Insert(0, lhs));
  }
  // Meh, no go. Forward to operator+(const&, const&).
  auto const &rhsC = rhs;
  return lhs + rhsC;
}

// C++11 21.4.8.1/4
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(TString<CharType, TraitType, Allocator, S> &&lhs,
                                                            TString<CharType, TraitType, Allocator, S> &&rhs)
{
  return std::move(lhs.Append(rhs));
}

// C++11 21.4.8.1/5
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const CharType *                                  lhs,
                                                            const TString<CharType, TraitType, Allocator, S> &rhs)
{
  //
  TString<CharType, TraitType, Allocator, S> result;
  const auto                                 len = TString<CharType, TraitType, Allocator, S>::traits_type::length(lhs);
  result.Reserve(len + rhs.Size());
  result.Append(lhs, len).Append(rhs);
  return result;
}

// C++11 21.4.8.1/6
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const CharType *                             lhs,
                                                            TString<CharType, TraitType, Allocator, S> &&rhs)
{
  //
  const auto len = TString<CharType, TraitType, Allocator, S>::traits_type::length(lhs);
  if (rhs.Capacity() >= len + rhs.Size())
  {
    // Good, at least we don't need to reallocate
    rhs.Insert(rhs.begin(), lhs, lhs + len);
    return std::move(rhs);
  }
  // Meh, no go. Do it by hand since we have len already.
  TString<CharType, TraitType, Allocator, S> result;
  result.Reserve(len + rhs.Size());
  result.Append(lhs, len).Append(rhs);
  return result;
}

// C++11 21.4.8.1/7
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(CharType                                          lhs,
                                                            const TString<CharType, TraitType, Allocator, S> &rhs)
{
  TString<CharType, TraitType, Allocator, S> result;
  result.Reserve(1 + rhs.Size());
  result.PushBack(lhs);
  result.Append(rhs);
  return result;
}

// C++11 21.4.8.1/8
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(CharType                                     lhs,
                                                            TString<CharType, TraitType, Allocator, S> &&rhs)
{
  //
  if (rhs.Capacity() > rhs.Size())
  {
    // Good, at least we don't need to reallocate
    rhs.Insert(rhs.begin(), lhs);
    return std::move(rhs);
  }
  // Meh, no go. Forward to operator+(E, const&).
  auto const &rhsC = rhs;
  return lhs + rhsC;
}

// C++11 21.4.8.1/9
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const TString<CharType, TraitType, Allocator, S> &lhs,
                                                            const CharType *                                  rhs)
{
  typedef typename TString<CharType, TraitType, Allocator, S>::size_type   size_type;
  typedef typename TString<CharType, TraitType, Allocator, S>::traits_type traits_type;

  TString<CharType, TraitType, Allocator, S> result;
  const size_type                            len = traits_type::length(rhs);
  result.Reserve(lhs.Size() + len);
  result.Append(lhs).Append(rhs, len);
  return result;
}

// C++11 21.4.8.1/10
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(TString<CharType, TraitType, Allocator, S> &&lhs,
                                                            const CharType *                             rhs)
{
  //
  return std::move(lhs += rhs);
}

// C++11 21.4.8.1/11
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(const TString<CharType, TraitType, Allocator, S> &lhs,
                                                            CharType                                          rhs)
{
  TString<CharType, TraitType, Allocator, S> result;
  result.Reserve(lhs.Size() + 1);
  result.Append(lhs);
  result.PushBack(rhs);
  return result;
}

// C++11 21.4.8.1/12
template <typename CharType, class TraitType, class Allocator, class S>
inline TString<CharType, TraitType, Allocator, S> operator+(TString<CharType, TraitType, Allocator, S> &&lhs,
                                                            CharType                                     rhs)
{
  //
  return std::move(lhs += rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return lhs.Size() == rhs.Size() && lhs.compare(rhs) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                       const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return rhs == lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator==(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                       const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return lhs.compare(rhs) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                       const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator!=(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                       const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(const TString<CharType, TraitType, Allocator, S> &lhs,
                      const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return lhs.compare(rhs) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                      const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return lhs.compare(rhs) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                      const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return rhs.compare(lhs) > 0;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(const TString<CharType, TraitType, Allocator, S> &lhs,
                      const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                      const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                      const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                       const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator<=(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                       const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return !(rhs < lhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(const TString<CharType, TraitType, Allocator, S> &                     lhs,
                       const typename TString<CharType, TraitType, Allocator, S>::value_type *rhs)
{
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S>
inline bool operator>=(const typename TString<CharType, TraitType, Allocator, S>::value_type *lhs,
                       const TString<CharType, TraitType, Allocator, S> &                     rhs)
{
  return !(lhs < rhs);
}

// C++11 21.4.8.8
template <typename CharType, class TraitType, class Allocator, class S>
void swap(TString<CharType, TraitType, Allocator, S> &lhs, TString<CharType, TraitType, Allocator, S> &rhs)
{
  lhs.Swap(rhs);
}

// TODO: make this faster.
template <typename CharType, class TraitType, class Allocator, class S>
inline std::basic_istream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                          typename TString<CharType, TraitType, Allocator, S>::traits_type>
    &operator>>(std::basic_istream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                                   typename TString<CharType, TraitType, Allocator, S>::traits_type> &is,
                TString<CharType, TraitType, Allocator, S> &                                          str)
{
  typedef std::basic_istream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                             typename TString<CharType, TraitType, Allocator, S>::traits_type>
                                  _istream_type;
  typename _istream_type::sentry  sentry(is);
  size_t                          extracted = 0;
  typename _istream_type::iostate err       = _istream_type::goodbit;
  if (sentry)
  {
    auto n = is.width();
    if (n <= 0) { n = str.MaxSize(); }
    str.Erase();
    for (auto got = is.rdbuf()->sgetc(); extracted != size_t(n); ++extracted)
    {
      if (got == TraitType::eof())
      {
        err |= _istream_type::eofbit;
        is.width(0);
        break;
      }
      if (isspace(got)) { break; }
      str.PushBack(got);
      got = is.rdbuf()->snextc();
    }
  }
  if (!extracted) { err |= _istream_type::failbit; }
  if (err) { is.setstate(err); }
  return is;
}

template <typename CharType, class TraitType, class Allocator, class S>
inline std::basic_ostream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                          typename TString<CharType, TraitType, Allocator, S>::traits_type>
    &operator<<(std::basic_ostream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                                   typename TString<CharType, TraitType, Allocator, S>::traits_type> &os,
                const TString<CharType, TraitType, Allocator, S> &                                    str)
{
#if _LIBCPP_VERSION
  typedef std::basic_ostream<typename TString<CharType, TraitType, Allocator, S>::value_type,
                             typename TString<CharType, TraitType, Allocator, S>::traits_type>
                                 _ostream_type;
  typename _ostream_type::sentry _s(os);
  if (_s)
  {
    typedef std::ostreambuf_iterator<typename TString<CharType, TraitType, Allocator, S>::value_type,
                                     typename TString<CharType, TraitType, Allocator, S>::traits_type>
           _Ip;
    size_t __len  = str.Size();
    bool   __left = (os.flags() & _ostream_type::adjustfield) == _ostream_type::left;
    if (__pad_and_output(
            _Ip(os), str.Data(), __left ? str.Data() + __len : str.Data(), str.Data() + __len, os, os.fill())
            .failed())
    {
      os.setstate(_ostream_type::badbit | _ostream_type::failbit);
    }
  }
#elif defined(_MSC_VER)
  typedef decltype(os.precision()) streamsize;
  // MSVC doesn't define __ostream_insert
  os.write(str.Data(), static_cast<streamsize>(str.Size()));
#else
  std::__ostream_insert(os, str.Data(), str.Size());
#endif
  return os;
}

template <typename E1, class TraitType, class Allocator, class S>
constexpr typename TString<E1, TraitType, Allocator, S>::size_type TString<E1, TraitType, Allocator, S>::npos;

// basic_string compatibility routines

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator==(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return lhs.compare(0, lhs.Size(), rhs.data(), rhs.size()) == 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator==(const std::basic_string<CharType, TraitType, A2> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return rhs == lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator!=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator!=(const std::basic_string<CharType, TraitType, A2> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(lhs == rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<(const TString<CharType, TraitType, Allocator, S> &lhs,
                      const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return lhs.compare(0, lhs.Size(), rhs.data(), rhs.size()) < 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>(const TString<CharType, TraitType, Allocator, S> &lhs,
                      const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return lhs.compare(0, lhs.Size(), rhs.data(), rhs.size()) > 0;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<(const std::basic_string<CharType, TraitType, A2> &lhs,
                      const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return rhs > lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>(const std::basic_string<CharType, TraitType, A2> &lhs,
                      const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return rhs < lhs;
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return !(lhs > rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>=(const TString<CharType, TraitType, Allocator, S> &lhs,
                       const std::basic_string<CharType, TraitType, A2> &rhs)
{
  return !(lhs < rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator<=(const std::basic_string<CharType, TraitType, A2> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(lhs > rhs);
}

template <typename CharType, class TraitType, class Allocator, class S, class A2>
inline bool operator>=(const std::basic_string<CharType, TraitType, A2> &lhs,
                       const TString<CharType, TraitType, Allocator, S> &rhs)
{
  return !(lhs < rhs);
}

typedef TString<char> fbstring;

// fbstring is relocatable
// template <class TraitType, class R, class Allocator, class S>
// FOLLY_ASSUME_RELOCATABLE(String<TraitType, R, Allocator, S>);

// Compatibility function, to make sure toStdString(s) can be called
// to convert a std::string or fbstring variable s into type std::string
// with very little overhead if s was already std::string
inline std::string toStdString(const folly::fbstring &s) { return std::string(s.data(), s.size()); }

inline const std::string &toStdString(const std::string &s) { return s; }

// If called with a temporary, the compiler will select this overload instead
// of the above, so we don't return a (lvalue) reference to a temporary.
inline std::string &&toStdString(std::string &&s) { return std::move(s); }

} // namespace Sparrow
