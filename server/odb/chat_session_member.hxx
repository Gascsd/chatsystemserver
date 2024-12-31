// 聊天会话成员表映射对象

#pragma once
#include <string>
#include <cstddef> // std::size_t
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace zht_im
{
    #pragma db object table("chat_session_member")
    class ChatSessionMember
    {
        friend class odb::access;
    public:
        ChatSessionMember() {}
        ChatSessionMember(const std::string &ssid, const std::string &uid) 
            :_session_id(ssid), _user_id(uid) {}

        std::string session_id() const { return _session_id; }
        void session_id(const std::string &ssid) { _session_id = ssid; }

        std::string user_id() const { return _user_id; }
        void user_id(const std::string &uid) { _user_id = uid; }

    private:
        #pragma db id auto
        unsigned long _id;
        #pragma db type("varchar(64)") index
        std::string _session_id;
        #pragma db type("varchar(64)")
        std::string _user_id;
    };
} // namespace zht_im

// odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time ../../../odb/chat_session_member.hxx