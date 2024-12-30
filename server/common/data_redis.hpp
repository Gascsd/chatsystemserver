#include <sw/redis++/redis.h>
#include <iostream>

namespace zht_im
{
    class RedisClientFactory
    {
    public:
        static std::shared_ptr<sw::redis::Redis> create(
            const std::string &host, int port, int db, bool keep_alive)
        {
            sw::redis::ConnectionOptions opts;
            opts.host = host;
            opts.port = port;
            opts.db = db;
            opts.keep_alive = keep_alive;
            auto res = std::make_shared<sw::redis::Redis>(opts);
            return res;
        }
    };
    class Session
    {
    public:
        using ptr = std::shared_ptr<Session>;
    private:
        std::shared_ptr<sw::redis::Redis> _redis_client;
    public:
        Session(const std::shared_ptr<sw::redis::Redis> &redis_client)
            : _redis_client(redis_client) {}
        void append(const std::string &ssid, const std::string &uid)
        {
            _redis_client->set(ssid, uid);
        }
        void remove(const std::string &ssid)
        {
            _redis_client->del(ssid);
        }
        sw::redis::OptionalString uid(const std::string &ssid)
        {
            return _redis_client->get(ssid);
        }
    };

    class Status
    {
    public:
        using ptr = std::shared_ptr<Status>;
    private:
        std::shared_ptr<sw::redis::Redis> _redis_client;
    public:
        Status(const std::shared_ptr<sw::redis::Redis> &redis_client)
            :_redis_client(redis_client) {}
        void append(const std::string &uid)
        {
            _redis_client->set(uid, "");
        }
        void remove(const std::string &uid)
        {
            _redis_client->del(uid);
        }
        bool exists(const std::string &uid)
        {
            auto res = _redis_client->get(uid);
            if(res) return true;
            return false;
        }
    };

    class Codes
    {
    public:
       using ptr = std::shared_ptr<Codes>;
    private:
        std::shared_ptr<sw::redis::Redis> _redis_client;
    public:
        Codes(const std::shared_ptr<sw::redis::Redis> &redis_client)
            :_redis_client(redis_client) {}
        /**
         * @param cid 验证码ID
         * @param code 验证码
         * @param second 过期时间,单位s
         */
        bool append(const std::string &cid, const std::string &code, 
            int second = 300) // 默认过期时间是5min
        {
            std::chrono::milliseconds t = std::chrono::milliseconds(second * 1000);
            return _redis_client->set(cid, code, t);
        }
        bool equal(const std::string &cid, const std::string &vcode)
        {
            if (vcode == code(cid))
            {
                remove(cid);
                return true;
            }
            return false;
        }
        void remove(const std::string &cid)
        {
            _redis_client->del(cid);
        }
        sw::redis::OptionalString code(const std::string &cid)
        {
            return _redis_client->get(cid);
        }
    };
} // namespace zht_im
