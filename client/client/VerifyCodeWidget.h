#pragma once

#include <QWidget>
#include <QRandomGenerator>
#include <QPaintEvent>

class VerifyCodeWidget  : public QWidget
{
	Q_OBJECT

public:
	VerifyCodeWidget(QWidget *parent);
	~VerifyCodeWidget();

	QString generateVerifyCode();						// 用于生成随机的验证码
	void refreshVerifyCode();							// 重新生成验证码并显示到界面
	bool checkVerifyCode(const QString& verifyCode);	// 检验验证码是否正确

	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	QRandomGenerator randomGenerator;	// 随机数生成器
	QString verifyCode = "";			// 保存验证码的值

};
