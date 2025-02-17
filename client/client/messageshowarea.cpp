#include "messageshowarea.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QScrollBar>
#include <QFileDialog>
#include <QMenu>
#include <QAction>

#include "debug.h"
#include "utils.hpp"
#include "userinfowidget.h"
#include "mainwidget.h"
#include "Toast.h"
#include "model/DataCenter.h"
#include "SoundRecorder.h"

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

void MessageShowArea::scrollToEnd()
{
    // 延时一段时间之后再执行
    QTimer* timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]() {
        int maxValue = this->verticalScrollBar()->maximum();
        this->verticalScrollBar()->setValue(maxValue);
        timer->stop();
        timer->deleteLater();
    });
    timer->start(500);
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
    avatarBtn->setStyleSheet("QPushButton { border: none }");
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
        contentWidget = makeImageMessagtItem(isLeft, message._fileId, message._content);
        break;
    case FILE_TYPE:
        contentWidget = makeFileMessagtItem(isLeft, message._fileId, message._fileName, message._content);
        break;
    case SPEECH_TYPE:
        contentWidget = makeSpeechMessagtItem(isLeft, message);
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

    // 6. 当用户修改了昵称之后，需要同步修改此处的昵称
    if (!isLeft) // 只针对自己的消息做下列操作
    {
        DataCenter* dataCenter = DataCenter::getInstance();
        connect(dataCenter, &DataCenter::changeNicknameDone, messageItem, [=]() {
            nameLabel->setText(dataCenter->getMyself()->_nickname + " | " + message._time);
            });

        connect(dataCenter, &model::DataCenter::changeAvatarDone, messageItem, [=]() {
            UserInfo* myself = dataCenter->getMyself();
            avatarBtn->setIcon(myself->_avatar);
            });
    }
    return messageItem;
}

QWidget *MessageItem::makeTextMessagtItem(bool isLeft, const QString &text)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(isLeft, text, TEXT_TYPE, "", "", QByteArray());
    return messageContentLabel;
}

QWidget *MessageItem::makeImageMessagtItem(bool isLeft, const QString& fileId, const QByteArray& content)
{
    MessageImageLabel* messageImageLabel = new MessageImageLabel(fileId, content, isLeft);
    return messageImageLabel;
}

QWidget *MessageItem::makeFileMessagtItem(bool isLeft, const QString& fileId, const QString fileName, const QByteArray& content)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(isLeft, "[文件]" + fileName, FILE_TYPE, fileId, fileName, QByteArray());
    return messageContentLabel;
}

QWidget *MessageItem::makeSpeechMessagtItem(bool isLeft, const Message& message)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(isLeft, "[语音]", message._messageType, message._fileId, "", QByteArray());
    return messageContentLabel;
}

MessageContentLabel::MessageContentLabel(bool isLeft, const QString& text, model::MessageType messageType, 
    const QString& fileId, const QString& fileName, const QByteArray& content)
    : isLeft(isLeft), messageType(messageType), fileId(fileId), fileName(fileName), content(content)
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

    if (messageType == model::TEXT_TYPE) return;

    if (content.isEmpty())
    {
        DataCenter* dataCenter = DataCenter::getInstance();
        connect(dataCenter, &DataCenter::getSingleFileDone, this, &MessageContentLabel::updateUI);
        dataCenter->getSingleFileAsync(fileId);
    }
    else
    {
        // content不为空，说明这个数据已经有了，就直接把 表示加载状态的变量设为true
        this->loadContentDone = true;
    }
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

void MessageContentLabel::mousePressEvent(QMouseEvent* event)
{
    // 鼠标点击之后，触发文件另存为
    if (event->button() == Qt::LeftButton) // 左键按下文件消息
    {
        if (loadContentDone == false)
        {
            Toast::showMessage("数据尚未加载成功，请稍后重试");
            return;
        }
        if (this->messageType == FILE_TYPE)
        {
            saveAsFile(this->fileName, this->content);
        }
        else if (this->messageType == SPEECH_TYPE) // 点击语音消息就播放
        {
            SoundRecorder* soundRecorder = SoundRecorder::getInstance();
            this->label->setText("播放中...");
            connect(soundRecorder, &SoundRecorder::soundPlayDone, this, &MessageContentLabel::playDone, Qt::UniqueConnection);
            soundRecorder->startPlay(this->content);
        }
        else
        {
            // 其他消息啥都不做
        }
    }
}

