#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace Sparrow
{

template <class T> class Singleton
{

  public:
    static T &GetInstance()
    {
        static T instance;
        return instance;
    }
    constexpr Singleton(const Singleton &) = delete;
    // constexpr Singleton(const Singleton&&)      = delete;

    constexpr Singleton &operator=(const Singleton &) = delete;
    // constexpr Singleton& operator=(const Singleton&&) = delete;

  protected:
    Singleton() = default;
    ~Singleton() = default;
};

class ConsoleLogger
{
  public:
    ConsoleLogger();
    ~ConsoleLogger();

    ConsoleLogger(ConsoleLogger &) = delete;
    void operator=(ConsoleLogger &) = delete;

  public:
    std::shared_ptr<spdlog::logger> console;
};

class ExternalLogger
{
  public:
    ExternalLogger();
    ~ExternalLogger();
    ExternalLogger(ExternalLogger &) = delete;
    void operator=(ExternalLogger &) = delete;

  public:
    std::shared_ptr<spdlog::logger> external;
};

class Log
{
  public:
    static std::shared_ptr<spdlog::logger> &GetConsoleLogger()
    {
        static ConsoleLogger instance;
        return instance.console;
    }
    static std::shared_ptr<spdlog::logger> &GetExternalLogger()
    {
        static ExternalLogger instance;
        return instance.external;
    }

  private:
};

#define OUT_INFO(...) ::Sparrow::Log::GetConsoleLogger()->info(__VA_ARGS__)
#define OUT_WARN(...) ::Sparrow::Log::GetConsoleLogger()->warn(__VA_ARGS__)
#define OUT_TRACE(...) ::Sparrow::Log::GetConsoleLogger()->trace(__VA_ARGS__)
#define OUT_ERROR(...) ::Sparrow::Log::GetConsoleLogger()->error(__VA_ARGS__)
#define OUT_CRITICAL(...) ::Sparrow::Log::GetConsoleLogger()->critical(__VA_ARGS__)

#define LOG_INFO(...) ::Sparrow::Log::GetExternalLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Sparrow::Log::GetExternalLogger()->warn(__VA_ARGS__)
#define LOG_TRACE(...) ::Sparrow::Log::GetExternalLogger()->trace(__VA_ARGS__)
#define LOG_ERROR(...) ::Sparrow::Log::GetExternalLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Sparrow::Log::GetExternalLogger()->critical(__VA_ARGS__)



} // namespace Sparrow
#include "Core/Inline/Log-spd.inl"