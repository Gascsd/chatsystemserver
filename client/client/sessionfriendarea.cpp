#include "sessionfriendarea.h"
#include "mainwidget.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QStyleOption>
#include <QPainter>

#include "model/DataCenter.h"
#include "utils.hpp"
#include "debug.h"

SessionFriendArea::SessionFriendArea(QWidget *parent)
    : QScrollArea{parent}
{
    // 1. 设置必要的属性
    this->setWidgetResizable(true); // 开启滚动效果
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(46, 46, 46); }");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; }");
    this->setStyleSheet("QWidget { border: none }");

    // 2. 创建 Widget 创建出来
    container = new QWidget();
    container->setFixedWidth(310);
    this->setWidget(container);

    // 3. 给这个 widget 指定一个布局管理器，以便后续添加元素尽情
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);
    container->setLayout(layout);

    // 简单测试一下滚动的效果
    // for(int i = 0; i < 50; ++i)
    // {
    //     QPushButton *btn = new QPushButton();
    //     btn->setText("按钮");
    //     layout->addWidget(btn);
    // }

    // 构造临时数据调试，之前删掉
#if TEST_UI
    QIcon icon(":/resource/image/defaultAvatar.png");
    for(int i = 0; i < 30; ++i)
    {
        this->addItem(SessionItemType, QString::number(i), icon, "张三" + QString::number(i), "最后一条消息" + QString::number(i));
    }
#endif
}

void SessionFriendArea::clear()
{
    QLayout* layout = container->layout();
    // 遍历布局管理器，依次删除
    while(layout->count() != 0)
    {
        QLayoutItem* item = layout->takeAt(0);
        if(item->widget())
        {
            // 这里new出来的对象没有添加到布局管理器，所以需要手动释放
            delete item->widget();
        }
    }
}

void SessionFriendArea::addItem(ItemType itemType, const QString &id, const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = nullptr;
    if(itemType == SessionItemType)
        item = new SessionItem(this, id, avatar, name, text);
    else if(itemType == FriendItemType)
        item = new FriendItem(this, id, avatar, name, text);
    else if(itemType == ApplyItemType)
        item = new ApplyItem(this, id, avatar, name);
    else
    {
        LOG() << "错误的 ItemType, ItemType=" <<itemType;
        return;
    }
    container->layout()->addWidget(item);
}



void SessionFriendArea::clickItem(int index)
{
    if(index < 0 || index >= container->layout()->count())
    {
        LOG() << "点击元素下标超出范围 index=" << index;
        return;
    }
    QLayoutItem *layoutItem = container->layout()->itemAt(index);
    if(layoutItem == nullptr || layoutItem->widget() == nullptr)
    {
        LOG() << "指定的元素不存在 index = " << index;
        return;
    }
    SessionFriendItem* item = dynamic_cast<SessionFriendItem*>(layoutItem->widget());
    item->select();
}

/***************************滚动区域的Item实现******************************/


SessionFriendItem::SessionFriendItem(QWidget *owner, const QIcon &avatar, const QString &name, const QString &text)
    : _owner(owner)
{
    this->setFixedHeight(70);
    this->setStyleSheet("QWidget { background-color: rgb(231, 231, 231); }");

    // 创建网格布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(20, 0, 0, 0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(0);
    this->setLayout(layout);

    // 创建头像
    QPushButton* avatarButton = new QPushButton();
    avatarButton->setFixedSize(50, 50);
    avatarButton->setIconSize(QSize(50, 50));
    avatarButton->setIcon(avatar);
    avatarButton->setStyleSheet("QPushButton {border: none; }");
    avatarButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 创建名字
    QLabel* nameLabel = new QLabel();
    nameLabel->setText(name);
    nameLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: 600 }");
    nameLabel->setFixedHeight(35);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 创建消息预览的label
    messageLabel = new QLabel();
    messageLabel->setText(text);
    messageLabel->setFixedHeight(35);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 头像处于0，0位置，占据 2 行， 2 列
    layout->addWidget(avatarButton, 0, 0, 2, 2);
    // 名字处于0，2位置，占据 1 行 1 列
    layout->addWidget(nameLabel, 0, 2, 1, 8);
    // 消息预览处于1，2位置，占据 1 行 1 列
    layout->addWidget(messageLabel, 1, 2, 1, 8);
}

