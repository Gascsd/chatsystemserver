#include "mainwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "model/date.h"

#include "sessionfriendarea.h"

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
    windowRight->setMinimumWidth(500);

    windowLeft->setStyleSheet("QWidget { background-color: rgb(46, 46, 46); }");
    windowMiddle->setStyleSheet("QWidget { background-color: rgb(234, 232, 231); }");
    windowRight->setStyleSheet("QWidget { background-color: rgb(240, 240, 240); }");

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
    userAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
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
    layout->setSpacing(0);
    windowMiddle->setLayout(layout);

    searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(30);
    searchEdit->setPlaceholderText("    搜索");
    searchEdit->setStyleSheet("QLineEdit { border-radius: 5px; background-color: rgb(226, 226, 226); padding-left: 5px; }");

    addFriendButton = new QPushButton();
    addFriendButton->setFixedSize(30, 30);
    addFriendButton->setIcon(QIcon(":/resource/image/cross.png"));
    addFriendButton->setStyleSheet(
        "QPushButton { border-radius: 5px; background-color: rgb(226, 226, 226); }"
        "QPushButton:pressed { background-color: rgb(240, 240, 240); }"
        );

    SessionFriendArea* sessionFriendArea = new SessionFriendArea();

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

}

void MainWidget::initSignalSlot()
{
    // 连接信号槽，处理标签页按钮切换的问题
    connect(sessionTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToSession);
    connect(friendTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToFriend);
    connect(applyTableButton, &QPushButton::clicked, this, &MainWidget::switchTableToApply);
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
}
// 加载好友列表
void MainWidget::loadFriendList()
{
    LOG() << "加载好友列表";
}
// 加载申请列表
void MainWidget::loadApplyList()
{
    LOG() << "加载申请列表";
}
