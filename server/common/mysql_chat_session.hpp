#pragma once
#include "mysql.hpp"
#include "chat_session.hxx"
#include "chat_session-odb.hxx"

namespace zht_im
{
    class ChatSessionTable
    {
    public:
        using ptr = std::shared_ptr<ChatSessionTable>;

    private:
        std::shared_ptr<odb::core::database> _db;

    public:
        ChatSessionTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        // 新增会话信息
        bool insert(ChatSession &cs)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                _db->persist(cs);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("新增会话失败 {}:{} ", cs.chat_session_name(), e.what());
                return false;
            }
            return true;
        }
        bool remove(const std::string &ssid)
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<ChatSession> query;
                typedef odb::result<ChatSession> result;
                _db->erase_query<ChatSession>(query::chat_session_id == ssid);

                // 在业务逻辑中进行外键约束处理
                typedef odb::query<ChatSessionMember> mquery;
                typedef odb::result<ChatSessionMember> mresult;
                _db->erase_query<ChatSessionMember>(mquery::session_id == ssid);
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除会话失败 {}:{} ", ssid, e.what());
                return false;
            }
            return true;
        }
        bool remove(const std::string &uid, const std::string &pid) // 单聊会话删除，根据单聊会话的两个成员
        {
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<SingleChatSession> squery;
                typedef odb::result<SingleChatSession> sresult;
                auto res = _db->query_one<SingleChatSession>(
                    squery::csm1::user_id == uid && 
                    squery::csm2::user_id == pid && 
                    squery::css::chat_session_type == ChatSessionType::SINGLE);
                if(!res)
                {
                    LOG_ERROR("没有{}-{}的单聊会话", uid, pid);
                    return false;
                }
                std::string cssid = res->chat_session_id;

                typedef odb::query<ChatSession> cquery;
                _db->erase_query<ChatSession>(cquery::chat_session_id == cssid);
                // 在业务逻辑中进行外键约束处理
                typedef odb::query<ChatSessionMember> mquery;
                _db->erase_query<ChatSessionMember>(mquery::session_id == cssid);
                trans.commit();
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("删除会话失败 {}-{}:{} ", uid, pid, e.what());
                return false;
            }
            return true;
        }
        // 通过会话ID获取会话信息
        std::shared_ptr<ChatSession> select(const std::string &ssid)
        {
            std::shared_ptr<ChatSession> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<ChatSession> query;
                typedef odb::result<ChatSession> result;
                res = std::shared_ptr<ChatSession>(_db->query_one<ChatSession>(query::chat_session_id == ssid));
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("通过会话ID获取会话信息失败 {} ", ssid);
            }
            return res;
        }
        std::vector<SingleChatSession> singleChatSession(const std::string &uid)
        {
            std::vector<SingleChatSession> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<SingleChatSession> query;
                typedef odb::result<SingleChatSession> result;
                // 要查询的的uid是被申请者的用户ID
                result r(_db->query<SingleChatSession>(
                    query::css::chat_session_type == zht_im::ChatSessionType::SINGLE &&
                    query::csm1::user_id == uid &&
                    query::csm2::user_id != query::csm1::user_id));
                for (result::iterator it = r.begin(); it != r.end(); ++it)
                {
                    res.push_back(*it);
                }
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("获取用户 {} 的单聊会话失败 {} ", uid, e.what());
            }
            return res;
        }
        std::vector<GroupChatSession> groupChatSession(const std::string &uid)
        {
            std::vector<GroupChatSession> res;
            try
            {
                odb::transaction trans(_db->begin()); // 获取事务对象，开启事务
                typedef odb::query<GroupChatSession> query;
                typedef odb::result<GroupChatSession> result;
                result r(_db->query<GroupChatSession>(
                    query::css::chat_session_type == ChatSessionType::GROUP &&
                    query::csm::user_id == uid));
                for (result::iterator it = r.begin(); it != r.end(); ++it)
                {
                    res.push_back(*it);
                }
                trans.commit(); // 提交事务
            }
            catch (const std::exception &e)
            {
                LOG_ERROR("获取用户 {} 的群聊会话失败 {} ", uid, e.what());
            }
            return res;
        }
    };
} // namespace zht_im