void SessionFriendItem::paintEvent(QPaintEvent *event)
{
    // 调用基类的paintEvent（如果有的话）
    QWidget::paintEvent(event);

    // 初始化QStyleOption
    QStyleOption opt;
    opt.initFrom(this);

    // 创建QPainter实例并检查是否激活
    QPainter p(this);
    if (!p.isActive())
    {
        qWarning("QPainter failed to activate.");
        return;
    }

    // 设置抗锯齿等渲染提示（可选）
    p.setRenderHint(QPainter::Antialiasing);

    // 使用当前样式绘制部件
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SessionFriendItem::mousePressEvent(QMouseEvent *event)
{
    (void)event;
    select();
}

void SessionFriendItem::enterEvent(QEnterEvent *event) // 鼠标悬停时状态
{
    (void)event;
    if(this->selected) return;
    this->setStyleSheet("QWidget { background-color: rgb(216, 215, 215); }");
}

void SessionFriendItem::leaveEvent(QEvent *event) // 鼠标移走恢复
{
    (void)event;
    // 如果当前item是选中状态,则不受鼠标移动影响
    if(this->selected) return;
    this->setStyleSheet("QWidget { background-color: rgb(231, 231, 231); }");
}

void SessionFriendItem::select()
{
    // 在鼠标点击时触发这个函数
    // 遍历其他元素,还原其他item的背景色
    const QObjectList children = this->parentWidget()->children();
    for(QObject* child : children)
    {
        if(!child->isWidgetType())
        {
            continue;
        }
        SessionFriendItem *item = dynamic_cast<SessionFriendItem*>(child);
        if(item->selected)
        {
            item->selected = false;
            item->setStyleSheet("QWidget { background-color: rgb(231, 231, 231); }");
        }
    }
    // 修改背景色
    this->setStyleSheet("QWidget { background-color: rgb(200, 198, 197); }");
    this->selected = true;

    this->active();
}

void SessionFriendItem::active()
{
    // 这是父类中的
}

SessionItem::SessionItem(QWidget *owner, const QString &chatSessionId, const QIcon &avatar, const QString &name, const QString &lastMessage)
    : SessionFriendItem(owner, avatar, name, lastMessage), chatSessionId(chatSessionId)
{
    // 处理更新最后一条消息的信号
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    connect(dataCenter, &model::DataCenter::updateLastMessage, this, &SessionItem::updateLastMessage);

}

void SessionItem::active()
{
    // 点击之后要加载历史会话内容
    LOG() << "点击 SessionItem 触发的逻辑, chatSessionId=" << chatSessionId;

    // 记载历史会话内容需要涉及到：当前内存中数据的操作、网络通信、界面的变更，所以用主界面来操作
    MainWidget*  mainWidget = MainWidget::getInstance();
    mainWidget->loadRecentMessage(chatSessionId);
}

void SessionItem::updateLastMessage(const QString &chatSessionId)
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if(this->chatSessionId != chatSessionId) // 如果当前SessionItem不是正在发送消息的，就直接return
        return;
    // chatSessionId匹配才更新最后一条消息
    auto messageList = dataCenter->getRecentMessages(chatSessionId);
    if(messageList == nullptr || messageList->size() == 0) // 当前会话没有消息
        return;

    const Message& lastMessage = messageList->back();
    LOG() << "messageList中存放的最后一条消息为：" << lastMessage._content;
    // 明确文本显示的内容
    QString text;
    if(lastMessage._messageType == model::TEXT_TYPE)
        text = lastMessage._content;
    else if (lastMessage._messageType == model::IMAGE_TYPE)
        text = "[图片]";
    else if (lastMessage._messageType == model::SPEECH_TYPE)
        text = "[语音]" + QString::number(lastMessage._content.size());
    else if (lastMessage._messageType == model::FILE_TYPE)
        text = "[文件]" + lastMessage._fileName;
    else
    {
        LOG() << "错误的消息类型";
        return;
    }

    // 显示内容到界面上
    // 这里之后还需要考虑接收到的未读消息，之后再处理
    this->messageLabel->setText(text);
}

FriendItem::FriendItem(QWidget *owner, const QString &userId, const QIcon &avatar, const QString &name, const QString &description)
    : SessionFriendItem(owner, avatar, name, description), userId(userId)
{

}

void FriendItem::active()
{
    // TODO 点击之后要切换到对应的会话
    LOG() << "点击 FriendItem 触发的逻辑, userId=" << userId;
    MainWidget* mainWidget = MainWidget::getInstance();
    mainWidget->switchSession(userId);
}

ApplyItem::ApplyItem(QWidget *owner, const QString &userId, const QIcon &avatar, const QString &name)
    : SessionFriendItem(owner, avatar, name, ""), userId(userId)
{
    // 1. 移除父类的MessageLabel
    QGridLayout* layout = dynamic_cast<QGridLayout*>(this->layout());
    layout->removeWidget(messageLabel);
    delete messageLabel; // 小心内存泄露

    // 2. 创建两个按钮出来
    QPushButton* acceptBtn = new QPushButton();
    acceptBtn->setText("同意");
    QPushButton* rejectBtn = new QPushButton();
    rejectBtn->setText("拒绝");

    // 3. 添加到布局管理器中
    layout->addWidget(acceptBtn, 1,2,1,1);
    layout->addWidget(rejectBtn, 1,3,1,1);
}

void ApplyItem::active()
{
    // 本身就不需要实现
    LOG() << "点击 ApplyItem 触发的逻辑, userId=" << userId;
}



















