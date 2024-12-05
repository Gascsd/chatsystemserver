#include <brpc/channel.h>
#include <thread>
#include "main.pb.h"

void callback(brpc::Controller* cntl, ::example::EchoResponse* response) 
{
    std::unique_ptr<brpc::Controller> cntl_guard(cntl);
    std::unique_ptr<example::EchoResponse> resp_guard(response);
    if (cntl->Failed() == true) 
    {
        std::cout << "Rpc调用失败：" << cntl->ErrorText() << std::endl;
        return;
    }
    std::cout << "收到响应: " << response->message() << std::endl;
}

int main(int argc, char* argv[])
{
    // 1. 构造一个channel信道，连接服务器
    brpc::ChannelOptions options;
    options.connect_timeout_ms = -1; // 连接等待超时时间
    options.timeout_ms = -1; // rpc请求等待超时时间
    options.max_retry = 3; // 请求重试次数
    options.protocol = "baidu_std"; // 序列化协议，默认使用baidu
    brpc::Channel channel;
    channel.Init("127.0.0.1:8080", &options);
    // 2. 构造EchoService_Stub对象，用于进行rpc调用
    example::EchoService_Stub stub(&channel);
    // 3. 进行rpc调用
    example::EchoRequest req;
    req.set_message("你好，世界");
    brpc::Controller* cntl = new brpc::Controller();
    example::EchoResponse *resp = new example::EchoResponse();

    auto clusure = google::protobuf::NewCallback(callback, cntl, resp);
    stub.Echo(cntl, &req, resp, clusure); // 异步调用，这里就需要等待了
    std::cout << "异步调用结束！\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // stub.Echo(cntl, &req, resp, nullptr);
    // if(cntl->Failed() == true)
    // {
    //     std::cout << "rpc调用失败:" << cntl->ErrorText() << std::endl;
    //     return -1;
    // }
    // std::cout << "收到响应：" << resp->message() << std::endl;
    // delete cntl;
    // delete resp;
    return 0;
}