#pragma once

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include "model/data.h"

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
	void clickSearchBtn();
	void initScrollArea(QGridLayout* layout);
	void clickSearchBtnDone();

	QWidget* container; // 持有所有历史消息

	QLineEdit* searchEdit;
	QRadioButton* keyRadioBtn;
	QRadioButton* timeRadioBtn;
	QDateTimeEdit* beginTimeEdit;
	QDateTimeEdit* endTimeEdit;
};

// 支持显示图片消息
class ImageButton : public QPushButton
{
	Q_OBJECT
public:
	ImageButton(const QString& fileId, const QByteArray& content);
	void updateUI(const QString& fileId, const QByteArray& content);

private:
	QString fileId;
};

// 支持显示文件消息
class FileLabel : public QLabel
{
	Q_OBJECT
public:
	FileLabel(const QString& fileId, const QString& fileName);
	void getContentDone(const QString& fileId, const QByteArray& content);
	void mousePressEvent(QMouseEvent* event) override;

private:
	QString fileId;
	QString fileName;
	QByteArray content;
	bool loadDone = false;
};

// 支持显示语音消息
// 支持显示文件消息
class SpeechLabel : public QLabel
{
	Q_OBJECT
public:
	SpeechLabel(const QString& fileId);
	void getContentDone(const QString& fileId, const QByteArray& content);
	void mousePressEvent(QMouseEvent* event) override;

private:
	QString fileId;
	QByteArray content;
	bool loadDone = false;
};