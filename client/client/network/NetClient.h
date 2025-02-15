#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QWebSocket>
#include <QProtobufSerializer>
#include <QNetworkReply>

#include "../model/data.h"

// 此处为了避免 "循环包含" 问题, 就需要使用 前置声明 代替包含头文件
namespace model {
class DataCenter;
}  // end model

namespace network {
     
class NetClient : public QObject
{
    Q_OBJECT
private:
    // 定义重要常量. ip 都暂时使用本地的环回 ip. 端口号约定成 9000 和 9001
    const QString HTTP_URL = "http://127.0.0.1:9000";
    const QString WEBSOCKET_URL = "ws://127.0.0.1:9001/ws";

public:
    NetClient(model::DataCenter* dataCenter);

    static QString makeRequestId(); // 生成请求Id

    void ping(); // 验证网络联通

    void initWebsocket();

    void sendAuth(); // 发送身份认证请求

    void handleWsResponse(const zht_im::NotifyMessage& notifyMessage);
    void handleWsMessage(const model::Message& message);
    void handleWsRemoveFriend(const QString& userId);
    void handleWsAddFriendApply(const model::UserInfo& userInfo);
    void handleWsAddFriendProcess(const model::UserInfo& userInfo, bool agree);
    void handleWsSessionCreate(const model::ChatSessionInfo& chatSessionInfo);


    QNetworkReply* sendHttpRequest(const QString& apiPath, const QByteArray& body); // 封装发送请求的逻辑

    template<typename T>
    std::shared_ptr<T> handleHttpResponse(QNetworkReply* httpResp, bool* ok, QString* reason) // 封装处理响应的逻辑（包括判定HTTP正确定，反序列化，判断业务上的正确定）
    {
        // 1. 判定HTTP层面是否出错
        if (httpResp->error() != QNetworkReply::NoError)
        {
            *ok = false;
            *reason = httpResp->errorString();
            httpResp->deleteLater();
            return std::shared_ptr<T>();
        }
        // 2. 获取到响应的body
        QByteArray respBody = httpResp->readAll();
        // 3. 反序列化body
        std::shared_ptr<T> respObj = std::make_shared<T>();
        respObj->deserialize(&serializer, respBody);

        // 4. 判断业务处理是否正确
        if (!respObj->success())
        {
            *ok = false;
            *reason = respObj->errmsg();
            httpResp->deleteLater();
            return std::shared_ptr<T>();
        }
        *ok = true;
        httpResp->deleteLater();
        return respObj;
    }

    void getMyself(const QString& loginSessionId);
    void getFriendList(const QString& loginSessionId);
    void getChatSessionList(const QString& loginSessionId);
    void getApplyList(const QString& loginSessionId);
    void getRecentMessagesList(const QString& loginSessionId, const QString& chatSessionId, bool updateUI);
    void setMessage(const QString &loginSessionId, const QString chatSessionId, model::MessageType messageType,
                               const QByteArray &content, const QString extraInfo = "");

    void receiverMessage(const QString& chatSessionId);

    void changeNickname(const QString& loginSessionId, const QString& nickname);
    void changeDescription(const QString& loginSessionId, const QString& description);
    void getVerifyCode(const QString& phone);
    void changePhone(const QString& loginSessionId, const QString& phone, const QString& verifyCodeId, const QString& verifyCode);
    void changeAvatar(const QString& loginSessionId, const QByteArray& imageBytes);
    void deleteFriend(const QString& loginSessionId, const QString& userId);
    void addFriendApply(const QString& loginSessionId, const QString& userId);
    void acceptFriendApply(const QString& loginSessionId, const QString& userId);
    void rejectFriendApply(const QString& loginSessionId, const QString& userId);
    void createGroupChatSession(const QString& loginSessionId, const QList<QString>& userIdList);
    void getMemberList(const QString& loginSessionId, const QString& chatSessionId);

private:
    model::DataCenter* dataCenter;

    // http 客户端
    QNetworkAccessManager httpClient;

    // websocket 客户端
    QWebSocket websocketClient;

    // 序列化器
    QProtobufSerializer serializer;

};

}  // end network

#endif // NETCLIENT_H

