#pragma once
#include "mysql.hpp"
#include "relation.hxx"
#include "relation-odb.hxx"

namespace zht_im
{
    class RelationTable
    {
    public:
        using ptr = std::shared_ptr<RelationTable>;

    private:
        std::shared_ptr<odb::core::database> _db;

    public:
        RelationTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        // 新增关系信息
        bool insert(std::string uid, std::string pid)
        {
            // {1,2} {2,1} 构造两个关系
            try
            {
                Relation r1(uid, pid);
                Relation r2(pid, uid);
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->persist(r1);
                _db->persist(r2);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增好友关系信息失败 {}-{}:{} ", uid, pid, e.what());
                return false;
            }
            return true;
        }
        // 移除关系信息
        bool remove(const std::string &uid, const std::string &pid)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<Relation> query;
                typedef odb::result<Relation> result;
                _db->erase_query<Relation>(query::user_id == uid && query::peer_id == pid);
                _db->erase_query<Relation>(query::user_id == pid && query::peer_id == uid);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除好友关系信息失败 {}-{}:{} ", uid, pid, e.what());
                return false;
            }
            return true;
        }
        // 判断关系是否存在
        bool exists(const std::string &uid, const std::string &pid)
        {
            typedef odb::query<Relation> query;
            typedef odb::result<Relation> result;
            bool ret = false; 
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                result r = _db->query<Relation>(query::user_id == uid && query::peer_id == pid);
                ret = !r.empty();
                trans.commit(); // 这里有一个坑，在提交事务之后，r的内容将会被改变
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("获取好友关系失败 {}-{} : {}", uid, pid, e.what());
            }
            return ret;
        }
        // 获取指定用户的好友ID
        std::vector<std::string> friends(const std::string& uid)
        {
            std::vector<std::string> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<Relation> query;
                typedef odb::result<Relation> result;
                result r(_db->query<Relation>(query::user_id == uid));
                for(result::iterator it = r.begin(); it != r.end(); ++it)
                {
                    res.push_back(it->peer_id());
                }
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("查询{}的好友关系失败 {} ", uid, e.what());
            }
            return res;
        }
    };
}