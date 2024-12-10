#include "../common/icsearch.hpp"
#include <gflags/gflags.h>

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    std::vector<std::string> host_list = {"http://127.0.0.1:9200/"};
    auto client = std::make_shared<elasticlient::Client>(host_list);
    
    ESIndex index(client, "test_user");
    bool ret = index.append("nickname").append("phone", "keyword", "standard", true).create();
    if(ret == false)
    {
        LOG_ERROR("索引创建失败");
        return -1;
    }
    LOG_INFO("索引创建成功");
    // 数据新增
    ret = ESInsert(client, "test_user").append("nickname", "张三")
        .append("phone", "15566667777").insert("00001");
    if(ret == false)
    {
        LOG_ERROR("数据插入失败");
        return -1;
    }
    LOG_INFO("数据新增成功");
    // 数据修改（同一id，内容不同就是修改）
        ret = ESInsert(client, "test_user").append("nickname", "张三")
        .append("phone", "13344445555").insert("00001");
    if(ret == false)
    {
        LOG_ERROR("数据更新失败");
        return -1;
    }
    LOG_INFO("数据更新成功");

    Json::Value user = ESSearch(client, "test_user")
        .append_should_match("phone.keyword", "13344445555")
        // .append_must_not_terms("name.keyword", {"张三"})
        .search();
    if(user.empty() || user.isArray() == false)
    {
        LOG_ERROR("结果为空或者结果不是一个数组");
        return -1;
    }
    LOG_INFO("数据查询成功");
    int sz = user.size();
    for(int i = 0; i < sz; ++i)
    {
        std::cout << user[i]["_source"]["nickname"].asString() << std::endl;
    }
    ret = ESRemove(client, "test_user").remove("00001");
    if(ret == false)
    {
        LOG_ERROR("删除数据失败");
        return -2;
    }
    LOG_INFO("数据删除成功");

    return 0;
}