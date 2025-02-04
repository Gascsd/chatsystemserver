#include "HistoryMessageWidget.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>

#include "debug.h"

using namespace model;

HistoryItem* HistoryItem::makeHistoryItem(const Message& message)
{
	// 1. 创建对象
	HistoryItem* item = new HistoryItem();
	item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	
	// 2. 创建布局
	QGridLayout* layout = new QGridLayout();
	layout->setVerticalSpacing(0);
	layout->setHorizontalSpacing(10);
	layout->setContentsMargins(0, 0, 0, 0);
	item->setLayout(layout);

	// 3. 创建头像
	QPushButton* avatarBtn = new QPushButton();
	avatarBtn->setFixedSize(40, 40);
	avatarBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	avatarBtn->setIconSize(QSize(40, 40));
	avatarBtn->setIcon(message._sender._avatar);
	avatarBtn->setStyleSheet("QPushButton { border: none; }");

	// 4. 创建昵称和时间
	QLabel* nameLabel = new QLabel();
	nameLabel->setText(message._sender._nickname + " | " + message._time);
	nameLabel->setFixedHeight(20); // 设置为头像的一半
	nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// 5. 消息内容部分
	QWidget* contentWidget = nullptr;
	if (message._messageType == IMAGE_TYPE)
	{

	}
	else if (message._messageType == FILE_TYPE)
	{

	}
	else if (message._messageType == TEXT_TYPE)
	{
		QLabel* label = new QLabel();
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		label->setWordWrap(true);
		label->setText(QString(message._content));
		label->adjustSize(); // 让 label 能够自动设置大小
		contentWidget = label;
	}
	else if (message._messageType == SPEECH_TYPE)
	{

	}
	else
	{
		LOG() << "错误的消息类型 messageType=" << message._messageType;
	}

	// 6. 把上述控件添加到布局
	layout->addWidget(avatarBtn, 0, 0, 2, 1);
	layout->addWidget(nameLabel, 0, 1, 1, 1);
	layout->addWidget(contentWidget, 1, 1, 5, 1);

	return item;
}



