#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include "model/data.h"

using model::UserInfo;

// 表示一个好友搜索结果
class FriendResultItem : public QWidget
{
	Q_OBJECT
public:
	FriendResultItem(const UserInfo& userInfo);

private:
	const UserInfo& userInfo;
};

class AddFriendDialog  : public QDialog
{
	Q_OBJECT
public:
	AddFriendDialog(QWidget *parent);
	~AddFriendDialog();

	void initResultArea();

	void addResult(const UserInfo& userInfo);
	void clear();

	void setSearchKey(const QString& searchkey);
private:
	QGridLayout* layout;			// 整个窗口的网格布局
	QWidget* resultContainer;		// 保存搜索好友的结果
	QLineEdit* searchEdit;			// 搜索框
};
