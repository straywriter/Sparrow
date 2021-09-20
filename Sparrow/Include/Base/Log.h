#pragma once

#include <External/spdlog.h>
using namespace spdlog;

#include <ctime>
#include <iostream>
#include <memory>
#include <Base/Format.h>
#include <stdio.h>

namespace Sparrow
{
class Console
{

public:
  template <typename... Args>
  inline static void Trace(fmt::format_string<Args...> fmt, Args &&...args)
  {
    // default_logger_raw()->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l] %v")

    default_logger_raw()->trace(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Debug(fmt::format_string<Args...> fmt, Args &&...args)
  {
    default_logger_raw()->debug(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Info(fmt::format_string<Args...> fmt, Args &&...args)
  {
    default_logger_raw()->info(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Warn(fmt::format_string<Args...> fmt, Args &&...args)
  {
    default_logger_raw()->warn(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Error(fmt::format_string<Args...> fmt, Args &&...args)
  {
    default_logger_raw()->error(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Critical(fmt::format_string<Args...> fmt, Args &&...args)
  {
    default_logger_raw()->critical(fmt, std::forward<Args>(args)...);
  }
};

class Log
{
private:
  static std::shared_ptr<spdlog::logger> file_log;
  // = []() -> std::shared_ptr<spdlog::logger>
  // { return std::move(spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt")); }();

public:
  template <typename... Args>
  inline static void Info(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->info(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Trace(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->trace(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Debug(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->debug(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Warn(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->warn(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Error(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->error(fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline static void Critical(fmt::format_string<Args...> fmt, Args &&...args)
  {
    file_log->critical(fmt, std::forward<Args>(args)...);
  }

public:
};

std::shared_ptr<spdlog::logger> Log::file_log = []() -> std::shared_ptr<spdlog::logger>
{
  auto temp_time = std::time(nullptr);

  auto temp_log = spdlog::create_async_nb<spdlog::sinks::basic_file_sink_mt>(
      "Log", Sparrow::Format("Logs/{:%Y-%m-%d %H%M%S}.log", *std::localtime(&temp_time)));
  temp_log->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%=10l] %v");
// #ifdef DEBUG
  temp_log->set_level(spdlog::level::debug);
// #else
//   temp_log->set_level(spdlog::level::info);
// #endif
  return std::move(temp_log);
}();

} // namespace Sparrow
