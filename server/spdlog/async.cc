#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
int main()
{
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::flush_on(spdlog::level::debug);
    spdlog::set_level(spdlog::level::debug);

    // auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("basic_logger", "async-basic.log");
    auto logger = spdlog::stdout_color_mt<spdlog::async_factory>("basic_logger");
    
    logger->set_pattern("[%n][%H:%M:%S][%t][%-8l] %v");

    logger->trace("This is a {} message", "trace");
    logger->debug("This is a {} message", "debug");
    logger->info("This is a {} message", "info");
    logger->warn("This is a {} message", "warn");
    logger->error("This is a {} message", "error");
    logger->critical("This is a {} message", "critical");

    std::cout << "日志输出完毕" << std::endl;
    
    return 0;
}