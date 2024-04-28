#include <iostream>
#include "../Common/httplib.h" // 阻塞式多线程网络库

using namespace httplib;

int main()
{
    // 用户请求的服务，我们要给它做服务路由
    Server svr;
    
    // 获取所有的题目列表
    //                                          客户端请求        服务端响应内容
    svr.Get("/all_questions", [](const Request &request, Response &response)
    {
        response.set_content("这是所有的题目列表", "text/plain;charset=utf-8");
    });

    // 用户要根据题目编号，获取题目的内容
    // /questions/100 -> 正则匹配
    // R"(...)"，语法特性：raw string，让“\”成为普通字符，不用做转义
    svr.Get(R"(/numbers/(\d+))", [&](const Request &request, Response &response)
    {
        std::string number = request.matches[1]; // 正则匹配到的数字(\d+)
        response.set_content("这是指定的一道题：" + number, "text/plain;charset=utf-8");
    });

    // 用户提交代码，使用我们的判题功能：
    // 1. 每道题的测试用例
    // 2. compile_and_run

    svr.Get(R"(/judge/(\d+))", [](const Request &request, Response & response)
    {
        std::string number = request.matches[1];
        response.set_content("指定题目的判题：" + number, "text/plain; charset=utf-8");
    });

    svr.set_base_dir("./wwwroot");



    svr.listen("0.0.0.0", 8888);
    return 0;
} 