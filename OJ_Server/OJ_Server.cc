#include <iostream>
#include "./OJ_Controller.hpp"
#include "../Common/httplib.h" // 阻塞式多线程网络库
#include "../Common/Daemon.hpp"
#include <fcntl.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

using namespace httplib;
using namespace ns_controller;

// #define DEAMON_ON

static Controller *ctrl_ptr = nullptr;

void Recovery(int signo)
{
    ctrl_ptr->RestoreService();
}

int main()
{
    signal(SIGQUIT, Recovery); // ctrl + "\"

    cout << "pid: " << getpid() << endl;

    srand(time(nullptr));

#ifdef DEAMON_ON
    // 守护进程化
    Daemon(false, true);
    std::string path = "/home/chen/OJServerLog/ojserverlog_";
    path += std::to_string(getpid()).c_str();
    path += ".log";
    int log_fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(log_fd, fileno(stdout));
    dup2(log_fd, fileno(stderr));
    // close(log_fd);
#endif


    // 用户请求的服务，我们要给它做服务路由
    Server svr;
    Controller ctrl;
    ctrl_ptr = &ctrl;
    
    // 获取所有的题目列表
    //                                          客户端请求        服务端响应内容
    svr.Get("/all_questions", [&ctrl](const Request &request, Response &response)
    {
        // 返回一张包含有所有题目的html网页
        std::string html;
        ctrl.AllQuestions(&html);

        response.set_content(html, "text/html; charset=utf-8");
    });

    // 用户要根据题目编号，获取题目的内容
    // /questions/100 -> 正则匹配
    // R"(...)"，语法特性：raw string，让“\”成为普通字符，不用做转义
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request &request, Response &response)
    {
        std::string number = request.matches[1]; // 下标1是正则匹配到的数字(\d+)
        std::string html;
        ctrl.Question(number, &html);
        response.set_content(html, "text/html; charset=utf-8");
    });

    // 用户提交代码，使用我们的判题功能： 1. 每道题的测试用例 2. compile_and_run

    // 前端给到的json串：
    // // in_json:
    // {
    //     "code" : "#include...", 
    //     "input" : "程序的标准输入的内容"
    // };

    // 我们服务器要返回给前端网页的json串：
    // out_json:
    // {
    //     "status" : "0", 
    //     "reason" : "", 
    //     "stdout" : "",
    //     "stderr" : ""
    // };
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &request, Response &response)
    {
        std::string number = request.matches[1];
        std::string result_json_str;
        ctrl.Judge(number, request.body, &result_json_str);
        response.set_content(result_json_str, "application/json; charset=utf-8");

        // response.set_content("指定题目的判题：" + number, "text/plain; charset=utf-8");
    });


    LOG(INFO) << "listening on 13000" << std::endl;

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8888);
    return 0;
} 