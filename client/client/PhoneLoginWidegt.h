#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTimer>
#include <QPushButton>

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

private:
	bool isLoginMode = true;
	QLineEdit* phoneEdit;
	QLineEdit* verifyCodeEdit;
	QTimer* timer;
	int leftTime = 30;
	QPushButton* sendVerifyCodeBtn;
	QPushButton* submitBtn;
	QString currentPhone = "";
};
