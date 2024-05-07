#pragma once

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../Common/Utility.hpp" 

namespace ns_log
{
#ifndef LOG_VERSION_2
    using namespace ns_utility;

    // 日志等级
    enum LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Fatal
    };

    enum
    {
        Screen = 10,
        OneFile,
        ClassFile
    };

    const int defaultstyle = Screen;
    const std::string default_filename = "log.";
    const std::string logdir = "log";

    std::string LevelToString(int level)
    {
        switch (level)
        {
        case Debug:
            return "Debug";
        case Info:
            return "Info";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";

        default:
            return "Unknown";
        }
    }

    inline std::ostream& Log(const std::string& level, const std::string& file_name, int line)
    {
        // 添加日志等级
        std::string message = "[";
        message += level;
        message += "]";

        // 添加报错文件名称
        message += "[";
        message += file_name;
        message += "]";

        // 添加报错行
        message += "[";
        message += std::to_string(line);
        message += "]";

        // 日志时间戳
        message += "[";
        message += TimeUtility::GetTimeStamp();
        message += "]";

        // cout 本质 内部是包含缓冲区的
        std::cout << message; //不要endl进行刷新

        return std::cout;
    }

    #define LOG(level) Log(#level, __FILE__, __LINE__)

#else
    class _Log
    {
    public:
        _Log()
            : style(defaultstyle), filename(default_filename)
        {
            mkdir(logdir.c_str(), 0775);
        }

        ~_Log() = default;

        void Enable(int sty)
        {
            style = sty;
        }

        std::string TimeStampExLocalTime()
        {
            time_t currtime = time(nullptr);
            struct tm *curr = localtime(&currtime);
            char time_buffer[128];
            snprintf(time_buffer, sizeof(time_buffer), "%d-%d-%d %d:%d:%d", curr->tm_year + 1900, curr->tm_mon + 1, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec);
            return time_buffer;
        }

        void WriteLogToOneFile(const std::string &logname, const std::string &message)
        {
            std::ofstream out(logname, std::ios::app);
            if (!out.is_open())
            {
                return;
            }
            out.write(message.c_str(), message.size());
            out.close();
        }

        void WriteLogToClassFile(const std::string &levelstr, const std::string &message)
        {
            std::string logname = logdir;
            logname += "/";
            logname += filename;
            logname += levelstr;
            WriteLogToOneFile(logname, message);
        }

        void WriteLog(const std::string &levelstr, const std::string &message)
        {
            switch (style)
            {
            case Screen:
                std::cout << message;
                break;
            case OneFile:
                WriteLogToClassFile("all", message);
                break;
            case ClassFile:
                WriteLogToClassFile(levelstr, message);
                break;
            default:
                break;
            }
        }

        // LogMessage(LogLevel, "%s, %d, %f,...", ...); // C风格日志接口
        void LogMessage(LogLevel level, const char *format, ...)
        {
            char right_buffer[1024];

            va_list args;           // char*
            va_start(args, format); // 让args指向可变参数部分
            vsnprintf(right_buffer, sizeof(right_buffer), format, args);
            va_end(args); // args = nullptr

            char left_buffer[1024];
            std::string levelstr = LevelToString(level);
            std::string currtime = TimeStampExLocalTime();
            std::string idstr = std::to_string(getpid());

            snprintf(left_buffer, sizeof(left_buffer), "[%-7s][%s][%s] ",
                     levelstr.c_str(), currtime.c_str(), idstr.c_str());

            // printf("%s%s\n", left_buffer, right_buffer);

            std::string loginfo = left_buffer;
            loginfo += right_buffer;
            loginfo += "\n";

            WriteLog(levelstr, loginfo);
        }

    private:
        int style;
        std::string filename;
    };

    _Log lg;

#endif

}
