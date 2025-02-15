#ifndef CHOOSEFRIENDDIALOG_H
#define CHOOSEFRIENDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

class ChooseFriendDialog;

// 选择好友窗口的一个元素： 好友项
class ChooseFriendItem : public QWidget
{
    Q_OBJECT
public:

    ChooseFriendItem(ChooseFriendDialog* owner, const QString& userId, const QIcon& avatar, const QString& name, bool checked);

    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    const QString& getUserId() const
    {
        return userId;
    }
    QCheckBox* getCheckBox() const
    {
        return checkBox;
    }
private:
    bool isHover = false; // 鼠标是否悬停在当前Item

    QCheckBox* checkBox;
    QPushButton* avatarBtn;
    QLabel* nameLabel;
    ChooseFriendDialog* owner; // 记录是谁持有当前的Item，此处的QWidget 是一个ChooseFriendDialog 类型

    QString userId; // 记录了当前的Item对应的UserId
};

// 选择好友的窗口
class ChooseFriendDialog : public QDialog
{
    Q_OBJECT
public:
    ChooseFriendDialog(QWidget* parent, const QString& userId);

    void initLeft(QHBoxLayout* layout);
    void initRight(QHBoxLayout* layout);
    void clickOkBtn();
    QList<QString> generateMemberList();

    void initData();

    void addFriend(const QString& userId, const QIcon& avatar, const QString& name, bool checked);
    void addSelectedFriend(const QString& userId, const QIcon& avatar, const QString& name);
    void deleteSelectedFriend(const QString& userId);

private:
    QWidget* totalContainer;
    QWidget* selectContainer;
    QString userId;
};

#endif // CHOOSEFRIENDDIALOG_H
