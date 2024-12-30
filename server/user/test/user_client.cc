
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <thread>
#include "etcd.hpp"
#include "channel.hpp"
#include "utils.hpp"
#include "user.pb.h"
#include "base.pb.h"


DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(user_service, "/service/user_service", "服务监控根目录");

zht_im::ServiceManager::ptr _user_channels;
zht_im::UserInfo user_info;
std::string login_ssid;
std::string new_nickname = "小猪爸爸";


// TEST(用户子服务测试, 用户注册测试)
// {
//     LOG_DEBUG("用户注册测试");
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);
//     user_info.set_nickname("猪妈妈");

//     zht_im::UserRegisterReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_nickname(user_info.nickname());
//     req.set_password("123456");

//     zht_im::UserRegisterRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.UserRegister(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 用户登录测试)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::UserLoginReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_nickname(new_nickname);
//     req.set_password("123456");

//     zht_im::UserLoginRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.UserLogin(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
//     login_ssid = rsp.login_session_id();
// }

// TEST(用户子服务测试, 用户头像设置测试)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::SetUserAvatarReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_user_id(user_info.user_id());
//     req.set_session_id(login_ssid);
//     req.set_avatar(user_info.avatar());

//     zht_im::SetUserAvatarRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.SetUserAvatar(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
// TEST(用户子服务测试, 用户签名设置测试)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::SetUserDescriptionReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_session_id(login_ssid);
//     req.set_user_id(user_info.user_id());
//     req.set_description(user_info.description());

//     zht_im::SetUserDescriptionRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.SetUserDescription(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
// TEST(用户子服务测试, 用户昵称设置测试)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::SetUserNicknameReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_session_id(login_ssid);
//     req.set_user_id(user_info.user_id());
//     req.set_nickname(new_nickname);

//     zht_im::SetUserNicknameRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.SetUserNickname(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
// TEST(用户子服务测试, 用户信息获取测试)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::GetUserInfoReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_session_id(login_ssid);
//     req.set_user_id(user_info.user_id());

//     zht_im::GetUserInfoRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.GetUserInfo(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
//     ASSERT_EQ(user_info.user_id(), rsp.user_info().user_id());
//     ASSERT_EQ(new_nickname, rsp.user_info().nickname());
//     ASSERT_EQ(user_info.description(), rsp.user_info().description());
//     ASSERT_EQ("", rsp.user_info().phone());
//     ASSERT_EQ(user_info.avatar(), rsp.user_info().avatar());
// }

// void set_user_avatar(const std::string& uid, const std::string &avatar)
// {
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::SetUserAvatarReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_user_id(uid);
//     req.set_session_id(login_ssid);
//     req.set_avatar(avatar);

//     zht_im::SetUserAvatarRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.SetUserAvatar(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 批量用户信息获取测试)
// {
//     set_user_avatar("用户ID1", "小猪佩奇的头像数据");
//     set_user_avatar("用户ID2", "小猪乔治的头像数据");
//     set_user_avatar("4c6c-d3cc-e7c2-0000", "小猪爸爸的头像数据");

//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::GetMultiUserInfoReq req;
//     req.set_request_id(zht_im::uuid());
//     req.add_users_id("用户ID1");
//     req.add_users_id("用户ID2");
//     req.add_users_id("4c6c-d3cc-e7c2-0000");

//     zht_im::GetMultiUserInfoRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.GetMultiUserInfo(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
    

//     auto users_map = *rsp.mutable_users_info();

//     zht_im::UserInfo user1 = (users_map)["用户ID1"];
//     ASSERT_EQ("用户ID1", user1.user_id());
//     ASSERT_EQ("小猪佩奇", user1.nickname());
//     ASSERT_EQ("我很懒没有签名1", user1.description());
//     ASSERT_EQ("手机号1", user1.phone());
//     ASSERT_EQ("小猪佩奇的头像数据", user1.avatar());

