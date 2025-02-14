#include "messageeditarea.h"

#include <QHBoxLayout>
#include <QScrollBar>

#include "historymessagewidget.h"
#include "mainwidget.h"
#include "model/DataCenter.h"
#include "Toast.h"

MessageEditArea::MessageEditArea(QWidget *parent) : QWidget{parent}
{
    // 1. 设置必要的属性
    this->setFixedHeight(200);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 2. 创建垂直方向布局管理器,管理 上中下三层
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(10, 0, 10, 10);
    this->setLayout(vlayout);

    // 3. 创建水平方向布局管理器,管理发送不同消息的按钮
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0 , 0);
    hlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vlayout->addLayout(hlayout);

    // 4. 将上方的四个按钮创建并添加到水平方向布局管理器
    QString btnStyle = "QPushButton {background-color: rgb(245, 245, 245); border: none; } QPushButton:pressed {background-color: rgb(255, 255, 255); }";
    QSize btnSize(35, 35), iconSize(25, 25);

    sendImageBtn = new QPushButton();
    sendImageBtn->setFixedSize(btnSize);
    sendImageBtn->setIconSize(iconSize);
    sendImageBtn->setIcon(QIcon(":/resource/image/image.png"));
    sendImageBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendImageBtn);

    sendFileBtn = new QPushButton();
    sendFileBtn->setFixedSize(btnSize);
    sendFileBtn->setIconSize(iconSize);
    sendFileBtn->setIcon(QIcon(":/resource/image/file.png"));
    sendFileBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendFileBtn);

    sendSpeechBtn = new QPushButton();
    sendSpeechBtn->setFixedSize(btnSize);
    sendSpeechBtn->setIconSize(iconSize);
    sendSpeechBtn->setIcon(QIcon(":/resource/image/sound.png"));
    sendSpeechBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendSpeechBtn);

    showHistoryBtn = new QPushButton();
    showHistoryBtn->setFixedSize(btnSize);
    showHistoryBtn->setIconSize(iconSize);
    showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
    showHistoryBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(showHistoryBtn);

    // 5. 添加输入框(多行)
    textEdit = new QPlainTextEdit();
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEdit->setStyleSheet("QPlainTextEdit { border: none; background-color: transparent; font-size: 14px; padding: 10px; }");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(45, 45, 45)}");
    vlayout->addWidget(textEdit);

    // 6. 添加发送文本消息的按钮
    sendTextBtn = new QPushButton();
    sendTextBtn->setText("发送");
    sendTextBtn->setFixedSize(120, 40);
    QString style = "QPushButton {font-size: 16px; color: rgb(49, 199, 122); border: none; background-color: rgb(233, 233, 233); border-radius: 10px}";
    style += "QPushButton:hover {background-color: rgb(210, 210, 210);} ";
    style += "QPushButton:pressed {background-color: rgb(198, 198, 198);} ";
    sendTextBtn->setStyleSheet(style);
    vlayout->addWidget(sendTextBtn, 0, Qt::AlignRight | Qt::AlignVCenter);

    // 7. 统一初始化信号槽
    initSignalSlot();
}

void MessageEditArea::initSignalSlot()
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    // 1. 关联显示历史窗口
    connect(showHistoryBtn, &QPushButton::clicked, this, [=]() {
        HistoryMessageWidget* historyMessageWidget = new HistoryMessageWidget(this);
        historyMessageWidget->exec();
    });
    // 2. 关联发送文本消息
    connect(sendTextBtn, &QPushButton::clicked, this, &MessageEditArea::SendTextMessage);
    connect(dataCenter, &model::DataCenter::sendMessageDone, this, &MessageEditArea::addSelfMessage);
    connect(dataCenter, &model::DataCenter::receiveMessageDone, this, &MessageEditArea::addOtherMessage);
}

void MessageEditArea::SendTextMessage()
{
    // 1. 判读是否有会话被选中
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if(dataCenter->getCurrentChatSessionId().isEmpty())
    {
        LOG() << "当前没有选中任何会话";
        Toast::showMessage("当前没有选中任何会话");
        return;
    }
    // 2. 获取输入框中的内容并清空输入框
    const QString content = textEdit->toPlainText().trimmed(); // 使用trimmed方法把字符串两边的空白字符去掉
    if(content.isEmpty())
    {
        LOG() << "不能发送空消息";
        return;
    }
    textEdit->setPlainText("");
    // 3. 通过网络发送消息
    dataCenter->sendTextMessageAsync(dataCenter->getCurrentChatSessionId(), content);

}
// 针对自己发的消息处理，显示到消息展示区
void MessageEditArea::addSelfMessage(model::MessageType messageType, const QByteArray &content, const QString &extraInfo)
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    QString currentChatSessionId = dataCenter->getCurrentChatSessionId();
    // 1. 构造消息并添加到dataCenter中
    Message message = Message::makeMessage(messageType, currentChatSessionId, *dataCenter->getMyself(), content, extraInfo);
    dataCenter->addMessage(message);

    // 2. 把新消息添加到消息展示区
    MainWidget* mainWidget = MainWidget::getInstance();
    MessageShowArea* messageShowArea = mainWidget->getMessageShowArea();
    messageShowArea->addMessage(false, message);

    // 3. 控制消息滚动区域滚动到末尾
    messageShowArea->scrollToEnd();

    // 4. 发送一个信号，通知会话列表更新最后一条消息的内容
    // LOG() << "消息发送成功，内容为：" << content;
    emit dataCenter->updateLastMessage(currentChatSessionId);
}

void MessageEditArea::addOtherMessage(const model::Message& message)
{
    //1. 通过界面，拿到消息展示区
    MainWidget* mainWidget = MainWidget::getInstance();
    MessageShowArea* messageShowArea = mainWidget->getMessageShowArea();
    messageShowArea->addMessage(true, message);
    messageShowArea->scrollToEnd();
    Toast::showMessage("收到消息");
}





