#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "../Common/Utility.hpp"
#include "../Common/httplib.h"
#include "../Common/Log.hpp"

// #define MySQL 1;
#include "OJ_Model.hpp"
#include "OJ_View.hpp"

#include <jsoncpp/json/json.h>

// 定义DOCKER_LOAD_BALANCE宏来启用Docker内置的负载均衡
#define DOCKER_LOAD_BALANCE

namespace ns_controller
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_utility;
    using namespace ns_model;
    using namespace ns_view;
    using namespace httplib;
    
    // 提供服务的主机
    class Machine
    {
    public:
        std::string ip;  // 编译服务的ip
        int port;        // 编译服务的端口号
        uint64_t load;   // 负载
        std::mutex *mtx; // mutex是禁止拷贝的，必须使用指针

    public:
        Machine()
            :ip(""), port(0), load(0), mtx(nullptr)
        {}

        ~Machine() {}

    public:
        // 提升主机负载值
        void IncreaseLoad()
        {
            if(mtx)
            {
                mtx->lock();
                load++;
                mtx->unlock();
            }
        }

        // 减少主机负载值
        void DecreaseLoad()
        {
            if(mtx) mtx->lock();
            load--;
            if(mtx) mtx->unlock();
        }

        // 将主机的负载值清零
        void ResetLoad()
        {
            if(mtx) mtx->lock();
            load = 0;
            if(mtx) mtx->unlock();
        }

        // 获取主机负载
        uint64_t GetLoad()
        {
            uint64_t _load;
            if(mtx) mtx->lock();
            _load = load;
            if(mtx) mtx->unlock();

            return _load;
        }
    };

#ifndef DOCKER_LOAD_BALANCE
    const std::string service_machine = "./conf/service_machine.conf";

    // 负载均衡模块
    class LoadBalancingModule
    {
    private:
        std::vector<Machine> machines; // 提供编译服务的"主机"数组，数组下标就是主机的id
        std::vector<int> online;       // 所有在线的主机id
        std::vector<int> offline;      // 所有离线的主机id
        std::mutex mtx;                // 保证LoadBalancingModule的线程安全

    public:
        LoadBalancingModule()
        {
            assert(LoadConf(service_machine));
            LOG(INFO) << "加载所有主机信息： " << service_machine << " 成功" << "\n";
        }
        ~LoadBalancingModule() {}

    public:
        bool LoadConf(const std::string &machine_conf)
        {
            std::ifstream in(machine_conf);
            if(!in.is_open())
            {
                LOG(Fatal) << "加载配置文件：" << machine_conf << " 失败" << "\n";
                return false;
            }
            std::string line;
            while(std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtility::SplitString(line, &tokens, ":");
                if(tokens.size() != 2)
                {

                    LOG(Warning) << "切分：" << line << " 失败，切分出了 " << tokens.size() << "个子串" << "\n";
                    continue;
                }
                Machine m;
                m.ip = tokens[0];
                m.port = stoi(tokens[1]);
                m.mtx = new std::mutex();
                online.push_back(machines.size());
                machines.push_back(m);
            }

            in.close();
            return true;
        }


        /// @brief 选择负载相对最低的主机，并返回其id和指向主机对象的指针
        /// @param id 输出型参数，主机id
        /// @param m 输出型参数，主机对象地址
        /// @return 是否成功
        bool SmartChoice(int *id, Machine **m)
        {
            // 1. 使用选择好的主机（更新该主机的负载）
            // 2. 如果负载过大，让该主机离线
            mtx.lock();

            // 负载均衡算法：
            // 1. 随机数法 + hash
            // 2. 轮询 + 随机（绝对平均）

            int online_num = online.size();
            if(online_num == 0)
            {
                LOG(Fatal) << "所有后端编译主机全部离线，请尽快修复！" << "\n";
                mtx.unlock();
                return false;
            }

            // // 1. 随机数法
            // int rand_id = rand() % online.size();
            // *id = online[rand_id];
            // *m = &machines[online[rand_id]];

            // for debug
            cout << "-------------当前所有的负载--------------\n";
            for(auto& e : machines)
            {
                cout << e.load << " ";
            }
            cout << "\n----------------------------------------\n";

            // 2. 轮询法
            // 遍历找到负载最小的机器
            *id = online[0];
            *m = &machines[online[0]];

            uint64_t min_load = machines[online[0]].GetLoad();
            for(int i = 0; i < online_num; i++)
            {
                uint64_t curr_load = machines[online[i]].GetLoad();
                if(min_load > curr_load)
                {
                    min_load = curr_load;
                    *id = online[i];
                    *m = &machines[online[i]];
                }
            }

            mtx.unlock();
            return true;
        }

        // 当所有主机都离线了，统一上线所有机器
        void LetOnline()
        {
            mtx.lock();
            online.insert(online.end(), offline.begin(), offline.end());
            offline.erase(offline.begin(), offline.end());
            mtx.unlock();

            LOG(INFO) << "所有的主机都上线啦!" << "\n";
        }

        // 让某台主机下线
        void LetOffline(int id)
        {
            mtx.lock();
            for(auto iter = online.begin(); iter != online.end(); iter++)
            {
                if(*iter == id)
                {
                    // 已经找到要离线的主机了
                    online.erase(iter);
                    offline.push_back(id);
                    break;
                }
            }
            mtx.unlock();
        }

        void ShowMachines()
        {
            mtx.lock();
            std::cout << "当前在线主机列表: ";
            for(auto &id : online)
            {
                std::cout << id << " ";
            } 
            std::cout << std::endl;
            std::cout << "当前离线主机列表：";
            for(auto &id : offline)
            {
                std::cout << id << " ";
            }
            std::cout << std::endl;
            mtx.unlock();
        }

    };
