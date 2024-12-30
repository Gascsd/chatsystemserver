#include "../../../common/data_redis.hpp"
#include "../../../common/logger.hpp"
#include <gflags/gflags.h>
#include <thread>

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(ip, "127.0.0.1", "这是服务器的IP地址,格式:127.0.0.1");
DEFINE_int32(port, 6379, "这是服务器的端口,格式:8080");
DEFINE_int32(db, 0, "库的编号,默认0号");
DEFINE_bool(keep_alive, true, "是否进行长连接保活");

void session_test(std::shared_ptr<sw::redis::Redis>& client)
{
    zht_im::Session session_hdl(client);
    session_hdl.append("会话ID1", "用户ID1");
    session_hdl.append("会话ID2", "用户ID2");
    session_hdl.append("会话ID3", "用户ID3");
    session_hdl.append("会话ID4", "用户ID4");

    session_hdl.remove("会话ID2");
    session_hdl.remove("会话ID3");

    auto res1 = session_hdl.uid("会话ID1");
    if(res1) std::cout << *res1 << std::endl;
    auto res2 = session_hdl.uid("会话ID2");
    if(res2) std::cout << *res2 << std::endl;
    auto res3 = session_hdl.uid("会话ID3");
    if(res3) std::cout << *res3 << std::endl;
    auto res4 = session_hdl.uid("会话ID4");
    if(res4) std::cout << *res4 << std::endl;
}

void status_test(const std::shared_ptr<sw::redis::Redis> &client)
{
    zht_im::Status status_hdl(client);
    status_hdl.append("用户ID1");
    status_hdl.append("用户ID2");
    status_hdl.append("用户ID3");
    status_hdl.append("用户ID4");

    status_hdl.remove("用户ID2");

    if(status_hdl.exists("用户ID1")) std::cout << "用户1在线!" << std::endl; 
    if(status_hdl.exists("用户ID2")) std::cout << "用户2在线!" << std::endl; 
    if(status_hdl.exists("用户ID3")) std::cout << "用户3在线!" << std::endl; 
    if(status_hdl.exists("用户ID4")) std::cout << "用户4在线!" << std::endl; 
}

void Code_test(const std::shared_ptr<sw::redis::Redis> &client)
{
    zht_im::Codes code_hdl(client);

    code_hdl.append("验证码ID1", "验证码1", std::chrono::milliseconds(3000));
    code_hdl.append("验证码ID2", "验证码2", std::chrono::milliseconds(3000));
    code_hdl.append("验证码ID3", "验证码3", std::chrono::milliseconds(3000));

    code_hdl.remove("验证码ID2");

    auto y1 = code_hdl.code("验证码ID1");
    auto y2 = code_hdl.code("验证码ID2");
    auto y3 = code_hdl.code("验证码ID3");
    if(y1) std::cout << *y1 << std::endl;
    if(y2) std::cout << *y2 << std::endl;
    if(y3) std::cout << *y3 << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    auto y4 = code_hdl.code("验证码ID1");
    auto y5 = code_hdl.code("验证码ID2");
    auto y6 = code_hdl.code("验证码ID3");
    if(!y4) std::cout << "验证码ID1不存在" << std::endl;
    if(!y5) std::cout << "验证码ID2不存在" << std::endl;
    if(!y6) std::cout << "验证码ID3不存在" << std::endl;
}

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto client = zht_im::RedisClientFactory::create(FLAGS_ip, FLAGS_port, FLAGS_db, FLAGS_keep_alive);
    session_test(client);
    status_test(client);
    Code_test(client);
    return 0;
}