#include "Compile_And_Run.hpp"
#include "../Common/httplib.h"
#include "../Common/Daemon.hpp"

#define DEAMON_ON

using namespace ns_compile_and_run;
using namespace httplib;

// 通过http post 让client 给我们上传一个json数据

// in_json:
// {
//     "code" : "#include...", 
//     "input" : "", 
//     "cpu_limit" : 1, 
//     "mem_limit" : 10240
// };
// out_json:
// {
//     "status" : "0", 
//     "reason" : "", 
//     "stdout" : "",
//      "stderr" : ""
// };


void Usage(const char* proc)
{
    std::cerr << "Usage: " << "\n\t" << proc << " server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }


#ifdef DEAMON_ON
    // 守护进程化
    Daemon(false, true);
    std::string path = "/home/chen/OJServerLog/compileserverlog_";
    path += std::to_string(getpid()).c_str();
    path += ".log";
    int log_fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(log_fd, fileno(stdout));
    dup2(log_fd, fileno(stderr));
    // close(log_fd);
#endif


    // using Handler = std::function<void(const Request &, Response &)>;
    Server svr;

    svr.Get("/hello", [](const Request &req, Response &resp){
        // 用来进行基本测试
        resp.set_content("hello httplib, 你好httplib!", "text/plain;charset=utf-8");
    });

    svr.Post("/compile_and_run", [](const Request &req, Response &resp){
        // 用户请求的服务正文是一个json串
        std::string in_json_str = req.body;
        std::string out_json_str;
        if(!in_json_str.empty())
        {
            CompileAndRun::Start(in_json_str, &out_json_str);
            resp.set_content(out_json_str, "application/json;charset=utf-8");
        }
    });

    std::cout << "listen..." << std::endl;
    // svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", atoi(argv[1]));

    return 0;
}