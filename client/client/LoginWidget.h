#pragma once

#include <QWidget>

class LoginWidget  : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent);
	~LoginWidget();

private:
	bool isLoginMode = true;
};
