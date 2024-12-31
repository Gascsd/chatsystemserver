
#include <gflags/gflags.h>
#include <thread>
#include "etcd.hpp"
#include "channel.hpp"
#include "transmite.pb.h"
#include "logger.hpp"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(transmite_service, "/service/transmite_service", "服务监控根目录");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    auto sm = std::make_shared<zht_im::ServiceManager>();
    sm->declared(FLAGS_transmite_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    // 3. 通过RPC信道管理对象，获取提供的echo服务的信道
    auto channel = sm->choose(FLAGS_transmite_service);
    if (!channel)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // 4. 发起echoRpc调用
    zht_im::MsgTransmitService_Stub stub(channel.get());

    zht_im::NewMessageReq req;
    req.set_request_id("消息转发请求测试ID1");
    req.set_user_id("3ff2-7ad3-b7de-0000");
    req.set_chat_session_id("会话ID1");
    req.mutable_message()->set_message_type(zht_im::MessageType::STRING);
    req.mutable_message()->mutable_string_message()->set_content("测试消息内容");

    zht_im::GetTransmitTargetRsp rsp;
    brpc::Controller cntl;
    stub.GetTransmitTarget(&cntl, &req, &rsp, nullptr);
    if (cntl.Failed() == true) {
        std::cout << "Rpc调用失败：" << cntl.ErrorText() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    if (rsp.success() == false) {
        std::cout << rsp.errmsg() << std::endl;
        return -1;
    }
    std::cout << "msg_id: " << rsp.message().message_id() << std::endl;
    std::cout << "chat_session_id: " << rsp.message().chat_session_id() << std::endl;
    std::cout << "timestamp: " << rsp.message().timestamp() << std::endl;
    std::cout << "sender: " << rsp.message().sender().nickname() << std::endl;
    std::cout << "message: " << rsp.message().message().string_message().content() << std::endl;
    for(int i = 0; i < rsp.target_id_list_size(); ++i)
    {
        std::cout << "target:" << rsp.target_id_list(i) << std::endl;
    }

    return 0;
}