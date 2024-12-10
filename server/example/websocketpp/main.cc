#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

// 0. 定义server类型
typedef websocketpp::server<websocketpp::config::asio> server_t;

void onOpen(websocketpp::connection_hdl hdl)
{
    std::cout << "websocket 长连接建立成功" << std::endl;
}
void onClose(websocketpp::connection_hdl hdl)
{
    std::cout << "websocket 长连接断开" << std::endl;
}
void onMessage(server_t *server, websocketpp::connection_hdl hdl, server_t::message_ptr msg)
{
    // 1. 获取有效载荷内容
    std::string body = msg->get_payload();
    std::cout << "收到消息: " << body << std::endl;
    // 2. 对客户端响应
    auto conn = server->get_con_from_hdl(hdl);
    conn->send(body + "-回显", websocketpp::frame::opcode::value::text);
}
int main()
{
    // 1. 实例化服务器对象
    server_t server;
    // 2. 初始化日志输出（关闭）
    server.set_access_channels(websocketpp::log::alevel::none);
    // 3. 初始化asio框架
    server.init_asio();
    // 4. 设置回调函数（连接建立/关闭和消息处理）
    server.set_open_handler(onOpen);
    server.set_close_handler(onClose);
    auto msg_hdl = std::bind(onMessage, &server, std::placeholders::_1, std::placeholders::_2);
    server.set_message_handler(msg_hdl);
    // 5. 启用地址重用&&设置监听端口
    server.set_reuse_addr(true);
    server.listen(8080);
    // 6. 开始监听
    server.start_accept();
    // 7. 启动服务器
    server.run();
    return 0;
}