//     zht_im::UserInfo user2 = (users_map)["用户ID2"];
//     ASSERT_EQ("用户ID2", user2.user_id());
//     ASSERT_EQ("小猪乔治", user2.nickname());
//     ASSERT_EQ("我很懒没有签名2", user2.description());
//     ASSERT_EQ("手机号2", user2.phone());
//     ASSERT_EQ("小猪乔治的头像数据", user2.avatar());

//     zht_im::UserInfo user3 = (users_map)["4c6c-d3cc-e7c2-0000"];
//     ASSERT_EQ("4c6c-d3cc-e7c2-0000", user3.user_id());
//     ASSERT_EQ("小猪爸爸", user3.nickname());
//     ASSERT_EQ("这是猪爸爸", user3.description());
//     ASSERT_EQ("", user3.phone());
//     ASSERT_EQ("小猪爸爸的头像数据", user3.avatar());
// }
std::string code_id;
void get_code(const std::string &phone)
{
    // if(phone.empty()) phone = user_info.phone();
    auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
    ASSERT_TRUE(channel);

    zht_im::PhoneVerifyCodeReq req;
    req.set_request_id(zht_im::uuid());
    req.set_phone_number(phone);

    zht_im::PhoneVerifyCodeRsp rsp;
    brpc::Controller cntl;
    zht_im::UserService_Stub stub(channel.get());
    stub.GetPhoneVerifyCode(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    code_id = rsp.verify_code_id();
}
// TEST(用户子服务测试, 手机号注册)
// {
//     get_code();
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::PhoneRegisterReq req;
//     req.set_request_id(zht_im::uuid());
//     req.set_phone_number(user_info.phone());
//     req.set_verify_code_id(code_id);
//     std::cout << "手机号注册时，输入验证码:";
//     std::string code;
//     cin >> code;
//     req.set_verify_code(code);

//     zht_im::PhoneRegisterRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.PhoneRegister(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
// TEST(用户子服务测试, 手机号设置)
// {
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//     get_code();
//     auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
//     ASSERT_TRUE(channel);

//     zht_im::SetUserPhoneNumberReq req;
//     req.set_request_id(zht_im::uuid());
//     std::cout << "手机号设置时，输入用户ID:";
//     std::string user_id;
//     cin >> user_id;
//     req.set_user_id(user_id);
//     req.set_phone_number("19159303352");
//     req.set_phone_verify_code_id(code_id);
//     std::cout << "手机号设置时，输入验证码:";
//     std::string code;
//     cin >> code;
//     req.set_phone_verify_code(code);

//     zht_im::SetUserPhoneNumberRsp rsp;
//     brpc::Controller cntl;
//     zht_im::UserService_Stub stub(channel.get());
//     stub.SetUserPhoneNumber(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
TEST(用户子服务测试, 手机号登录)
{
    std::cout << "手机号登录时，输入手机号:";
    std::string phone;
    cin >> phone;
    get_code(phone);
    auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
    ASSERT_TRUE(channel);

    zht_im::PhoneLoginReq req;
    req.set_request_id(zht_im::uuid());
    
    req.set_phone_number(phone);
    req.set_verify_code_id(code_id);
    std::cout << "手机号登录时，输入验证码:";
    std::string code;
    cin >> code;
    req.set_verify_code(code);

    zht_im::PhoneLoginRsp rsp;
    brpc::Controller cntl;
    zht_im::UserService_Stub stub(channel.get());
    stub.PhoneLogin(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    std::cout << "手机登录会话ID" << rsp.login_session_id() << endl;
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    _user_channels = std::make_shared<zht_im::ServiceManager>();
    _user_channels->declared(FLAGS_user_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, _user_channels.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, _user_channels.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);
    user_info.set_nickname("猪爸爸");
    user_info.set_user_id("4c6c-d3cc-e7c2-0000");
    user_info.set_description("这是猪爸爸");
    user_info.set_phone("19159303352");
    user_info.set_avatar("猪妈妈头像数据");



    return RUN_ALL_TESTS();
}