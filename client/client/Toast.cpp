#include "Toast.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
Toast::Toast(const QString& text)
{
	// 1. 设置窗口基本属性
	this->setFixedSize(500, 100);
	this->setWindowTitle("消息通知");
	this->setWindowIcon(QIcon(":/resource/image/logo.png"));
	this->setStyleSheet("QWidget { background-color: rgb(220, 220, 220); border-radius: 10px; }");
	this->setAttribute(Qt::WA_DeleteOnClose);
	// 去掉窗口的标题栏
	this->setWindowFlag(Qt::FramelessWindowHint);

	// 2. 移动位置
	QScreen* screen = QApplication::primaryScreen();
	int width = screen->size().width();
	int height = screen->size().height();

	int x = (width - this->width()) / 2;
	int y = (height - this->height()) / 2 - 300;
	this->move(x, y);

	// 3. 添加一个布局管理器
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);

	// 4. 创建显示文本的 Label
	QLabel* label = new QLabel();
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet("QLabel { font-size: 35px; }");
	label->setText(text);

	layout->addWidget(label);

	// 5. 实现 2s 之后自动关闭
	QTimer* timer = new QTimer();
	connect(timer, &QTimer::timeout, this, [=]() {
		timer->stop();
		this->close();
	});
	timer->start(2000);
}

void Toast::showMessage(const QString& text)
{
	Toast* toast = new Toast(text);
	toast->show();
}

Toast::~Toast()
{}
