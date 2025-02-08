#include "messageshowarea.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <qlabel.h>
#include <qscrollbar.h>

#include "debug.h"
#include "utils.hpp"
#include "userinfowidget.h"
#include "mainwidget.h"

using namespace model;

MessageShowArea::MessageShowArea()
{
    // LOG() << "构造 MessageShowArea";
    // 1. 初始化基本属性
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidgetResizable(true);
    // 设置滚动条样式
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(240, 240, 240); }");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0;}");
    this->setStyleSheet("QScrollArea { border: none }");

    // 2. 创建 Container 这样的widget,作为包含内部元素的容器
    container = new QWidget();
    this->setWidget(container);

    // 3. 给container 添加布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    container->setLayout(layout);

    // 添加构造测试数据
#if TEST_UI
    UserInfo sender;
    sender._userId = "1000";
    sender._nickname = "张三";
    sender._phone = "12343214321";
    sender._description = "从今天开始认真写代码";
    sender._avatar = QIcon(":/resource/image/defaultAvatar.png");
    Message message = Message::makeMessage(model::TEXT_TYPE, "", sender, QString("这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息").toUtf8(), "");
    this->addMessage(false, message);
    for(int i = 0; i < 30; ++i)
    {
        // LOG() << "添加第" << i << "条数据";
        UserInfo sender;
        sender._userId = QString::number(i);
        sender._phone = "12343214321";
        sender._description = "从今天开始认真写代码";
        sender._nickname = "张三" + QString::number(i);
        sender._avatar = QIcon(":/resource/image/defaultAvatar.png");
        Message message = Message::makeMessage(model::TEXT_TYPE, "", sender, (QString("这是一条测试消息") + QString::number(i)).toUtf8(), "");
        this->addMessage(true, message);
    }
#endif
}

void MessageShowArea::addMessage(bool isLeft, const Message &message)
{
    // 构造 MessageItem 添加到布局管理器
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    container->layout()->addWidget(messageItem);
}

void MessageShowArea::addFrontMessage(bool isLeft, const Message &message)
{
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(container->layout());
    layout->insertWidget(0, messageItem);
}

void MessageShowArea::clear()
{
    // 遍历布局管理器,删除所有元素
    QLayout* layout = container->layout();
    for(int i = layout->count() - 1; i >= 0; --i)
    {
        QLayoutItem* item = layout->takeAt(i);
        if(item != nullptr && item->widget() != nullptr)
        {
            delete item->widget();
        }
    }
}

MessageItem::MessageItem(bool isLeft) : isLeft(isLeft) {}

MessageItem *MessageItem::makeMessageItem(bool isLeft, const Message &message)
{
    MessageItem* messageItem = new MessageItem(isLeft);
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(30, 10, 40, 0);
    layout->setSpacing(10);
    // 这个MessageItem 最低不能低于100
    messageItem->setMinimumHeight(100);
    messageItem->setLayout(layout);

    // 2. 创建头像
    QPushButton* avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40, 40);
    avatarBtn->setIconSize(QSize(40, 40));
    avatarBtn->setIcon(message._sender._avatar);
    if(isLeft)
    {
        layout->addWidget(avatarBtn, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    }
    else
    {
        layout->addWidget(avatarBtn, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    }

    // 3. 创建名字和时间
    QLabel* nameLabel = new QLabel();
    nameLabel->setText(message._sender._nickname + " | " + message._time);
    nameLabel->setAlignment(Qt::AlignBottom);
    nameLabel->setStyleSheet("QLabel { font-size:12px; color: rgb(178, 178, 178)}");
    if(isLeft)
    {
        layout->addWidget(nameLabel, 0 ,1);
    }
    else
    {
        layout->addWidget(nameLabel, 0, 0, Qt::AlignRight);
    }

    // 4. 创建消息体
    QWidget* contentWidget = nullptr;
    switch (message._messageType)
    {
    case TEXT_TYPE:
        contentWidget = makeTextMessagtItem(isLeft, message._content);
        break;
    case IMAGE_TYPE:
        contentWidget = makeImageMessagtItem();
        break;
    case FILE_TYPE:
        contentWidget = makeFileMessagtItem();
        break;
    case SPEECH_TYPE:
        contentWidget = makeSpeechMessagtItem();
        break;
    default:
        LOG() << "错误的消息类型 MessageType=" << message._messageType;
        break;
    }
    if(isLeft)
    {
        layout->addWidget(contentWidget, 1, 1);
    }
    else
    {
        layout->addWidget(contentWidget, 1, 0);
    }

    // 5. 连接信号槽，处理用户点击头像的操作
    connect(avatarBtn, &QPushButton::clicked, messageItem, [=]() {
        MainWidget* mainWidget = MainWidget::getInstance();
        UserInfoWidget* userInfoWidget = new UserInfoWidget(mainWidget, message._sender);
        userInfoWidget->exec();
    });

    return messageItem;
}

QWidget *MessageItem::makeTextMessagtItem(bool isLeft, const QString &text)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(isLeft, text);
    return messageContentLabel;
}