HistoryMessageWidget::HistoryMessageWidget(QWidget *parent)
	: QDialog(parent)
{
	// 1. 设置基本属性
	this->setFixedSize(600, 600);
	this->setWindowTitle("历史消息");
	this->setWindowIcon(QIcon(":/resource/image/logo.png"));
	this->setStyleSheet("QWidget { background-color: rgb(255, 255, 255); }");
	this->setAttribute(Qt::WA_DeleteOnClose);

	// 2. 创建布局管理器
	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(10);
	layout->setContentsMargins(30, 30, 30, 0);
	layout->setAlignment(Qt::AlignTop);
	this->setLayout(layout);

	// 3. 创建单选按钮
	QRadioButton* keyRadioBtn = new QRadioButton();
	QRadioButton* timeRadioBtn = new QRadioButton();
	keyRadioBtn->setText("按关键字查询");
	timeRadioBtn->setText("按时间查询");

	keyRadioBtn->setChecked(true); // 默认按关键字查询

	layout->addWidget(keyRadioBtn, 0, 0, 1, 2);
	layout->addWidget(timeRadioBtn, 0, 2, 1, 2);

	// 4. 创建搜索框
	QLineEdit* searchEdit = new QLineEdit();
	searchEdit->setFixedHeight(50);
	searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	searchEdit->setPlaceholderText("请输入要搜索的关键字");
	searchEdit->setStyleSheet("QLineEdit { border: none; border-radius: 10px; background-color: rgb(240, 240, 240); font-size: 16px; padding: 10px}");
	layout->addWidget(searchEdit, 1, 0, 1, 8);

	// 5. 创建搜索按钮
	QPushButton* searchBtn = new QPushButton();
	searchBtn->setFixedSize(50, 50);
	searchBtn->setIconSize(QSize(30, 30));
	searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	searchBtn->setIcon(QIcon(":/resource/image/search.png"));

	QString btnStyle = "QPushButton { border: none; background-color: rgb(240, 240, 240); border-radius: 10px; }";
	btnStyle += "QPushButton:hover { background-color: rgb(220, 220, 220);}";
	btnStyle += "QPushButton:pressed { background-color: rgb(200, 200, 200);}";
	searchBtn->setStyleSheet(btnStyle);
	layout->addWidget(searchBtn, 1, 8, 1, 1);

	// 6. 创建时间相关的空间
	QLabel* beginTag = new QLabel();
	QLabel* endTag = new QLabel();
	beginTag->setText("开始时间");
	endTag->setText("结束时间");
	QDateTimeEdit* beginTimeEdit = new QDateTimeEdit();
	QDateTimeEdit* endTimeEdit = new QDateTimeEdit();
	beginTimeEdit->setFixedHeight(45);
	endTimeEdit->setFixedHeight(45);
	beginTag->hide();
	endTag->hide();
	beginTimeEdit->hide();
	endTimeEdit->hide();

	// 7. 创建滚动区域
	initScrollArea(layout);

	// 8. 设置槽函数
	connect(keyRadioBtn, &QRadioButton::clicked, this, [=]() {
		layout->removeWidget(beginTag);
		beginTag->hide();
		layout->removeWidget(beginTimeEdit);
		beginTimeEdit->hide();
		layout->removeWidget(endTag);
		endTag->hide();
		layout->removeWidget(endTimeEdit);
		endTimeEdit->hide();

		layout->addWidget(searchEdit, 1, 0, 1, 8);
		searchEdit->show();
	});
	connect(timeRadioBtn, &QRadioButton::clicked, this, [=]() {
		layout->removeWidget(searchEdit);
		searchEdit->hide();

		layout->addWidget(beginTag, 1, 0, 1, 1);
		layout->addWidget(beginTimeEdit, 1, 1, 1, 3);
		layout->addWidget(endTag, 1, 4, 1, 1);
		layout->addWidget(endTimeEdit, 1, 5, 1, 3);

		beginTag->show();
		beginTimeEdit->show();
		endTag->show();
		endTimeEdit->show();
	});

	// 构造测试数据
#ifdef TEST_UI
	for (int i = 0; i < 30; ++i)
	{
		UserInfo sender;
		sender._userId = QString::number(1000 + i);
		sender._nickname = "张三" + QString::number(i);
		sender._description = "这是签名";
		sender._phone = "12343214321";
		sender._avatar = QIcon(":/resource/image/defaultAvatar.png");
		Message message = Message::makeMessage(TEXT_TYPE, "", sender, QString("消息内容消息内容消息内容消息内容消息内容" + QString::number(i)).toUtf8(), "");
		this->addHistoryMessage(message);
	}
#endif // TEST_UI

}

HistoryMessageWidget::~HistoryMessageWidget()
{}

void HistoryMessageWidget::addHistoryMessage(const Message & message)
{
	HistoryItem* item = HistoryItem::makeHistoryItem(message);
	container->layout()->addWidget(item);
}

void HistoryMessageWidget::clear()
{
	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(container->layout());
	for (int i = layout->count() - 1; i >= 0; --i)
	{
		QWidget* w = layout->itemAt(i)->widget();
		if (w == nullptr) continue;
		layout->removeWidget(w);
		w->deleteLater();
	}
}

void HistoryMessageWidget::initScrollArea(QGridLayout* layout)
{
	// 1. 创建滚动区域对象
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setWidgetResizable(true);
	scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0; }");
	scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(255, 255, 255); }");
	scrollArea->setStyleSheet("QScrollArea { border: none; }");
	//layout->addWidget(scrollArea, 1, 0, 1, 9);

	// 2. 创建 QWidget
	container = new QWidget();
	container->setObjectName("container");
	container->setStyleSheet("#container {background-color: rgb(255, 255, 255); }");
	scrollArea->setWidget(container);

	// 3. 给这个QWidget创建布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->setSpacing(10);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->setAlignment(Qt::AlignTop);
	container->setLayout(vlayout);

	layout->addWidget(scrollArea, 2, 0, 1, 9);
}

