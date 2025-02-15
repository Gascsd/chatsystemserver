#include "sessiondetailwidget.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include "debug.h"
#include "choosefrienddialog.h"
#include "model/DataCenter.h"

AvatarItem::AvatarItem(const QIcon &avatar, const QString &name)
{
    // 1. 设置基本属性
    this->setFixedSize(70, 80);

    // 2. 创建布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignHCenter);
    this->setLayout(layout);

    // 3. 创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(45, 45);
    avatarBtn->setIconSize(QSize(45, 45));
    avatarBtn->setIcon(avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none }");

    // 4. 创建名字
    nameLabel = new QLabel();
    nameLabel->setText(name);
    QFont font("微软雅黑");
    nameLabel->setFont(font);
    nameLabel->setAlignment(Qt::AlignCenter);

    // 5. 对名字进行截断
    const int MAX_WIDTH = 65;
    QFontMetrics metrics(font);
    int totalWidth = metrics.horizontalAdvance(name);
    if(totalWidth >= MAX_WIDTH)
    {
        QString tail = "...";
        int tailWidth = metrics.horizontalAdvance(tail);
        int availableWidth = MAX_WIDTH - tailWidth;
        int availableSize = name.size() * ((double)availableWidth / totalWidth);
        QString newName = name.left(availableSize);
        nameLabel->setText(newName + tail);
    }
    else
    {
        nameLabel->setText(name);
    }
    layout->addWidget(avatarBtn);
    layout->addWidget(nameLabel);
}

SessionDetailWidget::SessionDetailWidget(QWidget *parent, const model::UserInfo& userInfo)
    : userInfo(userInfo)
{
    // 1. 设置基本属性
    this->setWindowTitle("会话详情");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setFixedSize(300, 300);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setStyleSheet("QWidget { background-color: rgb(255, 255, 255); }");

    QPoint cursorPos = QCursor::pos();
    int newX = cursorPos.x() - this->width();  // 确保窗口的右边缘与鼠标位置对齐
    int newY = cursorPos.y();                  // Y坐标保持不变，使得窗口顶部与鼠标的Y坐标对齐
    this->move(newX, newY);

    // 2. 创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(50, 0, 50, 0);
    this->setLayout(layout);

    // 3. 添加 创建群聊 按钮
    AvatarItem* creatrGroupBtn = new AvatarItem(QIcon(":/resource/image/cross.png"), "添加");
    layout->addWidget(creatrGroupBtn, 0, 0);

    // 4. 添加当前用户信息 （临时构造的假数据）
#if TEST_UI
    AvatarItem* currentUser = new AvatarItem(QIcon(":/resource/image/defaultAvatar.png"), "张三123456");
    layout->addWidget(currentUser, 0, 1);
#endif
    AvatarItem* currentUser = new AvatarItem(userInfo._avatar, userInfo._nickname);
    layout->addWidget(currentUser, 0, 1);


    // 5. 添加删除好友按钮
    deleteFriendBtn = new QPushButton();
    deleteFriendBtn->setFixedHeight(50);
    deleteFriendBtn->setText("删除好友");
    QString style = "QPushButton { border: 1px solid rgb(90, 90, 90); border-radius: 5px;}";
    style += "QPushButton:pressed { background-color rgb(235, 235, 235);}";;
    deleteFriendBtn->setStyleSheet(style);
    layout->addWidget(deleteFriendBtn, 1, 0, 1, 3);

    // 6. 添加信号槽，处理 创建群聊 按钮
    connect(creatrGroupBtn->getAvatar(), &QPushButton::clicked, this, [=]() {
        ChooseFriendDialog* chooseFriendDialog = new ChooseFriendDialog(this, userInfo._userId);
        chooseFriendDialog->exec();
    });

    connect(deleteFriendBtn, &QPushButton::clicked, this, &SessionDetailWidget::clickDeleteFriendBtn);
}

void SessionDetailWidget::clickDeleteFriendBtn()
{
    auto result = QMessageBox::warning(this, "确认删除", "确认删除该好友", QMessageBox::Ok | QMessageBox::Cancel);
    if (result != QMessageBox::Ok)
    {
        LOG() << "用户取消了好友删除";
        return;
    }
    
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->deleteFriendAsync(this->userInfo._userId);

    this->close();
}
