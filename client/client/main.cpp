#include "mainwidget.h"

#include <QApplication>
#include "utils.hpp"
#include "LoginWidget.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if TEST_SKIP_LOGIN
    MainWidget* w = MainWidget::getInstance();
    w->show();
#else
    LoginWidget* loginWidget = new LoginWidget(nullptr);
    loginWidget->show();
#endif // TEST_SKIP_LOGIN

    return a.exec();
}
