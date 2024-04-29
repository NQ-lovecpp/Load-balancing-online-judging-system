#pragma once

#include <iostream>
#include <string>

#include "../Common/Utility.hpp"
#include "../Common/httplib.h"
#include "../Common/Log.hpp"
#include "OJ_Model.hpp"
#include "OJ_View.hpp"

namespace ns_controller
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_utility;
    using namespace ns_model;
    using namespace ns_view;
    
    class Controller
    {
    private:
        Model _model;
        View _view;
    public:

        
        /// @brief 根据题目数据构建网页
        /// @param html 输出型参数，html内容的字符串
        /// @return 是否成功
        bool AllQuestions(std::string *html)
        {
            bool ret = true;
            vector<struct Question> all;
            if(_model.GetAllQuestions(&all))
            {
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

        void Judge(const std::string in_json, std::string *out_json)
        {
            
        }

        Controller() {}
        ~Controller() {}
    };
}