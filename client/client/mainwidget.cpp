#include "mainwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "utils.hpp"
#include "sessionfriendarea.h"
#include "sessiondetailwidget.h"
#include "selfinfowidget.h"
#include "groupsessiondetailwidget.h"
#include "addfrienddialog.h"

#include "debug.h"
#include "model/DataCenter.h"

MainWidget* MainWidget::instance = nullptr;

MainWidget* MainWidget::getInstance()
{
    if (instance == nullptr)
    {
        // 以桌面作为父窗口
        instance = new MainWidget(nullptr);
    }
    return instance;
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("我的聊天室");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->resize(800, 700);

    initMainWindow();   // 初始化主窗口样式布局
    initLeftWindow();   // 初始化左侧窗口布局
    initMiddleWindow(); // 初始化中间窗口布局
    initRightWindow();  // 初始化右侧窗口布局
    initSignalSlot();   // 初始化信号槽
}

MainWidget::~MainWidget() {}

void MainWidget::initMainWindow()
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(0); // 将内部元素之前的间隔设置为0
    layout->setContentsMargins(0, 0, 0, 0); // layout里面的元素距离四个边界的距离设置为0
    this->setLayout(layout);

    windowLeft = new QWidget();
    windowMiddle = new QWidget();
    windowRight = new QWidget();

    windowLeft->setFixedWidth(60);
    windowMiddle->setFixedWidth(310);
    windowRight->setMinimumWidth(700);

    windowLeft->setStyleSheet("QWidget { background-color: rgb(46, 46, 46); }");
    windowMiddle->setStyleSheet("QWidget { background-color: rgb(234, 232, 231); }");
    windowRight->setStyleSheet("QWidget { background-color: rgb(245, 245, 245); }");

    layout->addWidget(windowLeft);
    layout->addWidget(windowMiddle);
    layout->addWidget(windowRight);
}
void MainWidget::initLeftWindow()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->setContentsMargins(0, 50, 0, 0);
    windowLeft->setLayout(layout);

    // 添加用户头像
    userAvatar = new QPushButton();
    userAvatar->setFixedSize(45, 45);
    userAvatar->setIconSize(QSize(45, 45));
    // 不要设置默认头像
    //userAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    userAvatar->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: transparent; border: none; }"
        "QPushButton:pressed { background-color: transparent; border: none; }"
        );
    layout->addWidget(userAvatar, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 添加会话页标签
    sessionTableButton = new QPushButton();
    sessionTableButton->setFixedSize(40, 40);
    sessionTableButton->setIconSize(QSize(30, 30));
    sessionTableButton->setIcon(QIcon(":/resource/image/session_active.png"));
    sessionTableButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: transparent; border: none; }"
        "QPushButton:pressed { background-color: transparent; border: none; }"
        );
    layout->addWidget(sessionTableButton, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 添加好友标签页按钮
    friendTableButton = new QPushButton();
    friendTableButton->setFixedSize(40, 40);
    friendTableButton->setIconSize(QSize(30, 30));
    friendTableButton->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    friendTableButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: transparent; border: none; }"
        "QPushButton:pressed { background-color: transparent; border: none; }"
        );
    layout->addWidget(friendTableButton, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 添加好友申请标签页按钮
    applyTableButton = new QPushButton();
    applyTableButton->setFixedSize(40, 40);
    applyTableButton->setIconSize(QSize(25, 25));
    applyTableButton->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    applyTableButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: transparent; border: none; }"
        "QPushButton:pressed { background-color: transparent; border: none; }"
        );
    layout->addWidget(applyTableButton, 1, Qt::AlignTop | Qt::AlignHCenter);

    layout->addStretch(20);
}
void MainWidget::initMiddleWindow()
{
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(10);
    windowMiddle->setLayout(layout);

    searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(30);
    searchEdit->setPlaceholderText("     搜索");
    searchEdit->setStyleSheet("QLineEdit { border-radius: 5px; background-color: rgb(226, 226, 226); padding-left: 5px; }");

    addFriendButton = new QPushButton();
    addFriendButton->setFixedSize(30, 30);
    addFriendButton->setIcon(QIcon(":/resource/image/cross.png"));
    addFriendButton->setStyleSheet(
        "QPushButton { border-radius: 5px; background-color: rgb(226, 226, 226); }"
        "QPushButton:pressed { background-color: rgb(247, 247, 247); }"
        );

    sessionFriendArea = new SessionFriendArea();

    QWidget *spacer1 = new QWidget();
    spacer1->setFixedWidth(10);
    QWidget *spacer2 = new QWidget();
    spacer2->setFixedWidth(10);
    QWidget *spacer3 = new QWidget();
    spacer3->setFixedWidth(10);

    layout->addWidget(spacer1, 0, 0);
    layout->addWidget(searchEdit, 0, 1);
    layout->addWidget(spacer2, 0, 2);
    layout->addWidget(addFriendButton, 0, 3);
    layout->addWidget(spacer3, 0, 4);
    layout->addWidget(sessionFriendArea, 1, 0, 1, 5);
}
void MainWidget::initRightWindow()
{
    LOG() << "初始化右侧窗口";
    // 1. 创建右侧窗口的布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    windowRight->setLayout(vlayout);

    // 2. 创建上方标题栏
    QWidget* titleWidget = new QWidget;
    titleWidget->setFixedHeight(62);
    titleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleWidget->setObjectName("titleWidget");
    titleWidget->setStyleSheet("#titleWidget { border-bottom: 1px solid rgb(230, 230, 230); border-left:1px solid rgb(230, 230, 230);}");
    vlayout->addWidget(titleWidget);

    // 3. 给标题栏添加标题 label 和 一个按钮
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(10, 0, 10, 0); // 让标题行距离边界有点距离
    titleWidget->setLayout(hlayout);

    sessionTitleLabel = new QLabel();
    sessionTitleLabel->setStyleSheet("QLabel { font-size: 20px; border-bottom: 1px solid rgb(230, 230, 230);}");
#if TEST_UI
    // 为了测试界面而临时增加的,实际上应该从服务器获取
    sessionTitleLabel->setText("这是会话标题");
#endif
    hlayout->addWidget(sessionTitleLabel);

    extraBtn = new QPushButton();
    extraBtn->setFixedSize(30, 30);
    extraBtn->setIconSize(QSize(30, 30));
    extraBtn->setIcon(QIcon(":/resource/image/more.png"));
    extraBtn->setStyleSheet("QPushButton { border:none; background-color: rgb(245,245,245);}"
                            "QPushButton:pressed {background-color: rgb(220,220,220);}");
    hlayout->addWidget(extraBtn);

    // 4. 添加消息展示区 --宽度和高度都要自适应
    LOG() << "添加消息展示区";
    messageShowArea = new MessageShowArea();
    vlayout->addWidget(messageShowArea);

    // 5. 添加消息编辑区 -- 宽度自适应,高度固定
    LOG() << "添加消息编辑区";
    messageEditArea = new MessageEditArea();
    vlayout->addWidget(messageEditArea, 0, Qt::AlignBottom);
}

