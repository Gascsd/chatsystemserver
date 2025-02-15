#include "choosefrienddialog.h"
#include "debug.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QPainter>

#include "model/DataCenter.h"
#include "Toast.h"

ChooseFriendItem::ChooseFriendItem(ChooseFriendDialog* owner, const QString& userId, const QIcon& avatar, const QString& name, bool checked)
    : owner(owner), userId(userId)
{
    // 1. 设置控件的基本属性
    this->setFixedHeight(50);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 2. 设置布局管理器
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(20, 0, 20, 0);
    this->setLayout(layout);

    // 3. 创建复选框
    checkBox = new QCheckBox();
    checkBox->setChecked(checked);
    checkBox->setFixedSize(25, 25);
    QString style = "QCheckBox { background-color: transparent; } ";
    style += " QCheckBox::indicator { width: 20px; height: 20px; image: url(:/resource/image/unchecked.png); }";
    style += "QCheckBox::indicator:checked { image: url(:/resource/image/checked.png); }";
    checkBox->setStyleSheet(style);

    // 4. 创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40, 40);
    avatarBtn->setIconSize(QSize(40, 40));
    avatarBtn->setIcon(avatar);

    // 5. 创建名字
    nameLabel = new QLabel;
    nameLabel->setText(name);
    nameLabel->setStyleSheet("QLabel { background-color: transparent; }");

    // 6. 添加到布局管理器中
    layout->addWidget(checkBox);
    layout->addWidget(avatarBtn);
    layout->addWidget(nameLabel);

    // 7. 连接复选框的信号槽
    connect(checkBox, &QCheckBox::toggled, this, [=](bool checked) {
        if (checked) // 如果已经选中，就添加到右侧
        {
            owner->addSelectedFriend(userId, avatar, name);
        }
        else // 如果没有选中，就从右侧删除
        {
            owner->deleteSelectedFriend(userId);
        }
    });
}

void ChooseFriendItem::paintEvent(QPaintEvent* event)
{
    (void)event;
    // 根据鼠标的进入状态，来决定绘制不同的颜色
    QPainter painter(this);
    if (isHover) // 绘制成深色
    {
        painter.fillRect(this->rect(), QColor(230, 230, 230));
    }
    else // 绘制成浅色
    {
        painter.fillRect(this->rect(), QColor(255, 255, 255));
    }
}

void ChooseFriendItem::enterEvent(QEnterEvent* event)
{
    (void)event;
    isHover = true;
    this->update(); // update相当于更新界面
    //this->repaint();
}

void ChooseFriendItem::leaveEvent(QEvent* event)
{
    (void)event;
    isHover = false;
    this->update(); // update相当于更新界面
    //this->repaint();
}



ChooseFriendDialog::ChooseFriendDialog(QWidget* parent, const QString& userId)
    : QDialog(parent), userId(userId)
{
    // 1. 设置窗口的基本属性
    this->setWindowTitle("选择好友");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    this->setFixedSize(750, 550);
    this->setStyleSheet("QDialog { background-color: rgb(255, 255, 255); }");
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 2. 创建布局管理器
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    // 3. 针对左侧窗口进行初始化
    initLeft(layout);

    // 4. 针对右侧窗口进行初始化
    initRight(layout);

    // 5. 加载数据到当前窗口中
    initData();
}

void ChooseFriendDialog::initLeft(QHBoxLayout* layout)
{
    // 1. 创建一个滚动区域
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; } ");
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255) } ");
    scrollArea->setStyleSheet("QScrollArea { border:none; } ");
    layout->addWidget(scrollArea, 1);

    // 2. 创建QWidget 设置到滚动区
    totalContainer = new QWidget();
    totalContainer->setStyleSheet("QWidget { background-color: rgb(255, 255, 255); } ");
    scrollArea->setWidget(totalContainer);

    // 创建左侧子窗口内的 垂直布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    totalContainer->setLayout(vlayout);

    // 4. 这里还需要进一步添加vlayout的元素
    // 先构造测试数据，之后接入服务区再详细处理
#if TEST_UI
    QIcon defaultAvatar(":/resource/image/defaultAvatar.png");
    for (int i = 0; i < 30; ++i)
    {
        this->addFriend(QString::number(1000 + i), defaultAvatar, "张三" + QString::number(i), false);
    }
#endif // TEST_UI

}

