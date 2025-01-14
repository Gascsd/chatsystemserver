
#include <gflags/gflags.h>
#include <thread>
#include <gtest/gtest.h>
#include "etcd.hpp"
#include "channel.hpp"
#include "transmite.pb.h"
#include "logger.hpp"
#include "utils.hpp"


DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(transmite_service, "/service/transmite_service", "服务监控根目录");

std::shared_ptr<zht_im::ServiceManager> sm;

void string_message(const std::string& uid, const std::string& sid, const std::string& msg)
{
    auto channel = sm->choose(FLAGS_transmite_service);
    ASSERT_TRUE(channel);
    zht_im::MsgTransmitService_Stub stub(channel.get());
    zht_im::NewMessageReq req;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    req.set_chat_session_id(sid);
    req.mutable_message()->set_message_type(zht_im::MessageType::STRING);
    req.mutable_message()->mutable_string_message()->set_content(msg);
    zht_im::GetTransmitTargetRsp rsp;
    brpc::Controller cntl;
    stub.GetTransmitTarget(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void image_message(const std::string& uid, const std::string& sid, const std::string& msg)
{
    auto channel = sm->choose(FLAGS_transmite_service);
    ASSERT_TRUE(channel);
    zht_im::MsgTransmitService_Stub stub(channel.get());
    zht_im::NewMessageReq req;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    req.set_chat_session_id(sid);
    req.mutable_message()->set_message_type(zht_im::MessageType::IMAGE);
    req.mutable_message()->mutable_image_message()->set_image_content(msg);
    zht_im::GetTransmitTargetRsp rsp;
    brpc::Controller cntl;
    stub.GetTransmitTarget(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void speech_message(const std::string& uid, const std::string& sid, const std::string& msg)
{
    auto channel = sm->choose(FLAGS_transmite_service);
    ASSERT_TRUE(channel);
    zht_im::MsgTransmitService_Stub stub(channel.get());
    zht_im::NewMessageReq req;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    req.set_chat_session_id(sid);
    req.mutable_message()->set_message_type(zht_im::MessageType::SPEECH);
    req.mutable_message()->mutable_speech_message()->set_file_contents(msg);
    zht_im::GetTransmitTargetRsp rsp;
    brpc::Controller cntl;
    stub.GetTransmitTarget(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void file_message(const std::string& uid, const std::string& sid, const std::string& filename, const std::string& content)
{
    auto channel = sm->choose(FLAGS_transmite_service);
    ASSERT_TRUE(channel);
    zht_im::MsgTransmitService_Stub stub(channel.get());
    zht_im::NewMessageReq req;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    req.set_chat_session_id(sid);
    req.mutable_message()->set_message_type(zht_im::MessageType::FILE);
    req.mutable_message()->mutable_file_message()->set_file_name(filename);
    req.mutable_message()->mutable_file_message()->set_file_size(content.size());
    req.mutable_message()->mutable_file_message()->set_file_contents(content);
    zht_im::GetTransmitTargetRsp rsp;
    brpc::Controller cntl;
    stub.GetTransmitTarget(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    sm = std::make_shared<zht_im::ServiceManager>();
    sm->declared(FLAGS_transmite_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    string_message("3ff2-7ad3-b7de-0000", "会话ID1", "吃饭了吗？");
    string_message("cb7d-01fc-63c8-0001", "会话ID1", "吃了兰州拉面");
    image_message("3ff2-7ad3-b7de-0000", "会话ID1", "可爱表情包图片数据");
    speech_message("3ff2-7ad3-b7de-0000", "会话ID1", "语音：吃饭了吗？数据");
    file_message("3ff2-7ad3-b7de-0000", "会话ID1", "这是文件名称", "这是文件数据");

    return 0;
}