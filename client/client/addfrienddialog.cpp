#include "addfrienddialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include "model/DataCenter.h"
#include "debug.h"


FriendResultItem::FriendResultItem(const UserInfo& userInfo)
	: userInfo(userInfo)
{
	// 1. 设置基本属性
	this->setFixedHeight(70);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// 2. 创建布局管理器
	QGridLayout* layout = new QGridLayout();
	layout->setVerticalSpacing(0);
	layout->setHorizontalSpacing(10);
	layout->setContentsMargins(10, 0, 20, 0);
	this->setLayout(layout);

	// 3. 创建头像
	QPushButton* avatatBtn = new QPushButton();
	avatatBtn->setFixedSize(50, 50);
	avatatBtn->setIconSize(QSize(50, 50));
	avatatBtn->setIcon(userInfo._avatar);

	// 4. 创建昵称
	QLabel* nameLabel = new QLabel;
	nameLabel->setFixedHeight(35);
	nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	nameLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: 700 }");
	nameLabel->setText(userInfo._nickname);

	// 5. 创建个性签名
	QLabel* descLabel = new QLabel;
	descLabel->setFixedHeight(35);
	descLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	descLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	descLabel->setStyleSheet("QLabel { font-size: 14px; }");
	descLabel->setText(userInfo._description);

	// 6. 创建添加好友按钮
	addBtn = new QPushButton();
	addBtn->setFixedSize(100, 50);
	addBtn->setText("添加好友");
	QString btnStyle = "QPushButton { border: none; background-color: rgb(127, 217, 97); border-radius: 10px }";
	btnStyle += "QPushButton:pressed { background-color: rgb(200, 200, 200); }";
	addBtn->setStyleSheet(btnStyle);

	// 7. 把上述内容添加到布局管理器
	layout->addWidget(avatatBtn, 0, 0, 2, 1);
	layout->addWidget(nameLabel, 0, 1);
	layout->addWidget(descLabel, 1, 1);
	layout->addWidget(addBtn, 0, 2, 2, 1);

	// 8. 连接信号槽
	connect(addBtn, &QPushButton::clicked, this, &FriendResultItem::clickAddBtn);
}

void FriendResultItem::clickAddBtn()
{
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	dataCenter->addFriendApplyAsync(userInfo._userId);
	addBtn->setEnabled(false);
	addBtn->setText("已申请");
	addBtn->setStyleSheet("QPushButton { border: none; color: rgb(255, 255, 255); background-color: rgb(200, 200, 200); border-radius: 10px; }");
}


AddFriendDialog::AddFriendDialog(QWidget *parent)
	: QDialog(parent)
{
	// 1. 设置基本属性
	this->setFixedSize(500, 500);
	this->setWindowTitle("添加好友");
	this->setWindowIcon(QIcon(":/resource/image/logo.png"));
	this->setStyleSheet("QDialog { background-color: rgb(255, 255, 255); }");
	this->setAttribute(Qt::WA_DeleteOnClose);

	// 2. 创建布局管理器
	layout = new QGridLayout();
	layout->setSpacing(10);
	layout->setContentsMargins(20, 20, 20, 0);
	layout->setAlignment(Qt::AlignTop);
	this->setLayout(layout);

	// 3. 创建搜索框
	searchEdit = new QLineEdit();
	searchEdit->setFixedHeight(50);
	searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QString style = "QLineEdit { border: none; border-radius: 10px; font-size: 16px; background-color: rgb(240, 240, 240); padding-left: 5px}";
	searchEdit->setStyleSheet(style);
	searchEdit->setPlaceholderText("按手机号/用户序号/昵称搜索");
	layout->addWidget(searchEdit, 0, 0, 1, 8);

	// 4. 创建搜索按钮
	QPushButton* searchBtn = new QPushButton();
	searchBtn->setFixedSize(50, 50);
	searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	searchBtn->setIconSize(QSize(30, 30));
	searchBtn->setIcon(QIcon(":/resource/image/search.png"));
	QString btnStyle = "QPushButton { border: none; background-color: rgb(240, 240, 240); border-radius: 10px; }";
	btnStyle += "QPushButton:hover { background-color: rgb(220, 220, 220);}";
	btnStyle += "QPushButton:pressed { background-color: rgb(200, 200, 200);}";
	searchBtn->setStyleSheet(btnStyle);
	layout->addWidget(searchBtn, 0, 8, 1, 1);

	// 5. 添加一个滚动区域
	this->initResultArea();

	// 构造假数据
#if TEST_UI
	QIcon avatar(":/resource/image/defaultAvatar.png"); 
	for (int i = 0; i < 20; ++i)
	{
		UserInfo* userInfo = new UserInfo();
		userInfo->_userId = QString::number(1000 + i);
		userInfo->_nickname = "张三" + QString::number(i);
		userInfo->_description = "这是一段个性签名";
		userInfo->_avatar = avatar;
		this->addResult(*userInfo);
		// LOG() << "构造搜索结果假数据" << i;
	}
#endif // TEST_UI


	// 6. 连接信号槽
	connect(searchBtn, &QPushButton::clicked, this, &AddFriendDialog::clickSearchBtn);
}

AddFriendDialog::~AddFriendDialog()
{}

void AddFriendDialog::initResultArea()
{
	// 1. 创建滚动区域对象
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setWidgetResizable(true);
	scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0; }");
	scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255); }");
	scrollArea->setStyleSheet("QScrollArea { border: none; }");
	layout->addWidget(scrollArea, 1, 0, 1, 9);

	// 2. 创建 QWidget
	resultContainer = new QWidget();
	resultContainer->setObjectName("resultContainer");
	resultContainer->setStyleSheet("#resultContainer {background-color: rgb(255, 255, 255); }");
	scrollArea->setWidget(resultContainer);

	// 3. 给这个QWidget创建布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->setAlignment(Qt::AlignTop);
	resultContainer->setLayout(vlayout);
}

void AddFriendDialog::addResult(const UserInfo& userInfo)
{
	FriendResultItem* item = new FriendResultItem(userInfo);
	resultContainer->layout()->addWidget(item);
}

void AddFriendDialog::clear()
{
	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(resultContainer->layout());
	for (int i = layout->count() - 1; i >= 0; --i)
	{
		QLayoutItem* layoutItem = layout->takeAt(i);
		if (layoutItem == nullptr || layoutItem->widget() == nullptr)
			continue;
		delete layoutItem->widget();
	}
}

void AddFriendDialog::setSearchKey(const QString& searchkey)
{
	searchEdit->setText(searchkey);
}

void AddFriendDialog::clickSearchBtn()
{
	const QString& text = searchEdit->text();
	if (text.isEmpty())
	{
		return;
	}
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	connect(dataCenter, &model::DataCenter::searchUserDone, this, &AddFriendDialog::clickSearchBtnDone, Qt::UniqueConnection);
	dataCenter->searchUserAsync(text);
}

void AddFriendDialog::clickSearchBtnDone()
{
	// 1. 拿到 DataCenter 中的搜索结果
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	QList<UserInfo>* searchResult = dataCenter->getSearchUserResult();
	if (searchResult == nullptr)
		return;

	this->clear();
	for (const auto& u : *searchResult)
	{
		this->addResult(u);
	}
}