void MainWidget::initSignalSlot()
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();

    // 连接信号槽，处理标签页按钮切换的问题
    connect(sessionTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToSession);
    connect(friendTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToFriend);
    connect(applyTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToApply);

    // 点击自己头像的信号槽
    connect(userAvatar, &QPushButton::clicked, this, [=]() {
        SelfInfoWidget* selfInfoWidget = new SelfInfoWidget(this);
        selfInfoWidget->exec(); // 弹出模态会话框
    });

    // 点击会话详情按钮，弹出会话详情窗口
    connect(extraBtn, &QPushButton::clicked, this, [=]() {
        // 判断是单聊还是群聊
#if TEST_GROUP_SESSION_DETAIL
        bool isSingleChat = false; // 这个值根据当前选中的实际会话确定，后面接入服务器再确定
#else
        bool isSingleChat = true;
#endif
        if (isSingleChat) // 单聊
        {
            SessionDetailWidget* sessionDetailWidget = new SessionDetailWidget(this);
            sessionDetailWidget->exec();
        }
        else // 群聊
        {
            GroupSessionDetailWidget* groupSessionDetailWidget = new GroupSessionDetailWidget(this);
            groupSessionDetailWidget->exec();
        }
    });

    // 点击 addFriendButton 按钮，弹出添加好友窗口
    connect(addFriendButton, &QPushButton::clicked, this, [=]() {
        AddFriendDialog* addFriendDialog = new AddFriendDialog(this);
        addFriendDialog->exec();
    });

    // 修改搜索框内容 弹出添加好友对话框
    connect(searchEdit, &QLineEdit::textEdited, this, [=]() {
        // 取出当前输入框中的内容，并且设置到新弹窗中的输入框
        const QString& searchKey = searchEdit->text();

        AddFriendDialog* addFriendDialog = new AddFriendDialog(this);
        addFriendDialog->setSearchKey(searchKey);

        searchEdit->setText("");

        addFriendDialog->exec();
    });


    // 获取个人信息
    connect(dataCenter, &model::DataCenter::getMyselfDone, this, [=]() {
        // 从DataCenter中拿到响应的结果,取出头像显示到界面上
        auto myself = dataCenter->getMyself();
        userAvatar->setIcon(myself->_avatar);
        });
    dataCenter->getMyselfAsync();

    // 在客户端初始化的时候就获取好友列表、会话列表、申请列表
    loadFriendList();
    loadSessionList();
    loadApplyList();
}

