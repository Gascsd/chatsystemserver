#include "icsearch.hpp"
#include "user.hxx"
#include "message.hxx"

namespace zht_im
{
    class ESClientFactory
    {
    public:
        static std::shared_ptr<elasticlient::Client> create(const std::vector<std::string> &host_list)
        {
            return std::make_shared<elasticlient::Client>(host_list);
        }
    };
    class ESUser
    {
    public:
        using ptr = std::shared_ptr<ESUser>;
    private:
        std::shared_ptr<elasticlient::Client> _es_client;

        const std::string _uid_key = "user_id";
        const std::string _nickname_key = "nickname";
        const std::string _description_key = "description";
        const std::string _phone_key = "phone";
        const std::string _avatar_key = "avatar_id";

    public:
        ESUser(const std::shared_ptr<elasticlient::Client> &es_client)
            : _es_client(es_client) {}
        bool createIndex()
        {
            bool ret = ESIndex(_es_client, "user")
                           .append(_uid_key, "keyword", "standard", true)
                           .append(_nickname_key)
                           .append(_phone_key, "keyword", "standard", true)
                           .append(_description_key, "text", "standard", false)
                           .append(_avatar_key, "keyword", "standard", false)
                           .create();
            if (ret == false)
            {
                LOG_INFO("用户信息索引创建失败");
                return false;
            }
            LOG_INFO("用户信息索引创建成功");
            return true;
        }
        bool appendData(const std::string &uid, const std::string &phone, const std::string &nickname,
                        const std::string &description, const std::string &avatar_id)
        {
            auto ret = ESInsert(_es_client, "user")
                           .append(_uid_key, uid)
                           .append(_phone_key, phone)
                           .append(_nickname_key, nickname)
                           .append(_description_key, description)
                           .append(_avatar_key, avatar_id)
                           .insert(uid);
            if (ret == false)
            {
                LOG_ERROR("数据插入/更新失败");
                return false;
            }
            LOG_INFO("数据更新成功");
            return true;
        }
        bool appendData(std::shared_ptr<User> &user)
        {
            std::string phone = user->phone() ? *user->phone() : "";
            std::string nickname = user->nickname() ? *user->nickname() : "";
            std::string description = user->description() ? *user->description() : "";
            std::string avatar_id = user->avatar_id() ? *user->avatar_id() : "";

            auto ret = ESInsert(_es_client, "user")
                           .append(_uid_key, user->user_id())
                           .append(_phone_key, phone)
                           .append(_nickname_key, nickname)
                           .append(_description_key, description)
                           .append(_avatar_key, avatar_id)
                           .insert(user->user_id());
            if (ret == false)
            {
                LOG_ERROR("数据插入/更新失败");
                return false;
            }
            LOG_INFO("数据更新成功");
            return true;
        }
        std::vector<User> search(const std::string &key, const std::vector<std::string> &uid_list)
        {
            std::vector<User> ret;
            Json::Value res = ESSearch(_es_client, "user")
                .append_should_match(_phone_key + ".keyword", key)
                .append_should_match(_uid_key + ".keyword", key)
                .append_should_match(_nickname_key, key)
                .append_must_not_terms(_uid_key + ".keyword", uid_list)
                .search();
            if(res.isArray() == false)
            {
                LOG_ERROR("用户搜索结果为空,或者结果不是数组类型");
                return ret;
            }
            int sz = res.size();
            LOG_DEBUG("检索结果条目数量: {}", sz);
            for(int i = 0; i < sz; ++i)
            {
                User user;
                user.user_id(res[i]["_source"][_uid_key].asString());
                user.nickname(res[i]["_source"][_nickname_key].asString());
                user.description(res[i]["_source"][_description_key].asString());
                user.phone(res[i]["_source"][_phone_key].asString());
                user.avatar_id(res[i]["_source"][_avatar_key].asString());
                ret.push_back(user);
            }
            return ret;
        }
    };

