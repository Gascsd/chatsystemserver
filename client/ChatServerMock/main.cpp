#include "widget.h"

#include <QApplication>
#include <QDebug>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "HTTP 服务器准备启动"; 

    HttpServer* httpServer = HttpServer::getInstance();
    if (!httpServer->init())
    {
        qDebug() << "HTTP 服务器启动失败";
        return 1;
    }
    qDebug() << "HTTP 服务器启动成功"; 

    WebSocketServer* websocketServer = WebSocketServer::getInstance();
    if (!websocketServer->init())
    {
        qDebug() << "websocke 服务器启动失败";

    }
    qDebug() << "websocke 服务器启动成功";

    Widget w;
    w.show();
    return a.exec();
}
