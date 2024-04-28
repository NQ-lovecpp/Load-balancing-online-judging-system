#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <unordered_map>

#include "../Common/Utility.hpp"
#include "../Common/Log.hpp"

// 根据题目list文件：加载所有的题目信息到内存中
// model：主要用来和数据进行交互，对外提供访问数据的接口

namespace ns_model
{
    using namespace std;
    using namespace ns_log;
    
    struct Question
    {
        std::string number; // 题目编号，唯一
        std::string title;  // 题目的标题
        std::string star;   // 难度：简单 中等 困难
        int cpu_limit;      // 时间限制
        int mem_limit;      // 题目的空间要求
        std::string description;   // 题目的描述
        std::string default_code;  // 在线编辑器的预设代码
        std::string test_cases;    // 题目的测试用例，需要和defalut_code拼接形成目标代码
    };

    const std::string questions_list_path = "./Questions/questions.list";
    const std::string question_folder_path = "./Questions/";

    class Model
    {
    private:
        std::unordered_map<string, Question> question_hash;  // 题号映射到题目细节
    public:

        bool LoadQuestionList(const std::string &q_list_path)
        {
            // 加载配置文件："./Questions/questions.list"
            std::ifstream in(questions_list_path);
            if(!in.is_open())
            {
                LOG(Fatal) << "加载题库失败，请检查是否存在题库文件" << "\n";
                return false;
            }
            std::string line;
            while(getline(in, line))
            {
                vector<string> tokens;
                StringUtility::SplitString(line, &tokens, " ");

                if(tokens.size() != 5)
                {
                    LOG(Warning) << "加载部分题目失败，请检查文件格式" << "\n";
                    continue;
                }
                // "1 两数之和 简单 1 30000"
                Question _q;
                _q.number = tokens[0];
                _q.title = tokens[1];
                _q.star = tokens[2];
                _q.cpu_limit = stoi(tokens[3]);
                _q.mem_limit = stoi(tokens[4]);

                std::string current_path = question_folder_path;
                current_path += _q.number;
                current_path += "/";

                FileUtility::ReadFile(current_path + "description.cpp", &(_q.description), true);
                FileUtility::ReadFile(current_path + "default_template_code.cpp", &(_q.default_code), true);
                FileUtility::ReadFile(current_path + "test_cases.cpp", &(_q.test_cases), true);

                question_hash[_q.number] = _q;

            }

            LOG(Info) << "题库加载成功!" << "\n";

            in.close();
        }

        bool GetAllQuestions(vector<Question> *out)
        {
            if(question_hash.size() == 0)
            {
                LOG(Error) << "用户获取题库失败" << "\n";
                return false;
            }
            for(const auto& kv : question_hash)
            {
                out->push_back(kv.second);
            }
            return true;
        }

        bool GetOneQuestion(const std::string &number, Question *q)
        {
            const auto& iter = question_hash.find(number);
            if(iter == question_hash.end())
            {
                LOG(Error) << "用户获取题目失败，题目编号：" << number << "\n";
                return false;
            }
            (*q) = iter->second;
            return true;
        }

        Model()
        {
            assert(LoadQuestionList(questions_list_path));
        }
        ~Model();
    };
    
}