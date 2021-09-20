#include <Base/Log.h>

#include <gtest/gtest.h>
using namespace Sparrow;
TEST(String, Rebuild)
{

  Console::Info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  Console::Info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  Console::Warn("Easy padding in numbers like {:08d}", 12);
  Console::Critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  Console::Debug("Support for floats {:03.2f}", 1.23456);
  Console::Info("Positional args are {1} {0}..", "too", "supported");
  Console::Info("{:>8} aligned, {:<8} aligned", "right", "left");

  Log::Info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  Log::Warn("Easy padding in numbers like {:08d}", 12);
  Log::Critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  Log::Info("Support for floats {:03.2f}", 1.23456);
  Log::Debug("Positional args are {1} {0}..", "too", "supported");
  Log::Info("{:>8} aligned, {:<8} aligned", "right", "left");

  // Runtime log levels
}