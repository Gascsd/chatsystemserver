#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include <QScrollArea>

/**
 * 下面是整个滚动区域的实现
 */

class SessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit SessionFriendArea(QWidget *parent = nullptr);

    // 清空该区域的所有Item
    void clear();

    // 添加一个Item到该区域中
    void addItem(const QIcon &avatar, const QString &name, const QString &text);

private:
    // 后续往 container 内部的 layout 中添加元素，就能够触发 QScrollArea 的滚动效果
    QWidget* container;

signals:
};


/**
 * 滚动区域中的Item的实现
 */
class SessionFriendItem : public QWidget
{
    Q_OBJECT
public:
    SessionFriendItem(QWidget* owner, const QIcon &avatar, const QString &name, const QString &text);

private:
    QWidget* _owner;

    bool selected = false; // 用来表示当前 Item 是否“选中”状态
};

#endif // SESSIONFRIENDAREA_H
