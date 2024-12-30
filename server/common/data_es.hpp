#include "icsearch.hpp"
#include "user.hxx"

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
}