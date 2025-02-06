#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QHttpServer>
#include <QProtobufSerializer>
#include <QWebSocketServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    static HttpServer* getInstance();
    bool init();  // 初始化 HttpServer

    uint16_t httpPort = 9000;


private:
    HttpServer();

private:
    static HttpServer* instance;
    QHttpServer httpServer;
    QProtobufSerializer serializer;

signals:
};


class WebSocketServer : public QObject
{
    Q_OBJECT

private:
    static WebSocketServer* instance;
    WebSocketServer() : websockerServer ("websocket server", QWebSocketServer::NonSecureMode) {}

    QWebSocketServer websockerServer;

public:
    static WebSocketServer* getInstance();
    uint16_t websocketPort = 9001;

    bool init();

signals:
};

#endif // SERVER_H
