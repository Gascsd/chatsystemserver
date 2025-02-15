#ifndef SESSIONDETAILWIDGET_H
#define SESSIONDETAILWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "model/data.h"

// 表示 一个头像+一个名字 的组合控件
class AvatarItem : public QWidget
{
    Q_OBJECT
public:
    AvatarItem(const QIcon& avatar, const QString& name, const QString& userId);
    QPushButton* getAvatar()
    {
        return avatarBtn;
    }
    void clickUserItem();
private:
    QPushButton* avatarBtn;
    QLabel* nameLabel;
    QString userId;
};

// 表示 单聊会话详情 窗口
class SessionDetailWidget : public QDialog
{
    Q_OBJECT
public:
    SessionDetailWidget(QWidget* parent, const model::UserInfo& userInfo = model::UserInfo());

    void clickDeleteFriendBtn();

private:
    QPushButton* deleteFriendBtn;

    model::UserInfo userInfo;
};

#endif // SESSIONDETAILWIDGET_H
