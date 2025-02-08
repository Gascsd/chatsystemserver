#include "selfinfowidget.h"
#include "debug.h"
#include <QGridLayout>



SelfInfoWidget::SelfInfoWidget(QWidget *parent) : QDialog(parent)
{
    // 1. 设置窗口属性
    this->setFixedSize(500, 250);
    this->setWindowTitle("个人信息");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));
    // 窗口被关闭时自动销毁
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->move(QCursor::pos());

    // 2. 创建布局管理器
    QGridLayout* layout = new QGridLayout();
    layout->setVerticalSpacing(5);
    layout->setHorizontalSpacing(10);
    layout->setContentsMargins(20, 20, 20, 0);
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);

    // 3. 创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75, 75);
    avatarBtn->setIconSize(QSize(75, 75));
    avatarBtn->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    avatarBtn->setStyleSheet("QPushButton { border: none; background-color: transparent}");
    layout->addWidget(avatarBtn, 0, 0, 3, 1);

    QString labelStyle = "QLabel { font-size: 14px; font-weight: 800; }";
    QString btnStyle = "QPushButton { border: none; background-color: transparent; }";
    btnStyle += "QPushButton:pressed { background-color: rgb(210, 210, 210); }";
    QString editStyle = "QLineEdit { border: none; border-radius:5px; padding-left:2px; }";

    int height = 30;

    // 4. 添加用户ID 的显示
    idTag = new QLabel();
    idTag->setFixedSize(50, height);
    idTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    idTag->setText("序号");
    idTag->setStyleSheet(labelStyle);

    idLabel = new QLabel();
    idLabel->setFixedHeight(height);
    idLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 5. 添加用户昵称显示
    nameTag = new QLabel();
    nameTag->setFixedSize(50, height);
    nameTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nameTag->setText("昵称");
    nameTag->setStyleSheet(labelStyle);

    nameLabel = new QLabel();
    nameLabel->setFixedHeight(height);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    nameModifyBtn = new QPushButton();
    nameModifyBtn->setFixedSize(70, 25);
    nameModifyBtn->setIconSize(QSize(20, 20));
    nameModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    nameModifyBtn->setStyleSheet(btnStyle);

    nameEdit = new QLineEdit();
    nameEdit->setFixedHeight(height);
    nameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    nameEdit->setStyleSheet(editStyle);
    nameEdit->hide();

    nameSubmitBtn = new QPushButton();
    nameSubmitBtn->setFixedSize(70, 25);
    nameSubmitBtn->setIconSize(QSize(20, 20));
    nameSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    nameSubmitBtn->setStyleSheet(btnStyle);
    nameSubmitBtn->hide();

    // 6. 添加个性签名
    descTag = new QLabel();
    descTag->setFixedSize(50, height);
    descTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    descTag->setText("签名");
    descTag->setStyleSheet(labelStyle);

    descLabel = new QLabel();
    descLabel->setFixedHeight(height);
    descLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    descModifyBtn = new QPushButton();
    descModifyBtn->setFixedSize(70, 25);
    descModifyBtn->setIconSize(QSize(20, 20));
    descModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    descModifyBtn->setStyleSheet(btnStyle);

    descEdit = new QLineEdit();
    descEdit->setFixedHeight(height);
    descEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    descEdit->setStyleSheet(editStyle);
    descEdit->hide();

    descSubmitBtn = new QPushButton();
    descSubmitBtn->setFixedSize(70, 25);
    descSubmitBtn->setIconSize(QSize(20, 20));
    descSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    descSubmitBtn->setStyleSheet(btnStyle);
    descSubmitBtn->hide();

    // 7. 添加手机号
    phoneTag = new QLabel();
    phoneTag->setFixedSize(50, height);
    phoneTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    phoneTag->setText("电话");
    phoneTag->setStyleSheet(labelStyle);

    phoneLabel = new QLabel();
    phoneLabel->setFixedHeight(height);
    phoneLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    phoneModifyBtn = new QPushButton();
    phoneModifyBtn->setFixedSize(70, 25);
    phoneModifyBtn->setIconSize(QSize(20, 20));
    phoneModifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
    phoneModifyBtn->setStyleSheet(btnStyle);

    phoneEdit = new QLineEdit();
    phoneEdit->setFixedHeight(height);
    phoneEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    phoneEdit->setStyleSheet(editStyle);
    phoneEdit->hide();

    phoneSubmitBtn = new QPushButton();
    phoneSubmitBtn->setFixedSize(70, 25);
    phoneSubmitBtn->setIconSize(QSize(20, 20));
    phoneSubmitBtn->setIcon(QIcon(":/resource/image/submit.png"));
    phoneSubmitBtn->setStyleSheet(btnStyle);
    phoneSubmitBtn->hide();

    // 8. 添加验证码
    verifyCodeTag = new QLabel();
    verifyCodeTag->setFixedSize(50, height);
    verifyCodeTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    verifyCodeTag->setText("验证码");
    verifyCodeTag->setStyleSheet(labelStyle);
    verifyCodeTag->hide();

    verifyCodeEdit = new QLineEdit();
    verifyCodeEdit->setFixedHeight(height);
    verifyCodeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    verifyCodeEdit->setStyleSheet(editStyle);
    verifyCodeEdit->hide();

    getVerifyCodeBtn = new QPushButton();
    getVerifyCodeBtn->setText("获取验证码");
    getVerifyCodeBtn->setStyleSheet("QPushButton { border: none; background-color: transparent;  } "
                                    "QPushButton:pressed { background-color: rgb(231, 231, 231)}");
    getVerifyCodeBtn->setFixedSize(70, height);
    getVerifyCodeBtn->hide();

    // 9. 添加到布局管理器 -- 第0列被头像占用
    layout->addWidget(idTag, 0, 1);
    layout->addWidget(idLabel, 0, 2);

    layout->addWidget(nameTag, 1, 1);
    layout->addWidget(nameLabel, 1, 2);
    layout->addWidget(nameModifyBtn, 1, 3);

    layout->addWidget(descTag, 2, 1);
    layout->addWidget(descLabel, 2, 2);
    layout->addWidget(descModifyBtn, 2, 3);

    layout->addWidget(phoneTag, 3, 1);
    layout->addWidget(phoneLabel, 3, 2);
    layout->addWidget(phoneModifyBtn, 3, 3);

