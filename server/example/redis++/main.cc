#include <sw/redis++/redis++.h>
#include <gflags/gflags.h>
#include <iostream>
#include <string>
#include <vector>

DEFINE_string(ip, "127.0.0.1", "这是服务器的IP地址,格式:127.0.0.1");
DEFINE_int32(port, 6379, "这是服务器的端口,格式:8080");
DEFINE_int32(db, 0, "库的编号,默认0号");
DEFINE_bool(keep_alive, true, "是否进行长连接保活");

void print(sw::redis::Redis &client)
{
    auto user1 = client.get("会话ID1");
    if (user1) std::cout << *user1 << std::endl;
    auto user2 = client.get("会话ID2");
    if (user2) std::cout << *user2 << std::endl;
    auto user3 = client.get("会话ID3");
    if (user3) std::cout << *user3 << std::endl;
    auto user4 = client.get("会话ID4");
    if (user4) std::cout << *user4 << std::endl;
    auto user5 = client.get("会话ID5");
    if (user5) std::cout << *user5 << std::endl;
}

void add_string(sw::redis::Redis &client)
{
    client.set("会话ID1", "用户ID1");
    client.set("会话ID2", "用户ID2");
    client.set("会话ID3", "用户ID3");
    client.set("会话ID4", "用户ID4");
    client.set("会话ID5", "用户ID5");

    client.del("会话ID3");

    client.set("会话ID5", "用户ID5555"); // 数据存在就修改，不存在就新增
    print(client);
}

void expired_test(sw::redis::Redis &client)
{
    // 本次修改的时候，不仅修改了val，而且新增了过期时间
    client.set("会话ID1", "用户ID11111", std::chrono::milliseconds(1000));

    print(client);
    std::cout << "------------------休眠2s---------------\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    print(client);
}

void list_test(sw::redis::Redis &client)
{
    client.rpush("群聊1", "成员1");
    client.rpush("群聊1", "成员2");
    client.rpush("群聊1", "成员3");
    client.rpush("群聊1", "成员4");
    client.rpush("群聊1", "成员5");

    std::vector<std::string> users;
    client.lrange("群聊1", 0, -1, std::back_inserter(users));

    for(auto user : users)
    {
        std::cout << user << std::endl;
    }
}

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    // 1. 实例化redis对象，构造连接选项，连接服务器
    sw::redis::ConnectionOptions opts;
    opts.host = FLAGS_ip;
    opts.port = FLAGS_port;
    opts.db = FLAGS_db;
    opts.keep_alive = FLAGS_keep_alive;

    sw::redis::Redis client(opts);

    // 2. 添加、删除、获取字符串键值对
    add_string(client);
    // 3. 时间控制数据有效时间的操作
    expired_test(client);
    // 4. 列表的操作，实现数据的右插左获取
    std::cout << "----------------------------\n";
    list_test(client);
    return 0;
}