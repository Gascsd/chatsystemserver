#pragma once
#include <string>
#include <cstddef> // std::size_t
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace zht_im
{
#pragma db object table("user")
    class User
    {
    private:
        friend class odb::access;
#pragma db id auto
        unsigned long _id;
#pragma db type("varchar(64)") index unique
        std::string _user_id;
#pragma db type("varchar(64)") index unique
        odb::nullable<std::string> _nickname;    // 用户昵称不一定存在
        odb::nullable<std::string> _description; // 用户签名不一定存在
#pragma db type("varchar(64)")
        odb::nullable<std::string> _password; // 用户密码不一定存在
#pragma db type("varchar(64)") index unique
        odb::nullable<std::string> _phone; // 用户手机号不一定存在
#pragma db type("varchar(64)")
        odb::nullable<std::string> _avatar_id; // 用户头像文件ID不一定存在

    public:
        User() {}
        // 用户名--新增用户 -- 用户ID，昵称，密码
        User(const std::string &uid, const std::string &nickname, const std::string &password)
            : _user_id(uid), _nickname(nickname), _password(password)
        {
        }
        // 手机号--新增用户 -- 用户ID，手机号，昵称=手机号
        User(const std::string &uid, const std::string &phone)
            : _user_id(uid), _phone(phone), _nickname(uid)
        {
        }
        ~User() {}
        std::string user_id() { return _user_id; }
        void user_id(const std::string &val) { _user_id = val; }

        odb::nullable<std::string> nickname() { return _nickname; }
        void nickname(const std::string &val) { _nickname = val; }

        odb::nullable<std::string> description() { return _description; }
        void description(const std::string &val) { _description = val; }

        odb::nullable<std::string> password() { return _password; }
        void password(const std::string &val) { _password = val; }

        odb::nullable<std::string> phone() { return _phone; }
        void phone(const std::string &val) { _phone = val; }

        odb::nullable<std::string> avatar_id() { return _avatar_id; }
        void avatar_id(const std::string &val) { _avatar_id = val; }
    };
}
// odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time ../../../odb/user.hxx