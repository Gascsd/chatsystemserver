#include <gflags/gflags.h>
#include <thread>
#include "../common/etcd.hpp"
#include "../common/channel.hpp"
#include "main.pb.h"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(call_service, "/service/echo", "服务监控根目录");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    auto sm = std::make_shared<ServiceManager>();
    sm->declared(FLAGS_call_service);
    auto put_cb = std::bind(&ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    Discovery::ptr dclient = std::make_shared<Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    while (1)
    {
        // 3. 通过RPC信道管理对象，获取提供的echo服务的信道
        auto channel = sm->choose(FLAGS_call_service);
        if (!channel)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return -1;
        }
        // 4. 发起echoRpc调用
        example::EchoService_Stub stub(channel.get());
        example::EchoRequest req;
        req.set_message("你好，世界");
        brpc::Controller *cntl = new brpc::Controller();
        example::EchoResponse *resp = new example::EchoResponse();
        stub.Echo(cntl, &req, resp, nullptr);
        if (cntl->Failed() == true)
        {
            std::cout << "rpc调用失败:" << cntl->ErrorText() << std::endl;
            delete cntl;
            delete resp;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        std::cout << "收到响应：" << resp->message() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 

    std::this_thread::sleep_for(std::chrono::seconds(600));

    return 0;
}