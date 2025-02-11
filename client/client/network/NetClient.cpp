#include "NetClient.h"

#include <QNetworkReply>
#include <QUuid>
#include "../utils.hpp"
#include "../model/data.h"
#include "../model/DataCenter.h"

namespace network
{
	NetClient::NetClient(model::DataCenter* dataCenter) 
		: dataCenter(dataCenter)
	{
		initWebsocket();
	}
	void NetClient::ping()
	{
		QNetworkRequest httpReq;
		httpReq.setUrl(QUrl(HTTP_URL + "/ping"));

		QNetworkReply* httpResp = httpClient.get(httpReq);
		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			if (httpResp->error() != QNetworkReply::NoError)
			{
				LOG() << "HTTP 请求失败 " << httpResp->errorString();
				httpResp->deleteLater();
				return;
			}
			QByteArray body = httpResp->readAll();
			LOG() << "响应内容 " << body;
			httpResp->deleteLater();
		});
	}
	void NetClient::initWebsocket()
	{
		// 1. 准备好所有需要的信号槽
		connect(&websocketClient, &QWebSocket::connected, this, [=]() {
			LOG() << "websocket 连接成功";
			sendAuth();
			});
		connect(&websocketClient, &QWebSocket::disconnected, this, [=]() {
			LOG() << "websocket 连接断开";
			});
		connect(&websocketClient, &QWebSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
			LOG() << "websocket 连接出错" << error;
			});
		connect(&websocketClient, &QWebSocket::textMessageReceived, this, [=](const QString& message) {
			LOG() << "websocket 收到文本消息" << message;
			});
		connect(&websocketClient, &QWebSocket::binaryMessageReceived, this, [=](const QByteArray& byteArray) {
			LOG() << "websocket 收到二进制消息" << byteArray.length();
			// TODO
			});
		// 2. 和服务器真正建立连接
		websocketClient.open(WEBSOCKET_URL);
	}
	void NetClient::sendAuth()
	{
		zht_im::ClientAuthenticationReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(dataCenter->getLoginSessionId());
		QByteArray bdoy = req.serialize(&serializer);
		websocketClient.sendBinaryMessage(bdoy);
		LOG() << "[WS身份认证] requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId();
	}
	QString NetClient::makeRequestId()
	{
		// 确保每个请求的Id都是唯一的
		// 通过UUID实现
		return "R" + QUuid::createUuid().toString().sliced(25, 12);
	}
	QNetworkReply* NetClient::sendHttpRequest(const QString& apiPath, const QByteArray& body)
	{
		QNetworkRequest httpReq;
		httpReq.setUrl(QUrl(HTTP_URL + apiPath));
		httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-protobuf");
		QNetworkReply* httpResp = httpClient.post(httpReq, body);
		return httpResp;
	}
	// 在这里完成具体的网络通信
	void NetClient::getMyself(const QString& loginSessionId)
	{
		// 1. 构造http请求 body
		zht_im::GetUserInfoReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[获取个人信息] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId;

		// 2. 构造HTTP 请求并发送
		QNetworkReply* httpResp = sendHttpRequest("/service/user/get_user_info", body);

		// 3. 通过信号槽，获取到当前响应
		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::GetUserInfoRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[获取个人信息] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetMyself(resp);

			// 通知调用逻辑，已经处理完毕响应
			emit dataCenter->getMyselfDone(); // 这是在发起一个信号

			LOG() << "[获取个人信息] 处理响应完毕 requestId=" << req.requestId();
			});
	}
	void NetClient::getFriendList(const QString& loginSessionId)
	{
		zht_im::GetFriendListReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[获取好友列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId;

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_friend_list", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::GetFriendListRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[获取好友列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetFriendList(resp);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->getFriendListDone(); // 这是在发起一个信号

			LOG() << "[获取好友列表] 处理响应完毕 requestId=" << req.requestId();
			});
	}
	void NetClient::getChatSessionList(const QString& loginSessionId)
	{
		zht_im::GetChatSessionListReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[获取会话列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId;

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_chat_session_list", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::GetChatSessionListRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[获取会话列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetChatSessionList(resp);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->getChatSessionListDone(); // 这是在发起一个信号

			LOG() << "[获取会话列表] 处理响应完毕 requestId=" << req.requestId();
			});
	}

    void NetClient::getApplyList(const QString &loginSessionId)
    {
        zht_im::GetPendingFriendEventListReq req;
        req.setRequestId(makeRequestId());
        req.setSessionId(loginSessionId);
        QByteArray body = req.serialize(&serializer);
        LOG() << "[获取好友申请列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId;

        QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_pending_friend_events", body);

        connect(httpResp, &QNetworkReply::finished, this, [=]() {
            bool ok = false;
            QString reason = "";
            auto resp = handleHttpResponse<zht_im::GetPendingFriendEventListRsp>(httpResp, &ok, &reason);
            if (!ok)
            {
                LOG() << "[获取好友申请列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
                return;
            }
            // 把响应保存到DataCenter中
            dataCenter->resetApplyList(resp);

            // 发送信号通知调用逻辑，已经处理完毕响应
            emit dataCenter->getApplyListDone(); // 这是在发起一个信号

            LOG() << "[获取好友申请列表] 处理响应完毕 requestId=" << req.requestId();
        });
    }

    void NetClient::getRecentMessagesList(const QString &loginSessionId, const QString &chatSessionId)
    {
        zht_im::GetRecentMsgReq req;
        req.setRequestId(makeRequestId());
        req.setSessionId(loginSessionId);
        req.setChatSessionId(chatSessionId);
        req.setMsgCount(50); // 这里暂时先定下获取最近50条消息
        QByteArray body = req.serialize(&serializer);
        LOG() << "[获取最近消息列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId << " chatSessionId=" << chatSessionId;

        QNetworkReply* httpResp = sendHttpRequest("/service/message_storage/get_recent", body);

        connect(httpResp, &QNetworkReply::finished, this, [=]() {
            bool ok = false;
            QString reason = "";
            auto resp = handleHttpResponse<zht_im::GetRecentMsgRsp>(httpResp, &ok, &reason);
            if (!ok)
            {
                LOG() << "[获取最近消息列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
                return;
            }
            // 把响应保存到DataCenter中
            dataCenter->resetRecentMessage(chatSessionId, resp);

            // 发送信号通知调用逻辑，已经处理完毕响应
            emit dataCenter->getRecentMessageListDone(chatSessionId); // 这是在发起一个信号

            LOG() << "[获取最近消息列表] 处理响应完毕 requestId=" << req.requestId();
        });
    }
    // extraInfo可以传递扩展消息，尤其是文件消息的文件名
    void NetClient::setMessage(const QString &loginSessionId, const QString chatSessionId, model::MessageType messageType,
                               const QByteArray &content, const QString extraInfo)
    {
        zht_im::NewMessageReq req;
        req.setRequestId(makeRequestId());
        req.setSessionId(loginSessionId);
        req.setChatSessionId(chatSessionId);

        // 构造messageContent
        zht_im::MessageContent messageContent;
        if(messageType == model::TEXT_TYPE)
        {
            messageContent.setMessageType(zht_im::MessageTypeGadget::STRING);
            zht_im::StringMessageInfo stringMessageInfo;
            stringMessageInfo.setContent(content);
            messageContent.setStringMessage(stringMessageInfo);
        }
        else if (messageType == model::IMAGE_TYPE)
        {
            messageContent.setMessageType(zht_im::MessageTypeGadget::IMAGE);
            zht_im::ImageMessageInfo imageMessageInfo;
            imageMessageInfo.setFileId(""); // 这个是由服务器生成的
            imageMessageInfo.setImageContent(content);
            messageContent.setImageMessage(imageMessageInfo);
        }
        else if (messageType == model::FILE_TYPE)
        {
            messageContent.setMessageType(zht_im::MessageTypeGadget::FILE);
            zht_im::FileMessageInfo fileMessageInfo;
            fileMessageInfo.setFileId(""); // 这个是由服务器生成的
            fileMessageInfo.setFileSize(content.size());
            fileMessageInfo.setFileName(extraInfo);
            fileMessageInfo.setFileContents(content);
            messageContent.setFileMessage(fileMessageInfo);
        }
        else if (messageType == model::SPEECH_TYPE)
        {
            messageContent.setMessageType(zht_im::MessageTypeGadget::SPEECH);
            zht_im::SpeechMessageInfo speechMessageInfo;
            speechMessageInfo.setFileId(""); // 这个是由服务器生成的
            speechMessageInfo.setFileContents(content);
            messageContent.setSpeechMessage(speechMessageInfo);
        }
        else
        {
            LOG() << "错误的消息类型，messageType=" << messageType;
        }
        req.setMessage(messageContent);
        QByteArray body = req.serialize(&serializer);
        LOG() << "[获取最近消息列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << loginSessionId <<
            " chatSessionId=" << chatSessionId << " messageType=" << req.message().messageType();

        QNetworkReply* httpResp = sendHttpRequest("/service/message_transmit/new_message", body);

        connect(httpResp, &QNetworkReply::finished, this, [=]() {
            bool ok = false;
            QString reason = "";
            auto resp = handleHttpResponse<zht_im::NewMessageRsp>(httpResp, &ok, &reason);
            if (!ok)
            {
                LOG() << "[获取最近消息列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
                return;
            }

            // 发送信号通知调用逻辑，已经处理完毕响应
            emit dataCenter->sendMessageDone(messageType, content, extraInfo); // 这是在发起一个信号

            LOG() << "[获取最近消息列表] 处理响应完毕 requestId=" << req.requestId();
        });
    }
} // namespace network
