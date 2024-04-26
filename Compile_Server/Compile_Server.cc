#include "Compile_And_Run.hpp"

using namespace ns_compile_and_run;

int main()
{
    // 通过http 让client 给我们上传一个json数据
    
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

    std::string in_json_str;  // 输入的json串
    std::string out_json_str;
    Json::Value in_json_value;
    in_json_value["code"] = R"(
        #include <iostream>
        using namespace std;
        int main()
        {
            while(true);
            cout << "hello world!!!" << endl; 
            return 0;
        }
    )";
    in_json_value["input"] = "";
    in_json_value["cpu_limit"] = 1;
    in_json_value["mem_limit"] = 10240 * 30;

    Json::FastWriter writer;
    in_json_str = writer.write(in_json_value);
    
    CompileAndRun::Start(in_json_str, &out_json_str);

    std::cout << out_json_str << std::endl;
    return 0;
}