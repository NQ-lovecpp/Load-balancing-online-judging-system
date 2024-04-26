#pragma once

#include "Complier.hpp"
#include "Runner.hpp"
#include "../Common/Log.hpp"
#include "../Common/Utility.hpp"
#include <signal.h>
#include <jsoncpp/json/json.h>

namespace ns_compile_and_run
{
    using namespace ns_compiler;
    using namespace ns_runner;
    using namespace ns_log;
    using namespace ns_utility;

    class CompileAndRun
    {
    private:
        /* data */
    public:
        /**
         * code > 0: 进程收到信号，退出
         * code < 0: 整个过程非运行报错（代码为空，编译报错）
         * code = 0: 整个过程全部完成
         * 
         * 可扩展
         */
        static std::string CodeToDetail(int code, const std::string &file_name)
        {
            std::string detail;
            switch (code)
            {
            case -1:
                detail = "提交的代码是空的";
                break;
            case -2:
                detail = "未知错误";
                break;
            case -3:
                detail = "提交的代码编译错误:\n";
                detail += FileUtility::ReadFile(PathUtility::CompilerError(file_name), &detail, true);
                break;
            case 0:
                detail = "编译运行成功";
                break;
            case SIGABRT: // 6
                detail = "内存使用超出范围";
                break;
            case SIGXCPU: // 24
                detail = "CPU使用超时";
                break;
            default:
                detail = "未知：" + std::to_string(code);
                break;
            }

            return detail;
        }

        /***************************************
         * 输入:
         *   code： 用户提交的代码
         *   input: 用户给自己提交的代码对应的输入，不做处理 - 后期扩展
         *   cpu_limit: 时间要求
         *   mem_limit: 空间要求
         *
         * 输出:
         *   必填：
         *   status: 状态码
         *   reason: 请求结果
         *   选填：
         *   stdout: 我的程序运行完的结果
         *   stderr: 我的程序运行完的错误结果
         *
         * 参数：
         * in_json: {"code": "#include...", "input": "","cpu_limit":1, "mem_limit":10240}
         * out_json: {"status":"0", "reason":"","stdout":"","stderr":"",}
         * **************************************/
        static void Start(const std::string &in_json, std::string *out_json)
        {
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value); // 暂时不处理差错

            // 对json串反序列化
            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit = in_value["mem_limit"].asInt();

            Json::Value out_value;
            int status_code = 0;
            int run_result = 0;
            std::string file_name;

            if (code.size() == 0)
            {
                status_code = -1; // 代码为空
                goto END;
            }

            file_name = FileUtility::GetUniqueFileName();

            // 形成源代码文件
            if (!FileUtility::WriteFile(PathUtility::Src(file_name), code))
            {
                status_code = -2; // 未知错误
                goto END;
            }

            if (!Compiler::Compile(file_name))
            {
                // 编译失败
                status_code = -3;
                goto END;
            }

            run_result = Runner::Run(file_name, cpu_limit, mem_limit);
            if (run_result < 0)
            {
                status_code = -2;
            }
            else if (run_result > 0)
            {
                // 程序运行崩溃了
                status_code = run_result;
            }
            else
            {
                // 运行成功
                status_code = 0;
            }
        END:
            out_value["status"] = status_code;
            out_value["reason"] = CodeToDetail(status_code, file_name);
            if (status_code == 0)
            {
                // 整个过程全部成功
                std::string _stdout;
                std::string _stderr;
                FileUtility::ReadFile(PathUtility::Stdout(file_name), &_stdout, true);
                FileUtility::ReadFile(PathUtility::Stderr(file_name), &_stderr, true);
                out_value["stdout"] = _stdout;
                out_value["stderr"] = _stderr;
            }
            Json::StyledWriter writer;
            *out_json = writer.write(out_value);
        }

        CompileAndRun() {}
        ~CompileAndRun() {}
    };
}

// // 处理差错
// out_value["status"] = 1;
// out_value["reason"] = "用户提交的代码是空的";

// // 反序列化 - TODO
// return;

// // 写文件失败
// out_value["status"] = -1; // 未知错误
// out_value["reason"] = "提交的代码发生了未知错误"; // 我们当然知道哪里出错了，只不过不想让用户得知服务器的错误原因
// // 序列化
// return;

// // 编译失败了
// out_value["status"] = -3; // 代码编译的时候发生错误
// out_value["reason"] = FileUtility::ReadFile(PathUtility::CompilerError(file_name));

// // 序列化
// return;

// // 服务器内部错误
// out_value["status"] = -2; // 未知错误
// out_value["reason"] = "提交的代码发生了未知错误";
// // 序列化
// return;

// // 程序运行崩溃
// out_value["status"] = run_result; // 运行报错了
// out_value["reason"] = CodeToDetail(run_result);

// // 序列化