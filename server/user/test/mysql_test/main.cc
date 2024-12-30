#include "../../../odb/user.hxx"
#include "../../../common/data_mysql.hpp"
#include "../../../common/logger.hpp"
#include "user-odb.hxx"
#include <gflags/gflags.h>

DEFINE_bool(run_mode, false, "程序运行模式模式,false-调试;true-发布");
DEFINE_string(log_file, "", "发布模式下,用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下,用于指定日志输出等级");


void insert(zht_im::UserTable &user)
{
    auto user1 = std::make_shared<zht_im::User>("uid1", "昵称1", "123456");
    user.insert(user1);

    auto user2 = std::make_shared<zht_im::User>("uid2", "12344445555");
    user.insert(user2);
}

void update_by_id(zht_im::UserTable &user_tb)
{
    auto user = user_tb.select_by_id("uid1");
    user->description("我很懒，不写签名");
    user_tb.update(user);
}
void update_by_phone(zht_im::UserTable &user_tb)
{
    auto user = user_tb.select_by_phone("12344445555");
    user->password("123321");
    user_tb.update(user);
}

void update_by_nickname(zht_im::UserTable &user_tb)
{
    auto user = user_tb.select_by_nickname("昵称1");
    if(user.get() != nullptr)
    {
        user->nickname("昵称2");
        user_tb.update(user);
    }
}

void select_users(zht_im::UserTable &user_tb)
{
    std::vector<std::string> id_list = {"uid1", "uid2"};
    auto res = user_tb.select_multi_users(id_list);
    for(auto& user : res)
    {
        if(user.nickname()) std::cout << *user.nickname() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    zht_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto db = zht_im::ODBFactory::create("root", "zht1125x", "127.0.0.1", "zht_im", "utf8", 0, 1);

    zht_im::UserTable user(db);

    // insert(user);
    // update_by_id(user);
    // update_by_phone(user);
    // update_by_nickname(user);
    select_users(user);
    return 0;
}