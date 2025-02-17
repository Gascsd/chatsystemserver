#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTimer>
#include <QPushButton>
#include <QLabel>

class PhoneLoginWidegt  : public QWidget
{
	Q_OBJECT

public:
	PhoneLoginWidegt(QWidget *parent);
	~PhoneLoginWidegt();

	void sendVerifyCode();
	void sendVerifyCodeDone();
	void countDone();
	void clickSubmitBtn();
	void phoneLoginDone(bool ok, const QString& reason);
	void phoneRegisterDone(bool ok, const QString& reason);
	void switchMode();

private:
	bool isLoginMode = true;
	QLineEdit* phoneEdit;
	QLineEdit* verifyCodeEdit;
	QTimer* timer;
	int leftTime = 30;
	QPushButton* sendVerifyCodeBtn;
	QPushButton* submitBtn;
	QString currentPhone = "";
	QLabel* titleLabel;
	QPushButton* userModeBtn;
	QPushButton* switchModeBtn;
};