#else
    // Docker负载均衡模式下的简化版LoadBalancingModule
    class LoadBalancingModule
    {
    public:
        LoadBalancingModule() 
        {
            LOG(INFO) << "使用Docker容器负载均衡模式，通过单一服务名访问编译服务" << "\n";
        }
        ~LoadBalancingModule() {}

        // 在Docker负载均衡模式下，使用单一服务名
        void GetDockerServiceAddress(std::string *ip, int *port)
        {
            // 使用Docker Compose中定义的服务名称
            *ip = "117.72.15.209";
            *port = 8081;
            
            LOG(INFO) << "使用编译服务: " << *ip << ":" << *port << "\n";
        }

        // 以下方法在Docker模式下为空实现，保持API兼容性
        void LetOnline() {}
        void LetOffline(int id) {}
        void ShowMachines() {}
    };
#endif


    // 核心业务逻辑的控制器
    class Controller
    {
    private:
        Model _model;                       // 提供后台数据
        View _view;                         // 提供html渲染功能
        LoadBalancingModule _load_balancer; // 核心负载均衡器
    public:
        // 让所有主机上线的恢复接口
        void RestoreService()
        {
            _load_balancer.LetOnline();
            _load_balancer.ShowMachines();
        }

        // 获取推理页面
        bool GetInferPage(std::string *html)
        {
            return _view.GetInferPage(html);
        }

        // 获取整个题库的所有题目
        bool AllQuestions(std::string *html)
        {
            // 1. 调用model，获取所有题目信息
            vector<struct Question> all;
            if(_model.GetAllQuestions(&all))
            {
                // 题目按照难度进行排序
                std::sort(all.begin(), all.end(), [](const struct Question &q1, const struct Question &q2){
                    return stoi(q1.number) < stoi(q2.number);
                });

                // 2. 调用view，获取题目列表页面
                _view.AllExpandHtml(all, html);
                return true;
            }

            *html = "获取题目失败！请检查文件路径";
            return false;
        }

        // 获取单个题目信息
        bool Question(const std::string &number, std::string *html)
        {
            // 1. 调用model，获取指定题目信息
            struct Question q;
            if(_model.GetOneQuestion(number, &q))
            {
                // 2. 调用view，获取单个题目详情页面
                _view.OneExpandHtml(q, html);
                return true;
            }

            *html = "指定题目：" + number + " 不存在";
            return false;
        }

        // 判题
        void Judge(const std::string &number, const std::string in_json, std::string *out_json)
        {
            // 1. 根据题目编号，直接拿到测试用例代码
            struct Question q;
            _model.GetOneQuestion(number, &q);

            // 2. 将用户提交的代码和测试用例代码进行合并
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value);

            std::string user_code = in_value["code"].asString();

            // 构造后台编译服务所需的json串
            Json::Value compile_request;
            compile_request["code"] = user_code + q.test_cases;
            compile_request["input"] = in_value["input"].asString(); // 输入用例
            compile_request["cpu_limit"] = q.cpu_limit;
            compile_request["mem_limit"] = q.mem_limit;

            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_request);

