#pragma once
#include "mysql.hpp"
#include "chat_session_member.hxx"
#include "chat_session_member-odb.hxx"
namespace zht_im
{
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
                for (auto &csm : csm_list)
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