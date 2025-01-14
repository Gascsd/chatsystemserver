#pragma once
#include "mysql.hpp"
#include "message.hxx"
#include "message-odb.hxx"

namespace zht_im
{
    class MessageTable
    {
    public:
        using ptr = std::shared_ptr<MessageTable>;
    private:
        std::shared_ptr<odb::core::database> _db;
    public:
        MessageTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        ~MessageTable() {}
        bool insert(Message &msg)
        {
            try
            {
                odb::transaction trans(_db->begin());
                _db->persist(msg);
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("新增消息失败 {}:{}", msg.message_id(), e.what());
                return false;
            }
            return true;
        }
        bool remove(const std::string &ssid)
        {
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<Message> query;
                typedef odb::result<Message> result;
                _db->erase_query<Message>(query::session_id == ssid);
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("删除会话所有消息失败 {}:{}", ssid, e.what());
                return false;
            }
            return true;
        }
        std::vector<Message> recent(const std::string &ssid, int count)
        {
            std::vector<Message> res;
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<Message> query;
                typedef odb::result<Message> result;
                std::stringstream cond;
                cond << "session_id='" << ssid << "' ";
                cond << "order by create_time desc limit " << count;
                result r(_db->query<Message>(cond.str()));
                for(result::iterator it(r.begin()); it != r.end(); ++it)
                {
                    res.push_back(*it);
                }
                std::reverse(res.begin(), res.end());
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("获取最近{}条消息失败 {}-{}", count, ssid, e.what());
            }
            return res;
        }
        std::vector<Message> range(const std::string &ssid, boost::posix_time::ptime &stime, boost::posix_time::ptime &etime)
        {
            std::vector<Message> res;
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<Message> query;
                typedef odb::result<Message> result;
                result r(_db->query<Message>(query::session_id == ssid && query::create_time >= stime && query::create_time <= etime));
                for(result::iterator it(r.begin()); it != r.end(); ++it)
                {
                    res.push_back(*it);
                }
                trans.commit();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("获取{}到{}的区间消息失败 {}-{}", boost::posix_time::to_simple_string(stime), boost::posix_time::to_simple_string(etime), ssid, e.what());
            }
            return res;
        }
    };
} // namespace zht_im
