#pragma once
#include "mysql.hpp"
#include "user.hxx"
#include "user-odb.hxx"
/**
 * 功能：
 *   用户注册， 用户登录， 验证码获取，手机号注册，手机号登录， 获取用户信息， 用户信息修改
 * 所需提供的接口：
 *   用户信息新增， 通过昵称获取用户信息， 通过手机号获取用户信息，通过用户ID获取用户信息，通过多个用户ID获取多个用户信息， 信息修改
 */
namespace zht_im
{
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
} // namespace zht_im
