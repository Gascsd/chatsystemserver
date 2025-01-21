
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <algorithm>
#include "mysql_chat_session.hpp"
#include "mysql_apply.hpp"
#include "mysql_relation.hpp"

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");

std::shared_ptr<zht_im::RelationTable> rtb;
std::shared_ptr<zht_im::FriendApplyTable> ftb;
std::shared_ptr<zht_im::ChatSessionTable> cstb;

// TEST(relation单元测试, 新增和查找用户关系测试)
// {
//     rtb->insert("用户ID1", "用户ID2");
//     rtb->insert("用户ID1", "用户ID3");
//     auto res = rtb->friends("用户ID1");
//     sort(res.begin(), res.end());
//     ASSERT_EQ(res[0], "用户ID2");
//     ASSERT_EQ(res[1], "用户ID3");
// }
// TEST(relation单元测试, 删除用户关系测试)
// {
//     rtb->remove("用户ID2", "用户ID1");
//     auto res = rtb->friends("用户ID1");
//     sort(res.begin(), res.end());
//     ASSERT_EQ(res[0], "用户ID3");
//     ASSERT_EQ(res.size(), 1);
// }
// TEST(relation单元测试, 查看用户关系测试)
// {
//     ASSERT_FALSE(rtb->exists("用户ID2", "用户ID1"));
//     ASSERT_TRUE(rtb->exists("用户ID3", "用户ID1"));
//     rtb->remove("用户ID3", "用户ID1");
// }

void apply_insert_test()
{
    zht_im::FriendApply fa1("事件ID1", "用户ID1", "用户ID2");
    ftb->insert(fa1);
    zht_im::FriendApply fa2("事件ID2", "用户ID1", "用户ID3");
    ftb->insert(fa2);
    zht_im::FriendApply fa3("事件ID3", "用户ID3", "用户ID2");
    ftb->insert(fa3);
}
void apply_exists_test()
{
    std::cout << ftb->exists("用户ID2", "用户ID3") << std::endl;
    std::cout << ftb->exists("用户ID1", "用户ID3") << std::endl;
    std::cout << ftb->exists("用户ID1", "用户ID2") << std::endl;
    std::cout << ftb->exists("用户ID1", "用户ID4") << std::endl;
}

void apply_remove_test()
{
    ftb->remove("用户ID2", "用户ID3");
    ftb->remove("用户ID1", "用户ID2");
    ftb->remove("事件ID2");
}
void apply_select_test()
{
    // zht_im::FriendApply fa1("事件ID4", "用户ID1", "用户ID2");
    // ftb->insert(fa1);
    auto res = ftb->applyUsers("用户ID2");
    for(auto& e : res)
    {
        std::cout << e << std::endl;
    }
}
void chat_insert_test()
{
    zht_im::ChatSession cs1("会话ID1", "会话名称1", zht_im::ChatSessionType::SINGLE);
    zht_im::ChatSession cs2("会话ID2", "会话名称2", zht_im::ChatSessionType::GROUP);
    cstb->insert(cs1);
    cstb->insert(cs2);
}
void chat_select_test()
{
    auto ret = cstb->select("会话ID1");
    std::cout << ret->chat_session_id() << std::endl;
    std::cout << ret->chat_session_name() << std::endl;
    std::cout << (int)ret->chat_session_type() << std::endl;
}
void chat_single_test()
{
    auto res = cstb->singleChatSession("用户ID4");
    for(auto &info : res)
    {
        std::cout << info.chat_session_id << std::endl;
        std::cout << info.friend_id << std::endl;
    }
}
void chat_group_test()
{
    auto res = cstb->groupChatSession("用户ID1");
    for(auto &info : res)
    {
        std::cout << info.chat_session_id << std::endl;
        std::cout << info.chat_session_name << std::endl;
    }
}
void chat_remove_test()
{
    cstb->remove("会话ID3");
}
void chat_remove_test2()
{
    cstb->remove("用户ID4", "用户ID5");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto db = zht_im::ODBFactory::create("root", "zht1125x", "127.0.0.1", "zht_im", "utf8", 0, 1);
    rtb = std::make_shared<zht_im::RelationTable>(db);
    ftb = std::make_shared<zht_im::FriendApplyTable>(db);
    cstb = std::make_shared<zht_im::ChatSessionTable>(db);


    // apply_insert_test();
    // apply_remove_test();
    // apply_select_test();
    // apply_exists_test();
    // chat_insert_test();
    // chat_select_test();
    // chat_single_test();
    // std::cout << "----------------------------\n";
    // chat_group_test();
    // chat_remove_test();
    chat_remove_test2();
    return 0;
}