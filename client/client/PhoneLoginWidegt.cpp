#include "PhoneLoginWidegt.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "LoginWidget.h"
#include "mainwidget.h"
#include "Toast.h"
#include "model/DataCenter.h"

PhoneLoginWidegt::PhoneLoginWidegt(QWidget *parent)
	: QWidget(parent)
{
	// 1. 设置窗口基本属性
	this->setFixedSize(400, 350);
	this->setWindowTitle("登录");
	this->setWindowIcon(QIcon(":/resource/image/logo.png"));
	this->setStyleSheet("QWidget { background-color: rgb(255, 255, 255); }");
	this->setAttribute(Qt::WA_DeleteOnClose);

	// 2. 创建布局管理器
	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(10); // 增加间距以提高可读性
	layout->setContentsMargins(50, 0, 50, 0);
	this->setLayout(layout);

	// 3. 创建标题
	titleLabel = new QLabel();
	titleLabel->setText("登录");
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	titleLabel->setFixedHeight(50);
	titleLabel->setStyleSheet("QLabel { font-size: 40px; font-weight: 600; }");

	// 4. 创建手机号输入框
	QString editStyle = "QLineEdit { border: none; border-radius: 10px; font-size: 20px; background-color: rgb(240, 240, 240); padding-left: 5px;}";
	phoneEdit = new QLineEdit();
	phoneEdit->setFixedHeight(40);
	phoneEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	phoneEdit->setPlaceholderText("手机号");
	phoneEdit->setStyleSheet(editStyle);

	// 5. 创建验证码输入框
	verifyCodeEdit = new QLineEdit();
	verifyCodeEdit->setFixedHeight(40);
	verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	verifyCodeEdit->setPlaceholderText("短信验证码");
	verifyCodeEdit->setStyleSheet(editStyle);

	// 6. 创建发送验证码按钮
	sendVerifyCodeBtn = new QPushButton();
	sendVerifyCodeBtn->setText("发送验证码");
	sendVerifyCodeBtn->setFixedSize(100, 40);
	QString btnModeStyle = "QPushButton { border: none; border-radius: 10px; background-color: transparent }";
	btnModeStyle += "QPushButton:pressed { background-color: rgb(240, 240, 240) }";
	sendVerifyCodeBtn->setStyleSheet(btnModeStyle);

	// 7. 创建提交按钮
	submitBtn = new QPushButton();
	submitBtn->setText("登录");
	submitBtn->setFixedHeight(40);
	submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QString btnGreenStyle = "QPushButton { border: none; border-radius: 10px; background-color: rgb(44, 182, 61); color: rgb(255, 255, 255)}";
	btnGreenStyle += "QPushButton:pressed { background-color: rgb(6, 173, 86); }";
	submitBtn->setStyleSheet(btnGreenStyle);

	// 8. 创建切换到用户名密码登录按钮
	userModeBtn = new QPushButton(this);
	userModeBtn->setText("用户名密码登录");
	userModeBtn->setFixedSize(100, 40);
	userModeBtn->setStyleSheet(btnModeStyle);

	// 10. 创建切换到注册按钮
	switchModeBtn = new QPushButton();
	switchModeBtn->setText("注册");
	switchModeBtn->setFixedHeight(40);
	switchModeBtn->setStyleSheet(btnModeStyle);

	// 11. 添加上述内容到布局管理器
	layout->addWidget(titleLabel, 0, 0, 1, 5);
	layout->addWidget(phoneEdit, 1, 0, 1, 5);
	layout->addWidget(verifyCodeEdit, 2, 0, 1, 4);
	layout->addWidget(sendVerifyCodeBtn, 2, 4, 1, 1);
	layout->addWidget(submitBtn, 3, 0, 1, 5);
	layout->addWidget(userModeBtn, 4, 0, 1, 1);
	layout->addWidget(switchModeBtn, 4, 4, 1, 1);

	// 12. 添加信号槽
	connect(switchModeBtn, &QPushButton::clicked, this, &PhoneLoginWidegt::switchMode);

	connect(userModeBtn, &QPushButton::clicked, this, [=]() {
		LoginWidget* loginWidget = new LoginWidget(nullptr);
		loginWidget->show();
		this->close();
	});

	connect(sendVerifyCodeBtn, &QPushButton::clicked, this, &PhoneLoginWidegt::sendVerifyCode);

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &PhoneLoginWidegt::countDone);

	connect(submitBtn, &QPushButton::clicked, this, &PhoneLoginWidegt::clickSubmitBtn);
}

