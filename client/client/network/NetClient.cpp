#include "NetClient.h"

#include <QNetworkReply>
#include <QUuid>
#include "../Toast.h"
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
			zht_im::NotifyMessage notifyMessage;
			notifyMessage.deserialize(&serializer, byteArray);
			handleWsResponse(notifyMessage);
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
	void NetClient::handleWsResponse(const zht_im::NotifyMessage& notifyMessage)
	{
		// LOG() << "调用handleWsResponse, 通知类型为：" << notifyMessage.notifyType();
		if (notifyMessage.notifyType() == zht_im::NotifyTypeGadget::FRIEND_ADD_APPLY_NOTIFY)
		{
			model::UserInfo userInfo;
			userInfo.load(notifyMessage.friendAddApply().userInfo());
			handleWsAddFriendApply(userInfo);
		}
		else if (notifyMessage.notifyType() == zht_im::NotifyTypeGadget::FRIEND_ADD_PROCESS_NOTIFY)
		{
			model::UserInfo userInfo;
			userInfo.load(notifyMessage.friendProcessResult().userInfo());
			bool agree = notifyMessage.friendProcessResult().agree();
			handleWsAddFriendProcess(userInfo, agree);
		}
		else if (notifyMessage.notifyType() == zht_im::NotifyTypeGadget::CHAT_SESSION_CREATE_NOTIFY)
		{
			model::ChatSessionInfo chatSessionInfo;
			chatSessionInfo.load(notifyMessage.newChatSessionInfo().chatSessionInfo());
			handleWsSessionCreate(chatSessionInfo);
		}
		else if (notifyMessage.notifyType() == zht_im::NotifyTypeGadget::CHAT_MESSAGE_NOTIFY)
		{
			// 1. 收到消息，把pb中的message 转成客户端的message类型
			model::Message message;
			message.load(notifyMessage.newMessageInfo().messageInfo());
			handleWsMessage(message);
		}
		else if (notifyMessage.notifyType() == zht_im::NotifyTypeGadget::FRIEND_REMOVE_NOTIFY)
		{
			const QString& userId = notifyMessage.friendRemove().userId();
			handleWsRemoveFriend(userId);
		}
		else
		{
			LOG() << "错误的通知类型";
		}
	}
	void NetClient::handleWsMessage(const model::Message& message)
	{
		QList<model::Message>* messageList = dataCenter->getRecentMessagesList(message._chatSessionId);
		// 1. 判断当前消息所属会话，是否在消息列表中
		if (messageList == nullptr) // 如果不存在就要通过网络从服务器获取
		{
			connect(dataCenter, &model::DataCenter::getRecentMessageListDoneNoUI, this, &NetClient::receiverMessage, Qt::UniqueConnection);
			dataCenter->getRecentMessagesListAsync(message._chatSessionId, false);
		}
		else // 如果已经存在在消息列表，就直接尾插
		{
			messageList->push_back(message);
			receiverMessage(message._chatSessionId);
		}
		// 2.1 判断当前会话是否被选中，如果是，就直接显示在消息展示区，并更新会话列表中显示的最后一条消息

		// 2.2 如果不是，就更新新消息到会话列表中最后一条消息，并新增未读消息个数

	}

	void NetClient::handleWsRemoveFriend(const QString& userId)
	{
		model::UserInfo* userInfo = dataCenter->findFriendById(userId);
		// 1. 删除DataCenter中的数据
		dataCenter->removeFriend(userId);
		// 2. 更新窗口
		emit dataCenter->deleteFriendDone();
		if (userInfo != nullptr)
		{
			QString msg = "您已经被" + userInfo->_nickname + "删除";
			Toast::showMessage(msg);
		}
	}

	void NetClient::handleWsAddFriendApply(const model::UserInfo& userInfo)
	{
		// 1. 将数据添加到好友申请列表中
		QList<model::UserInfo>* applyList = dataCenter->getApplyList();
		if (applyList == nullptr)
		{
			LOG() << "客户端没有加载到好友申请列表";
			return;
		}
		applyList->push_front(userInfo); //这里要头插
		// 2. 通知界面更新
		emit dataCenter->receiveFriendApplyDone();
	}

	void NetClient::handleWsAddFriendProcess(const model::UserInfo& userInfo, bool agree)
	{
		if (agree)
		{
			QList<model::UserInfo>* friendList = dataCenter->getFriendList();
			if (friendList == nullptr)
			{
				LOG() << "客户端没有加载好友列表";
				return;
			}
			friendList->push_front(userInfo);

			emit dataCenter->receiveFriendProcessDone(userInfo._nickname, agree);
		}
		else
		{
			emit dataCenter->receiveFriendProcessDone(userInfo._nickname, agree);
		}
	}

	void NetClient::handleWsSessionCreate(const model::ChatSessionInfo& chatSessionInfo)
	{
		QList<model::ChatSessionInfo>* chatSessiaonList = dataCenter->getChatSessionList();
		if (chatSessiaonList == nullptr)
		{
			LOG() << "客户端没有加载会话列表";
			return;
		}
		chatSessiaonList->push_front(chatSessionInfo);
		emit dataCenter->receiveSessionCreateDone();
	}

	// 接下来都是http的操作

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

    void NetClient::getRecentMessagesList(const QString &loginSessionId, const QString &chatSessionId, bool updateUI)
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
            if(updateUI) // 如果需要更新界面
				emit dataCenter->getRecentMessageListDone(chatSessionId); // 这是在发起一个信号
			else
				emit dataCenter->getRecentMessageListDoneNoUI(chatSessionId);
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

	void NetClient::receiverMessage(const QString& chatSessionId)
	{
		// 1. 先需要判定一下，当前消息所在会话是否是用户选中的
		if (dataCenter->getCurrentChatSessionId() == chatSessionId)
		{
			// 如果是，就在消息展示区新增数据
			const model::Message& lastMessage = dataCenter->getRecentMessagesList(chatSessionId)->back();
			// 通过信号，让 NetClient 模块，能够通知界面 （消息展示区）
			emit dataCenter->receiveMessageDone(lastMessage);
		}
		else
		{
			// 如果不是，就更新未读消息数目
			dataCenter->addUnread(chatSessionId);

		}
		// 统一更新会话消息预览
		emit dataCenter->updateLastMessage(chatSessionId);
	}

	void NetClient::changeNickname(const QString& loginSessionId, const QString& nickname)
	{
		zht_im::SetUserNicknameReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setNickname(nickname);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[修改用户昵称] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" 
			<< loginSessionId << " nickname=" << nickname;

		QNetworkReply* httpResp = sendHttpRequest("/service/user/set_nickname", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::SetUserNicknameRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[修改用户昵称] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetNickname(nickname);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->changeNicknameDone(); // 这是在发起一个信号

			LOG() << "[修改用户昵称] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::changeDescription(const QString& loginSessionId, const QString& description)
	{
		zht_im::SetUserDescriptionReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setDescription(description);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[修改用户签名] 发送请求 requestId=" << req.requestId() << ", loginSessionId="
			<< loginSessionId << " description=" << description;

		QNetworkReply* httpResp = sendHttpRequest("/service/user/set_description", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::SetUserDescriptionRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[修改用户签名] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetDescription(description);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->changeDescription(); // 这是在发起一个信号

			LOG() << "[修改用户签名] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::getVerifyCode(const QString& phone)
	{
		zht_im::PhoneVerifyCodeReq req;
		req.setRequestId(makeRequestId());
		req.setPhoneNumber(phone);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[获取手机验证码] 发送请求 requestId=" << req.requestId() << ", phone=" << phone;

		QNetworkReply* httpResp = sendHttpRequest("/service/user/get_phone_verify_code", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::PhoneVerifyCodeRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[获取手机验证码] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetVerifyCodeId(resp->verifyCodeId());

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->getVerifyCodeDone(); // 这是在发起一个信号

			LOG() << "[获取手机验证码] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::changePhone(const QString& loginSessionId, const QString& phone, const QString& verifyCodeId, const QString& verifyCode)
	{
		zht_im::SetUserPhoneNumberReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setPhoneNumber(phone);
		req.setPhoneVerifyCodeId(verifyCodeId);
		req.setPhoneVerifyCode(verifyCode);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[修改手机号] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", phone=" << req.phoneNumber() << ", verifyCodeId=" << req.phoneVerifyCodeId() 
			<< ", verifyCode=" << req.phoneVerifyCode();

		QNetworkReply* httpResp = sendHttpRequest("/service/user/set_phone", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::SetUserPhoneNumberRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[修改手机号] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetPhone(phone);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->changePhoneDone(); // 这是在发起一个信号

			LOG() << "[修改手机号] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::changeAvatar(const QString& loginSessionId, const QByteArray& imageBytes)
	{
		zht_im::SetUserAvatarReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setAvatar(imageBytes);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[修改头像] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId();

		QNetworkReply* httpResp = sendHttpRequest("/service/user/set_avatar", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::SetUserAvatarRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[修改头像] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 把响应保存到DataCenter中
			dataCenter->resetAvatar(imageBytes);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->changeAvatarDone(); // 这是在发起一个信号

			LOG() << "[修改头像] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::deleteFriend(const QString& loginSessionId, const QString& userId)
	{
		zht_im::FriendRemoveReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setPeerId(userId);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[删除好友] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId() << ", peerId=" << req.peerId();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/remove_friend", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::FriendRemoveRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[删除好友] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 删除DataCenter中对引发给的好友信息
			dataCenter->removeFriend(userId);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->deleteFriendDone(); // 这是在发起一个信号

			LOG() << "[删除好友] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::addFriendApply(const QString& loginSessionId, const QString& userId)
	{
		zht_im::FriendAddReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setRespondentId(userId);
		QByteArray body = req.serialize(&serializer);
		LOG() << "[添加好友申请] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId() 
			<< ", userId=" <<  req.respondentId();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/add_friend_apply", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::FriendAddRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[添加好友申请] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->addFriendApplyDone(); // 这是在发起一个信号

			LOG() << "[添加好友申请] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::acceptFriendApply(const QString& loginSessionId, const QString& userId)
	{
		zht_im::FriendAddProcessReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setAgree(true);
		req.setApplyUserId(userId);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[同意好友申请] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", userId=" << req.applyUserId();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/add_friend_process", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::FriendAddProcessRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[同意好友申请] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}
			// 做一个好友申请的处理 1）从好友申请列表中删除 2） 把对应的用户信息添加到好友列表
			model::UserInfo applyUser = dataCenter->removeFriendApplyList(userId);
			QList<model::UserInfo>* friendList = dataCenter->getFriendList();
			friendList->push_front(applyUser);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->acceptFriendApplyDone(); // 这是在发起一个信号

			LOG() << "[同意好友申请] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::rejectFriendApply(const QString& loginSessionId, const QString& userId)
	{
		zht_im::FriendAddProcessReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setAgree(false);
		req.setApplyUserId(userId);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[拒绝好友申请] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", userId=" << req.applyUserId();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/add_friend_process", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::FriendAddProcessRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[拒绝好友申请] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			dataCenter->removeFriendApplyList(userId);

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->rejectFriendApplyDone(); // 这是在发起一个信号

			LOG() << "[拒绝好友申请] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::createGroupChatSession(const QString& loginSessionId, const QList<QString>& userIdList)
	{
		zht_im::ChatSessionCreateReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		QString groupName = dataCenter->getMyself()->_nickname;
		for (auto& id : userIdList)
		{
			model::UserInfo* userInfo = dataCenter->findFriendById(id);
			if (userInfo != nullptr) groupName += userInfo->_nickname;
		}
		if (groupName.size() > 20) groupName = groupName.left(17) + "...";
		req.setChatSessionName(groupName);
		req.setMemberIdList(userIdList);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[创建群聊会话] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", userIdList=" << userIdList;

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/create_chat_session", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::ChatSessionCreateRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[创建群聊会话] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->createGroupChatSessionDone(); // 这是在发起一个信号

			LOG() << "[创建群聊会话] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::getMemberList(const QString& loginSessionId, const QString& chatSessionId)
	{
		zht_im::GetChatSessionMemberReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setChatSessionId(chatSessionId);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[获取会话成员列表] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", chatSessionId=" << req.chatSessionId();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/get_chat_session_member", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::GetChatSessionMemberRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[获取会话成员列表] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			dataCenter->resetMemberList(chatSessionId, resp->memberInfoList());

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->getMemberListDone(chatSessionId); // 这是在发起一个信号

			LOG() << "[获取会话成员列表] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::searchUser(const QString& loginSessionId, const QString& searchKey)
	{
		zht_im::FriendSearchReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setSearchKey(searchKey);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[搜索用户] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", searchKey=" << req.searchKey();

		QNetworkReply* httpResp = sendHttpRequest("/service/friend/search_friend", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::FriendSearchRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[搜索用户] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			dataCenter->resetSearchUserResult(resp->userInfo());

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->searchUserDone(); // 这是在发起一个信号

			LOG() << "[搜索用户] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::searchMessage(const QString& loginSessionId, const QString& chatSessionId, const QString& searchKey)
	{
		zht_im::MsgSearchReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setChatSessionId(chatSessionId);
		req.setSearchKey(searchKey);

		QByteArray body = req.serialize(&serializer);
		LOG() << "[按关键字搜索历史消息] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", chatSessionId=" << req.chatSessionId() << ", searchKey=" << req.searchKey();

		QNetworkReply* httpResp = sendHttpRequest("/service/message_storage/search_history", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::MsgSearchRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[按关键字搜索历史消息] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			dataCenter->resetSearchMessageResult(resp->msgList());

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->searchMessageDone(); // 这是在发起一个信号

			LOG() << "[按关键字搜索历史消息] 处理响应完毕 requestId=" << req.requestId();
			});
	}

	void NetClient::searchMessageByTime(const QString& loginSessionId, const QString& chatSessionId, const QDateTime& beginTime, const QDateTime& endTime)
	{
		zht_im::GetHistoryMsgReq req;
		req.setRequestId(makeRequestId());
		req.setSessionId(loginSessionId);
		req.setChatSessionId(chatSessionId);
		req.setStartTime(beginTime.toSecsSinceEpoch());
		req.setOverTime(endTime.toSecsSinceEpoch());

		QByteArray body = req.serialize(&serializer);
		LOG() << "[按时间搜索历史消息] 发送请求 requestId=" << req.requestId() << ", loginSessionId=" << req.sessionId()
			<< ", chatSessionId=" << req.chatSessionId() << ", beginTime=" << beginTime << ", endTime=" << endTime;

		QNetworkReply* httpResp = sendHttpRequest("/service/message_storage/get_history", body);

		connect(httpResp, &QNetworkReply::finished, this, [=]() {
			bool ok = false;
			QString reason = "";
			auto resp = handleHttpResponse<zht_im::GetHistoryMsgRsp>(httpResp, &ok, &reason);
			if (!ok)
			{
				LOG() << "[按时间搜索历史消息] 出错, requestId=" << req.requestId() << ", reason = " << reason;
				return;
			}

			dataCenter->resetSearchMessageResult(resp->msgList());

			// 发送信号通知调用逻辑，已经处理完毕响应
			emit dataCenter->searchMessageDone(); // 这是在发起一个信号

			LOG() << "[按时间搜索历史消息] 处理响应完毕 requestId=" << req.requestId();
			});
	}

} // namespace network
