
#include "Core/Log.h"

using namespace Sparrow;
#include <memory>

int main()
{

    //  auto  console = spdlog::stdout_color_mt("console");
    // console->set_pattern("%^[%T] %n: %v%$");
    OUT_INFO("women bushi wer");

    LOG_WARN("wefasdf");

    // spdlog::register_logger(console);
    // console->set_level(spdlog::level::trace);
    // console->flush_on(spdlog::level::trace);

    // console->info("afads");

    //   auto l = spdlog::stdout_color_st("test");
    //     l->set_pattern("%+");
    //     l->info("Test stdout_color_st");
    //     spdlog::drop_all();
}