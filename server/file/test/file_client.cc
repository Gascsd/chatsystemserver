// 编写一个file客户端程序，对文件存储子服务进行单元测试

// 1. 封装四个接口进行rpc调用，实现对四个业务接口的测试

#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <thread>
#include "etcd.hpp"
#include "channel.hpp"
#include "logger.hpp"
#include "file.pb.h"
#include "base.pb.h"
#include "utils.hpp"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(file_service, "/service/file_service", "服务监控根目录");

zht_im::ServiceChannel::ChannelPtr channel;
std::string single_file_id;

TEST(put_test, single_file)
{
    // 1. 读取当前目录下的指定文件数据
    std::string body;
    ASSERT_TRUE(zht_im::readFile("./Makefile", body));
    // 2. 实例化rpc调用客户端对象，发起rpc调用
    zht_im::FileService_Stub stub(channel.get());

    zht_im::PutSingleFileReq req;
    req.set_request_id("1111");
    req.mutable_file_data()->set_file_name("Makefile");
    req.mutable_file_data()->set_file_size(body.size());
    req.mutable_file_data()->set_file_content(body);

    brpc::Controller *cntl = new brpc::Controller();
    zht_im::PutSingleFileRsp *rsp = new zht_im::PutSingleFileRsp();
    stub.PutSingleFile(cntl, &req, rsp, nullptr);
    ASSERT_FALSE(cntl->Failed());
    // 3. 监测返回值中上传是否成功
    ASSERT_TRUE(rsp->success());
    ASSERT_EQ(rsp->file_info().file_size(), body.size());
    ASSERT_EQ(rsp->file_info().file_name(), "Makefile");
    single_file_id = rsp->file_info().file_id();
    LOG_DEBUG("文件ID: {} ", rsp->file_info().file_id());
}

TEST(get_test, single_file)
{
    // 1. 发起rpc调用，进行文件下载
    zht_im::FileService_Stub stub(channel.get());
    zht_im::GetSingleFileReq req;
    zht_im::GetSingleFileRsp *rsp;
    req.set_request_id("2222");
    req.set_file_id(single_file_id);
    
    brpc::Controller *cntl = new brpc::Controller();
    rsp = new zht_im::GetSingleFileRsp();
    stub.GetSingleFile(cntl, &req, rsp, nullptr);
    ASSERT_FALSE(cntl->Failed());
    ASSERT_TRUE(rsp->success());
    // 2. 将文件数据存储到文件中
    ASSERT_EQ(single_file_id, rsp->file_data().file_id());
    zht_im::writeFile("makefile_single_file_download.txt", rsp->file_data().file_content());
}

std::vector<std::string> multi_file_id = {"base.pb.h", "file.pb.h"};

TEST(put_test, multi_file)
{
    // 1. 读取当前目录下的指定文件数据
    std::vector<std::string> body(2);
    ASSERT_TRUE(zht_im::readFile(multi_file_id[0], body[0]));
    ASSERT_TRUE(zht_im::readFile(multi_file_id[1], body[1]));
    // 2. 实例化rpc调用客户端对象，发起rpc调用
    zht_im::FileService_Stub stub(channel.get());

    zht_im::PutMultiFileReq req;
    req.set_request_id("3333");

    auto file_data = req.add_file_data();
    file_data->set_file_name(multi_file_id[0]);
    file_data->set_file_size(body[0].size());
    file_data->set_file_content(body[0]);

    file_data = req.add_file_data();
    file_data->set_file_name(multi_file_id[1]);
    file_data->set_file_size(body[1].size());
    file_data->set_file_content(body[1]);

    brpc::Controller *cntl = new brpc::Controller();
    zht_im::PutMultiFileRsp *rsp = new zht_im::PutMultiFileRsp();
    stub.PutMultiFile(cntl, &req, rsp, nullptr);
    ASSERT_FALSE(cntl->Failed());
    // 3. 监测返回值中上传是否成功
    ASSERT_TRUE(rsp->success());
    for(int i = 0; i < rsp->file_info_size(); ++i)
    {
        ASSERT_EQ(rsp->file_info(i).file_size(), body[i].size());
        ASSERT_EQ(rsp->file_info(i).file_name(), multi_file_id[i]);
        multi_file_id[i] = rsp->file_info(i).file_id();
        LOG_ERROR("文件ID: {}", rsp->file_info(i).file_id());
    }
}

TEST(get_test, multi_file)
{
    // 1. 发起rpc调用，进行文件下载
    zht_im::FileService_Stub stub(channel.get());
    zht_im::GetMultiFileReq req;
    zht_im::GetMultiFileRsp *rsp;
    req.set_request_id("4444");
    req.add_file_id_list(multi_file_id[0]);
    req.add_file_id_list(multi_file_id[1]);
    
    brpc::Controller *cntl = new brpc::Controller();
    rsp = new zht_im::GetMultiFileRsp();
    stub.GetMultiFile(cntl, &req, rsp, nullptr);
    ASSERT_FALSE(cntl->Failed());
    ASSERT_TRUE(rsp->success());
    // 2. 将文件数据存储到文件中
    ASSERT_TRUE(rsp->file_data().find(multi_file_id[0]) != rsp->file_data().end());
    ASSERT_TRUE(rsp->file_data().find(multi_file_id[1]) != rsp->file_data().end());
    auto map = rsp->file_data();
    zht_im::writeFile("base_multi_download_file1", map[multi_file_id[0]].file_content());
    zht_im::writeFile("file_multi_download_file2", map[multi_file_id[1]].file_content());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    // 1. 构造rpc信道管理对象
    auto sm = std::make_shared<zht_im::ServiceManager>();
    sm->declared(FLAGS_file_service);
    auto put_cb = std::bind(&zht_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&zht_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    // 2. 构造服务发现对象
    zht_im::Discovery::ptr dclient = std::make_shared<zht_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    // 3. 通过RPC信道管理对象，获取提供的echo服务的信道
    channel = sm->choose(FLAGS_file_service);
    if (!channel)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    return RUN_ALL_TESTS();
}
