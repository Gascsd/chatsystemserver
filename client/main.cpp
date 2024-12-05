#include "mainwidget.h"

#include <QApplication>

#include "model/date.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LOG() << "hello";

    MainWidget* w = MainWidget::getInstance();
    w->show();
    return a.exec();
}
