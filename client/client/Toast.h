#pragma once

#include <QWidget>

class Toast  : public QWidget
{
	Q_OBJECT

public:
	// 此处不需要指定父窗口，全局通知的父窗口就是桌面
	Toast(const QString& text);

	static void showMessage(const QString& text);

	~Toast();
};
