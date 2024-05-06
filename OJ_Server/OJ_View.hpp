#pragma once

#include <iostream>
#include <string>
#include <ctemplate/template.h>

#include "OJ_Model.hpp"

        // struct Question
        // {
        //     std::string number; // 题目编号，唯一
        //     std::string title;  // 题目的标题
        //     std::string star;   // 难度：简单 中等 困难
        //     int cpu_limit;      // 时间限制
        //     int mem_limit;      // 题目的空间要求
        //     std::string description;   // 题目的描述
        //     std::string default_code;  // 在线编辑器的预设代码
        //     std::string test_cases;    // 题目的测试用例，需要和defalut_code拼接形成目标代码
        // };

namespace ns_view
{
    using namespace ns_model;

    const std::string template_path = "./template_html/";
    class View
    {
    private:
        
    public:

        /// @brief 将所有题目信息的数组，加载到html字符串中
        /// @param questions 输入一个包含所有题目信息的数组
        /// @param html 输出一个题目列表网页的html字符串
        void AllExpandToHtml(const std::vector<struct Question> &questions, std::string *html)
        {
            // 题目的编号 题目的标题 题目的难度
            // 1. 形成路径
            std::string src_html = template_path + "all_questions.html";

            // 2. 形成字典
            ctemplate::TemplateDictionary root("all_questions");
            for(const auto &q : questions)
            {
                ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("question_list");
                sub->SetValue("number", q.number);
                sub->SetValue("title", q.title);
                sub->SetValue("star", q.star);
            }

            // 3. 获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            
            // 4. 开始渲染
            tpl->Expand(html, &root); 
        }

        /// @brief 将一个题目生成对应题目网页的html字符串
        /// @param q 输入一个题目对象
        /// @param html 输出一个题目页面的html字符串
        void OneExpandToHtml(const struct Question &q, std::string *html)
        {
            // 1. 形成路径
            std::string src_html = template_path + "one_question.html";

            // 2. 形成数据字典
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", q.number);
            root.SetValue("title", q.title);
            root.SetValue("star", q.star);
            root.SetValue("description", q.description);
            // 改正编辑器内不能正确渲染“<  >”的bug
            std::string modefied_defautlt_code = StringUtility::EscapeHtml(q.default_code);
            root.SetValue("pre_code", modefied_defautlt_code);

            // 3. 获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            
            // 4. 开始渲染
            tpl->Expand(html, &root); 
        }

        View() {}
        ~View() {}
    };
}