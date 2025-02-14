#include "selfinfowidget.h"
#include "debug.h"
#include "Toast.h"
#include "model/DataCenter.h"
#include <QTimer>
#include <QFileDialog>



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
    layout = new QGridLayout();
    layout->setVerticalSpacing(5);
    layout->setHorizontalSpacing(10);
    layout->setContentsMargins(20, 20, 20, 0);
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);

    // 3. 创建头像
    avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(75, 75);
    avatarBtn->setIconSize(QSize(75, 75));
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
    avatarBtn->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
#endif

    // 10. 连接信号槽
    initSingalISlot();

    // 11. 加载数据到界面
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    model::UserInfo* myself = dataCenter->getMyself();
    if (myself != nullptr)
    {
        // 就把个人信息显示到界面上
        avatarBtn->setIcon(myself->_avatar);
        idLabel->setText(myself->_userId);
        nameLabel->setText(myself->_nickname);
        descLabel->setText(myself->_description);
        phoneLabel->setText(myself->_phone);
    }
}

void SelfInfoWidget::initSingalISlot()
{
    connect(nameModifyBtn, &QPushButton::clicked, this, [=]() {
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

    connect(descModifyBtn, &QPushButton::clicked, this, [=]() {
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

    connect(phoneModifyBtn, &QPushButton::clicked, this, [=]() {
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

    connect(nameSubmitBtn, &QPushButton::clicked, this, &SelfInfoWidget::clickNameSubmitBtn);
    connect(descSubmitBtn, &QPushButton::clicked, this, &SelfInfoWidget::clickDescSubmitBtn);
    connect(getVerifyCodeBtn, &QPushButton::clicked, this, &SelfInfoWidget::clickGetVerifyCodeBtn);
    connect(phoneSubmitBtn, &QPushButton::clicked, this, &SelfInfoWidget::clickPhoneSubmitBtn);
    connect(avatarBtn, &QPushButton::clicked, this, &SelfInfoWidget::clickAvatarBtn);
}

void SelfInfoWidget::clickNameSubmitBtn()
{
    // 1. 从输入框中拿到修改后的昵称
    const QString& nickname = nameEdit->text();
    if (nickname.isEmpty()) return;
    // 2. 发送网络请求
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    connect(dataCenter, &model::DataCenter::changeNicknameDone, this, &SelfInfoWidget::clickNameSubmitBtnDone, Qt::UniqueConnection);
    dataCenter->changeNicknameAsync(nickname);
}

void SelfInfoWidget::clickNameSubmitBtnDone()
{
    // 对界面进行切换
    // 把输入框中的文本设置为空
    layout->removeWidget(nameEdit);
    nameEdit->hide();
    layout->addWidget(nameLabel, 1, 2);
    nameLabel->show();
    nameLabel->setText(nameEdit->text());

    layout->removeWidget(nameSubmitBtn);
    nameSubmitBtn->hide();
    layout->addWidget(nameModifyBtn, 1, 3);
    nameModifyBtn->show();
}

void SelfInfoWidget::clickDescSubmitBtn()
{
    // 1. 从输入框中拿到修改后的签名
    const QString& description = descEdit->text();
    if (description.isEmpty()) return;
    // 2. 发送网络请求
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    connect(dataCenter, &model::DataCenter::changeDescription, this, &SelfInfoWidget::clickDescSubmitBtnDone, Qt::UniqueConnection);
    dataCenter->changeDescriptionAsync(description);
}

void SelfInfoWidget::clickDescSubmitBtnDone()
{
    layout->removeWidget(descEdit);
    descEdit->hide();
    layout->addWidget(descLabel, 2, 2);
    descLabel->show();
    descLabel->setText(descEdit->text());

    layout->removeWidget(descSubmitBtn);
    descSubmitBtn->hide();
    layout->addWidget(descModifyBtn, 2, 3);
    descModifyBtn->show();
}

void SelfInfoWidget::clickGetVerifyCodeBtn()
{
    // 1. 获取到输入框中的手机号
    const QString& phone = phoneEdit->text();
    if (phone.isEmpty()) return;
    // 2. 给服务器发送请求
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    connect(dataCenter, &model::DataCenter::getVerifyCodeDone, this, [=]() {
        // 这里不需要做其他操作，只需要提示一下，验证码已经发送
        Toast::showMessage("短信验证码已发送");
        });
    dataCenter->getVerifyCodeAsync(phone);

    // 3. 把当前要修改的手机号保存起来
    phoneToChange = phone;
    // 4. 点击获取验证码之后，需要暂时禁用按钮并且倒计时，这是为了避免频繁发送短信请求
    getVerifyCodeBtn->setEnabled(false);
    QTimer* timer = new QTimer(this);
    leftTime = 30;
    connect(timer, &QTimer::timeout, this, [=]() {
        if (leftTime <= 1) // 倒计时结束
        {
            getVerifyCodeBtn->setEnabled(true);
            getVerifyCodeBtn->setText("获取验证码");
            timer->stop();
            timer->deleteLater();
            return;
        }
        --leftTime;
        getVerifyCodeBtn->setText(QString::number(leftTime) + "s");
        });
    timer->start(1000);
}

void SelfInfoWidget::clickPhoneSubmitBtn()
{
    // 1. 先判定当前验证码是否收到
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    QString verifyCodeId = dataCenter->getVerifyCodeId();
    if (verifyCodeId.isEmpty())
    {
        LOG() << "服务器尚未返回验证码ID，稍后重试";
        Toast::showMessage("服务器尚未返回响应，稍后重试");
        return;
    }
    // 不能在这个位置做清空，因为可能由于没有输入验证码，导致这里把验证码ID清空之后，重新输入验证码会导致找不到验证码ID
    // 2. 获取到用户输入的验证码
    const QString& verifyCode = verifyCodeEdit->text();
    if (verifyCode.isEmpty())
    {
        LOG() << "验证码不能为空";
        Toast::showMessage("验证码不能为空");
        return;
    }
    // 如果已经拿到当前verifyCodeId，就可以清空DataCenter中的值，以便于下一次点击提交按钮的时候，上述逻辑仍然有效
    dataCenter->resetVerifyCodeId("");
    verifyCodeEdit->setText("");

    // 3. 发送请求，把当前的验证码信息发送给服务器
    connect(dataCenter, &model::DataCenter::changePhoneDone, this, &SelfInfoWidget::clickPhoneSubmitBtnDone, Qt::UniqueConnection);
    dataCenter->changePhoneAsync(phoneToChange, verifyCodeId, verifyCode);
    // 4. 让验证码按钮的倒计时停止
    leftTime = 1;
}

void SelfInfoWidget::clickPhoneSubmitBtnDone()
{
    // 1. 隐藏控件
    layout->removeWidget(verifyCodeTag);
    layout->removeWidget(verifyCodeEdit);
    layout->removeWidget(getVerifyCodeBtn);
    layout->removeWidget(phoneEdit);
    layout->removeWidget(phoneSubmitBtn);
    verifyCodeTag->hide();
    verifyCodeEdit->hide();
    getVerifyCodeBtn->hide();
    phoneEdit->hide();
    phoneSubmitBtn->hide();

    // 2. 显示控件
    layout->addWidget(phoneLabel, 3, 2);
    layout->addWidget(phoneModifyBtn, 3, 3);
    phoneLabel->show();
    phoneLabel->setText(phoneToChange);
    phoneModifyBtn->show();
}

void SelfInfoWidget::clickAvatarBtn()
{
    // 1. 弹出对话框，选择文件
    QString filrer = "Image Files (*.png *.jpg *.jpeg)";
    QString imagePath = QFileDialog::getOpenFileName(this, "选择头像", QDir::homePath(), filrer);
    if (imagePath.isEmpty()) // 用户取消了
    {
        LOG() << "用户未选择任何头像";
        return;
    }
    // 2. 根据路径，读取到图片的内容
    QByteArray imageBytes = loadFileToByteArray(imagePath);

    // 3. 发送请求
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    connect(dataCenter, &model::DataCenter::changeAvatarDone, this, &SelfInfoWidget::clickAvatarBtnDone, Qt::UniqueConnection);
    dataCenter->changeAvatarAsync(imageBytes);
}

void SelfInfoWidget::clickAvatarBtnDone()
{
    // 把设置的头像，更新到界面上
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    avatarBtn->setIcon(dataCenter->getMyself()->_avatar);
}
