#pragma once

#include <QObject>
#include "date.h"
namespace model
{
	class DataCenter : public QObject
	{
		Q_OBJECT

	public:

		static DataCenter* getInstance();
		void initDataFile(); // 初始化数据文件
		void saveDataFile(); // 存储数据到文件中
		void loadDataFile(); // 加载数据文件

	private:
		DataCenter();
		~DataCenter();
		static DataCenter* instance;

		QString loginChatSessionId;									// 当前客户端登录到服务器的 登录会话ID
		UserInfo* myself = nullptr;									// 当前的用户信息
		QList<UserInfo>* friendList = nullptr;						// 当前用户的好友列表
		QList<ChatSessionInfo>* chatSessionList = nullptr;			// 会话列表
		QString currentChatSessionId = "";							// 当前选中的会话是哪一个
		QHash<QString, QList<UserInfo>>* memberList = nullptr;		// 每个会话中的成员有哪些 -- 针对群聊 (K:ChatSessionId, V: 成员列表)
		QList<UserInfo>* applyList = nullptr;						// 待处理的好友申请
		QHash<QString, QList<Message>>* recentMessage = nullptr;	// 每个会话的最近消息   (K:ChatSessionId, V: 消息列表)
		QHash<QString, int>* unreadMessageCount = nullptr;			// 存储每个会话的未读消息个数 (K:ChatSessionId, V: 未读消息个数)
		QList<UserInfo>* searchUserResult = nullptr;				// 用户的好友搜索结果
		QList<Message>* searchMessage = nullptr;					// 历史消息结果
		QString currentVerifyCodeId = "";							// 短信验证码Id
	};
} // namespace model