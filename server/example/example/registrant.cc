#include <gflags/gflags.h>
#include <thread>
#include <brpc/server.h>
#include "../common/etcd.hpp"
#include "../common/channel.hpp" 
#include "main.pb.h"


DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "这是服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/echo/instance", "当前实例名称");
DEFINE_string(access_name, "127.0.0.1:7070", "当前实例的外部访问地址");
DEFINE_int32(listen_port, 7070, "RPC服务器监听端口");


class EchoServiceImpl : public example::EchoService 
{
public:
    EchoServiceImpl() {}
    ~EchoServiceImpl() {}

    void Echo(google::protobuf::RpcController* controller,
                       const ::example::EchoRequest* request,
                       ::example::EchoResponse* response,
                       ::google::protobuf::Closure* done)
    {
        brpc::ClosureGuard rpc_guard(done);
        std::cout << "收到消息：" << request->message() << std::endl;

        std::string str = request->message() + "--这是响应";
        response->set_message(str);
        // done->Run();
    }
}; 

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);


    // 1. 构造echo服务  
    // 关闭brpc的默认日志输出
    logging::LoggingSettings settings;
    settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    logging::InitLogging(settings);
    // 2. 构造服务器对象
    brpc::Server server;
    // 3. 向服务器对象中，新增EchoService服务
    EchoServiceImpl echo_server;
    int ret = server.AddService(&echo_server, brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE);
    if(ret == -1)
    {
        std::cout << "添加RPC服务失败" << std::endl;
        return -1;
    }
    // 4. 启动服务器
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1; // 连接空闲超时时间-超时关闭连接，-1默认不关闭
    options.num_threads = 1; // io线程数量
    ret = server.Start(FLAGS_listen_port, &options);
    if(ret == -1)
    {
        std::cout << "启动服务器失败" << std::endl;
        return -1;
    }
    // 4. 注册服务
    Registrant::ptr rclient = std::make_shared<Registrant>(FLAGS_etcd_host);
    rclient->registry(FLAGS_base_service + FLAGS_instance_name, FLAGS_access_name);

    server.RunUntilAskedToQuit(); //休眠等待运行结束

    return 0;
}