void ChooseFriendDialog::initRight(QHBoxLayout* layout)
{
    // 1. 创建右侧的布局管理器
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(20, 0, 20, 20);
    layout->addLayout(gridLayout, 1);

    // 2. 创建 提示 label
    QLabel* tipLabel = new QLabel();
    tipLabel->setText("选择联系人");
    tipLabel->setFixedHeight(30);
    tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tipLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tipLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: 700; }");

    // 3. 创建滚动区域
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255) } ");
    scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; } ");
    scrollArea->setStyleSheet("QScrollArea { border:none; } ");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 4. 创建滚动区域的QWidget
    selectContainer = new QWidget();
    selectContainer->setObjectName("selectContainer");
    selectContainer->setStyleSheet("#selectContainer { background-color: rgb(255, 255, 255); }");
    scrollArea->setWidget(selectContainer);

    // 5. 创建 selectContainer 中的 垂直布局
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    selectContainer->setLayout(vlayout);

    // 6. 创建底部按钮
    QString style = "QPushButton { color: rgb(6, 174, 86); background-color: rgb(240, 240, 240); border: none; border-radius: 5px;}"
        "QPushButton:hover { background-color: rgb(220, 220, 220); }"
        "QPushButton:pressed { background-color: rgb(200, 200, 200); }";

    QPushButton* okBtn = new QPushButton();
    okBtn->setFixedHeight(40);
    okBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    okBtn->setText("完成");
    okBtn->setStyleSheet(style);
    
    QPushButton* cancelBtn = new QPushButton();
    cancelBtn->setFixedHeight(40);
    cancelBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    cancelBtn->setText("取消");
    cancelBtn->setStyleSheet(style);

    // 7. 添加到布局管理器
    gridLayout->addWidget(tipLabel, 0, 0, 1, 9);
    gridLayout->addWidget(scrollArea, 1, 0, 1, 9);
    gridLayout->addWidget(okBtn, 2, 1, 1, 3);
    gridLayout->addWidget(cancelBtn, 2, 5, 1, 3);

    // 构造数据测试界面
#if TEST_UI
    QIcon defaultAvatar(":/resource/image/defaultAvatar.png");
    for (int i = 0; i < 30; ++i)
    {
        this->addSelectedFriend(QString::number(1000 + i), defaultAvatar, "张三" + QString::number(i));
    }
#endif // TEST_UI

    // 8. 添加信号槽
    connect(okBtn, &QPushButton::clicked, this, &ChooseFriendDialog::clickOkBtn);
    connect(cancelBtn, &QPushButton::clicked, this, [=]() {
        this->close();
        });
}

void ChooseFriendDialog::clickOkBtn()
{
    // 根据选中的好友列表中的元素，得到所有要创建群聊会话的用户ID列表
    QList<QString> userIdList = generateMemberList();
    if (userIdList.size() < 3)
    {
        Toast::showMessage("当前选中的成员不足3个，无法创建群聊");
        return;
    }
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->createGroupChatSessionAsync(userIdList);

    this->close();
}

QList<QString> ChooseFriendDialog::generateMemberList()
{
    QList<QString> ret;
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getMyself() == nullptr)
    {
        LOG() << "个人信息尚未加载";
        return ret;
    }
    ret.push_back(dataCenter->getMyself()->_userId);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(selectContainer->layout());
    for (int i = 0; i < layout->count(); ++i)
    {
        auto* item = layout->itemAt(i);
        if (item == nullptr || item->widget() == nullptr)
            continue;
        auto* chooseFriendItem = dynamic_cast<ChooseFriendItem*>(item->widget());
        ret.push_back(chooseFriendItem->getUserId());
    }
    return ret;
}

void ChooseFriendDialog::initData()
{
    // 遍历好友列表中的所有元素，添加到这个界面的窗口
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    QList<model::UserInfo>* friendList = dataCenter->getFriendList();
    if (friendList == nullptr)
    {
        LOG() << "加载数据时发现好友列表没有被加载";
        return;
    }
    for (auto it = friendList->begin(); it != friendList->end(); ++it)
    {
        if (it->_userId == userId)
        {
            this->addSelectedFriend(it->_userId, it->_avatar, it->_nickname);
        }
        this->addFriend(it->_userId, it->_avatar, it->_nickname, it->_userId == userId);
    }
}

void ChooseFriendDialog::addFriend(const QString& userId, const QIcon& avatar, const QString& name, bool checked)
{
    ChooseFriendItem* item = new ChooseFriendItem(this, userId, avatar, name, checked);
    totalContainer->layout()->addWidget(item);
}

void ChooseFriendDialog::addSelectedFriend(const QString& userId, const QIcon& avatar, const QString& name)
{
    ChooseFriendItem* item = new ChooseFriendItem(this, userId, avatar, name, true);
    selectContainer->layout()->addWidget(item);
}

void ChooseFriendDialog::deleteSelectedFriend(const QString& userId)
{
    // 遍历 selectContainer 中的每个 Item，判断是否是需要删除的
    QVBoxLayout* vlayout = dynamic_cast<QVBoxLayout*>(selectContainer->layout());
    for (int i = vlayout->count() - 1; i >= 0; --i)
    {
        auto* item = vlayout->itemAt(i);
        if (item == nullptr || item->widget() == nullptr)
            continue;
        ChooseFriendItem* chooseFriendItem = dynamic_cast<ChooseFriendItem*>(item->widget());
        if (chooseFriendItem->getUserId() != userId)
            continue;
        vlayout->removeWidget(chooseFriendItem);
        // delete chooseFriendItem; // 如果直接delete，会程序崩溃，因为信号槽内部还在使用
        chooseFriendItem->deleteLater();
    }

    // 遍历 totalContainer 取消对应的Item的勾选状态
    QVBoxLayout* vlayoutLeft = dynamic_cast<QVBoxLayout*>(totalContainer->layout());
    for (int i = 0; i < vlayoutLeft->count(); ++i)
    {
        auto* item = vlayoutLeft->itemAt(i);
        if (item == nullptr || item->widget() == nullptr)
            continue;
        ChooseFriendItem* chooseFriendItem = dynamic_cast<ChooseFriendItem*>(item->widget());
        if (chooseFriendItem->getUserId() != userId)
            continue;
        chooseFriendItem->getCheckBox()->setChecked(false);
    }
}
