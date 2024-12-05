#include "sessionfriendarea.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>

#include "debug.h"

SessionFriendArea::SessionFriendArea(QWidget *parent)
    : QScrollArea{parent}
{
    // 1. 设置必要的属性
    this->setWidgetResizable(true); // 开启滚动效果
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(46, 46, 46); }");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; }");

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
#ifdef TEST_UI
    QIcon icon(":/resource/image/defaultAvatar.png");
    for(int i = 0; i < 30; ++i)
    {
        this->addItem(icon, "张三" + QString::number(i), "最后一条消息" + QString::number(i));
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

void SessionFriendArea::addItem(const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = new SessionFriendItem(this, avatar, name, text);
    container->layout()->addWidget(item);
}


/***************************滚动区域的Item实现******************************/


SessionFriendItem::SessionFriendItem(QWidget *owner, const QIcon &avatar, const QString &name, const QString &text)
    : _owner(owner)
{
    this->setFixedHeight(70);
    this->setStyleSheet("QWidget { background-color: rgb(231, 231, 231); }");

    // 创建网格布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
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
    QLabel* messageLabel = new QLabel();
    messageLabel->setText(text);
    messageLabel->setFixedHeight(35);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 头像处于0，0位置，占据 2 行， 2 列
    layout->addWidget(avatarButton, 0, 0, 2, 2);
    // 名字处于0，2位置，占据 1 行 1 列
    layout->addWidget(nameLabel, 0, 2, 1, 1);
    // 消息预览处于1，2位置，占据 1 行 1 列
    layout->addWidget(messageLabel, 1, 2, 1, 1);
}
