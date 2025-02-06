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
} // namespace network
