#ifndef MESSAGESHOWAREA_H
#define MESSAGESHOWAREA_H

#include <QLabel>
#include <QScrollArea>
#include <QWidget>

#include "model/date.h"

using model::Message;

class MessageShowArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageShowArea();

    void addMessage(bool isLeft, const Message& message); // 尾插
    void addFrontMessage(bool isLeft, const Message& message); // 头茬
    void clear(); // 清空消息

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
    static QWidget* makeImageMessagtItem();
    static QWidget* makeFileMessagtItem();
    static QWidget* makeSpeechMessagtItem();

private:
    bool isLeft;
};


/**
 * 创建类表示"文本消息"正文部分
 */
class MessageContentLabel : public QWidget
{
public:
    MessageContentLabel(bool isLeft, const QString& text);

    void paintEvent(QPaintEvent* event) override;

private:
    QLabel* label;
    bool isLeft;
};

#endif // MESSAGESHOWAREA_H
