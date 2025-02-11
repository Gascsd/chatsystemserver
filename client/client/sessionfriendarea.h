#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>


// 滚动区域中的Item类型
enum ItemType
{
    SessionItemType,
    FriendItemType,
    ApplyItemType
};

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

    /**
     * @brief 添加一个 item 到该区域中
     * @param itemType 表示要添加的item类型
     * @param id 根据不同的item类型有不同的含义
     * 如果是FriendItem / ApplyItem 就是userId,如果是SessionItem,就是会话ID
     */
    void addItem(ItemType itemType, const QString& id, const QIcon &avatar, const QString &name, const QString &text);

    // 通过index下标选中某个指定的item
    void clickItem(int index);

private:
    // 后续往 container 内部的 layout 中添加元素，就能够触发 QScrollArea 的滚动效果
    QWidget* container;

signals:
};




/**
 * 滚动区域中的Item的实现 基类
 */
class SessionFriendItem : public QWidget
{
    Q_OBJECT
public:
    SessionFriendItem(QWidget* owner, const QIcon &avatar, const QString &name, const QString &text);

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

    void select();
    virtual void active(); // 实现item被点击之后的业务逻辑

private:
    QWidget* _owner;
    bool selected = false; // 用来表示当前 Item 是否“选中”状态

protected:
    QLabel* messageLabel;
};


/**
 * 会话 Item 的实现
 */
class SessionItem : public SessionFriendItem
{
    Q_OBJECT
public:
    SessionItem(QWidget* owner, const QString& chatSessionId, const QIcon& avatar, const QString& name, const QString& lastMessage);
    void active() override;

    void updateLastMessage(const QString& chatSessionId);

private:
    QString chatSessionId; // 当前会话ID
};

/**
 * 好友 Item 的实现
 */
class FriendItem : public SessionFriendItem
{
    Q_OBJECT
public:
    FriendItem(QWidget* owner, const QString& userId, const QIcon& avatar, const QString& name, const QString& description);
    void active() override;

private:
    QString userId; // 好友的用户ID
};

/**
 * 好友申请 Item 的实现
 */
class ApplyItem : public SessionFriendItem
{
    Q_OBJECT
public:
    ApplyItem(QWidget* owner, const QString& userId, const QIcon& avatar, const QString& name);
    void active() override;

private:
    QString userId; // 申请人用户ID
};


#endif // SESSIONFRIENDAREA_H
