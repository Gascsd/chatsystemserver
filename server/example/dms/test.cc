#include <gflags/gflags.h>
#include "../common/dms.hpp"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(key_id, "xxxxxxxxxxxxxx", "平台访问密钥ID");
DEFINE_string(key_secret, "xxxxxxxxxxxxxxxxx", "平台访问密钥");

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    DMSClient client(FLAGS_key_id, FLAGS_key_secret);
    client.Send("12344445555", "5678");

    return 0;
}