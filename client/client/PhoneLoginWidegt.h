#pragma once

#include <QWidget>

class PhoneLoginWidegt  : public QWidget
{
	Q_OBJECT

public:
	PhoneLoginWidegt(QWidget *parent);
	~PhoneLoginWidegt();

private:
	bool isLoginMode = true;
};