PhoneLoginWidegt::~PhoneLoginWidegt()
{}

void PhoneLoginWidegt::sendVerifyCode()
{
	const QString& phone = phoneEdit->text();
	if (phone.size() != 11)
	{
		Toast::showMessage("手机号格式不合法");
		return;
	}
	this->currentPhone = phone;
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	connect(dataCenter, &model::DataCenter::getVerifyCodeDone, this, &PhoneLoginWidegt::sendVerifyCodeDone, Qt::UniqueConnection);
	dataCenter->getVerifyCodeAsync(phone);

	timer->start(1000);
}

void PhoneLoginWidegt::sendVerifyCodeDone()
{
	Toast::showMessage("验证码已发送");
}

void PhoneLoginWidegt::countDone()
{
	if (leftTime <= 1)
	{
		sendVerifyCodeBtn->setEnabled(true);
		sendVerifyCodeBtn->setText("发送验证码");
		timer->stop();
		leftTime = 30;
		return;
	}
	leftTime -= 1;
	sendVerifyCodeBtn->setText(QString::number(leftTime) + "s");
	if (sendVerifyCodeBtn->isEnabled())
	{
		sendVerifyCodeBtn->setEnabled(false); 
	}
}

void PhoneLoginWidegt::clickSubmitBtn()
{
	const QString& phone = currentPhone;
	const QString verifyCode = verifyCodeEdit->text();
	if (phone.isEmpty() || verifyCode.isEmpty())
	{
		Toast::showMessage("手机号/验证码不能为空");
		return;
	}

	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	if (isLoginMode) // 登录
	{
		connect(dataCenter, &model::DataCenter::phoneLoginDone, this, &PhoneLoginWidegt::phoneLoginDone, Qt::UniqueConnection);
		dataCenter->phoneLoginAsync(phone, verifyCode);
	}
	else // 注册
	{
		connect(dataCenter, &model::DataCenter::phoneRegisterDone, this, &PhoneLoginWidegt::phoneRegisterDone, Qt::UniqueConnection);
		dataCenter->phoneRegisterAsync(phone, verifyCode);
	}
}

void PhoneLoginWidegt::phoneLoginDone(bool ok, const QString& reason)
{
	if (!ok)
	{
		Toast::showMessage("手机号登录失败:" + reason);
		return;
	}
	MainWidget* mainWidget = MainWidget::getInstance();
	mainWidget->show();
	this->close();
}

void PhoneLoginWidegt::phoneRegisterDone(bool ok, const QString& reason)
{
	if (!ok)
	{
		Toast::showMessage("注册失败:" + reason);
		return;
	}
	Toast::showMessage("注册成功:" + reason);
	switchMode();
}

void PhoneLoginWidegt::switchMode()
{
	if (isLoginMode) // 切换到注册
	{
		this->setWindowTitle("注册");
		titleLabel->setText("注册");
		submitBtn->setText("注册");
		userModeBtn->setText("用户名密码注册");
		switchModeBtn->setTabletTracking("登录");
	}
	else
	{
		this->setWindowTitle("登录");
		titleLabel->setText("登录");
		submitBtn->setText("登录");
		userModeBtn->setText("手机号登录");
		switchModeBtn->setTabletTracking("注册");
	}
	isLoginMode = !isLoginMode;
	verifyCodeEdit->clear();
	phoneEdit->clear();
	leftTime = 1;
}
