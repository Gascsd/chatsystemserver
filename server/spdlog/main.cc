#include "logger.hpp"
#include <gflags/gflags.h>

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    
    LOG_DEBUG("这是一个 {} 信息", "debug");
    LOG_INFO("这是一个 {} 信息", "info");
    LOG_WARN("这是一个 {} 信息", "warning");
    LOG_ERROR("这是一个 {} 信息", "error");
    LOG_FATAL("这是一个 {} 信息", "fatal");

    return 0;
}