
#include <gflags/gflags.h>
#include <thread>
#include <gtest/gtest.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "etcd.hpp"
#include "channel.hpp"
#include "message.pb.h"
#include "base.pb.h"
#include "user.pb.h"
#include "logger.hpp"
#include "utils.hpp"


DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(message_service, "/service/message_service", "服务监控根目录");

std::shared_ptr<zht_im::ServiceManager> sm;

void show_message(const zht_im::MessageInfo& msg, const std::string& test_type)
{
    std::cout << "-------------------------------" << test_type << "--------------------------------" << std::endl;
    std::cout << msg.message_id() << std::endl;
    std::cout << msg.chat_session_id() << std::endl;
    std::cout << boost::posix_time::to_simple_string(boost::posix_time::from_time_t(msg.timestamp())) << std::endl;
    std::cout << msg.sender().user_id() << std::endl;
    std::cout << msg.sender().nickname() << std::endl;
    std::cout << msg.sender().avatar() << std::endl;
    if(msg.message().message_type() == zht_im::MessageType::STRING)
    {
        std::cout << "文本消息: " << msg.message().string_message().content() << std::endl;
    }
    else if(msg.message().message_type() == zht_im::MessageType::IMAGE)
    {
        std::cout << "图片消息: " << msg.message().image_message().image_content() << std::endl;
    }
    else if(msg.message().message_type() == zht_im::MessageType::FILE)
    {
        std::cout << "文件消息: " << msg.message().file_message().file_contents() << std::endl;
        std::cout << "文件名称: " << msg.message().file_message().file_name() << std::endl;
    }
    else if(msg.message().message_type() == zht_im::MessageType::SPEECH)
    {
        std::cout << "语音消息: " << msg.message().speech_message().file_contents() << std::endl;
    }
    else
    {
        std::cout << "类型出错,文件类型是: " << msg.message().message_type() << std::endl;
        std::cout << msg.message().string_message().content() << endl;
    }
}

void range_test(const std::string &ssid, const boost::posix_time::ptime &stime, const boost::posix_time::ptime &etime)
{
    auto channel = sm->choose(FLAGS_message_service); // 获取通信信道
    if(!channel)
    {
        std::cout << "获取通信信道失败"  << std::endl;
        return;
    }
    ASSERT_TRUE(channel);
    zht_im::MsgStorageService_Stub stub(channel.get());
    zht_im::GetHistoryMsgReq req;
    req.set_request_id(zht_im::uuid());
    req.set_chat_session_id("会话ID1");
    req.set_start_time(boost::posix_time::to_time_t(stime));
    req.set_over_time(boost::posix_time::to_time_t(etime));
    zht_im::GetHistoryMsgRsp rsp;
    brpc::Controller cntl;
    stub.GetHistoryMsg(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.msg_list_size(); ++i)
    {
        auto msg = rsp.msg_list(i);
        show_message(msg, "获取区间消息");
    }
}

void recent_test(const std::string &ssid, int count)
{
    auto channel = sm->choose(FLAGS_message_service); // 获取通信信道
    if(!channel)
    {
        std::cout << "获取通信信道失败"  << std::endl;
        return;
    }
    ASSERT_TRUE(channel);
    zht_im::MsgStorageService_Stub stub(channel.get());
    zht_im::GetRecentMsgReq req;
    req.set_request_id(zht_im::uuid());
    req.set_chat_session_id("会话ID1");
    req.set_msg_count(count);
    zht_im::GetRecentMsgRsp rsp;
    brpc::Controller cntl;
    stub.GetRecentMsg(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.msg_list_size(); ++i)
    {
        auto msg = rsp.msg_list(i);
        show_message(msg, "获取最近消息");
    }
}

void search_test(const std::string &ssid, const std::string& key)
{
    auto channel = sm->choose(FLAGS_message_service); // 获取通信信道
    if(!channel)
    {
        std::cout << "获取通信信道失败"  << std::endl;
        return;
    }
    ASSERT_TRUE(channel);
    zht_im::MsgStorageService_Stub stub(channel.get());
    zht_im::MsgSearchReq req;
    req.set_request_id(zht_im::uuid());
    req.set_chat_session_id("会话ID1");
    req.set_search_key(key);
    zht_im::MsgSearchRsp rsp;
    brpc::Controller cntl;
    stub.MsgSearch(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.msg_list_size(); ++i)
    {
        auto msg = rsp.msg_list(i);
        show_message(msg, "获取关键字搜索消息");
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    sm = std::make_shared<zht_im::ServiceManager>();
    sm->declared(FLAGS_message_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    boost::posix_time::ptime stime(boost::posix_time::time_from_string("2025-01-7 00:00:00"));
    boost::posix_time::ptime etime(boost::posix_time::time_from_string("2025-01-19 00:00:00"));
    range_test("会话ID1", stime, etime);
    int cnt;
    std::cout << "请输入要获取的最近n条消息的数量:";
    std::cin >> cnt;
    recent_test("会话ID1", cnt);
    search_test("会话ID1", "兰州");
    return 0;
}