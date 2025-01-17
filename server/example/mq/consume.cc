#include "../../common/rabbitmq.hpp"
#include <gflags/gflags.h>

DEFINE_string(user, "root", "rabbitmq访问用户名");
DEFINE_string(passwd, "zht1125", "rabbitmq访问密码");
DEFINE_string(host, "127.0.0.1:5672", "rabbitmq访问服务器地址信息 host:port");

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");


void callback(const char* body, size_t size)
{
    std::string msg;
    msg.assign(body, size);
    std::cout << msg << std::endl;
}

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    zht_im::MQClient client(FLAGS_user, FLAGS_passwd, FLAGS_host);

    client.declareComponents("test-exchange", "test-queue");
    client.consume("test-queue", callback);

    std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}