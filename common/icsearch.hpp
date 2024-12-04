#include <elasticlient/client.h>
#include <cpr/cpr.h>
#include <json/json.h>
#include <memory>
#include <string>
#include <iostream>
#include "logger.hpp"


bool Serialize(const Json::Value &root, std::string &dst)
{
    Json::StreamWriterBuilder swb;
    swb.settings_["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    std::stringstream ss;
    int ret = sw->write(root, &ss);
    if(ret != 0)
    {
        return false;
    }
    dst = ss.str();
    return true;
}
bool UnSerialize(const std::string &src, Json::Value &val)
{
    Json::CharReaderBuilder crb;
    crb.settings_["emitUTF8"] = true;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
    std::string err;
    bool ret = cr->parse(src.c_str(), src.c_str() + src.size(), &val, &err);
    if(ret == false)
    {
        std::cout << "反序列化失败:" << err << std::endl;
        return false;
    }
    return true;
}


class ESIndex
{
private:
    std::string _name;
    std::string _type;
    Json::Value _properties;
    Json::Value _index;
    std::shared_ptr<elasticlient::Client> _client;
public:
    ESIndex(const std::string& name, const std::string& type)
    {
        Json::Value analysis;
        Json::Value analyzer;
        Json::Value ik;
        Json::Value tokenizer;
        tokenizer["tokenizer"] = "ik_max_word";
        ik["ik"] = tokenizer;
        analyzer["analyzer"] = ik;
        analysis["analysis"] = analyzer;
        _index["settings"] = analysis;

        Json::Value mappings;
        mappings["dynamic"] = true;



        std::string str;
        Serialize(_index, str);
        std::cout << str << std::endl;
    }
    void append(const std::string& key, const std::string& type = "text",
        const std::string& analyzer = "ik_max_word", bool enabled = true)
    {
        Josn::Value fields;
        fields["type"] = type;
        fields["analyzer"] = analyzer;
        if(enable == false) fields["enable"] = enabled;
        _properties[key] = fields;
    }
    bool create()
    {
        Json::Value mappings;
        mappings["dynamic"] = true;
        mappings["properties"] = _properties;
        _index["mappings"] = mappings;

        std::string body;
        bool ret = Serialize(_index, body);
        if(ret == false)
        {
            LOG_ERROR("索引序列化失败");
            return false;
        }
        // elasticlient::Client clent({"http://127.0.0.1:9200"});
        // 发起搜索请求
        try
        {
            auto rsp = _client->search("user", "_doc", "{\"query\": { \"match_all\":{} }}");
            std::cout << rsp.status_code << std::endl;
            std::cout << rsp.text << std::endl;
        }
        catch(std::exception& e)
        {
            std::cout << "请求失败" << e.what() << endl;
            return -1;
        }
    }
};