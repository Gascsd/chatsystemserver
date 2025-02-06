#include "mainwidget.h"

#include <QApplication>
#include "utils.hpp"
#include "LoginWidget.h"
#include "debug.h"
#include "model/DataCenter.h"

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

    //// 临时的测试代码
    //model::DataCenter* dataCenter = model::DataCenter::getInstance();
    //dataCenter->initDataFile();
    //dataCenter->saveDataFile();

#if TEST_NETWORK
    model::DataCenter* dataCenter = model::DataCenter::getInstance();
    dataCenter->ping();
#endif

    return a.exec();
}
