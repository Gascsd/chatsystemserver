#include <string>
#include <memory>  // std::auto_ptr
#include <cstdlib> // std::exit
#include <iostream>
#include <exception>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>

#include "person.hxx"
#include "person-odb.hxx"

int main()
{
    try
    {
        std::shared_ptr<odb::core::database> db(new odb::mysql::database("root", "zht1125x", "TestDB1", "127.0.0.1", 0, 0, "utf8"));

        if (!db)
        {
            return -1;
        }
        ptime p = boost::posix_time::second_clock::local_time();
        Person zhang("小张", 18, p);
        Person wang("小王", 19, p);
        typedef odb::query<Person> query;
        typedef odb::result<Person> result;
        {
            odb::core::transaction t(db->begin());
            size_t zid = db->persist(zhang);
            size_t wid = db->persist(wang);
            t.commit();
        }
        {
            // ptime p = boost::posix_time::time_from_string("2024-05-22 09:09:39");
            // ptime e = boost::posix_time::time_from_string("2024-05-22 09:13:29");
            odb::core::transaction t(db->begin());
            result r(db->query<Person>());
            for (result::iterator i(r.begin()); i != r.end(); ++i)
            {
                std::cout << "Hello, " << i->name() << " ";
                std::cout << i->age() << " " << i->update() << std::endl;
            }
            t.commit();
        }
    }
    catch (std::exception &e)
    {
        std::cout << "数据库连接异常: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
// 如果用到了 boost 库中的接口，需要链接库： -lodb-boost
// c++ -o mysql_test mysql_test.cpp person-odb.cxx -lodb-mysql -lodb -lodb-boost