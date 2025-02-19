#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "VerifyCodeWidget.h" 

class LoginWidget  : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent);
	~LoginWidget();
	void clickSubmitBtn();
	void userLoginDone(bool ok, const QString& reason);
	void userRegisterDone(bool ok, const QString& reason);
	void switchMode(); // 切换登录注册模式

private:
	bool isLoginMode = true;
	QLineEdit* usernameEdit;
	QLineEdit* passwdEdit;
	QLineEdit* verifyCodeEdit;
	VerifyCodeWidget* verifyCodeWidget;

	QLabel* titleLabel;
	QPushButton* submitBtn;
	QPushButton* phoneModeBtn;
	QPushButton* switchModeBtn;
};