#if TEST_UI
    idLabel->setText("1234");
    nameLabel->setText("张三");
    descLabel->setText("从今天开始认真敲代码");
    phoneLabel->setText("12343214321");
#endif

    // 10. 连接信号槽
    connect(nameModifyBtn, &QPushButton::clicked, this, [=](){
        // 把当前的 Label和Btn隐藏
        nameLabel->hide();
        nameModifyBtn->hide();
        layout->removeWidget(nameLabel);
        layout->removeWidget(nameModifyBtn);
        // 把 Edit 和 SubmitBnt显示
        nameEdit->show();
        nameSubmitBtn->show();
        layout->addWidget(nameEdit, 1, 2);
        layout->addWidget(nameSubmitBtn, 1, 3);
        // 把输入框中的内容进行设置
        nameEdit->setText(nameLabel->text());
    });

    connect(descModifyBtn, &QPushButton::clicked, this, [=](){
        // 把当前的 Label和Btn隐藏
        descLabel->hide();
        descModifyBtn->hide();
        layout->removeWidget(descLabel);
        layout->removeWidget(descModifyBtn);
        // 把 Edit 和 SubmitBnt显示
        descEdit->show();
        descSubmitBtn->show();
        layout->addWidget(descEdit, 2, 2);
        layout->addWidget(descSubmitBtn, 2, 3);
        // 把输入框中的内容进行设置
        descEdit->setText(descLabel->text());
    });

    connect(phoneModifyBtn, &QPushButton::clicked, this, [=](){
        phoneLabel->hide();
        phoneModifyBtn->hide();
        layout->removeWidget(phoneLabel);
        layout->removeWidget(phoneModifyBtn);
        layout->addWidget(verifyCodeTag, 4, 1);
        layout->addWidget(verifyCodeEdit, 4, 2);
        layout->addWidget(getVerifyCodeBtn, 4, 3);

        phoneEdit->show();
        phoneSubmitBtn->show();
        verifyCodeTag->show();
        verifyCodeEdit->show();
        getVerifyCodeBtn->show();
        layout->addWidget(phoneEdit, 3, 2);
        layout->addWidget(phoneSubmitBtn, 3, 3);

        phoneEdit->setText(phoneLabel->text());
    });
}
