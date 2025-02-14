#include "userinfowidget.h"
#include "model/DataCenter.h"
#include "mainwidget.h"
#include <QGridLayout>
#include <QMessagebox>

UserInfoWidget::UserInfoWidget(QWidget *parent, const UserInfo &userInfo)
    : userInfo(userInfo), QDialog(parent)
{
    // 1. 设置基本属性
    this->setFixedSize(400, 200);
    this->setWindowTitle("用户详情");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->move(QCursor::pos());

    // 2. 创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setVerticalSpacing(0);
    layout->setHorizontalSpacing(20);
    layout->setContentsMargins(40, 20, 0, 0);
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);

    // 3. 添加头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75, 75);
    avatarBtn->setIconSize(QSize(75, 75));
    avatarBtn->setIcon(userInfo._avatar);

    QString labelStyle = "QLabel { font-weight: 800; padding-left: 20px; }";
    QString btnStyle = "QPushButton { border: 1px solid rgb(100, 100, 100); border-radius: 5px; background-color: rgb(240, 240, 240); }";
    btnStyle += "QPushButton:pressed { background-color: rgb(205, 205, 205); }";

    int height = 30;
    int width = 80;

    // 4. 添加用户序号
    idTag = new QLabel();
    idTag->setText("序号");
    idTag->setStyleSheet(labelStyle);
    idTag->setFixedSize(width, height);
    idTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    idLabel = new QLabel();
    idLabel->setText(userInfo._userId);
    idLabel->setFixedSize(width, height);
    idLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 5. 添加用户昵称
    nameTag = new QLabel();
    nameTag->setText("昵称");
    nameTag->setStyleSheet(labelStyle);
    nameTag->setFixedSize(width, height);
    nameTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    nameLabel = new QLabel();
    nameLabel->setText(userInfo._nickname);
    nameLabel->setFixedSize(width, height);
    nameLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 6. 设置电话
    phoneTag = new QLabel();
    phoneTag->setText("电话");
    phoneTag->setStyleSheet(labelStyle);
    phoneTag->setFixedSize(width, height);
    phoneTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    phoneLabel = new QLabel();
    phoneLabel->setText(userInfo._phone);
    phoneLabel->setFixedSize(width, height);
    phoneLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 7. 添加功能按钮
    applyBtn = new QPushButton();
    applyBtn->setText("申请好友");
    applyBtn->setFixedSize(width, 30);
    applyBtn->setStyleSheet(btnStyle);

    sendMessageBtn = new QPushButton();
    sendMessageBtn->setText("发送消息");
    sendMessageBtn->setFixedSize(width, 30);
    sendMessageBtn->setStyleSheet(btnStyle);

    deleteFriendBtn = new QPushButton();
    deleteFriendBtn->setText("删除好友");
    deleteFriendBtn->setFixedSize(width, 30);
    deleteFriendBtn->setStyleSheet(btnStyle);

    // 8. 添加上述内容到布局管理器
    layout->addWidget(avatarBtn, 0, 0, 3, 1);

    layout->addWidget(idTag, 0, 1);
    layout->addWidget(idLabel, 0, 2);

    layout->addWidget(nameTag, 1, 1);
    layout->addWidget(nameLabel, 1, 2);

    layout->addWidget(phoneTag, 2, 1);
    layout->addWidget(phoneLabel, 2, 2);

    layout->addWidget(applyBtn, 3, 0);
    layout->addWidget(sendMessageBtn, 3, 1);
    layout->addWidget(deleteFriendBtn, 3, 2);

    // 9. 设置按钮的禁用关系
    // ---判定依据就是拿着当前好友的userId，在 DataCenter 的好友列表中查询
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    UserInfo* myFriend = dataCenter->findFriendById(this->userInfo._userId);
    if (myFriend == nullptr)
    {
        sendMessageBtn->setEnabled(false);
        deleteFriendBtn->setEnabled(false);
    }
    else
    {
        applyBtn->setEnabled(false);
    }
    // 10. 初始化信号槽
    initSignalSlot();
}

void UserInfoWidget::initSignalSlot()
{
    connect(sendMessageBtn, &QPushButton::clicked, this, [=]() {
        MainWidget* mainWidget = MainWidget::getInstance();
        mainWidget->switchSession(userInfo._userId);

        this->close();
        });
    connect(deleteFriendBtn, &QPushButton::clicked, this, &UserInfoWidget::clickDeleteFriendBtn);
    connect(applyBtn, &QPushButton::clicked, this, &UserInfoWidget::clickApplyBtn);
}

void UserInfoWidget::clickDeleteFriendBtn()
{
    auto result = QMessageBox::warning(this, "确认删除", "确认删除当前好友?", QMessageBox::Ok | QMessageBox::Cancel);
    if (result != QMessageBox::Ok)
    {
        LOG() << "删除好友取消";
        return;
    }
    // 2. 发送网络请求
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    // 由于此处的信号处理，都是在主窗口进行的，所有在主窗口捕捉结束信号
    dataCenter->deleteFriendAsync(userInfo._userId);

    // 3. 关闭窗口
    this->close();
}

void UserInfoWidget::clickApplyBtn()
{
    // 1. 发送好友申请
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->addFriendApplyAsync(userInfo._userId);
    // 2. 关闭窗口
    this->close();
}

