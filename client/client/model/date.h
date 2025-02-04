#pragma once

#include <QString>
#include <QIcon>
#include <QByteArray>
#include <QUuid>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

#include "../utils.hpp"

/**
 * 客户端中关于命名空间的约定：
 * 如果代码所在文件，就在项目的顶层目录中，就直接使用全局命名空间（不手动指定）
 * 如果代码所在文件，在某个子目录中，此时就指定一个和目录名字相同的命名空间
 */

// 创建命名空间
namespace model
{
    // 用户信息
    class UserInfo
    {
    public:
        QString _userId = "";            // 用户Id
        QString _nickname = "";          // 用户昵称
        QString _description = "";       // 用户签名
        QString _phone = "";             // 手机号码
        QIcon _avatar;                   //  用户头像
    };

    enum MessageType
    {
        TEXT_TYPE,      // 文本消息
        IMAGE_TYPE,     // 图片消息
        FILE_TYPE,      // 文件消息
        SPEECH_TYPE     // 语音消息
    };
    // 消息信息
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
            // 创建UUID并转换为字符串形式
            QString uuid = QUuid::createUuid().toString();

            // 移除UUID字符串两端的大括号
            uuid.remove(0, 1); // 移除开头的大括号
            uuid.chop(1);      // 移除末尾的大括号

            // 使用mid方法获取从索引24开始长度为12的子串，并在其前加上"M"
            return "M" + uuid.mid(24, 12);
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
        QIcon _avatar;                      // 用户头像，如果是单聊，就是对方的头像，群聊就是群聊的头像
        QString _userId = "";               // 对单聊来说，表示对方的userId，对于群聊来说为""
    public:
    };


} // namespace model