void MainWidget::switchTableToSession()
{
    // 1. 记录当前切换到哪一个标签页
    activeTab = SESSION_LIST;
    // 2. 调整图标显示情况，当前会话按钮设置为active，其他变成inactive
    sessionTableButton->setIcon(QIcon(":/resource/image/session_active.png"));
    friendTableButton->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTableButton->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    // 3. 在主窗口的中间部分，加载出会话信息数据
    this->loadSessionList();
}

void MainWidget::switchTableToFriend()
{
    // 1. 记录当前切换到哪一个标签页
    activeTab = FRIEND_LIST;
    // 2. 调整图标显示情况，当前会话按钮设置为active，其他变成inactive
    sessionTableButton->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTableButton->setIcon(QIcon(":/resource/image/friend_active.png"));
    applyTableButton->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    // 3. 在主窗口的中间部分，加载出会话信息数据
    this->loadFriendList();
}

void MainWidget::switchTableToApply()
{
    // 1. 记录当前切换到哪一个标签页
    activeTab = APPLY_LIST;
    // 2. 调整图标显示情况，当前会话按钮设置为active，其他变成inactive
    sessionTableButton->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTableButton->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTableButton->setIcon(QIcon(":/resource/image/apply_active.png"));
    // 3. 在主窗口的中间部分，加载出会话信息数据
    this->loadApplyList();
}

// 加载会话列表
void MainWidget::loadSessionList()
{
    LOG() << "加载会话列表";
    if (activeTab != SESSION_LIST) return;

    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getChatSessionList() != nullptr)// 从内存中获取
    {
        updateChatSessionList();
    }
    else // 从网络中加载
    {
        // 通过最后一个参数保证信号槽不会重复绑定
        connect(dataCenter, &model::DataCenter::getChatSessionListDone, this, &MainWidget::updateChatSessionList, Qt::UniqueConnection);
        dataCenter->getChatSessionListAsync();
    }
}
// 加载好友列表
void MainWidget::loadFriendList()
{
    LOG() << "加载好友列表";
    // 好友列表数据是在 DateCenter 中存储的
    // 判断其中是否有数据，如果有就直接加载，否则就从服务器获取
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getFriendList() != nullptr)// 从内存中获取
    {
        updateFriendList();
    }
    else // 从网络中加载
    {
        // 通过最后一个参数保证信号槽不会重复绑定
        connect(dataCenter, &model::DataCenter::getFriendListDone, this, &MainWidget::updateFriendList, Qt::UniqueConnection); 
        dataCenter->getFriendListAsync();
    }
}
// 加载申请列表
void MainWidget:: loadApplyList()
{
    LOG() << "加载申请列表";
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getApplyList() != nullptr)// 从内存中获取
    {
        updateApplyList();
    }
    else // 从网络中加载
    {
        // 通过最后一个参数保证信号槽不会重复绑定
        connect(dataCenter, &model::DataCenter::getApplyListDone, this, &MainWidget::updateApplyList, Qt::UniqueConnection);
        dataCenter->getApplyListAsync();
    }
}

