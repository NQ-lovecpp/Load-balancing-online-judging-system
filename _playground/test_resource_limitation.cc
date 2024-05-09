#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    std::cout << "get a signal, signo: " << signo << std::endl;
    exit(1);
}

int main()
{
    for(int i = 0; i <= 31; i++)
    {
        signal(i, handler);
    }

    // 限制累计运行时长
    struct rlimit time_limit;
    time_limit.rlim_cur = 1; // 一秒钟
    time_limit.rlim_max = RLIM_INFINITY; 
    setrlimit(RLIMIT_CPU, &time_limit);

    // 死循环
    while(1)
    {
        ;
    }


    // // 限制内存
    // struct rlimit mem_limit;
    // mem_limit.rlim_cur = 1024 * 1024 * 40;
    // mem_limit.rlim_max = RLIM_INFINITY;
    // setrlimit(RLIMIT_AS, &mem_limit);

    // int count = 0;
    // while(1)
    // {
    //     int *p = new int[1024 * 1024];
    //     count++;
    //     std::cout << "size: " << count << " MB" << std::endl;
    //     sleep(1);
    // }

    return 0;
}