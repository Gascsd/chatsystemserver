#include "GroupSessionDetailWidget.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>

#include "sessiondetailwidget.h"
#include "debug.h"
#include "model//DataCenter.h"
#include "mainwidget.h"

GroupSessionDetailWidget::GroupSessionDetailWidget(QWidget *parent)
	: QDialog(parent)
{
	// 1. 设置窗口的基本属性
	this->setFixedSize(410, 600);
	this->setWindowTitle("群聊详情");
	this->setWindowIcon(QIcon(":/resource/image/logo.png"));
	this->setStyleSheet("QDialog { background-color: rgb(255, 255, 255); }");
	this->setAttribute(Qt::WA_DeleteOnClose);

	// 2. 创建布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->setSpacing(10);
	vlayout->setContentsMargins(50, 20, 50, 50);
	vlayout->setAlignment(Qt::AlignTop);
	this->setLayout(vlayout);

	// 3 创建滚动区域
	// 3.1 创建 QScrollArea 对象
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255); }");
	scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0; }");
	scrollArea->setFixedSize(310, 350);
	scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	scrollArea->setStyleSheet("QWidget { background-color: transparent; border: none; }");

	// 3.2 创建一个 QScrollArea 内部的 QWidget对象
	QWidget* container = new QWidget(); // 注意：这里修正了变量名从 contaniner 到 container
	scrollArea->setWidget(container);

	// 3.3 给 container 里面添加网格布局
	glayout = new QGridLayout();
	glayout->setSpacing(10);
	glayout->setContentsMargins(0, 0, 0, 0);
	glayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	container->setLayout(glayout);

	// 3.4 把滚动区域添加到布局管理器
	vlayout->addWidget(scrollArea);

	// 4. 添加 “添加” 按钮
	AvatarItem* addBtn = new AvatarItem(QIcon(":/resource/image/cross.png"), "添加", "", nullptr);
	glayout->addWidget(addBtn, 0, 0);

	// 5. 添加 群聊名称 标志
	QLabel* groupNameTag = new QLabel();
	groupNameTag->setText("群聊名称");
	groupNameTag->setStyleSheet("QLabel { font-weight: 700; font-size: 16px }");
	groupNameTag->setFixedHeight(50);
	groupNameTag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	groupNameTag->setAlignment(Qt::AlignBottom);
	vlayout->addWidget(groupNameTag);

	// 6. 添加 真实的群聊名称
	// 6.1 创建水平布局
	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->setSpacing(0);
	hlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->addLayout(hlayout);
	// 6.2 创建名字的label
	groupnameLabel = new QLabel();
	groupnameLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	groupnameLabel->setFixedHeight(50);
	groupnameLabel->setStyleSheet("QLabel { font-size: 18px; }");
	hlayout->addWidget(groupnameLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
	//6.3 创建 修改按钮
	QPushButton* modifyBtn = new QPushButton();
	modifyBtn->setFixedSize(30, 30);
	modifyBtn->setIconSize(QSize(20, 20));
	modifyBtn->setIcon(QIcon(":/resource/image/modify.png"));
	modifyBtn->setStyleSheet("QPushButton { border:none; background-color: transparent; } QPushButton:pressed { background-color: rgb(230, 230, 230); }");
	hlayout->addWidget(modifyBtn, 0, Qt::AlignRight | Qt::AlignVCenter);

	// 7. 退出群聊按钮
	QPushButton* exitGroupBtn = new QPushButton();
	exitGroupBtn->setText("退出群聊");
	exitGroupBtn->setFixedHeight(50);
	exitGroupBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QString btnStyle = "QPushButton { border: 1px solid rgb(90, 90, 90); border-radius: 5px; background-color: transparent; }";
	btnStyle += "QPushButton:pressed { background-color: rgb(230, 230, 230); }";
	exitGroupBtn->setStyleSheet(btnStyle);
	vlayout->addWidget(exitGroupBtn);


	// 构造假数据测试
#if TEST_UI
	groupnameLabel->setText("这是一个群聊名称");
	QIcon avatar(":/resource/image/defaultAvatar.png");
	for (int i = 0; i < 20; ++i)
	{
		AvatarItem* item = new AvatarItem(avatar, "张三" + QString::number(i));
		this->addMember(item);
	}
#endif // TEST_UI

	// 8. 连接信号槽

	// 9. 从服务器加载数据
	initData();
}

void GroupSessionDetailWidget::addMember(AvatarItem* avatarItem)
{
	const int MAX_COL = 4;
	if (curCol >= MAX_COL) // 换行
	{
		curRow++;
		curCol = 0;
	}
	glayout->addWidget(avatarItem, curRow, curCol);
	++curCol;
}

void GroupSessionDetailWidget::initData()
{
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	connect(dataCenter, &model::DataCenter::getMemberListDone, this, &GroupSessionDetailWidget::initMembers);
	dataCenter->getMemberListAsync(dataCenter->getCurrentChatSessionId());
}

void GroupSessionDetailWidget::initMembers(const QString& chatSessionId)
{
	model::DataCenter* dataCenter = model::DataCenter::getInstance();
	QList<model::UserInfo>* memberList = dataCenter->getMemberList(chatSessionId);
	if (memberList == nullptr)
	{
		LOG() << "获取到成员列表为空 chatSessionId=" << chatSessionId;
		return;
	}
	for (const auto& u : *memberList)
	{
		AvatarItem* avatarItem = new AvatarItem(u._avatar, u._nickname, u._userId, this);
		addMember(avatarItem);
	}
	MainWidget* mainWidget = MainWidget::getInstance();
	QString sessionName = mainWidget->getSessionTitleLabel()->text();
	groupnameLabel->setText(sessionName); // 这里使用会话标题表示群聊名称
}

GroupSessionDetailWidget::~GroupSessionDetailWidget() {}
