#pragma once

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ns_utility
{
    const std::string temp_path = "./temp/";
    class PathUtility
    {
    private:
        
    public:
        PathUtility() {}
        ~PathUtility() {}

        /// @brief 拼接函数
        /// @param file_name 
        /// @param suffix 
        /// @return 路径+文件名+后缀
        static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        /**
         * @brief   构建源文件路径+后缀的完整文件名
         * @details 1234 -> ./temp/1234.cpp
         * @param   param1 参数1的描述
         */
        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }

        /**
         * @brief   构建源文件路径+后缀的完整文件名
         * @param   param1 参数1的描述
         */
        static std::string Exe(const std::string &file_name)
        {
            return AddSuffix(file_name, ".out");
        }

        /**
         * @brief   构建源文件路径+后缀的完整文件名
         * @param   param1 参数1的描述
         */
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
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
    };
    
}