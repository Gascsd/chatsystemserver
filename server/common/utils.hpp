#pragma once
// 实现项目中一些公共的工具类接口
// 1. 生成一个唯一ID的接口
// 2. 文件的读写操作接口

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <atomic>
#include <random>
#include <iomanip>

#include "logger.hpp"

namespace zht_im
{
    // 生成一个由16位随机字符组成的字符串作为唯一ID
    std::string uuid()
    {
        // 1. 生成6个0~255之间的随机数字（1字节转换为16进制字符） -- 生成12位16进制字符
        std::random_device rd;                                   // 设备随机数对象
        std::mt19937 generator(rd());                            // 以设备随机数位种子，实例化伪随机数对象
        std::uniform_int_distribution<int> distribution(0, 255); // 限定数据范围
        std::stringstream ss;
        for (int i = 0; i < 6; ++i)
        {
            if (i % 2 == 0 && i != 0)
                ss << "-";
            ss << std::setw(2) << std::setfill('0') << std::hex << distribution(generator);
        }
        ss << "-";
        // 2. 通过一个静态变量生成一个2字节的编号数字 --生成4位16进制数字字符
        static std::atomic<short> idx(0);
        short tmp = idx.fetch_add(1);
        ss << std::setw(4) << std::setfill('0') << std::hex << tmp;
        return ss.str();
    }

    // 生成4位随机验证码
    std::string vcode()
    {
        std::random_device rd;                                   // 设备随机数对象
        std::mt19937 generator(rd());                            // 以设备随机数位种子，实例化伪随机数对象
        std::uniform_int_distribution<int> distribution(1000, 9999); // 限定数据范围
        return std::to_string(distribution(generator));
    }

    // 读取到body中
    bool readFile(const std::string &filename, std::string &body)
    {
        std::ifstream ifs(filename, std::ios::binary);
        if (ifs.is_open() == false)
        {
            LOG_ERROR("打开文件 {} 失败", filename);
            return false;
        }
        body.clear();
        ifs.seekg(0, std::ios::end);
        size_t flen = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        body.resize(flen);
        ifs.read(&body[0], flen);
        if (ifs.good() == false)
        {
            LOG_ERROR("读取文件 {} 失败", filename);
            ifs.close();
            return false;
        }
        ifs.close();
        return true;
    }
    // 写入到filename中
    bool writeFile(const std::string &filename, const std::string &body)
    {
        std::ofstream ofs(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (ofs.is_open() == false)
        {
            LOG_ERROR("打开文件 {} 失败", filename);
            return false;
        }
        ofs.write(body.c_str(), body.size());
        if (ofs.good() == false)
        {
            LOG_ERROR("写入文件 {} 失败", filename);
            ofs.close();
            return false;
        }
        ofs.close();
        return true;
    }
}