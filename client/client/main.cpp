#include "mainwidget.h"

#include <QApplication>
#include "utils.hpp"
#include "LoginWidget.h"
#include "debug.h"
#include "model/DataCenter.h"
#include "SoundRecorder.h"

FILE* output = nullptr;

void msgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    (void)type;
    (void)context;
    QByteArray log = msg.toUtf8();
    fprintf(output, "%s\n", log.constData());
    fflush(output); // 确保数据写入磁盘
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if DEPOLY
    output = fopen("./log.txt", "a"); 
    qInstallMessageHandler(msgHandler);
#endif

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
    // dataCenter->getMyselfAsync();
#endif

    return a.exec();
}
