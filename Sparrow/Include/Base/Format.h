#pragma once

#include <External/fmt.h>

namespace Sparrow
{

template <typename... T>
inline auto Format(fmt::format_string<T...> fmt, T&&... args) -> std::string {
  return vformat(fmt, fmt::make_format_args(args...));
}


template <typename... T>
inline void Print(fmt::format_string<T...> fmt, T&&... args) {
  const auto& vargs = fmt::make_format_args(args...);
  return fmt::detail::is_utf8() ? vprint(fmt, vargs)
                           : fmt::detail::vprint_mojibake(stdout, fmt, vargs);
}

template <typename... T>
inline void Print(std::FILE* f, fmt::format_string<T...> fmt, T&&... args) {
  const auto& vargs = fmt::make_format_args(args...);
  return fmt::detail::is_utf8() ? vprint(f, fmt, vargs)
                           : fmt:detail::vprint_mojibake(f, fmt, vargs);
}


/** A formatting error such as invalid format string. */

class  FormatError : public std::runtime_error {
 public:
  explicit FormatError(const char* message) : std::runtime_error(message) {}
  explicit FormatError(const std::string& message)
      : std::runtime_error(message) {}
  FormatError(const FormatError&) = default;
  FormatError& operator=(const FormatError&) = default;
  FormatError(FormatError&&) = default;
  FormatError& operator=(FormatError&&) = default;
  ~FormatError() noexcept override = default;
};


template <typename OutputIt, typename... T,
          FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value)>
FMT_INLINE auto FormatTo(OutputIt out, format_string<T...> fmt, T&&... args)
    -> OutputIt {
  return vformat_to(out, fmt, fmt::make_format_args(args...));
}

template <typename OutputIt, typename... T,
          FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value)>
FMT_INLINE auto FormatToN(OutputIt out, size_t n, format_string<T...> fmt,
                            T&&... args) -> format_to_n_result<OutputIt> {
  return vformat_to_n(out, n, fmt, fmt::make_format_args(args...));
}



} // namespace Sparrow