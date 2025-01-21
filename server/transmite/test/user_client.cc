
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

void reg_user(const std::string &nickname, const std::string &pswd)
{
    auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
    ASSERT_TRUE(channel);

    zht_im::UserRegisterReq req;
    req.set_request_id(zht_im::uuid());
    req.set_nickname(nickname);
    req.set_password(pswd);

    zht_im::UserRegisterRsp rsp;
    brpc::Controller cntl;
    zht_im::UserService_Stub stub(channel.get());
    stub.UserRegister(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}
void set_user_avatar(const std::string& uid, const std::string &avatar)
{
    auto channel = _user_channels->choose(FLAGS_user_service); // 获取通信信道
    ASSERT_TRUE(channel);

    zht_im::SetUserAvatarReq req;
    req.set_request_id(zht_im::uuid());
    req.set_user_id(uid);
    req.set_session_id("测试登录会话ID1");
    req.set_avatar(avatar);

    zht_im::SetUserAvatarRsp rsp;
    brpc::Controller cntl;
    zht_im::UserService_Stub stub(channel.get());
    stub.SetUserAvatar(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
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
    reg_user("猪爸爸", "123456");
    reg_user("猪妈妈", "123456");
    reg_user("小猪佩奇", "123456");
    reg_user("小猪乔治", "123456");
    std::string fuid, muid, puid, quid;
    std::cout << "输入猪爸爸的uid";
    std::cin >> fuid;
    std::cout << "输入猪妈妈的uid";
    std::cin >> muid;
    std::cout << "输入小猪佩奇的uid";
    std::cin >> puid;
    std::cout << "输入小猪乔治的uid";
    std::cin >> quid;
    set_user_avatar(fuid, "猪爸爸头像数据");
    set_user_avatar(muid, "猪妈妈头像数据");
    set_user_avatar(puid, "小猪佩奇头像数据");
    set_user_avatar(quid, "小猪乔治头像数据");
    return 0;
}