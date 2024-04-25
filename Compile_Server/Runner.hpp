#pragma once

#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#include "../Common/Log.hpp"
#include "../Common/Utility.hpp"

namespace ns_runner
{
    using namespace ns_log;
    using namespace ns_utility;
    using namespace ns_runner;

    class Runner
    {
    private:

    public:
        Runner() {}
        ~Runner() {}

        /**
         * 返回值 > 0：程序异常，退出时收到了信号，返回值就是对应的信号编号
         * 返回值 = 0：程序正常运行完毕，结果在临时文件中
         * 返回值 < 0：内部错误
         * 
        */
        static int Run(const std::string &file_name)
        {
            /*********************************************
             * 程序运行：
             * 1. 代码跑完，结果正确
             * 2. 代码跑完，结果不正确
             * 3. 代码没跑完，异常了
             * Run需要考虑代码跑完，结果正确与否吗？？不考虑！
             * 结果正确与否：是由我们的测试用例决定的！
             * 我们只考虑：是否正确运行完毕
             *
             * 我们必须知道可执行程序是谁？
             * 一个程序在默认启动的时候
             * 标准输入: 不处理
             * 标准输出: 程序运行完成，输出结果是什么
             * 标准错误: 运行时错误信息
             * *******************************************/

            std::string _execute = PathUtility::Exe(file_name);
            std::string _stdin   = PathUtility::Stdin(file_name);
            std::string _stdout  = PathUtility::Stdout(file_name);
            std::string _stderr  = PathUtility::Stderr(file_name);

            umask(0);
            // 父进程打开的文件，fork之后子进程也是打开的！
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);

            if(_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
            {
                LOG(Error) << "运行时打开标准文件失败" << "\n";
                return -1; // 打开文件失败
            }

            // 创建子进程
            pid_t pid = fork();
            if(pid < 0)
            {
                LOG(Error) << "运行时创建子进程失败" << "\n";
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd); 
                return -2; // 创建子进程失败
            }
            else if(pid == 0)
            {
                // 把0 1 2重定向到文件中去
                dup2(_stdin_fd, stdin->_fileno);
                dup2(_stdout_fd, stdout->_fileno);
                dup2(_stderr_fd, stderr->_fileno);

                // 执行程序
                execl(_execute.c_str()/*我要执行谁*/, _execute.c_str()/*命令行参数*/, nullptr);
                exit(1);
            }
            else
            {// 父进程
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);

                int status = 0;
                waitpid(pid, &status, 0);
                LOG(Info) << "运行完毕, info: " << (status & 0x7F) << "\n"; 
                return status & 0x7F; // 返回错误信号值
            }
        }
    };
}
