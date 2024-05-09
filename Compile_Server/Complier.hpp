#pragma once

#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>   
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../Common/Utility.hpp"
#include "../Common/Log.hpp"

// 只负责进行代码的编译


namespace ns_compiler
{
    // 引入路径拼接功能
    using namespace ns_utility;
    using namespace ns_log;

    /**
     * @brief   这是一个简要描述函数或类功能的概述
     * @details 这里可以提供更详细的描述，包括参数、返回值、用法等
     * 
     * @param   param1 参数1的描述
     * @param   param2 参数2的描述
     * @return  返回值的描述
     */
    class Compiler
    {
    private:

    public:
        Compiler() {}
        ~Compiler() {}

        /**
         * @brief   
         * @details 传入的file_name没有后缀和路径：
         * test -> ./temp/test.cpp
         * test -> ./temp/test.out
         * test -> ./temp/test.stderr
         * 
         * @param   file_name 编译的文件名
         * @return  编译成功返回true，否则返回false
         */
        static bool Compile(const std::string &file_name)
        {
            pid_t pid = fork();
            if(pid < 0)
            {
                // 创建子进程失败
                LOG(Error) << "fork错误，创建子进程失败" << "\n";
                return false;
            }
            else if(pid == 0)
            {
                // 子进程：调用编译器，完成对代码的编译

                // 没有就创建+只写
                umask(0);
                int _stderr = open(PathUtility::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if(_stderr < 0)
                {
                    LOG(Warning) << "没有形成stderr文件" << "\n";
                    exit(1);
                }

                dup2(_stderr, stderr->_fileno); // 重定向到_stderr文件

                // exec系统调用不会影响进程打开的文件（不影响文件描述符表）
                // g++ target -o target src -std=c++11
                execlp("g++", "g++", "-o", PathUtility::Exe(file_name).c_str(), \
                 PathUtility::Src(file_name).c_str(), "-std=c++11", "-D", "COMPILER_ONLINE", nullptr); // 这里的nullptr表示结尾

                LOG(Error) << "g++未启动，可能是参数错误" << "\n";;
                exit(1); // 程序替换错误直接终止子进程
            }
            else
            {
                // 父进程
                // 阻塞等待子进程   
                waitpid(pid, nullptr, 0);

                // 编译是否成功就看：是否形成了同名可执行程序
                if(FileUtility::IsFileExists(PathUtility::Exe(file_name)))
                {
                    LOG(Info) << PathUtility::Src(file_name) << "编译成功" << "\n";
                    return true;
                }
                LOG(Error) << "编译失败，没有形成可执行文件" << "\n";
                return false;
            }
        }
    };
    
}