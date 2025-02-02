#pragma once

#include <QFileInfo>
#include <QIcon>
#include <QPixmap>
#include <QString>



static inline QString getFileName(const QString &path)
{
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

// 封装一个宏，作为打印日志的方式
#define TAG QString("[%1:%2]").arg(QString(getFileName(__FILE__)), QString::number(__LINE__))
#define LOG() qDebug().noquote() << TAG


// 工具函数
// 把时间戳转换成格式化的字符串
static inline QString formatTime(int64_t timeStamp)
{
    // 先把时间戳转换成QDateTime对象
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timeStamp);
    return dateTime.toString("MM-dd HH:mm:ss");
}
// 获取当前的秒级时间戳
static inline int64_t getTime()
{
    return QDateTime::currentSecsSinceEpoch();
}
// 根据QByteArray，转成QIcon
static inline QIcon makeIcon(const QByteArray &byteArray)
{
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    return QIcon(pixmap);
}

// 读写文件操作
// 从指定文件中，读取所有二进制内容，得到一个QByteArray
static inline QByteArray loadFileToByteArray(const QString &path)
{
    QFile file(path);
    bool ok = file.open(QFile::ReadOnly);
    if(!ok)
    {
        LOG() << "文件打开失败" << path;
        return QByteArray();
    }
    QByteArray content = file.readAll();
    file.close();
    return content;
}
// 把QByteArray的内容写入指定文件
static inline void writeByteArrayToFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    bool ok = file.open(QFile::WriteOnly);
    if(!ok)
    {
        LOG() << "文件打开失败" << path;
        return;
    }
    file.write(content);
    file.flush();
    file.close();
}
