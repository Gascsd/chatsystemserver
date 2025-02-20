#include "messageeditarea.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QFileDialog>
#include "historymessagewidget.h"
#include "mainwidget.h"
#include "model/DataCenter.h"
#include "SoundRecorder.h"
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

    // 6. 添加提示 : 录制中
    tipLabel = new QLabel();
    tipLabel->setText("录制中");
    tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tipLabel->setAlignment(Qt::AlignCenter);
    tipLabel->setFont(QFont("微软雅黑", 34, 600));
    vlayout->addWidget(tipLabel);
    tipLabel->hide();

    // 7. 添加发送文本消息的按钮
    sendTextBtn = new QPushButton();
    sendTextBtn->setText("发送");
    sendTextBtn->setFixedSize(120, 40);
    QString style = "QPushButton {font-size: 16px; color: rgb(49, 199, 122); border: none; background-color: rgb(233, 233, 233); border-radius: 10px}";
    style += "QPushButton:hover {background-color: rgb(210, 210, 210);} ";
    style += "QPushButton:pressed {background-color: rgb(198, 198, 198);} ";
    sendTextBtn->setStyleSheet(style);
    vlayout->addWidget(sendTextBtn, 0, Qt::AlignRight | Qt::AlignVCenter);



    // 8. 统一初始化信号槽
    initSignalSlot();
}

void MessageEditArea::initSignalSlot()
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    // 1. 关联显示历史窗口
    connect(showHistoryBtn, &QPushButton::clicked, this, [=]() {
        if (dataCenter->getCurrentChatSessionId() == "")
        {
            LOG() << "当前没有选择任何会话";
            return;
        }
        HistoryMessageWidget* historyMessageWidget = new HistoryMessageWidget(this);
        historyMessageWidget->exec();
    });
    // 2. 关联发送文本消息
    connect(sendTextBtn, &QPushButton::clicked, this, &MessageEditArea::SendTextMessage);
    connect(dataCenter, &model::DataCenter::sendMessageDone, this, &MessageEditArea::addSelfMessage);

    // 3. 关联收到消息信号槽
    connect(dataCenter, &model::DataCenter::receiveMessageDone, this, &MessageEditArea::addOtherMessage);

    // 4. 关联 发送图片 信号槽
    connect(sendImageBtn, &QPushButton::clicked, this, &MessageEditArea::clickSendImageBtn);

    // 5. 关联 发送文件 信号槽
    connect(sendFileBtn, &QPushButton::clicked, this, &MessageEditArea::clickSendFileBtn);

    // 6. 关联 发送语音 信号槽
    connect(sendSpeechBtn, &QPushButton::pressed, this, &MessageEditArea::soundRecordPressed);
    connect(sendSpeechBtn, &QPushButton::released, this, &MessageEditArea::soundRecordReleased);
    SoundRecorder* soundRecorder = SoundRecorder::getInstance();
    connect(soundRecorder, &SoundRecorder::soundRecordDone, this, &MessageEditArea::sendSpeech);

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


    // 把该会话调整到会话列表最顶上
    MainWidget* mainWidget = MainWidget::getInstance();
    auto chatSessionList = dataCenter->getChatSessionList();
    if (chatSessionList == nullptr)
    {
        LOG() << "客户端还未加载会话列表";
        return;
    }
    for (auto it = chatSessionList->begin(); it != chatSessionList->end(); ++it)
    {
        if (it->_chatSessionId == dataCenter->getCurrentChatSessionId())
        {
            auto backup = *it;
            chatSessionList->erase(it);
            chatSessionList->push_front(backup);
            break;
        }
    }
    mainWidget->updateChatSessionList();
    LOG() << "已经调整到最顶部会话";

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

void MessageEditArea::clickSendImageBtn()
{
    // 1. 判断是否选中会话
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getCurrentChatSessionId().isEmpty())
    {
        Toast::showMessage("当前没有选中任何会话，不能发送图片");
        return;
    }

    // 2. 弹出文件选择对话框
    QString filter = "Image Files (*.png *.jpg *.jpeg)";
    QString imagePath = QFileDialog::getOpenFileName(this, "选择图片", QDir::homePath(), filter);
    if (imagePath.isEmpty())
    {
        LOG() << "用户取消选择图片";
        return;
    }
    // 3. 读取文件内容
    QByteArray imageContent = loadFileToByteArray(imagePath);

    // 4. 发送请求
    dataCenter->sendImageMessageAsync(dataCenter->getCurrentChatSessionId(), imageContent);
}

void MessageEditArea::clickSendFileBtn()
{
    // 1. 判断是否选中会话
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getCurrentChatSessionId().isEmpty())
    {
        Toast::showMessage("当前没有选中任何会话，不能发送图片");
        return;
    }
    // 2. 弹出文件选择对话框
    QString filter = "*";
    QString path = QFileDialog::getOpenFileName(this, "选择文件", QDir::homePath(), filter);
    if (path.isEmpty())
    {
        LOG() << "用户取消选择文件";
        return;
    }
    // 3. 读取文件内容 -- 这里不考虑大文件的情况
    QByteArray content = loadFileToByteArray(path);

    // 4. 发送请求
    QFileInfo fileInfo(path);
    const QString fileName = fileInfo.fileName();
    dataCenter->sendFileMessageAsync(dataCenter->getCurrentChatSessionId(), fileName, content);

}

void MessageEditArea::soundRecordPressed()
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getCurrentChatSessionId().isEmpty())
    {
        Toast::showMessage("当前没有选中任何会话，不能发送语音");
        return;
    }
    sendSpeechBtn->setIcon(QIcon(":/resource/image/sound_active.png"));

    // 开始录音
    LOG() << "开始录音";
    SoundRecorder* soundRecorder = SoundRecorder::getInstance();
    soundRecorder->startRecord();

    tipLabel->show();
    textEdit->hide();
}

void MessageEditArea::soundRecordReleased()
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    if (dataCenter->getCurrentChatSessionId().isEmpty())
    {
        Toast::showMessage("当前没有选中任何会话，不能发送语音");
        return;
    }
    sendSpeechBtn->setIcon(QIcon(":/resource/image/sound.png"));

    // 停止录制
    LOG() << "停止录音";
    SoundRecorder* soundRecorder = SoundRecorder::getInstance();
    soundRecorder->stopRecord();

    tipLabel->hide();
    textEdit->show();
}

void MessageEditArea::sendSpeech(const QString& path)
{
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    // 1. 读取语音文件内容
    QByteArray content = loadFileToByteArray(path);
    if (content.isEmpty())
    {
        LOG() << "语音文件加载失败";
        return;
    }
    LOG() << "语音文件加载成功，准备发送";
    dataCenter->sendSpeechMessageAsync(dataCenter->getCurrentChatSessionId(), content);
}





