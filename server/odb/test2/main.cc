#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include "student.hxx"
#include "student-odb.hxx"
#include <gflags/gflags.h>
#include <memory>

DEFINE_string(host, "127.0.0.1", "这是MySQL服务器地址");
DEFINE_int32(port, 3306, "这是MySQL服务器端口");
DEFINE_string(db, "TestDB", "数据库默认名称");
DEFINE_string(user, "root", "默认登录用户");
DEFINE_string(passwd, "zht1125x", "默认密码");
DEFINE_string(cset, "utf8", "这是MySQL客户端字符集");
DEFINE_int32(max_pool, 3, "这是MySQL连接池最大连接数量");

void insert_classes(odb::mysql::database &db)
{
    try
    {
        // 3. 获取事务操作对象，开启事务
        odb::transaction trans(db.begin());
        // 4. 操作
        Classes c1("一年级一班");
        Classes c2("一年级二班");
        db.persist(c1);
        db.persist(c2);
        // 5. 提交事务
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "插入班级信息出错: " << e.what() << std::endl;
    }
}
void insert_student(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        Student s1(1, "张三", 18, 1);
        Student s2(2, "李四", 19, 1);
        Student s3(3, "王五", 18, 1);
        Student s4(4, "赵六", 20, 2);
        Student s5(5, "钱七", 18, 2);
        Student s6(6, "孙八", 23, 2);
        db.persist(s1);
        db.persist(s2);
        db.persist(s3);
        db.persist(s4);
        db.persist(s5);
        db.persist(s6);
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "插入学生信息出错: " << e.what() << std::endl;
    }
}

void update_student(odb::mysql::database &db, Student &stu)
{
    try
    {
        odb::transaction trans(db.begin());
        db.update(stu);
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "更新学生信息出错: " << e.what() << std::endl;
    }    
}

Student select_student(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        typedef odb::query<Student> query;
        typedef odb::result<Student> result;
        result r(db.query<Student>(query::name == "张三"));
        if(r.size() != 1)
        {
            std::cout << "数据量不对" << std::endl;
            return Student();
        }
        return *r.begin();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return Student();
}
void remove_student(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        typedef odb::query<Student> query;
        db.erase_query<Student>(query::name == "孙八");
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "删除学生信息出错: " << e.what() << std::endl;
    }    
}
void remove_classes(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        typedef odb::query<Classes> query;
        db.erase_query<Student>(query::id == 2);
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "删除班级信息出错: " << e.what() << std::endl;
    }    
}
void classes_student_view(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        typedef odb::query<classes_student> query;
        typedef odb::result<classes_student> result;
        result r(db.query<classes_student>(query::classes::name == "一年级一班"));
        for(auto it = r.begin(); it != r.end(); ++it)
        {
            std::cout << it->id << " " << it->sn << " " << it->name 
                << " " << *it->age << " " << it->classes_name << std::endl;
        }
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "查询指定班级信息出错: " << e.what() << std::endl;
    }    
}

void all_student_view(odb::mysql::database &db)
{
    try
    {
        odb::transaction trans(db.begin());
        typedef odb::query<Student> query;
        typedef odb::result<all_name> result;
        result r(db.query<all_name>(query::id == 1));
        for(auto it = r.begin(); it != r.end(); ++it)
        {
            std::cout << it->name << std::endl;
        }
        trans.commit();
    }
    catch(const std::exception& e)
    {
        std::cout<< "查询所有学生信息出错: " << e.what() << std::endl;
    }    
}

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    // 1. 构造连接池工厂对象
    std::unique_ptr<odb::mysql::connection_pool_factory> cpf(new odb::mysql::connection_pool_factory(FLAGS_max_pool, 0));
    // 2. 构造数据库操作对象
    odb::mysql::database db(FLAGS_user, FLAGS_passwd, FLAGS_db, FLAGS_host, FLAGS_port, "", FLAGS_cset, 0, std::move(cpf));
    // 4. 数据操作
    // insert_classes(db);
    // insert_student(db);
    // auto stu = select_student(db);
    // std::cout << stu.sn() << std::endl;
    // std::cout << stu.name() << std::endl;
    // std::cout << *stu.age() << std::endl;
    // std::cout << stu.classes_id() << std::endl;
    // stu.age(15);
    // update_student(db, stu);
    // remove_student(db);
    // classes_student_view(db);
    all_student_view(db);
    return 0;
}
