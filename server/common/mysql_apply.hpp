#pragma once
#include "mysql.hpp"
#include "friend_apply.hxx"
#include "friend_apply-odb.hxx"

namespace zht_im
{
    class FriendApplyTable
    {
    public:
        using ptr = std::shared_ptr<FriendApplyTable>;

    private:
        std::shared_ptr<odb::core::database> _db;

    public:
        FriendApplyTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        // 新增关系信息
        bool insert(FriendApply &ev)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->persist(ev);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增好友申请事件失败 {}-{}:{} ", ev.user_id(), ev.peer_id(), e.what());
                return false;
            }
            return true;
        }
        
        // 通过关系人员移除关系信息
        bool remove(const std::string &uid, const std::string &pid)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<FriendApply> query;
                typedef odb::result<FriendApply> result;
                _db->erase_query<FriendApply>(query::user_id == uid && query::peer_id == pid);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除好友申请事件失败 {}-{}:{} ", uid, pid, e.what());
                return false;
            }
            return true;
        }
        // 通过事件ID移除关系信息
        bool remove(const std::string &eid) 
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<FriendApply> query;
                typedef odb::result<FriendApply> result;
                _db->erase_query<FriendApply>(query::event_id == eid);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过事件ID移除关系信息失败 {}-{}:{} ", eid, e.what());
                return false;
            }
            return true;
        }
        bool exists(const std::string &eid)
        {
            typedef odb::query<FriendApply> query;
            typedef odb::result<FriendApply> result;
            result r;
            bool flag = false;
            try
            {
                odb::transaction trans(_db->begin());
                r = _db->query<FriendApply>(query::event_id == eid);
                flag = !flag;
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("获取好友申请事件失败 {} ", eid, e.what());
            }
            return flag;
        }
        bool exists(const std::string &uid, const std::string &pid)
        {
            typedef odb::query<FriendApply> query;
            typedef odb::result<FriendApply> result;
            result r;
            bool flag = false;
            try
            {
                odb::transaction trans(_db->begin());
                r = _db->query<FriendApply>(query::user_id == uid && query::peer_id == pid);
                if(!r.empty())
                    flag = !flag;
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("获取好友申请事件失败 {}-{} ", uid, pid, e.what());
            }
            return flag;
        }
        // 获取指定用户的好友申请者ID
        std::vector<std::string> applyUsers(const std::string& uid)
        {
            std::vector<std::string> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<FriendApply> query;
                typedef odb::result<FriendApply> result;
                // 要查询的的uid是被申请者的用户ID
                result r(_db->query<FriendApply>(query::peer_id == uid));
                for(result::iterator it = r.begin(); it != r.end(); ++it)
                {
                    res.push_back(it->user_id());
                }
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("查询{}的好友申请者ID失败 {} ", uid, e.what());
            }
            return res;
        }
    };
}