QWidget *MessageItem::makeImageMessagtItem()
{
    return nullptr;
}

QWidget *MessageItem::makeFileMessagtItem()
{
    return nullptr;
}

QWidget *MessageItem::makeSpeechMessagtItem()
{
    return nullptr;
}

MessageContentLabel::MessageContentLabel(bool isLeft, const QString &text)
    : isLeft(isLeft)
{
    // 设置 SizePolicy
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(16);

    this->label = new QLabel(this);
    this->label->setText(text);
    this->label->setFont(font);
    this->label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->label->setWordWrap(true); // 设置自动换行
    this->label->setStyleSheet("QLabel { padding: 0 10px; line-height: 1.2; background-color: transparent; } ");
}

// 会在该控件被显示的时候自动调用
void MessageContentLabel::paintEvent(QPaintEvent *event)
{
    // 1. 获取父元素的宽度
    QObject* object = this->parent();
    if(!object->isWidgetType()) return; // 如果不是预期的QWidget,就不管了
    QWidget* parent = dynamic_cast<QWidget*>(object);
    int width = parent->width() * 0.6;

    // 2. 计算当前文本的长度
    QFontMetrics metrics(this->label->font());
    int totalWidth = metrics.horizontalAdvance(this->label->text());

    // 3. 计算出需要的行数
    int rows = (totalWidth / (width - 40)) + 1;
    if(rows == 1)
    {
        width = totalWidth + 40; // 如果只要一行的话,文本可能很窄
    }

    // 4. 根据行数获取高度 (20表示上下各有10px 的宽度)
    int height = rows * (this->label->font().pixelSize() * 1.2) + 20;

    // 5. 绘制圆角矩形和箭头
    QPainter painer(this);
    QPainterPath path;
    painer.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿
    if(isLeft)
    {
        painer.setPen(QPen(QColor(255, 255, 255)));
        painer.setBrush(QColor(255, 255, 255));

        painer.drawRoundedRect(10,  0, width, height, 10, 10); // 绘制圆角矩形
        path.moveTo(10, 15);
        path.lineTo(0, 20);
        path.lineTo(10, 20);
        path.closeSubpath(); // 绘制的线形成闭合的多边形,才能使用 Brush 填色
        painer.drawPath(path);

        this->label->setGeometry(10, 0, width, height);
    }
    else
    {
        painer.setPen(QPen(QColor(149, 236, 105)));
        painer.setBrush(QColor(149, 236, 105));

        // 圆角矩形左侧边的横坐标位置
        int leftPos = this->width() - width - 10; // 10 是用来容纳箭头
        int rightPos = this->width() - 10;
        // 绘制圆角矩形
        painer.drawRoundedRect(leftPos, 0, width, height, 10, 10);
        // 绘制箭头
        path.moveTo(rightPos, 15);
        path.lineTo(rightPos + 10, 20);
        path.lineTo(rightPos, 25);
        path.closeSubpath();
        painer.drawPath(path);

        this->label->setGeometry(leftPos, 0, width, height);

        // 6. 重新设置父元素高度,确保父元素足够高,能够容纳下上述绘制消息区域的
        parent->setFixedHeight(height + 50);
    }
}
