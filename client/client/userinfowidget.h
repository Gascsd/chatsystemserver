#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "model/data.h"

using model::UserInfo;

class UserInfoWidget : public QDialog
{
    Q_OBJECT
public:
    UserInfoWidget(QWidget* parent, const UserInfo& userInfo);
    void initSignalSlot();
    void clickDeleteFriendBtn();
    void clickApplyBtn();

private:
    const UserInfo& userInfo; // 保存对应的 user 对象

    QPushButton* avatarBtn;
    QLabel* idTag;
    QLabel* idLabel;
    QLabel* nameTag;
    QLabel* nameLabel;
    QLabel* phoneTag;
    QLabel* phoneLabel;

    QPushButton* applyBtn;
    QPushButton* sendMessageBtn;
    QPushButton* deleteFriendBtn;
};

#endif // USERINFOWIDGET_H
