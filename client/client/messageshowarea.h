#ifndef MESSAGESHOWAREA_H
#define MESSAGESHOWAREA_H

#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include "model/data.h"

using model::Message;

class MessageShowArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageShowArea();

    void addMessage(bool isLeft, const Message& message); // 尾插
    void addFrontMessage(bool isLeft, const Message& message); // 头茬
    void clear(); // 清空消息

    void scrollToEnd(); // 滚动到末尾

private:
    QWidget* container;
};


/**
 * 表示一个消息元素
 * 要能够同时支持文本消息,文件消息,图片消息,语音消息
 * 目前暂时只考虑文本
 */
class MessageItem : public QWidget
{
    Q_OBJECT
public:
    MessageItem(bool isLeft);

    // 通过工厂模式构建消息对象
    static MessageItem* makeMessageItem(bool isLeft, const Message& message);

    static QWidget* makeTextMessagtItem(bool isLeft, const QString& text);
    static QWidget* makeImageMessagtItem(bool isLeft, const QString& fileId, const QByteArray& content);
    static QWidget* makeFileMessagtItem(bool isLeft, const QString& fileId, const QString fileName, const QByteArray& content);
    static QWidget* makeSpeechMessagtItem(bool isLeft, const Message& message);

private:
    bool isLeft;
};


/**
 * 创建类表示"文本/文件消息"正文部分
 */
class MessageContentLabel : public QWidget
{
    Q_OBJECT
public:
    MessageContentLabel(bool isLeft, const QString& text, model::MessageType messageType, 
        const QString& fileId, const QString &fileName, const QByteArray &content);

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    
    void updateUI(const QString fileId, const QByteArray& content);
    void saveAsFile(const QString& fileName, const QByteArray& content);
    void playDone();

    void contextMenuEvent(QContextMenuEvent* event) override;
    void speechConvertTextDone(const QString& fileId, const QString& text);

private:
    QLabel* label;
    bool isLeft;

    model::MessageType messageType;
    QString fileId;
    QByteArray content;
    QString fileName;

    bool loadContentDone = false; // 判断文件是否加载完毕
};

/**
* 创建类表示“图潘消息”正文部分
*/
class MessageImageLabel : public QWidget
{
    Q_OBJECT
public:
    MessageImageLabel(const QString& fileId, const QByteArray& content, bool isLeft);
    void updateUI(const QString& fileId, const QByteArray& content);
    void paintEvent(QPaintEvent* event) override;

private:

    QPushButton* imageBtn;
    QString fileId;             // 该图片在服务器上对应的文件ID
    QByteArray content;         // 图片的二进制数据
    bool isLeft;
};

#endif // MESSAGESHOWAREA_H
