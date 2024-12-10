#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

int main()
{
    // 设置全局刷新策略
    spdlog::flush_every(std::chrono::seconds(1)); // 设置刷新间隔是1s
    // 设置立即刷新策略
    spdlog::flush_on(spdlog::level::debug);
    // 设置全局的日志输出等级 （每个日志器可以独立设置）
    spdlog::set_level(spdlog::level::debug); // debug以上等级的日志输出
    // 创建同步日志器 （输出到标准输出/文件）
    // auto logger = spdlog::stdout_color_mt("sync_logger");
    auto logger = spdlog::basic_logger_mt("file_logger", "basic-log.log");
    // 设置指定日志器的刷新策略和日志等级
    // logger->flush_on(spdlog::level::debug);
    // logger->set_level(spdlog::level::debug);
    // 设置日志输出格式
    logger->set_pattern("[%n][%H:%M:%S][%t][%-8l]%v");
    logger->trace("this is a trace {}", "message");
    logger->debug("你好！");
    logger->info("hello world {}", 1);
    logger->warn("this is a warning");
    logger->error("this is an error");
    logger->critical("crash and burn");
    std::cout << "日志输出完毕" << std::endl;
    return 0;
}