#pragma once

#include <QWidget>
#include "data.h"
#include "../network/NetClient.h"
namespace model
{
	class DataCenter : public QObject
	{
		Q_OBJECT

	private:
		DataCenter();
		static DataCenter* instance;

        network::NetClient netClient;								// 让DateCenter持有NetClient实例

		QString loginSessionId;									// 当前客户端登录到服务器的 登录会话ID
		UserInfo* myself = nullptr;									// 当前的用户信息
		QList<UserInfo>* friendList = nullptr;						// 当前用户的好友列表
		QList<ChatSessionInfo>* chatSessionList = nullptr;			// 会话列表
		QString currentChatSessionId = "";							// 当前选中的会话是哪一个
		QHash<QString, QList<UserInfo>>* memberList = nullptr;		// 每个会话中的成员有哪些 -- 针对群聊 (K:ChatSessionId, V: 成员列表)
		QList<UserInfo>* applyList = nullptr;						// 待处理的好友申请
		QHash<QString, QList<Message>>* recentMessages = nullptr;	// 每个会话的最近消息   (K:ChatSessionId, V: 消息列表)
		QHash<QString, int>* unreadMessageCount = nullptr;			// 存储每个会话的未读消息个数 (K:ChatSessionId, V: 未读消息个数)
		QList<UserInfo>* searchUserResult = nullptr;				// 用户的好友搜索结果
		QList<Message>* searchMessage = nullptr;					// 历史消息结果
		QString currentVerifyCodeId = "";							// 短信验证码Id

	public:

		static DataCenter* getInstance();
		~DataCenter();

		void initDataFile(); // 初始化数据文件
		void saveDataFile(); // 存储数据到文件中
		void loadDataFile(); // 加载数据文件
		void clearUnread(const QString& chatSessionId); // 清空未读消息数目
		void addUnread(const QString& chatSessionId);	// 添加未读消息数目
		int getUnread(const QString& chatSessionId);	// 获取未读消息数目

		const QString& getLoginSessionId() const { return loginSessionId; }
		// 验证网络连通性
		void ping() { netClient.ping(); }

		// 通过网络获取到用户的个人信息，是一个异步函数，只负责发送HTTP请求
		void getMyselfAsync();
		UserInfo* getMyself();
		void resetMyself(std::shared_ptr<zht_im::GetUserInfoRsp> resp);

		void getFriendListAsync(); // 通过网络获取好友列表
		QList<UserInfo>* getFriendList() const { return friendList; }
		void resetFriendList(std::shared_ptr<zht_im::GetFriendListRsp> resp);

		void getChatSessionListAsync();
		QList<ChatSessionInfo>* getChatSessionList() const { return chatSessionList; }
		void resetChatSessionList(std::shared_ptr<zht_im::GetChatSessionListRsp> resp);

        // 获取申请列表
        void getApplyListAsync();
        QList<UserInfo>* getApplyList();
        void resetApplyList(std::shared_ptr<zht_im::GetPendingFriendEventListRsp> resp);

        // 获取指定会话的最近消息
        void getRecentMessagesListAsync(const QString &chatSessionId, bool updateUI);
        QList<Message>* getRecentMessagesList(const QString& chatSessionId);
        void resetRecentMessage(const QString& chatSessionId, std::shared_ptr<zht_im::GetRecentMsgRsp> resp);

        // 发送消息给服务器
		void sendTextMessageAsync(const QString& chatSessionId, const QString &content);

		// 修改用户昵称
		void changeNicknameAsync(const QString& nickname);
		void resetNickname(const QString& nickname);

		// 修改用户签名
		void changeDescriptionAsync(const QString& description);
		void resetDescription(const QString& description);

		// 获取短信验证码
		void getVerifyCodeAsync(const QString& phone); 
		void resetVerifyCodeId(const QString& verifyCodeId);
		const QString& getVerifyCodeId();				// 获取verifyCodeId
		
		// 修改手机号码
		void changePhoneAsync(const QString& phone, const QString& verifyCodeId, const QString& verifyCode);
		void resetPhone(const QString& phone);

		// 修改头像
		void changeAvatarAsync(const QByteArray& imageBytes);
		void resetAvatar(const QByteArray& avatar);

		// 删除好友
		void deleteFriendAsync(const QString& userId);
		void removeFriend(const QString& userId);

		// 添加好友申请
		void addFriendApplyAsync(const QString& userId);

		// 同意好友申请
		void acceptFriendApplyAsync(const QString& userId);
		UserInfo removeFriendApplyList(const QString& userId);

		// 拒绝好友申请
		void rejectFriendApplyAsync(const QString& userId);

		// 创建群聊
		void createGroupChatSessionAsync(const QList<QString>& userIdList);

		// 获取会话的成员列表
		void getMemberListAsync(const QString& chatSessionId);
		QList<UserInfo>* getMemberList(const QString& chatSessionId);
		void resetMemberList(const QString& chatSessionId, const QList<zht_im::UserInfo>& memberList);

		// 搜索用户
		void searchUserAsync(const QString& searchKey);
		QList<UserInfo>* getSearchUserResult();
		void resetSearchUserResult(const QList<zht_im::UserInfo>& userList);

        //   ---------------辅助函数------------
        
		ChatSessionInfo* findChatSessionById(const QString& chatSessionId);		// 根据会话ID查询会话信息
		ChatSessionInfo* findChatSessionByUserId(const QString& userId);		// 根据用户Id查找
		void topChatSessionInfo(const ChatSessionInfo& chatSessionInfo);		// 把指定会话信息放在头部
        void setCurrentChatSessionId(const QString& chatSessionId);				// 获取/设置当前选中的会话
        const QString& getCurrentChatSessionId();								// 获取/设置当前选中的会话
        void addMessage(const Message& message);								// 添加消息到dataCenter
		UserInfo* findFriendById(const QString& userId);						// 根据用户ID查询好友信息

	signals:
		//自定义信号
		void getMyselfDone();
		void getFriendListDone();
		void getChatSessionListDone();
        void getApplyListDone();
		void getRecentMessageListDone(const QString chatSessionId);
		void getRecentMessageListDoneNoUI(const QString chatSessionId);
        void sendMessageDone(model::MessageType messageType, const QByteArray& content, const QString& extraInfo);
        void updateLastMessage(const QString& chatSessionId);
		void receiveMessageDone(const Message& lastMessage);
		void changeNicknameDone();
		void changeDescription();
		void getVerifyCodeDone();
		void changePhoneDone();
		void changeAvatarDone();
		void clearCurrentSession();
		void deleteFriendDone();
		void addFriendApplyDone();
		void receiveFriendApplyDone();
		void acceptFriendApplyDone();
		void rejectFriendApplyDone();
		void receiveFriendProcessDone(const QString& nickname, bool agree);
		void createGroupChatSessionDone();
		void receiveSessionCreateDone();
		void getMemberListDone(const QString& chatSessionId);
		void searchUserDone();
	};  
} // namespace model