void MessageContentLabel::updateUI(const QString fileId, const QByteArray& content)
{
    if (fileId != this->fileId) return;
    this->content = content;
    this->loadContentDone = true;
    this->update();
}

void MessageContentLabel::saveAsFile(const QString& fileName, const QByteArray& content)
{
    QString filePath = QFileDialog::getSaveFileName(this, "另存为", QDir::homePath(), "*");
    if (filePath.isEmpty())
    {
        LOG() << "用户取消了文件另存为";
        return;
    }
    writeByteArrayToFile(filePath, content);
}

void MessageContentLabel::playDone()
{
    if (this->label->text() == "播放中...")
    {
        this->label->setText("[语音]");
    }
}

void MessageContentLabel::contextMenuEvent(QContextMenuEvent* event)
{
    if (messageType == SPEECH_TYPE)
    {
        QMenu* menu = new QMenu(this);
        menu->setStyleSheet(
            "QMenu::item {"
            "padding: 8px 48px 8px 30px;" /* 内边距，左边距较大以留出图标空间 */
            "border: 1px solid transparent;" /* 初始状态下无边框 */
            "}"
            "QMenu::icon {"
            "left: 10px;" /* 图标与左侧的距离 */
            "}"
            "QMenu::separator {"
            "height: 1px;" /* 分隔线的高度 */
            "background: #393e46;" /* 分隔线的颜色 */
            "margin-left: 10px;"
            "margin-right: 10px;"
            "}"
        );
        QAction* action = menu->addAction("语音转文字");
        connect(action, &QAction::triggered, this, [=]() {
            DataCenter* dataCenter = DataCenter::getInstance();
            connect(dataCenter, &model::DataCenter::speechConvertTextDone, this, &MessageContentLabel::speechConvertTextDone, Qt::UniqueConnection);
            dataCenter->speechConvertTextAsync(fileId, this->content);
            });
        menu->exec(event->globalPos());
        delete menu;
    }
    else
    {
        // 其他类型的消息暂时不处理
    }
}

void MessageContentLabel::speechConvertTextDone(const QString& fileId, const QString& text)
{
    if (this->fileId != fileId) return;
    this->label->setText("[语音转文字]" + text);
    this->update();
}

MessageImageLabel::MessageImageLabel(const QString& fileId, const QByteArray& content, bool isLeft)
    : fileId(fileId), content(content), isLeft(isLeft)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageBtn = new QPushButton(this);
    imageBtn->setStyleSheet("QPushButton { border: none; }");

    if (content.isEmpty()) // 如果文件内容为空，那么就得从服务器获取图片内容
    {
        model::DataCenter* dataCenter = model::DataCenter::getInstance();
        connect(dataCenter, &model::DataCenter::getSingleFileDone, this, &MessageImageLabel::updateUI);
        dataCenter->getSingleFileAsync(fileId);
    }
}

void MessageImageLabel::updateUI(const QString& fileId, const QByteArray& content)
{
    if (this->fileId != fileId) return;
    this->content = content;
    // 绘制到界面
    this->update();
}

void MessageImageLabel::paintEvent(QPaintEvent* event)
{
    // 1. 先拿到该元素的父元素，判断父元素的宽度
    QObject* object = this->parent();
    if (!object->isWidgetType()) return;
    QWidget* parent = dynamic_cast<QWidget*>(object);
    int width = parent->width() * 0.6;
    // LOG() << "绘制的图片宽度为:" << width;
    // 2. 加载二进制数据为图片未向
    QImage image;
    if (content.isEmpty()) // 此时图片响应数据还没有回来，显示默认图片
    {
        QByteArray tmpContent = loadFileToByteArray(":/resource/image/image.png");
        image.loadFromData(tmpContent);
    }
    else
    {
        image.loadFromData(content);
    }
    // 3. 针对图片进行缩放
    int height = 0;
    if (image.width() > width)
    {
        height = ((double)image.height() / image.width()) * width;
    }
    else
    {
        width = image.width();
        height = image.height();
    }
    QPixmap pixmap = QPixmap::fromImage(image);
    // imageBtn->setFixedSize(width, height);
    imageBtn->setIconSize(QSize(width, height));
    imageBtn->setIcon(QIcon(pixmap));

    // 4. 设置父元素高度能够容纳当前元素
    parent->setFixedHeight(height + 50);

    // 5. 确定按钮所在的位置
    if(isLeft)
    {
        imageBtn->setGeometry(10, 0, width, height);
    }
    else
    {
        int leftPos = this->width() - width - 10;
        imageBtn->setGeometry(leftPos, 0, width, height);
    }
}
