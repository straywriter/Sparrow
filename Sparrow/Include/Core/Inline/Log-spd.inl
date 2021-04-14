#pragma once
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

Sparrow::ConsoleLogger::ConsoleLogger()
{
    console = spdlog::stdout_color_mt("console");
    // console->set_pattern("%+");

    console->set_level(spdlog::level::trace);
    console->flush_on(spdlog::level::trace);
}
Sparrow::ConsoleLogger::~ConsoleLogger()
{
}

Sparrow::ExternalLogger::ExternalLogger()
{
    external = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
}

Sparrow::ExternalLogger::~ExternalLogger()
{
}