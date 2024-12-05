#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    static MainWidget* getInstance();

private:
    enum ActiveTab
    {
        SESSION_LIST,
        FRIEND_LIST,
        APPLY_LIST
    };

private:
    static MainWidget* instance;            // 单例模式对象

    ActiveTab activeTab = SESSION_LIST;     // 当前活动标签页
    // 子窗口对象
    QWidget *windowLeft;                    // 窗口左侧部分
    QWidget *windowMiddle;                  // 窗口中间部分
    QWidget *windowRight;                   // 窗口右侧部分
    // 左侧窗口对象
    QPushButton *userAvatar;                // 用户头像
    QPushButton *sessionTableButton;        // 会话标签页按钮
    QPushButton *friendTableButton;         // 好友标签页按钮
    QPushButton *applyTableButton;          // 好友申请标签页按钮
    // 中间窗口对象
    QLineEdit* searchEdit;                  // 用户搜索框
    QPushButton* addFriendButton;           // 添加好友按钮

private:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void initMainWindow();
    void initLeftWindow();
    void initMiddleWindow();
    void initRightWindow();

    void initSignalSlot();

    void switchTableToSession();
    void switchTableToFriend();
    void switchTableToApply();

    void loadSessionList();
    void loadFriendList();
    void loadApplyList();

};
#endif // MAINWIDGET_H
