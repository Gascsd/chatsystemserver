#include "DataCenter.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace model
{
	DataCenter* DataCenter::instance = nullptr;

	DataCenter* DataCenter::getInstance()
	{
		if (instance == nullptr)
			instance = new DataCenter();
		return instance;
	}

	void DataCenter::initDataFile()
	{
		// 构造文件路径
		QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QString filePath = basePath + "/ChatClient.json";
		LOG() << "filePath=" << filePath;

		QDir dir;
		if (!dir.exists(basePath))
		{
			dir.mkpath(basePath); 
		}

		// 创建文件
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			LOG() << "打开文件失败";
			return;
		}
		// 写入初始内容
		QString data = "{\n\n}";
		file.write(data.toUtf8());
		file.close();
	}

	void DataCenter::saveDataFile()
	{
		QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ChatClient.json";
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			LOG() << "打开文件失败 " << file.errorString();
			return;
		}
		// 按照Json的格式来写入
		QJsonObject jsonObj;
		jsonObj["loginSessionId"] = loginSessionId;

		QJsonObject jsonUnread;
		for (auto it = unreadMessageCount->begin(); it != unreadMessageCount->end(); ++it)
		{
			jsonUnread[it.key()] = it.value();
		}
		jsonObj["unread"] = jsonUnread;
		QJsonDocument jsonDoc(jsonObj);
		QString s = jsonDoc.toJson();
		file.write(s.toUtf8());

		file.close();
	}

	// 在 DateCenter 被实例化的时候调用
	void DataCenter::loadDataFile()
	{
		// 在加载之前，先针对文件进行初始化
		QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ChatClient.json";

		// 判断文件是否存在，不存在则初始化，并创建空白的 json 文件
		QFileInfo fileInfo(filePath);
		if (!fileInfo.exists())
		{
			initDataFile();
		}

		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			LOG() << "打开文件失败 " << file.errorString();
			return;
		}
		// 读取文件内容并且解析
		QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
		if (jsonDoc.isNull())
		{
			LOG() << "解析 JSON 文件失败，JSON 文件格式错误";
			return;
		}

		QJsonObject jsonObj = jsonDoc.object();
		loginSessionId = jsonObj["loginSessionId"].toString();
		LOG() << "loginSessionId=" << loginSessionId;
		unreadMessageCount->clear();
		QJsonObject jsonUnread = jsonObj["unread"].toObject();
		for (auto it = jsonUnread.begin(); it != jsonUnread.end(); ++it)
		{
			unreadMessageCount->insert(it.key(), it.value().toInt());
		}
		file.close();
	}

	void DataCenter::clearUnread(const QString& chatSessionId)
	{
		(*unreadMessageCount)[chatSessionId] = 0;
		saveDataFile(); // 防止程序崩溃出问题
	}

	void DataCenter::addUnread(const QString& chatSessionId)
	{
		(*unreadMessageCount)[chatSessionId]++;
		saveDataFile(); // 防止程序崩溃出问题
	}

	int DataCenter::getUnread(const QString& chatSessionId)
	{
		return (*unreadMessageCount)[chatSessionId];
	}

	void DataCenter::getMyselfAsync()
	{
		LOG() << "dataCenter 调用获取个人信息接口";
		// DataCenter 只是负责 "处理数据"， 真正访问网络进行通信，则是通过NetClient
		netClient.getMyself(loginSessionId);
	}

	UserInfo* DataCenter::getMyself()
	{
		return myself;
	}

	void DataCenter::resetMyself(std::shared_ptr<zht_im::GetUserInfoRsp> resp)
	{
		if (myself == nullptr)
		{
			myself = new UserInfo();
		}
		const zht_im::UserInfo& userInfo = resp->userInfo();
		myself->load(userInfo);
	}

	void DataCenter::getFriendListAsync()
	{
		netClient.getFriendList(loginSessionId);
	}

	void DataCenter::resetFriendList(std::shared_ptr<zht_im::GetFriendListRsp> resp)
	{
		if (friendList == nullptr)
			friendList = new QList<UserInfo>();
		friendList->clear();

		QList<zht_im::UserInfo>& friendListPB = resp->friendList();
		for (auto& f : friendListPB)
		{
			UserInfo userInfo;
			userInfo.load(f);
			friendList->push_back(userInfo);
		}
	}

	void DataCenter::getChatSessionListAsync()
	{
		netClient.getChatSessionList(loginSessionId);
	}

	void DataCenter::resetChatSessionList(std::shared_ptr<zht_im::GetChatSessionListRsp> resp)
	{
		if (chatSessionList == nullptr)
			chatSessionList = new QList<ChatSessionInfo>();
		chatSessionList->clear();

		auto& chatSessionPB = resp->chatSessionInfoList();
		for (auto& c : chatSessionPB)
		{
			ChatSessionInfo chatSessionInfo;
			chatSessionInfo.load(c);
			chatSessionList->push_back(chatSessionInfo);
		}
	}

    void DataCenter::getApplyListAsync()
    {
        netClient.getApplyList(loginSessionId);
    }

    QList<UserInfo> *DataCenter::getApplyList()
    {
        return applyList;
    }

    void DataCenter::resetApplyList(std::shared_ptr<zht_im::GetPendingFriendEventListRsp> resp)
    {
        if (applyList == nullptr)
            applyList = new QList<UserInfo>();
        applyList->clear();

        auto& applyListPB = resp->event();
        for (auto& a : applyListPB)
        {
            UserInfo userInfo;
            userInfo.load(a.sender());
            applyList->push_back(userInfo);
        }
    }

    QList<Message> *DataCenter::getRecentMessagesList(const QString &chatSessionId)
    {
        if (!recentMessages->contains(chatSessionId))
        {
            return nullptr;
        }
        return &(*recentMessages)[chatSessionId];
    }

    void DataCenter::resetRecentMessage(const QString &chatSessionId, std::shared_ptr<zht_im::GetRecentMsgRsp> resp)
    {
        // 拿到chatSessionId 对应的消息列表并清空
        QList<Message>& messageList = (*recentMessages)[chatSessionId];
        messageList.clear();

        for(auto& m : resp->msgList())
        {
            Message message;
            message.load(m);

            messageList.push_back(message);
        }
    }

    void DataCenter::sendTextMessageAsync(const QString &chatSessionId, const QString &content)
    {
        netClient.setMessage(loginSessionId, chatSessionId, MessageType::TEXT_TYPE, content.toUtf8());
    }

	void DataCenter::changeNicknameAsync(const QString& nickname)
	{
		netClient.changeNickname(loginSessionId, nickname);
	}

	void DataCenter::changeDescriptionAsync(const QString& description)
	{
		netClient.changeDescription(loginSessionId, description);
	}

	void DataCenter::resetDescription(const QString& description)
	{
		if (myself == nullptr)
		{
			return;
		}
		myself->_description = description;
	}

	void DataCenter::getVerifyCodeAsync(const QString& phone)
	{
		netClient.getVerifyCode(phone);
	}

	void DataCenter::resetVerifyCodeId(const QString& verifyCodeId)
	{
		currentVerifyCodeId = verifyCodeId;
	}

	const QString& DataCenter::getVerifyCodeId()
	{
		return currentVerifyCodeId;
	}

	void DataCenter::changePhoneAsync(const QString& phone, const QString& verifyCodeId, const QString& verifyCode)
	{
		netClient.changePhone(loginSessionId, phone, verifyCodeId, verifyCode);
	}

	void DataCenter::resetPhone(const QString& phone)
	{
		if (myself == nullptr)
		{
			return;
		}
		myself->_phone = phone;
	}

	void DataCenter::changeAvatarAsync(const QByteArray& imageBytes)
	{
		netClient.changeAvatar(loginSessionId, imageBytes);
	}

	void DataCenter::resetAvatar(const QByteArray& avatar)
	{
		if (myself == nullptr)
		{
			return;
		}
		myself->_avatar = makeIcon(avatar);
	}

	void DataCenter::deleteFriendAsync(const QString& userId)
	{
		netClient.deleteFriend(loginSessionId, userId);
	}

	void DataCenter::removeFriend(const QString& userId)
	{
		if (friendList == nullptr || chatSessionList == nullptr) return;
		friendList->removeIf([=](const UserInfo& userInfo) {
			return userInfo._userId == userId;
			});
		// 还要考虑会话列表
		chatSessionList->removeIf([=](const ChatSessionInfo& chatSessionInfo) {
			if (chatSessionInfo._userId == "") // 群聊不受影响
				return false;
			if (chatSessionInfo._userId == userId)
			{
				// 此处如果删除的会话正好是用户当前选中的会话，此时就需要把当前选中的会话内容(标题和消息列表)都清空
				if (chatSessionInfo._chatSessionId == this->getCurrentChatSessionId())
				{
					emit this->clearCurrentSession();
				}
				return true;
			}

			return false;
			});
	}

	void DataCenter::addFriendApplyAsync(const QString& userId)
	{
		netClient.addFriendApply(loginSessionId, userId);
	}

	void DataCenter::acceptFriendApplyAsync(const QString& userId)
	{
		netClient.acceptFriendApply(loginSessionId, userId);
	}

	UserInfo DataCenter::removeFriendApplyList(const QString& userId)
	{
		if (applyList == nullptr)
			return UserInfo();
		for (auto it = applyList->begin(); it != applyList->end(); ++it)
		{
			if (it->_userId == userId)
			{
				UserInfo toDelete = *it;
				applyList->erase(it);
				return toDelete;
			}
		}
		return UserInfo();
	}

	void DataCenter::rejectFriendApplyAsync(const QString& userId)
	{
		netClient.rejectFriendApply(loginSessionId, userId);
	}

	void DataCenter::createGroupChatSessionAsync(const QList<QString>& userIdList)
	{
		netClient.createGroupChatSession(loginSessionId, userIdList);
	}

	void DataCenter::getMemberListAsync(const QString& chatSessionId)
	{
		netClient.getMemberList(loginSessionId, chatSessionId);
	}

	QList<UserInfo>* DataCenter::getMemberList(const QString& chatSessionId)
	{
		if (!this->memberList->contains(chatSessionId))
			return nullptr;
		return &(*this->memberList)[chatSessionId];
	}

	void DataCenter::resetMemberList(const QString& chatSessionId, const QList<zht_im::UserInfo>& memberList)
	{
		auto& currentMemberList = (*this->memberList)[chatSessionId];
		currentMemberList.clear();
		for (const auto& m : memberList)
		{
			UserInfo userInfo;
			userInfo.load(m);
			currentMemberList.push_back(userInfo);
		}
	}

	void DataCenter::searchUserAsync(const QString& searchKey)
	{
		netClient.searchUser(loginSessionId, searchKey);
	}

	QList<UserInfo>* DataCenter::getSearchUserResult()
	{
		return searchUserResult;
	}

	void DataCenter::resetSearchUserResult(const QList<zht_im::UserInfo>& userList)
	{
		if (searchUserResult == nullptr)
			searchUserResult = new QList<UserInfo>();
		searchUserResult->clear();

		for (const auto& u : userList)
		{
			UserInfo userInfo;
			userInfo.load(u);
			searchUserResult->push_back(userInfo);
		}
	}

	void DataCenter::searchMessageAsync(const QString& searchKey)
	{
		netClient.searchMessage(loginSessionId, this->currentChatSessionId, searchKey);
	}

	void DataCenter::searchMessageByTimeAsync(const QDateTime& beginTime, const QDateTime& endTime)
	{
		netClient.searchMessageByTime(loginSessionId, this->currentChatSessionId, beginTime, endTime);
	}

	QList<Message>* DataCenter::getSearchMessageResult()
	{
		return searchMessageResult;
	}

	void DataCenter::resetSearchMessageResult(const QList<zht_im::MessageInfo>& msgList)
	{
		if (searchMessageResult == nullptr)
		{
			searchMessageResult = new QList<Message>();
		}
		searchMessageResult->clear();

		for (const auto& m : msgList)
		{
			Message msg;
			msg.load(m);
			searchMessageResult->push_back(msg);
		}
	}

	void DataCenter::resetNickname(const QString& nickname)
	{
		if (myself == nullptr)
		{
			return;
		}
		myself->_nickname = nickname;
	}

    ChatSessionInfo *DataCenter::findChatSessionById(const QString &chatSessionId)
    {
        if(chatSessionList == nullptr)
            return nullptr;
        for(auto& info : *chatSessionList)
        {
            if(info._chatSessionId == chatSessionId) return &info;
        }
        return nullptr;
    }

	ChatSessionInfo* DataCenter::findChatSessionByUserId(const QString& userId)
	{
		if (chatSessionList == nullptr)
			return nullptr;
		for (auto& info : *chatSessionList)
		{
			if (info._userId == userId) return &info;
		}
		return nullptr;
	}

	void DataCenter::topChatSessionInfo(const ChatSessionInfo& chatSessionInfo)
	{
		if (chatSessionList == nullptr)
			return ;
		auto it = chatSessionList->begin();
		for (; it != chatSessionList->end(); ++it)
		{
			if (it->_chatSessionId == chatSessionInfo._chatSessionId)
				break;
		}
		if (it == chatSessionList->end()) return; // 正常来说不会出现这种情况

		ChatSessionInfo backup = chatSessionInfo;
		chatSessionList->erase(it);

		chatSessionList->push_front(backup);
	}

    void DataCenter::setCurrentChatSessionId(const QString &chatSessionId)
    {
        currentChatSessionId = chatSessionId;
    }

    const QString &DataCenter::getCurrentChatSessionId()
    {
        return currentChatSessionId;
    }

    void DataCenter::addMessage(const Message &message)
    {
        QList<Message>& messageList = (*recentMessages)[message._chatSessionId];
        messageList.push_back(message);
        LOG() << "把消息添加到dataCenter中，消息内容为:" << message._content;
    }

	UserInfo* DataCenter::findFriendById(const QString& userId)
	{
		if (this->friendList == nullptr) return nullptr;
		for (auto& f : *friendList)
		{
			if (f._userId == userId) return &f;
		}
		return nullptr;
	}

    void DataCenter::getRecentMessagesListAsync(const QString &chatSessionId, bool updateUI)
    {
        netClient.getRecentMessagesList(loginSessionId, chatSessionId, updateUI);
    }

	DataCenter::DataCenter()  : netClient(this)
	{
		// 此处只是把这几个 hash 类型的属性 new 出实例. 其他的 QList 类型的属性, 都暂时不实例化.
		// 主要是为了使用 nullptr 表示 "非法状态"
		// 对于 hash 来说, 不关心整个 QHash 是否是 nullptr, 而是关心, 某个 key 对应的 value 是否存在~~
		// 通过 key 是否存在, 也能表示该值是否有效.
		recentMessages = new QHash<QString, QList<Message>>();
		memberList = new QHash<QString, QList<UserInfo>>();
		unreadMessageCount = new QHash<QString, int>();

		// 加载数据
		loadDataFile();
	}


	DataCenter::~DataCenter()
	{
		delete myself;
		delete friendList;
		delete chatSessionList;
		delete memberList;
		delete applyList;
		delete recentMessages;
		delete unreadMessageCount;
		delete searchUserResult;
		delete searchMessageResult;
	}
} // namespace model
