// #define MySQL

#ifdef MySQL

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <unordered_map>

#include "../Common/Utility.hpp"
#include "../Common/Log.hpp"

extern "C" {
#  include <mysql/plugin_auth_common.h>
#  include <mysql/mysql.h>
}


// 根据题目list文件：加载所有的题目信息到内存中
// model：主要用来和数据进行交互，对外提供访问数据的接口

namespace ns_model
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_utility;
    
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

    const std::string question_table_name = "oj_questions";
    const std::string host = "117.72.15.209";
    const std::string user = "chen";
    const std::string passwd = "Cydia4384!";
    const std::string database = "oj";
    const unsigned int port = 3306;

    class Model
    {
    public:

        Model() {}

        ~Model() {}

        bool QueryMySQL(const std::string &sql, vector<Question> *out)
        {
            // 1. 创建mysql句柄
            MYSQL *my = mysql_init(nullptr);

            // 2. 连接
            if(mysql_real_connect(my, host.c_str(), user.c_str(), passwd.c_str(),database.c_str(), port, nullptr, 0) == nullptr)
            {
                LOG(Fatal) << "连接MySQL数据库失败" << "\n";
            }
            LOG(Info) << "连接MySQL数据库成功" << "\n";

            // 设置连接的编码格式：utf-8
            mysql_set_character_set(my, "utf8");

            // 3. 执行sql
            if(0 != mysql_query(my, sql.c_str()))
            {
                LOG(Warning) << "sql语句执行失败：" << sql << "\n";
                return false;
            }

            // 4. 分析结果
            MYSQL_RES *res = mysql_store_result(my);
            int rows = mysql_num_rows(res);   // 获得的行数量
            int cols = mysql_num_fields(res); // 获得的列数量

            Question q;
            for(int i = 0; i < rows; i++)
            {
                // typedef char **MYSQL_ROW;
                MYSQL_ROW row = mysql_fetch_row(res);
                q.number             = row[0];
                q.title              = row[1];
                q.star               = row[2];
                q.description        = row[3];
                q.default_code       = row[4];
                q.test_cases         = row[5];
                q.cpu_limit     = atoi(row[6]);
                q.mem_limit     = atoi(row[7]);

                out->push_back(q);
            }

            // 5. 关闭连接
            free(res);
            mysql_close(my);
            return true;
        }

        /// @brief 将文件中的题目加载到Question对象数组中 - 本质是从后台取出所有题目信息，便于构建题目列表网页
        /// @param out Question对象数组
        /// @return 是否成功
        bool GetAllQuestions(vector<Question> *out)
        {
            const std::string sql = "select * from " + question_table_name;
            return QueryMySQL(sql, out);
        }

        /// @brief 将文件中的题目加载到Question对象数组中 - 本质是从后台取出单个题目信息，便于构建单个题目网页
        /// @param q Question对象
        /// @return 是否成功
        bool GetOneQuestion(const std::string &number, Question *q)
        {
            const std::string sql = "select * from " 
            + question_table_name + " where number=" + number;
            vector<Question> ret;
            if(QueryMySQL(sql, &ret))
            {
                if(ret.size() == 1)
                {
                    *q = ret[0];
                    return true;
                }
            }
            return false;
        }
    };
}

#else

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
    using namespace ns_utility;
    
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

        Model()
        {
            assert(LoadQuestionList(questions_list_path));
        }

        ~Model() {}

        // 加载题目列表 - 其实是一种初始化
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
                 
                FileUtility::ReadFile(current_path + "description.txt", &(_q.description), true);
                FileUtility::ReadFile(current_path + "default_template_code.cpp", &(_q.default_code), true);
                FileUtility::ReadFile(current_path + "test_cases.cpp", &(_q.test_cases), true);

                question_hash[_q.number] = _q;
            }

            LOG(Info) << "题库加载成功!" << "\n";

            in.close();
            return true;
        }

        /// @brief 将文件中的题目加载到Question对象数组中 - 本质是从后台取出所有题目信息，便于构建题目列表网页
        /// @param out Question对象数组
        /// @return 是否成功
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

        /// @brief 将文件中的题目加载到Question对象数组中 - 本质是从后台取出单个题目信息，便于构建单个题目网页
        /// @param out Question对象数组
        /// @return 是否成功
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
    };
}

#endif