void MainWidget::updateFriendList()
{
    // 如果当前不是好友标签页，就不渲染任何元素
    if (activeTab != FRIEND_LIST) return;

    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    auto friendList = dataCenter->getFriendList();

    // 先清空之前界面的数据再重新添加
    sessionFriendArea->clear();

    for (const auto& f : *friendList)
    {
        sessionFriendArea->addItem(FriendItemType, f._userId, f._avatar, f._nickname, f._description);
    }
}

void MainWidget::updateChatSessionList()
{
    // 如果当前不是好友标签页，就不渲染任何元素
    if (activeTab != SESSION_LIST) return;

    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    auto chatSessionList = dataCenter->getChatSessionList();

    // 先清空之前界面的数据再重新添加
    sessionFriendArea->clear();

    for (const auto& c : *chatSessionList)
    {
        // 根据消息类型判定显示最后一条消息的内容
        // LOG() << "收到的会话列表中最后一条消息, chatSessionId=" << c._chatSessionId << "消息类型=" << c._lastMessage._messageType << " chatSessionName=" << c._chatSessionName << " 消息内容=" << c._lastMessage._content;
        QString showMsg;
        if (c._lastMessage._messageType == model::TEXT_TYPE) showMsg = c._lastMessage._content;
        else if (c._lastMessage._messageType == model::SPEECH_TYPE) showMsg = QString("[语音] " + c._lastMessage._content.size() + '\"');
        else if (c._lastMessage._messageType == model::IMAGE_TYPE) showMsg = "[图片]";
        else if (c._lastMessage._messageType == model::FILE_TYPE) showMsg = QString("[文件] " + c._lastMessage._fileName);
        else
        {
            LOG() << "错误的消息类型 messageType=" << c._lastMessage._messageType;
        }
        sessionFriendArea->addItem(SessionItemType, c._userId, c._avatar, c._chatSessionName, showMsg);
    }
}

void MainWidget::updateApplyList()
{
    if (activeTab != APPLY_LIST) return;

    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    auto applyList = dataCenter->getApplyList();

    sessionFriendArea->clear();

    for (const auto& f : *applyList)
    {
        sessionFriendArea->addItem(ApplyItemType, f._userId, f._avatar, f._nickname, f._description);
    }
}

void MainWidget::loadRecentMessage(const QString &chatSessionId)
{
    // 先判断本地内存中是否存在数据，如果有就直接加载，如果没有的话就就从网络中获取
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getRecentMessages(chatSessionId) != nullptr)
    {
        updateRecentMessage(chatSessionId);
    }
    else
    {
        connect(dataCenter, &model::DataCenter::getRecentMessageListDone, this, &MainWidget::updateRecentMessage, Qt::UniqueConnection);
        dataCenter->getRecentMessagesListAsync(chatSessionId);
    }
}

void MainWidget::updateRecentMessage(const QString &chatSessionId)
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    auto recentMessage = dataCenter->getRecentMessages(chatSessionId);
    messageShowArea->clear();
    // 由于用户首先看到的是最近的消息，所以我们选择先加载最近消息
    for(int i =  recentMessage->size() - 1; i >= 0; --i)
    {
        const Message& message = recentMessage->at(i);
        bool isLeft = message._sender._userId != dataCenter->getMyself()->_userId;
        messageShowArea->addFrontMessage(isLeft, message);
    }
    // 设置会话标题
    model::ChatSessionInfo* chatSessionInfo = dataCenter->findChatSessionById(chatSessionId);
    if(chatSessionInfo != nullptr)
    {
        sessionTitleLabel->setText(chatSessionInfo->_chatSessionName);
    }
    // 保存当前选中的会话
    dataCenter->setCurrentChatSessionId(chatSessionId);

    // 自动把滚动条滚动到末尾
    messageShowArea->scrollToEnd();
}