    class ESMessage
    {
    public:
        using ptr = std::shared_ptr<ESMessage>;
    private:
        std::shared_ptr<elasticlient::Client> _es_client;

        const std::string _uid_key = "user_id";
        const std::string _chat_session_id_key = "chat_session_id";
        const std::string _message_id_key = "message_id";
        const std::string _create_time_key = "create_time";
        const std::string _content_key = "content";
    public:
        ESMessage(const std::shared_ptr<elasticlient::Client> &es_client)
            : _es_client(es_client) {}
        bool createIndex()
        {
            bool ret = ESIndex(_es_client, "message")
                           .append(_uid_key, "keyword", "standard", false)
                           .append(_message_id_key, "keyword", "standard", false)
                           .append(_create_time_key, "long", "standard", false)
                           .append(_chat_session_id_key, "keyword", "standard", true)
                           .append(_content_key)
                           .create();
            if (ret == false)
            {
                LOG_INFO("消息信息索引创建失败");
                return false;
            }
            LOG_INFO("消息信息索引创建成功");
            return true;
        }
        bool appendData(const std::string &uid, const std::string &message_id, const long create_time,
                        const std::string &chat_session_id, const std::string &content)
        {
            auto ret = ESInsert(_es_client, "message")
                           .append(_uid_key, uid)
                           .append(_message_id_key, message_id)
                           .append(_create_time_key, create_time)
                           .append(_chat_session_id_key, chat_session_id)
                           .append(_content_key, content)
                           .insert(message_id);
            if (ret == false)
            {
                LOG_ERROR("数据插入/更新失败");
                return false;
            }
            LOG_INFO("数据更新成功");
            return true;
        }
        bool appendData(std::shared_ptr<Message> &msg)
        {
            std::string content = msg->content() ? *msg->content() : "";
            static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
            boost::posix_time::time_duration diff = msg->create_time() - epoch;
            long create_time = diff.total_seconds();

            // 返回总秒数
            return diff.total_seconds();

            auto ret = ESInsert(_es_client, "message")
                           .append(_uid_key, msg->user_id())
                           .append(_message_id_key, msg->message_id())
                           .append(_create_time_key, create_time)
                           .append(_chat_session_id_key, msg->session_id())
                           .append(_content_key, content)
                           .insert(msg->message_id());
            if (ret == false)
            {
                LOG_ERROR("数据插入/更新失败");
                return false;
            }
            LOG_INFO("数据更新成功");
            return true;
        }
        bool remove(const std::string &mid)
        {
            bool ret = ESRemove(_es_client, "message").remove(mid);
            if (ret == false)
            {
                LOG_ERROR("消息数据删除失败");
                return false;
            }
            LOG_INFO("消息数据删除失败");
            return true;
        }
        std::vector<Message> search(const std::string &key, const std::string &ssid)
        {
            std::vector<Message> ret;
            Json::Value res = ESSearch(_es_client, "message")
                .append_must_term(_chat_session_id_key + ".keyword", ssid)
                .append_must_match(_content_key, key)
                .search();
            if(res.isArray() == false)
            {
                LOG_ERROR("用户搜索结果为空,或者结果不是数组类型");
                return ret;
            }
            int sz = res.size();
            LOG_DEBUG("检索结果条目数量: {}", sz);
            for(int i = 0; i < sz; ++i)
            {
                Message message;
                message.user_id(res[i]["_source"][_uid_key].asString());
                message.message_id(res[i]["_source"][_message_id_key].asString());
                boost::posix_time::ptime ctime(boost::posix_time::from_time_t(res[i]["_source"][_create_time_key].asInt64()));
                message.create_time(ctime);
                message.session_id(res[i]["_source"][_chat_session_id_key].asString());
                message.content(res[i]["_source"][_content_key].asString());
                ret.push_back(message);
            }
            return ret;
        }
    };
}