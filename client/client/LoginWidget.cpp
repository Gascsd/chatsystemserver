#include "LoginWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "Toast.h"

#include "PhoneLoginWidegt.h"

LoginWidget::LoginWidget(QWidget* parent)
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
	QLabel* titleLabel = new QLabel();
	titleLabel->setText("登录");
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFixedHeight(50);
	titleLabel->setStyleSheet("QLabel { font-size: 40px; font-weight: 600; }");

	// 4. 创建用户名输入框
	QString editStyle = "QLineEdit { border: none; border-radius: 10px; font-size: 20px; background-color: rgb(240, 240, 240); padding-left: 5px;}";
	QLineEdit* usernameEdit = new QLineEdit();
	usernameEdit->setFixedHeight(40);
	usernameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	usernameEdit->setPlaceholderText("用户名");
	usernameEdit->setStyleSheet(editStyle);

	// 5. 创建密码输入框
	QLineEdit* passwdEdit = new QLineEdit();
	passwdEdit->setFixedHeight(40);
	passwdEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	passwdEdit->setPlaceholderText("密码");
	passwdEdit->setStyleSheet(editStyle);
	passwdEdit->setEchoMode(QLineEdit::Password);

	// 6. 创建验证码输入框
	QLineEdit* verifyCodeEdit = new QLineEdit();
	verifyCodeEdit->setFixedHeight(40);
	verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	verifyCodeEdit->setPlaceholderText("验证码");
	verifyCodeEdit->setStyleSheet(editStyle);

	// 7. 创建显示验证码图片的空间
	QPushButton* verifyCodeWidget = new QPushButton();
	verifyCodeWidget->setText("验证码");
	verifyCodeWidget->setStyleSheet("QPushButton { border: none }");

	// 8. 创建登录按钮
	QPushButton* submitBtn = new QPushButton();
	submitBtn->setText("登录");
	submitBtn->setFixedHeight(40);
	submitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QString btnGreenStyle = "QPushButton { border: none; border-radius: 10px; background-color: rgb(44, 182, 61); color: rgb(255, 255, 255)}";
	btnGreenStyle += "QPushButton:pressed { background-color: rgb(6, 173, 86); }";
	submitBtn->setStyleSheet(btnGreenStyle);

	// 9. 创建切换到手机号登录按钮
	QPushButton* phoneModeBtn = new QPushButton(this);
	phoneModeBtn->setText("手机号登录");
	phoneModeBtn->setFixedHeight(40);
	QString btnModeStyle = "QPushButton { border: none; border-radius: 10px; background-color: transparent }";
	btnModeStyle += "QPushButton:pressed { background-color: rgb(240, 240, 240) }";
	phoneModeBtn->setStyleSheet(btnModeStyle); 

	// 10. 创建切换到注册按钮
	QPushButton* switchModeBtn = new QPushButton();
	switchModeBtn->setText("注册");
	switchModeBtn->setFixedHeight(40);
	switchModeBtn->setStyleSheet(btnModeStyle);

	// 11. 添加上述内容到布局管理器
	layout->addWidget(titleLabel, 0, 0, 1, 5);
	layout->addWidget(usernameEdit, 1, 0, 1, 5);
	layout->addWidget(passwdEdit, 2, 0, 1, 5);
	layout->addWidget(verifyCodeEdit, 3, 0, 1, 2);
	layout->addWidget(verifyCodeWidget, 3, 2, 1, 3); // 修改这里使得验证码输入框与按钮并排显示
	layout->addWidget(submitBtn, 4, 0, 1, 5);
	layout->addWidget(phoneModeBtn, 5, 0, 1, 1);
	layout->addWidget(switchModeBtn, 5, 3, 1, 2);

	// 12. 添加信号槽
	connect(switchModeBtn, &QPushButton::clicked, this, [=]() {
		if (isLoginMode) // 切换到注册
		{
			this->setWindowTitle("注册");
			titleLabel->setText("注册");
			submitBtn->setText("注册");
			phoneModeBtn->setText("手机号注册");
			switchModeBtn->setTabletTracking("登录");
		}
		else
		{
			this->setWindowTitle("登录");
			titleLabel->setText("登录");
			submitBtn->setText("登录");
			phoneModeBtn->setText("手机号登录");
			switchModeBtn->setTabletTracking("注册");
		}
		isLoginMode = !isLoginMode;
	});

	connect(phoneModeBtn, &QPushButton::clicked, this, [=]() {
		PhoneLoginWidegt* phoneLoginWidegt = new PhoneLoginWidegt(nullptr);
		phoneLoginWidegt->show();

		this->close();
	});

	connect(submitBtn, &QPushButton::clicked, this, [=]() {
		Toast::showMessage("测试一下通知窗口");
		});
}

LoginWidget::~LoginWidget()
{}
