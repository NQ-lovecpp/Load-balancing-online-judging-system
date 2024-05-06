#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <fstream>
#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <boost/algorithm/string.hpp>

namespace ns_utility
{
    const std::string temp_path = "./temp/";
    class PathUtility
    {
    private:
        
    public:
        PathUtility() {}
        ~PathUtility() {}

        // --- 编译时需要有的临时文件 ---

        /**
        * @brief 拼接函数
        * @return 路径+文件名+后缀
        */
        static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        /**
         * @brief   构建源代码文件的完整路径名（带后缀）
         * @details 1234 -> ./temp/1234.cpp
         * @param   param1 参数1的描述
         */
        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }

        /**
         * @brief   构建可执行文件的完整路径名（带后缀）
         * @param   param1 参数1的描述
         */
        static std::string Exe(const std::string &file_name)
        {
            return AddSuffix(file_name, ".out");
        }

        /**
         * @brief   构建编译错误的完整路径名（带后缀）
         * @param   param1 参数1的描述
         */
        static std::string CompilerError(const std::string &file_name)
        {
            return AddSuffix(file_name, ".compile_error");
        }


        // --- 运行时需要的临时文件 ---

        // 构建程序对应标准输入文件的完整路径（带后缀）
        static std::string Stdin(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }

        // 构建程序对应标准输入文件的完整路径（带后缀）
        static std::string Stdout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }

        // 构建程序对应标准输入文件的完整路径（带后缀）
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
    };

    class TimeUtility
    {
    private:

    public:
        static std::string GetTimeStamp()
        {
            time_t currtime = time(nullptr);
            struct tm *curr = localtime(&currtime);
            char time_buffer[128];
            snprintf(time_buffer, sizeof(time_buffer), "%d-%d-%d %d:%d:%d", curr->tm_year + 1900, curr->tm_mon + 1, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec);
            return time_buffer;
        }

        // 获取毫秒级时间戳
        static std::string GetTimeMs()
        {
            struct timeval _time;
            gettimeofday(&_time, nullptr);
            return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000);
        }
    };

    class FileUtility
    {
    private:

    public:
        FileUtility() {}
        ~FileUtility() {}

        /// @brief 判断文件是否存在
        /// @param path_name 完整的文件路径名
        static bool IsFileExists(const std::string &path_name)
        {
            struct stat st;
            // stat() stats the file pointed to by path and fills in buf.
            // stat retval: On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
            if(stat(path_name.c_str(), &st) == 0) 
            {
                // 获取属性成功，说明文件存在
                return true;
            }
            return false;
        }

        static std::string GetUniqueFileName()
        {
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtility::GetTimeMs();

            // 通过毫秒级时间戳+原子性递增的唯一值
            std::string uniq_id = std::to_string(id);
            return ms + "_" + uniq_id;
        }

        static bool WriteFile(const std::string &target_path, const std::string &content)
        {
            std::ofstream out(target_path, std::ios::binary);
            if(!out.is_open())
            {
                return false;
            }

            out.write(content.c_str(), content.size());
            out.close();
            return true;
        }
        
        /// @brief 从指定文件读取文件
        /// @param target_path 文件路径
        /// @param content 文件内容（输出型参数）
        /// @param keep 是否保留文件中的"\n"
        /// @return 
        static bool ReadFile(const std::string &target_path, std::string *content, bool keep = false)
        {
            std::ifstream in(target_path);
            if(!in.is_open())
            {
                std::cerr << "打开: " << target_path << "文件失败!" << std::endl;
                return false;
            }

            std::string line;
            while(std::getline(in, line))
            {
                (*content) += line;
                (*content) += (keep ? "\n" : "");
            }

            return true;
        }
    };

    class StringUtility
    {
    public:

        /**
         * @brief 切分字符串
         * @param str 输入的字符串
         * @param target 分割后的数组（输出型参数）
         * @param sep 期望的分隔符
        */
        static void SplitString(const std::string &str, std::vector<std::string> *target, const std::string &sep)
        {
            // 使用boost库中的split库
            boost::split((*target), str, boost::is_any_of(sep), boost::algorithm::token_compress_on);
        }

        // 用来转义特殊字符，解决如果ctemplate在html中插入“<  >”导致错误
        static std::string EscapeHtml(const std::string& input) 
        {
            std::string output;
            for (char c : input) 
            {
                switch (c) 
                {
                case '<':
                    output += "&lt;";
                    break;
                case '>':
                    output += "&gt;";
                    break;
                default:
                    output += c;
                }
            }
            return output;
        }

        StringUtility() {}
        ~StringUtility() {}

    };

}