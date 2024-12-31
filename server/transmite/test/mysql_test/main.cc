#include "../../../odb/chat_session_member.hxx"
#include "../../../common/data_mysql.hpp"
#include "../../../common/logger.hpp"
#include "chat_session_member-odb.hxx"
#include <gflags/gflags.h>

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");


void append_test(zht_im::ChatSessionMemberTable &tb)
{
    zht_im::ChatSessionMember cms1("会话ID1", "用户ID1");
    tb.append(cms1);
    zht_im::ChatSessionMember cms2("会话ID1", "用户ID2");
    tb.append(cms2);
    zht_im::ChatSessionMember cms3("会话ID2", "用户ID1");
    tb.append(cms3);
}
void multi_append_test(zht_im::ChatSessionMemberTable &tb)
{
    zht_im::ChatSessionMember cms1("会话ID3", "用户ID1");
    zht_im::ChatSessionMember cms2("会话ID3", "用户ID2");
    zht_im::ChatSessionMember cms3("会话ID3", "用户ID3");
    std::vector<zht_im::ChatSessionMember> cms_list = {cms1, cms2, cms3};
    tb.append(cms_list);
}
void remove_test(zht_im::ChatSessionMemberTable &tb)
{
    zht_im::ChatSessionMember cms3("会话ID2", "用户ID1");
    tb.remove(cms3);
}
void remove_all_test(zht_im::ChatSessionMemberTable &tb)
{
    tb.remove("会话ID3");
}
void ss_members(zht_im::ChatSessionMemberTable &tb)
{
    auto ret = tb.members("会话ID1");
    for(auto &id : ret)
    {
        std::cout << id << std::endl;
    }
}
int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto db = zht_im::ODBFactory::create("root", "zht1125x", "127.0.0.1", "zht_im", "utf8", 0, 1);

    zht_im::ChatSessionMemberTable csmt(db);
    // append_test(csmt);
    // multi_append_test(csmt);
    // remove_test(csmt);
    // ss_members(csmt);
    remove_all_test(csmt);
    return 0;
}