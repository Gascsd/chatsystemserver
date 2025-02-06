#include "server.h"

HttpServer* HttpServer::instance = nullptr;

HttpServer *HttpServer::getInstance()
{
    if(instance == nullptr)
    {
        instance = new HttpServer();
    }
    return instance;
}

HttpServer::HttpServer()
{
    
}

bool HttpServer::init()
{
    int ret = httpServer.listen(QHostAddress::Any, httpPort);

    // 配置路由
    httpServer.route("/ping", [](const QHttpServerRequest& req) {
        (void)req;
        qDebug() << "[http] 收到 ping 请求";
        return "pong";
    });

    return ret == httpPort;
}

WebSocketServer* WebSocketServer::instance = nullptr;

WebSocketServer* WebSocketServer::getInstance()
{
    if (instance == nullptr)
        instance = new WebSocketServer();
    return instance;
}

// 初始化websocket服务器
bool WebSocketServer::init()
{
    // 1. 连接信号槽
    connect(&websockerServer, &QWebSocketServer::newConnection, this, [=]() {
        qDebug() << "[websocket] 连接建立成功";

        QWebSocket* socket = websockerServer.nextPendingConnection();

        // 对这个socket进行剩余信号处理

        connect(socket, &QWebSocket::disconnected, this, [=]() {
            qDebug() << "[websocket] 连接断开";
            });
        connect(socket, &QWebSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
            qDebug() << "[websocket] 连接出错" << error;
            });
        connect(socket, &QWebSocket::textMessageReceived, this, [=](const QString& message) {
            qDebug() << "[websocket] 收到文本数据" << message;
            });
        connect(socket, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray& byteArray) {
            qDebug() << "[websocket] 收到二进制数据" << byteArray.length();
            });
        });
    // 2. 绑定端口，启动服务
    bool ok = websockerServer.listen(QHostAddress::Any, websocketPort);
    return ok;
}
