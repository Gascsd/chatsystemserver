
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <thread>
#include "etcd.hpp"
#include "channel.hpp"
#include "utils.hpp"
#include "user.pb.h"
#include "base.pb.h"
#include "friend.pb.h"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(friend_service, "/service/friend_service", "服务监控根目录");

zht_im::ServiceManager::ptr sm;

void apply_test(const std::string& uid1, const std::string& uid2)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::FriendAddReq req;
    zht_im::FriendAddRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_respondent_id(uid2);
    brpc::Controller cntl;
    stub.FriendAdd(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void get_apply_list(const std::string& uid)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::GetPendingFriendEventListReq req;
    zht_im::GetPendingFriendEventListRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    brpc::Controller cntl;
    stub.GetPendingFriendEventList(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.event_size(); ++i)
    {
        std::cout << "---------------------\n";
        std::cout << rsp.event(i).sender().user_id() << std::endl;
        std::cout << rsp.event(i).sender().nickname() << std::endl;
        std::cout << rsp.event(i).sender().avatar() << std::endl;
    }
}
void process_apply_test(const std::string& uid1, bool agree, const std::string& apply_uid)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::FriendAddProcessReq req;
    zht_im::FriendAddProcessRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_agree(agree);
    req.set_apply_user_id(apply_uid);
    brpc::Controller cntl;
    stub.FriendAddProcess(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    if(agree)
    {
        std::cout << rsp.new_session_id() << std::endl;
    }
}
void search_test(const std::string& uid1, const std::string& key)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::FriendSearchReq req;
    zht_im::FriendSearchRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_search_key(key);
    brpc::Controller cntl;
    stub.FriendSearch(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.user_info_size(); ++i)
    {
        std::cout << "-------------------------\n";
        std::cout << rsp.user_info(i).user_id() << std::endl;
        std::cout << rsp.user_info(i).nickname() << std::endl;
        std::cout << rsp.user_info(i).avatar() << std::endl;
    }
}
void friend_list_test(const std::string& uid1)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::GetFriendListReq req;
    zht_im::GetFriendListRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    brpc::Controller cntl;
    stub.GetFriendList(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.friend_list_size(); ++i)
    {
        std::cout << "-------------------------\n";
        std::cout << rsp.friend_list(i).user_id() << std::endl;
        std::cout << rsp.friend_list(i).nickname() << std::endl;
        std::cout << rsp.friend_list(i).avatar() << std::endl;
    }
}
void friend_remove(const std::string& uid1, const std::string& uid2)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::FriendRemoveReq req;
    zht_im::FriendRemoveRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_peer_id(uid2);
    brpc::Controller cntl;
    stub.FriendRemove(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void create_css_test(const std::string& uid1, const std::vector<std::string>& uid_list)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::ChatSessionCreateReq req;
    zht_im::ChatSessionCreateRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_chat_session_name("快乐一家人");
    for(auto& id : uid_list)
    {
        req.add_member_id_list(id);
    }
    brpc::Controller cntl;
    stub.ChatSessionCreate(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    std::cout << rsp.chat_session_info().chat_session_id() << std::endl;
    std::cout << rsp.chat_session_info().chat_session_name() << std::endl;
}
void css_member_test(const std::string& uid1, const std::string& cssid)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::GetChatSessionMemberReq req;
    zht_im::GetChatSessionMemberRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    req.set_chat_session_id(cssid);
    brpc::Controller cntl;
    stub.GetChatSessionMember(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.member_info_list_size(); ++i)
    {
        std::cout << "-------------------------\n";
        std::cout << rsp.member_info_list(i).user_id() << std::endl;
        std::cout << rsp.member_info_list(i).nickname() << std::endl;
        std::cout << rsp.member_info_list(i).avatar() << std::endl;
    }
}
void css_list_test(const std::string& uid1)
{
    auto channel = sm->choose(FLAGS_friend_service);
    if(!channel)
    {
        std::cout << "获取通信信道失败" << std::endl;
        return;
    }
    zht_im::FriendService_Stub stub(channel.get());
    zht_im::GetChatSessionListReq req;
    zht_im::GetChatSessionListRsp rsp;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid1);
    brpc::Controller cntl;
    stub.GetChatSessionList(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    for(int i = 0; i < rsp.chat_session_info_list_size(); ++i)
    {
        std::cout << "-------------------------\n";
        std::cout << rsp.chat_session_info_list(i).single_chat_friend_id() << std::endl;
        std::cout << rsp.chat_session_info_list(i).chat_session_id() << std::endl;
        std::cout << rsp.chat_session_info_list(i).chat_session_name() << std::endl;
        std::cout << rsp.chat_session_info_list(i).avatar() << std::endl;
        std::cout << "消息内容\n";
        std::cout << rsp.chat_session_info_list(i).prev_message().message_id() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().chat_session_id() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().timestamp() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().sender().user_id() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().sender().nickname() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().sender().avatar() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().message().file_message().file_contents() << std::endl;
        std::cout << rsp.chat_session_info_list(i).prev_message().message().file_message().file_name() << std::endl;
    }
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    
    // 1. 构造rpc信道管理对象
    sm = std::make_shared<zht_im::ServiceManager>();
    sm->declared(FLAGS_friend_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    // apply_test("8027-8369-e102-0004", "8fae-d68e-84b4-0005"); // 猪爸爸 -》 猪妈妈
    // apply_test("9e95-d6f2-326d-0006", "8fae-d68e-84b4-0005"); // 小猪佩奇 -》 猪妈妈
    // apply_test("dc75-5e49-61a8-0007", "8fae-d68e-84b4-0005"); // 小猪乔治 -》 猪妈妈

    // get_apply_list("8fae-d68e-84b4-0005");

    // process_apply_test("8fae-d68e-84b4-0005", true, "8027-8369-e102-0004"); // 猪妈妈同意猪爸爸
    // process_apply_test("8fae-d68e-84b4-0005", false, "9e95-d6f2-326d-0006");// 猪妈妈拒绝佩奇
    // process_apply_test("8fae-d68e-84b4-0005", true, "dc75-5e49-61a8-0007");// 猪妈妈同意乔治

    // search_test("8fae-d68e-84b4-0005", "猪"); // 猪妈妈搜索
    // std::cout << "+++++++++++++++++++++++++++++\n";
    // search_test("8027-8369-e102-0004", "猪"); // 猪爸爸搜索
    // std::cout << "=============================\n";
    // search_test("9e95-d6f2-326d-0006", "乔治"); // 小猪佩奇搜索

    // friend_list_test("8fae-d68e-84b4-0005"); // 猪妈妈好友列表
    // std::cout << "+++++++++++++++++++++++++++++\n";
    // friend_list_test("8027-8369-e102-0004"); // 猪爸爸好友列表
    // std::cout << "+++++++++++++++++++++++++++++\n";
    // friend_list_test("9e95-d6f2-326d-0006"); // 小猪佩奇好友列表
    // std::cout << "+++++++++++++++++++++++++++++\n";
    // friend_list_test("dc75-5e49-61a8-0007"); // 小猪乔治好友列表

    // friend_remove("8fae-d68e-84b4-0005", "dc75-5e49-61a8-0007"); // 猪妈妈删除小猪乔治
    // std::vector<std::string> uid_list = {"8027-8369-e102-0004", "8fae-d68e-84b4-0005", "9e95-d6f2-326d-0006", "dc75-5e49-61a8-0007"};
    // create_css_test("8027-8369-e102-0004", uid_list);

    // css_member_test("8027-8369-e102-0004", "b15c-34a4-5b44-0000");
    // std::cout << "+++++++++++++++++++++++++++++\n";
    // css_member_test("8fae-d68e-84b4-0005", "b15c-34a4-5b44-0000");

    css_list_test("8027-8369-e102-0004"); // 猪爸爸的消息

    return 0;
}