#pragma once

#include <QDialog>
#include <QGridLayout>
#include "model/date.h"

using model::Message;

// 一个历史消息元素
class HistoryItem : public QWidget
{
	Q_OBJECT

public:
	HistoryItem() {}

	static HistoryItem* makeHistoryItem(const Message& message);

private:

};


// 展示历史消息窗口
class HistoryMessageWidget  : public QDialog
{
	Q_OBJECT

public:
	HistoryMessageWidget(QWidget *parent);
	~HistoryMessageWidget();

	void addHistoryMessage(const Message& message);
	void clear();

private:
	void initScrollArea(QGridLayout* layout);

	QWidget* container; // 持有所有历史消息
};
