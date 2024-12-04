#include <iostream>
#include <json/json.h>
#include <jsoncpp/json/json.h>
#include <memory>
#include <sstream>

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


int main()
{
    char name[] = "张三";
    int age = 18;
    float score[] = {78.5, 80, 99};

    Json::Value root;
    root["name"] = name;
    root["age"] = age;
    root["score"].append(score[0]);
    root["score"].append(score[1]);
    root["score"].append(score[2]);

    std::string jsonstr;
    bool ret = Serialize(root, jsonstr);
    if(ret == false)
    {
        std::cout << "序列化错误" << std::endl;
        return 0;
    }
    std::cout << "序列化结果为:" << jsonstr << std::endl;
    
    Json::Value val;
    UnSerialize(jsonstr, val);
    std::cout << "name=" << val["name"].asString() << std::endl;
    std::cout << "age=" << val["age"].asInt() << std::endl;
    int n = val["score"].size();
    std::cout << "score ";
    for(int i = 0; i < n; ++i)
        std::cout << val["score"][i].asFloat() << " ";
    std::cout << std::endl;
    return 0;
}


void func()
{
	int* p = nullptr;
	cout << "before error" << endl;
	if (p == nullptr) { throw runtime_error("出现空指针解引用的情况"); }
	// *p = 10; // 报错
	cout << "after error" << endl;
}

int main()
{
	try
	{
		func();
	}
	catch (const runtime_error& e)
	{
		cout << "出现异常" << e.what() << endl;
	}
	catch (...)
	{
		cout << "出现未知异常" << endl;
	}
	return 0;
}