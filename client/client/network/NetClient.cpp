#include "NetClient.h"

#include <QNetworkReply>
#include <QUuid>
#include "../utils.hpp"
#include "../model/date.h"
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
} // namespace network
