#pragma once

#include <QString>
#include <QIcon>
#include <QByteArray>
#include <QUuid>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

/**
 * 关于命名空间的约定：
 * 如果代码所在文件，就在项目的顶层目录中，就直接使用全局命名空间（不手动指定）
 * 如果代码所在文件，在某个子目录中，此时就指定一个和目录名字相同的命名空间
 */

// 创建命名空间
namespace model
{
    static inline QString getFileName(const QString &path)
    {
        QFileInfo fileInfo(path);
        return fileInfo.fileName();
    }

    // 封装一个宏，作为打印日志的方式
#define TAG QString("[%1:%2]").arg(QString(model::getFileName(__FILE__)), QString::number(__LINE__))
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

    // 用户信息
    class UserInfo
    {
    private:
        QString _userId = "";            // 用户Id
        QString _nickname = "";          // 用户昵称
        QString _description = "";       // 用户签名
        QString _phone = "";             // 手机号码
        QIcon _acatar;                   //  用户头像
    public:

    };

    // 消息信息
    enum MessageType
    {
        TEXT_TYPE,      // 文本消息
        IMAGE_TYPE,     // 图片消息
        FILE_TYPE,      // 文件消息
        SPEECH_TYPE     // 语音消息
    };

    class Message
    {
    public:
        QString _messageId = "";                // 消息的编号
        QString _chatSessionId = "";            // 消息所属会话的编号
        QString _time = "";                     // 消息的时间，通过格式化的方式来表示
        MessageType _messageType = TEXT_TYPE;   // 消息类型
        UserInfo _sender;                       // 发送者信息
        QByteArray _content = "";               // 消息的正文
        QString _fileId = "";                   // 表示一个文件的身份表示，当消息类型为文件、图片、语音的时候才有效，当消息类型为文本的时候，为""
        QString _fileName = "";                 // 文件名，针对消息类型为文件的时候才有效，其他消息均为""
    public:
        // 此处extraInfo目前只是在消息类型为文件消息时，作为“文件名”的补充
        static Message makeMessage(MessageType messageType, const QString &chatSessionId, const UserInfo &sender,
                                    const QByteArray &content ,const QString &extraInfo)
        {
            if(messageType == TEXT_TYPE)
            {
                return makeTextMessage(chatSessionId, sender, content);
            }
            else if(messageType == IMAGE_TYPE)
            {
                return makeImageMessage(chatSessionId, sender, content);
            }
            else if(messageType == FILE_TYPE)
            {
                return makeFileMessage(chatSessionId, sender, content, extraInfo);
            }
            else if(messageType == SPEECH_TYPE)
            {
                return makeSpeechMessage(chatSessionId, sender, content);
            }
            else // 触发了未知的消息类型
            {
                return Message();
            }
        }

    private:
        // 通过这个方法生成唯一的messageId
        static QString makeId()
        {
            // 这里使用UUID的子串
            return "M" + QUuid::createUuid().toString().sliced(25, 12);
        }
        static Message makeTextMessage(const QString& chatSessionId, const UserInfo &sender,const QByteArray &content)
        {
            Message message;
            message._messageId = makeId();
            message._chatSessionId = chatSessionId;
            message._sender = sender;
            message._time = formatTime(getTime()); // 生成一个格式化时间
            message._content = content;
            message._messageType = TEXT_TYPE;
            // 对于文本消息来说，这两个属性不使用，设为""
            message._fileId = "";
            message._fileName = "";
            return message;
        }
        static Message makeImageMessage(const QString& chatSessionId, const UserInfo &sender,const QByteArray &content)
        {
            Message message;
            message._messageId = makeId();
            message._chatSessionId = chatSessionId;
            message._sender = sender;
            message._time = formatTime(getTime()); // 生成一个格式化时间
            message._content = content;
            message._messageType = IMAGE_TYPE;
            // fileId后续使用的时候再填充
            message._fileId = "";
            // 对于图片消息来说，fileName不使用，设为""
            message._fileName = "";
            return message;
        }
        static Message makeFileMessage(const QString& chatSessionId, const UserInfo &sender,const QByteArray &content, const QString &fileName)
        {
            Message message;
            message._messageId = makeId();
            message._chatSessionId = chatSessionId;
            message._sender = sender;
            message._time = formatTime(getTime()); // 生成一个格式化时间
            message._content = content;
            message._messageType = FILE_TYPE;
            // fileId和fileName后续使用的时候再填充
            message._fileId = "";
            message._fileName = fileName;
            return message;
        }
        static Message makeSpeechMessage(const QString& chatSessionId, const UserInfo &sender,const QByteArray &content)
        {
            Message message;
            message._messageId = makeId();
            message._chatSessionId = chatSessionId;
            message._sender = sender;
            message._time = formatTime(getTime()); // 生成一个格式化时间
            message._content = content;
            message._messageType = SPEECH_TYPE;
            // fileId和fileName后续使用的时候再填充
            message._fileId = "";
            message._fileName = "";
            return message;
        }
    };

    // 会话信息
    class ChatSessionInfo
    {
    private:
        QString _chatSessionId = "";        // 会话编号
        QString _chatSessionName = "";      // 会话名字，如果是单聊，就是对方的备注/昵称，群聊就是群聊的名字
        Message _lastMessage;               // 上一条会话信息
        QIcon _acatar;                      // 用户头像，如果是单聊，就是对方的头像，群聊就是群聊的头像
        QString _userId = "";               // 对单聊来说，表示对方的userId，对于群聊来说为""
    public:
    };


} // namespace model




