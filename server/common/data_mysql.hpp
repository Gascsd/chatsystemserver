// #define BUILD
// #ifndef BUILD // 如果没有定义build，表示处于开发阶段，就包含test中的头文件
// #include "/home/zht/chatsystemserver/server/user/test/mysql_test/user-odb.hxx"
// #include "/home/zht/chatsystemserver/server/odb/user.hxx"
// #else // 处于测试或执行阶段，包含CMakeLists.txt中提供的头文件搜搜位置
#include "user.hxx"
#include "user-odb.hxx"
#include "chat_session_member.hxx"
#include "chat_session_member-odb.hxx"
// #endif
#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>

#include "logger.hpp"

/**
 * 功能：
 *   用户注册， 用户登录， 验证码获取，手机号注册，手机号登录， 获取用户信息， 用户信息修改
 * 所需提供的接口：
 *   用户信息新增， 通过昵称获取用户信息， 通过手机号获取用户信息，通过用户ID获取用户信息，通过多个用户ID获取多个用户信息， 信息修改
 */

namespace zht_im
{
    class ODBFactory
    {
    public:
        static std::shared_ptr<odb::core::database> create(
            const std::string &user, const std::string &passwd,
            const std::string &host, const std::string &db,
            const std::string &cset, int port, int conn_pool_count)
        {
            std::unique_ptr<odb::mysql::connection_pool_factory> cpf(
                new odb::mysql::connection_pool_factory(conn_pool_count, 0));
            auto res = std::make_shared<odb::mysql::database>(user, passwd, db, host, port, "", cset, 0, std::move(cpf));
            return res;
        }
    };

    class UserTable
    {
    public:
        using ptr = std::shared_ptr<UserTable>;
    private:
        std::shared_ptr<odb::core::database> _db;

    public:
        UserTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        bool insert(const std::shared_ptr<User> &user)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->persist(user.get());
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增用户失败 {}:{} ", user->nickname(), e.what());
                return false;
            }
            return true;
        }
        bool update(const std::shared_ptr<User> &user)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->update(user.get());
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("更新用户失败 {} ", user->nickname());
                return false;
            }
            return true;
        }
        std::shared_ptr<User> select_by_nickname(const std::string &nickname)
        {
            std::shared_ptr<User> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<User> query;
                typedef odb::result<User> result;
                res = std::shared_ptr<User>(_db->query_one<User>(query::nickname == nickname));
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过昵称查询用户失败 {} ", nickname);
            }
            return res;
        }
        std::shared_ptr<User> select_by_phone(const std::string &phone)
        {
            std::shared_ptr<User> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<User> query;
                typedef odb::result<User> result;
                res = std::shared_ptr<User>(_db->query_one<User>(query::phone == phone));
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过手机号查询用户失败 {} ", phone);
            }
            return res;
        }
        std::shared_ptr<User> select_by_id(const std::string &user_id)
        {
            std::shared_ptr<User> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<User> query;
                typedef odb::result<User> result;
                res = std::shared_ptr<User>(_db->query_one<User>(query::user_id == user_id));
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过用户ID查询用户失败 {} ", user_id);
            }
            return res;
        }
        std::vector<User> select_multi_users(const std::vector<std::string> &id_list)
        {
            // select * from user where id in(id1, id2, ...)
            std::vector<User> res;
            std::stringstream ss;
            ss << "user_id in (";
            for (const auto &id : id_list)
                ss << "'" << id << "',";
            std::string condition = ss.str();
            condition.pop_back();
            condition += ")";
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<User> query;
                typedef odb::result<User> result;

                result r(_db->query<User>(condition));
                for (result::iterator it(r.begin()); it != r.end(); ++it)
                    res.push_back(*it);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过用户ID组查询用户组失败, 用户组为: {} ", condition);
            }
            return res;
        }
    };
    class ChatSessionMemberTable
    {
    public:
        using ptr = std::shared_ptr<ChatSessionMemberTable>;
    private:
        std::shared_ptr<odb::core::database> _db;
    public:
        ChatSessionMemberTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        bool append(ChatSessionMember &csm) // 会话成员新增
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->persist(csm);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增单会话成员失败 {}-{}:{} ", csm.session_id(), csm.user_id(), e.what());
                return false;
            }
            return true;
        }
        bool append(std::vector<ChatSessionMember> &csm_list)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                for(auto& csm : csm_list)
                {
                    _db->persist(csm);
                }
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增多会话成员失败 {}-{}:{} ", csm_list[0].session_id(), csm_list.size(), e.what());
                return false;
            }
            return true;
        }
        // 删除指定会话中的指定成员  -- ssid & uid
        bool remove(const ChatSessionMember &csm)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;
                _db->erase_query<ChatSessionMember>(query::session_id == csm.session_id() && query::user_id == csm.user_id());
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除单会话成员失败 {}-{}:{} ", csm.session_id(), csm.user_id(), e.what());
                return false;
            }
            return true;
        }
        // 删除会话的所有成员信息
        bool remove(const std::string &ssid)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;
                _db->erase_query<ChatSessionMember>(query::session_id == ssid);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除多会话成员失败 {}:{} ", ssid, e.what());
                return false;
            }
            return true;
        }
        std::vector<std::string> members(const std::string &ssid)
        {
            std::vector<std::string> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;

                result r(_db->query<ChatSessionMember>(query::session_id == ssid));
                for (result::iterator it(r.begin()); it != r.end(); ++it)
                    res.push_back(it->user_id());
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("获取会话成员失败: {}-{} ", ssid, e.what());
            }
            return res;
        }
    };
}