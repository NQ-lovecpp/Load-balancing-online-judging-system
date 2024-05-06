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

#define MySQL 1;
#include "OJ_Model.hpp"
#include "OJ_View.hpp"

#include <jsoncpp/json/json.h>

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


            // 2. 轮询法
            // 遍历找到负载最小的机器
            *id = online[0];
            *m = &machines[online[0]];

            uint64_t min_load = machines[online[0]].GetLoad();
            for(int i = 0; i < online_num; i++)
            {
                uint64_t curr_load = machines[online[i]].GetLoad();
                if(min_load <= curr_load)
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


    // 核心业务逻辑的控制器
    class Controller
    {
    private:
        Model _model;                       // 提供后台数据
        View _view;                         // 提供html渲染功能
        LoadBalancingModule _load_balancer; // 核心负载均衡器
    public:

        // 恢复后端编译服务，
        void RestoreService()
        {
            _load_balancer.LetOnline();
        }

        
        /// @brief 根据题目数据构建网页
        /// @param html 输出型参数，html内容的字符串
        /// @return 是否成功
        bool AllQuestions(std::string *html)
        {
            bool ret = true;
            vector<struct Question> all;
            if(_model.GetAllQuestions(&all))
            {
                std::sort(all.begin(), all.end(), [](const struct Question &q1, const struct Question &q2){
                    return stoi(q1.number) < stoi(q2.number);
                });
                // 获取题目成功，将所有题目构建成网页
                _view.AllExpandToHtml(all, html);
            }
            else
            {
                *html = "获取题目失败，形成题目列表失败";
                ret = false;
            }
            return ret;
        }        
        
        bool Question(const std::string &number, std::string *html)
        {
            bool ret = true;
            struct Question q;
            if(_model.GetOneQuestion(number, &q))
            {
                // 获取指定题目信息成功，将所有的题目数据建成网页
                _view.OneExpandToHtml(q, html);
            }
            else
            {
                *html = "指定题目：" + number + " 未能加载！";   
                ret = false;             
            }
            return ret;
        }

        /// @brief 判题的接口，它会调用编译服务集群
        /// @param number 题目编号
        /// @param in_json 请求
        /// @param out_json 结果
        void Judge(const std::string &number, const std::string in_json, std::string *out_json)
        {
            // LOG(Debug) << "传入的json串：\n" << in_json << "\n题号：" << number << "\n";
            
            // 0. 根据题目编号，拿到题目细节
            struct Question q;
            _model.GetOneQuestion(number, &q);

            // 1. in_json进行反序列化，得到题目的id，得到用户提交源代码
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json, in_value);
            std::string customer_code = in_value["code"].asString();

            // 2. 重新拼接 用户代码+测试用例代码 拼成一份新的代码
            Json::Value compile_value;
            compile_value["input"] = in_value["input"].asString();
            compile_value["code"] = customer_code + "\n" + q.test_cases;
            compile_value["cpu_value"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;
            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_value);

            // 3. 选择负载最低的主机
            // 规则：一直选择，直到主机可用，否则就是所有compile_server都挂了
            while(true)
            {
                int id = 0;
                Machine *m = nullptr;
                if(_load_balancer.SmartChoice(&id, &m) == false)
                {
                    break;
                }

                // 4. 发起http请求，得到结果
                Client cli(m->ip, m->port);
                m->IncreaseLoad(); 
                LOG(Info) << "选择主机成功，主机id：" << id
                          << "，地址端口号：" << m->ip << ":" << m->port 
                          << " ，当前负载：" << m->load
                          << "\n";

                _load_balancer.ShowMachines();  // 仅仅是为了用来调试

                if(auto res = cli.Post("/compile_and_run", compile_string, "application/json; charset=utf-8"))
                {
                    // 5. 如果成功，将结果赋值给out_json
                    if(res->status == 200)
                    {
                        *out_json = res->body;
                        m->DecreaseLoad();
                        LOG(Info) << "请求编译和运行服务成功..." << "\n";
                        break;
                    }
                    m->DecreaseLoad();
                }
                else
                {
                    // 请求失败
                    LOG(Error) << "当前请求的主机id：" << id
                               << " 详情：" << m->ip << ":" << m->port << " 可能已经离线..." << "\n";
                    // m->DecreaseLoad();
                    _load_balancer.LetOffline(id);
                    _load_balancer.ShowMachines();  // 仅仅是为了用来调试
                }

            }

        }

        Controller() {}
        ~Controller() {}
    };
}