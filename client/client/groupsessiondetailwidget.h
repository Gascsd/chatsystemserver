#pragma once

#include <QDialog>
#include <QGridLayout>

class AvatarItem;

class GroupSessionDetailWidget  : public QDialog
{
	Q_OBJECT

public:
	GroupSessionDetailWidget(QWidget *parent);

	void addMember(AvatarItem* avatarItem);

	~GroupSessionDetailWidget();

private:
	QGridLayout* glayout; // 滚动区域的布局管理器

	// 表示当前要添加的AvatarItem所在的行和列
	// 由于添加按钮占据了一个位置，所以从(0, 1)开始
	int curRow = 0;
	int curCol = 1;
};
