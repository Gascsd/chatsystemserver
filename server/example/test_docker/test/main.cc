#include <iostream>
#include <string>
#include <gflags/gflags.h>


DEFINE_string(text, "this is a text", "这是一段测试文字");

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "Hello world" << std::endl;
    std::cout << FLAGS_text << std::endl;

    return 0;
}