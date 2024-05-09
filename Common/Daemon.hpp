#pragma once

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const char* root = "/";
const char* dev_null = "/dev/null";

void Daemon(bool is_chdir, bool is_close_streams)
{
    // 1. 忽略可能程序异常退出的信后
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    // 2. 让自己不要成为组长
    if(fork() > 0)
    {
        // 父进程退出
        exit(0);
    }
    // 3. 子进程设置自己成为一个新的会话，后面的代码其实是子进程在走
    setsid();

    // 4. 每一个进程都有自己的CWD（当前工作目录），把它改成根目录
    if(is_chdir)
    {
        chdir(root);
    }

    // 5. 已经成为守护进程了，不需要和用户输入输出错误流进行关联了

    if(is_close_streams)
    {
        close(stdin->_fileno);
        close(stdout->_fileno);
        close(stderr->_fileno);
    }
    else
    {
        int fd = open(dev_null, O_RDWR);
        if(fd > 0)
        {
            dup2(fd, stdin->_fileno);
            dup2(fd, stdout->_fileno);
            dup2(fd, stdout->_fileno);
            close(fd);
        }
    }
}
