#ifndef SESSIONDETAILWIDGET_H
#define SESSIONDETAILWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// 表示 一个头像+一个名字 的组合控件
class AvatarItem : public QWidget
{
    Q_OBJECT
public:
    AvatarItem(const QIcon& avatar, const QString& name);
    QPushButton* getAvatar()
    {
        return avatarBtn;
    }
private:
    QPushButton* avatarBtn;
    QLabel* nameLabel;
};

// 表示 单聊会话详情 窗口
class SessionDetailWidget : public QDialog
{
    Q_OBJECT
public:
    SessionDetailWidget(QWidget* parent);

private:
    QPushButton* deleteFriendBtn;
};

#endif // SESSIONDETAILWIDGET_H