#ifndef DOCKER_LOAD_BALANCE
            // 3. 选择负载最低的主机，发起http请求，得到结果
            //    (1) 可能会因为主机挂掉导致请求失败，重新选择主机
            //    (2) 也可能所有主机都已经离线
            while(true)
            {
                int id = 0; // 主机ID
                Machine *m = nullptr;
                if(!_load_balancer.SmartChoice(&id, &m))
                {
                    // 所有主机都离线了
                    Json::Value resp_json;
                    resp_json["status"] = -1;
                    resp_json["reason"] = "所有主机都挂掉了，请等待恢复！";
                    resp_json["stdout"] = "";
                    resp_json["stderr"] = "";
                    
                    *out_json = writer.write(resp_json);
                    LOG(Fatal) << "后端编译主机全部挂掉，请运维同学尽快查看！" << "\n";
                    return;
                }

                Client cli(m->ip, m->port);
                m->IncreaseLoad();
                LOG(INFO) << "选择主机成功，主机id: " << id << "，主机ip：" << m->ip << "端口: " << m->port << "\n";

                if (auto res = cli.Post("/compile_and_run", compile_string, "application/json;charset=utf-8")) 
                {
                    // 正常交互完成
                    if (res->status == 200) 
                    {
                        // 4. 将结果返回，前端网页要显示结果
                        *out_json = res->body;
                        m->DecreaseLoad();
                        return;
                    }
                    m->DecreaseLoad();
                    LOG(Error) << "请求" << m->ip << ":" << m->port << "失败，尝试请求其他主机" << "\n";
                } 
                else 
                {
                    // 请求主机失败，不能够和主机建立正常连接，主机可能已经挂掉了
                    // 降低当前编译主机的优先级
                    LOG(Warning) << "当前请求的主机可能已经挂掉了, id: " << id << " ip: " << m->ip << " port: " << m->port << "\n";
                    m->ResetLoad();
                    _load_balancer.LetOffline(id);
                    _load_balancer.ShowMachines();
                    // 选择新的主机重新尝试
                }
            }
#else
            // Docker负载均衡模式下，直接按照http路径请求
            std::string service_ip;
            int service_port;
            _load_balancer.GetDockerServiceAddress(&service_ip, &service_port);
            
            Client cli(service_ip, service_port);
            LOG(INFO) << "使用Docker服务: " << service_ip << ":" << service_port << "\n";

            if (auto res = cli.Post("/compile_and_run", compile_string, "application/json;charset=utf-8")) 
            {
                // 正常交互完成
                if (res->status == 200) 
                {
                    // 将结果返回，前端网页要显示结果
                    *out_json = res->body;
                    return;
                }
                LOG(Error) << "请求" << service_ip << ":" << service_port << "失败，HTTP状态码: " << res->status << "\n";
            } 
            else 
            {
                // 请求失败，尝试使用下一个编译服务
                LOG(Warning) << "请求Docker服务失败: " << service_ip << ":" << service_port << "\n";
                
                // 构造错误响应
                Json::Value resp_json;
                resp_json["status"] = -1;
                resp_json["reason"] = "编译服务请求失败，请稍后重试！";
                resp_json["stdout"] = "";
                resp_json["stderr"] = "";
                
                *out_json = writer.write(resp_json);
            }
#endif
        }

        Controller() {}
        ~Controller() {}
    };
}