# 一、演示项目
**项目源码链接：** https://github.com/NQ-lovecpp/Load-balancing-online-judging-system

**前端网页：**
![](./ReadMePics/frontpage.gif)

**后端OJ服务器和负载均衡的编译服务集群：**
![](./ReadMePics/负载均衡.gif)



# 二、所用技术与开发环境
## 所用技术
1. C++ STL 标准库
2. Boost 准标准库(我们使用了其中的字符串切割方法)
3. cpp-httplib 第三方开源网络库
4. ctemplate 第三方(Google)开源前端网页渲染库
5. jsoncpp 第三方开源序列化、反序列化库
6. 负载均衡设计
7. 多进程、多线程
8. MySQL C connect
9. Ace前端在线编辑器
10. html/css/js/jquery/ajax

> [!Quote] 说明一下项目中使用到的多进程和多线程：
>
>**1. 多线程体现在我们使用的第三方库中使用了线程池接受并发请求：**
>
>- `cpp-httplib`是一个轻量级且高效的 C++ HTTP/HTTPS 客户端和服务器库，由 Hideaki Sone（yhirose）开发， 它提供一种简单易用的方式，在 C++ 应用程序中实现 HTTP 和 HTTPS 功能。
>
>- `cpp-httplib`采用了**多线程处理客户端的并发请求**，从而提高服务器性能。
> 
>**2. 多进程体现在我们的后端编译服务采用创建子进程的方式编译代码，从而不影响主执行**


## 开发环境
- CentOS7云服务器 -> 部署服务
- vscode -> 代码编辑和图形化的Git
- XShell -> 常用的终端
- DataGrip -> 连接数据库

# 三、项目宏观结构
我们的项目核心是三个模块：
1. Common : 公用代码和工具
2. Compile_Server : 用户代码的编译与运行模块
3. OJ_Server : 获取题目列表，查看题目编写题目界面，负载均衡，以及其他功能

## I. 风格：仿leetcode

目前只实现类似 `leetcode` 的**题目列表+在线编程功能**

## II. 结构：Browser-Server模式

>Browser-Server（B/S）模式，也被称为客户端-服务器（Client-Server）模式，是一种软件架构模式，常用于Web应用程序的开发和部署。在这种模式下，客户端通常是浏览器，服务器是后端应用程序或数据库服务器。

![](/ReadMePics/BS模式.png)

## III. 编写思路
1. 先编写compile_server
2. oj_server
3. version1 - 基于`文件`题库的在线OJ
4. 前端的页面设计
5. version2 - 基于`MySQL`题库的在线OJ
 

# 四、关于Git分支管理

我们设计了一个 git 分支管理结构和分支命名风格的建议。这套结构以明确的分支策略、基于不同阶段的版本管理和团队协作的流程为基础。**我们同时使用`VSCode`中的集成的Git和命令行下的Git共同完成版本控制**。

## Git 分支结构
1. **主分支（main）**
   - 主分支是项目的最终版本，稳定且可发布。
   - 只有经过充分测试和验证的代码才能合并到该分支。
   - 任何发布版本或正式生产环境使用的代码都来自于此。

2. **开发分支（develop）**
   - 用于当前开发阶段的主分支。所有新功能和代码改动最初在这里进行。
   - 测试团队可以在此分支上对代码进行测试和验证。

3. **功能分支（feature/xxx）**
   - 对于每个新功能或组件，创建一个功能分支，遵循 "feature/" 的命名规则。
   - 例如，`feature/compile_server`，`feature/oj_server` 等。
   - 在功能开发完成并经过初步测试后，将其合并到开发分支。

4. **修复分支（fix/xxx）**
   - 当有错误或漏洞需要修复时，创建一个修复分支，遵循 "fix/" 的命名规则。
   - 例如，`fix/bug123`，`fix/compile_error` 等。
   - 完成修复后，合并到开发分支或必要时直接合并到主分支。

5. **发布分支（release/xxx）**
   - 在准备发布新版本时，从开发分支创建一个发布分支，遵循 "release/" 的命名规则。
   - 例如，`release/v1.0`，`release/v2.0` 等。
   - 进行最终测试和bug修复后，合并到主分支并标记发布版本。
   
6. **热修复分支（hotfix/xxx）**
   - 当需要紧急修复生产环境的问题时，创建一个热修复分支，遵循 "hotfix/" 的命名规则。
   - 例如，`hotfix/critical_error`。
   - 完成热修复后，将其合并到主分支，同时也合并到开发分支。

## Git 分支命名风格
- 使用小写字母和连字符分隔单词，确保分支命名简洁易懂。
- 分支类型和特定名称之间用斜杠（/）分隔。
- 避免使用特殊字符和空格。

### 示例
- `main`
- `develop`
- `feature/compile_server`
- `feature/oj_server`
- `fix/compile_bug`
- `release/v1.0`
- `hotfix/critical_issue`

## Commit Message规范
在 Git 中，良好的提交信息（commit message）对于团队协作、代码维护和历史追踪至关重要。规范的提交信息可以使代码库更易于理解，并且更容易查找和定位问题。下面是一些编写提交信息的规范和最佳实践：

### 提交信息的基本结构
1. **简洁的标题行（Summary Line）**
   - 标题行应简洁明了，通常不超过50个字符。
   - 该行用于快速概述此次提交的目的。

2. **空行**
   - 标题行之后应有一个空行，以便于显示和解析。

3. **详细描述（Optional Description）**
   - 如果需要详细描述，可以在空行之后进行详细说明。
   - 此部分可以包含对代码改动的详细解释、背景、原因、相关问题的编号、用例、已知问题等。

### 提交信息实践
1. **简明扼要**
   - 提交信息应直接、简明，避免不必要的冗长。

2. **使用动词开头**
   - 标题行通常使用动词进行动词命令式语气，例如："Fix"、"Add"、"Update"、"Remove" 等。

3. **描述具体的变化**
   - 确保标题行能够反映代码改动的本质，例如："Fix compile error"、"Add user authentication"、"Update readme file"。

4. **关联相关信息**
   - 如果有相关的问题、任务、或用户故事，应该在提交信息中提到，例如："Related to issue #123"。

5. **避免不必要的信息**
   - 避免使用模糊词汇或无关的信息，例如 "Fix stuff"、"Update code"。

### 示例提交信息
```txt
Add user authentication

Implemented user authentication using JWT.
Created a new endpoint for user login.
Updated the database schema to store user tokens.
```

```txt
Fix compile error in compile_server

Resolved a compile-time error caused by a missing include directive.
Tested compile_server and ensured all tests passed.
```

```txt
Update README file with project details

Added project structure and technology stack to README.
Updated instructions for setting up the development environment.
```

### 使用工具验证提交信息
有些开发团队使用 Git 钩子或自动化工具来强制执行提交信息规范。在这种情况下，你可能需要遵循更严格的标准。

## VSCode中的Git可视化插件 - Git Graph

Git Graph 是 VSCode 的一款非常好用的插件，它用于可视化 Git 仓库的历史记录和操作。它提供了一个直观的界面来查看分支、提交和其他相关信息，帮助开发者更好地管理和了解他们的 Git 仓库的历史提交：

![](./ReadMePics/gitgraph.png)




# 五、Compiler_Server - 编译服务设计
>`Compiler_Server` 提供的服务：编译并运行代码，得到格式化的相关的结果。
>
>很明显，是存在两个步骤的：先编译，后运行。我们先分别编写编译模块和运行模块，最后再将它们拼接起来。

## 5.1 编译功能 (Compiler.hpp)
一个C/C++程序源文件，需要经过gcc/g++工具进行预处理、编译、汇编、链接最终形成可执行文件。

平时，我们将一个源文件编译形成可执行文件只需要在bash下键入命令`gcc/g++ -o target main.cpp` 一步到位即可。但是现在我们需要在程序中实现这个功能，就需要用到操作系统的系统调用接口：`exec*` 系列系统调用进行**进程程序替换**。

>我们考虑以下问题：
>1. 待编译的源文件从哪里来？Runner模块会传递文件名，根据文件名可以在temp文件夹中找到用户提交的代码，用户代码由Compile_And_Run模块从传入的json串中获得。
>2. 其次编译可能出现报错，失败信息如何得知？gcc/g++程序的报错默认是输出到stderr文件中的，那么我们可以将stderr文件重定向到指定目录（temp）下的 `.compile_error` 文件中。
>3. 如何知道编译是否成功？看是否形成可执行文件 `.out`。

![编译服务](/ReadMePics/编译服务.png)

### 拼接成目标路径的方法 (Utility.hpp)
因为我们希望用户代码及其编译结果和运行结果都存放在temp文件夹下，所以我们可以设计一下拼接成文件完整路径的方法，定义在`PathUtility类`中，它所提供的方法都设置为public的静态方法，供外部直接调用。
```cpp
const std::string temp_path = "./temp/";
class PathUtility
{
private:
   
public:
   PathUtility() {}
   ~PathUtility() {}

   // --- 编译时需要有的临时文件 ---
   static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
   {
      std::string path_name = temp_path;
      path_name += file_name;
      path_name += suffix;
      return path_name;
   }

   // 构建源代码文件的完整路径名（带后缀）
   static std::string Src(const std::string &file_name)
   {
      return AddSuffix(file_name, ".cpp");
   }

   // 构建可执行文件的完整路径名（带后缀）
   static std::string Exe(const std::string &file_name)
   {
      return AddSuffix(file_name, ".out");
   }

   // 构建编译错误的完整路径名（带后缀）
   static std::string CompilerError(const std::string &file_name)
   {
      return AddSuffix(file_name, ".compile_error");
   }


   // --- 运行时需要的临时文件 ---

   // 构建程序对应标准输入文件的完整路径（带后缀）
   static std::string Stdin(const std::string &file_name)
   {
      return AddSuffix(file_name, ".stdin");
   }

   // 构建程序对应标准输入文件的完整路径（带后缀）
   static std::string Stdout(const std::string &file_name)
   {
      return AddSuffix(file_name, ".stdout");
   }

   // 构建程序对应标准输入文件的完整路径（带后缀）
   static std::string Stderr(const std::string &file_name)
   {
      return AddSuffix(file_name, ".stderr");
   }
};
```

### 获取秒级时间戳并转换为"时:分:秒" (Utility.hpp)
这个静态方法是为了下面的日志功能做铺垫，因为我们希望打印出来的日志能够显示时间。
```cpp
static std::string GetTimeStamp()
{
   time_t currtime = time(nullptr);
   struct tm *curr = localtime(&currtime);
   char time_buffer[128];
   snprintf(time_buffer, sizeof(time_buffer), "%d-%d-%d %d:%d:%d", curr->tm_year + 1900, curr->tm_mon + 1, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec);
   return time_buffer;
}
```

>说明一下：
>- localtime是c语言提供的可以将时间转化为"时分秒"的函数，定义在`<ctime>`中
>- struct tm的定义如下：
>   ```cpp
>   struct tm {
>      int tm_sec;         /* seconds */
>      int tm_min;         /* minutes */
>      int tm_hour;        /* hours */
>      int tm_mday;        /* day of the month */
>      int tm_mon;         /* month */
>      int tm_year;        /* year */
>      int tm_wday;        /* day of the week */
>      int tm_yday;        /* day in the year */
>      int tm_isdst;       /* daylight saving time */
>   };
>   ```
>
>- 注意`tm_year`是从1900年开始计算的，记得要加上1900才能打印出当前年份

### 开放式日志接口 (Log.hpp)
实现一个简单版本的日志接口，可以像cout一样使用它：
```cpp
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
```

### 编译服务主体 (Compiler.hpp)

```cpp
static bool Compile(const std::string &file_name)
{
   pid_t pid = fork();
   if(pid < 0)
   {
         // 创建子进程失败
         LOG(Error) << "fork错误，创建子进程失败" << "\n";
         return false;
   }
   else if(pid == 0)
   {
         // 子进程：调用编译器，完成对代码的编译

         // 没有就创建+只写
         umask(0);
         int _stderr = open(PathUtility::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
         if(_stderr < 0)
         {
            LOG(Warning) << "没有形成stderr文件" << "\n";
            exit(1);
         }

         dup2(_stderr, stderr->_fileno); // 重定向到_stderr文件

         // exec系统调用不会影响进程已经打开的文件（不影响文件描述符表）
         // g++ target -o target src -std=c++11
         execlp("g++", "g++", "-o", PathUtility::Exe(file_name).c_str(), \
         PathUtility::Src(file_name).c_str(), "-std=c++11", "-D", "COMPILER_ONLINE", nullptr); // 这里的nullptr表示结尾

         LOG(Error) << "g++未启动，可能是参数错误" << "\n";;
         exit(1); // 程序替换错误直接终止子进程
   }
   else
   {
         // 父进程
         // 阻塞等待子进程   
         waitpid(pid, nullptr, 0);

         // 编译是否成功就看：是否形成了同名可执行程序
         if(FileUtility::IsFileExists(PathUtility::Exe(file_name)))
         {
            LOG(Info) << PathUtility::Src(file_name) << "编译成功" << "\n";
            return true;
         }
         LOG(Error) << "编译失败，没有形成可执行文件" << "\n";
         return false;
   }
}
```

>需要注意的是：调用dup2系统调用的时机在调用execlp之前，也就是说我们在程序替换前完成了重定向，而重定向不会影响进程已经打开的文件，也就是不影响子进程现存的文件描述符表，那么g++编译的报错信息就合乎预期地打印到了我们重定向的文件中了。

## 5.2 运行功能 (Runner.hpp)
运行功能的本质是创建子进程，让子进程去执行可执行文件：
- 在创建子进程时，使用了 `fork()` 来创建子进程。如果 `fork()` 返回的值大于零，表示这是父进程。
- 父进程在运行完子进程后，通过 `waitpid()` 等待子进程结束，并获取其退出状态。
  - `waitpid()` 的第二个参数 `status` 会保存子进程的退出状态，退出状态在正常和被信号所杀的情况下有所不同：![1](./ReadMePics/正常终止vs被信号所杀.png)
  - `status & 0x7F` 这个操作可以获得子进程的退出信号，如果这个值不为零，说明子进程异常退出。
  - 如果子进程正常结束，其退出码通常可以通过 `WIFEXITED(status)` 检查，然后通过 `WEXITSTATUS(status)` 获取实际的退出码。我们主要是判断信号，所以使用 `status & 0x7F` 来判断子进程是否因信号异常退出，因为十六进制`7F`按位与上状态码，刚好是退出信号：![](./ReadMePics/0x7F.png)


```cpp
class Runner
{
public:
   Runner() {}
   ~Runner() {}

   // 设置执行用户代码的子进程占用空间和cpu时间的方法
   static void SetProcLimit(int _cpu, int _memory)
   {
      // 限制累计运行时长
      struct rlimit time_rlimit;
      time_rlimit.rlim_cur = _cpu;
      time_rlimit.rlim_max = RLIM_INFINITY; 
      setrlimit(RLIMIT_CPU, &time_rlimit);

      // 限制内存
      struct rlimit mem_rlimit;
      mem_rlimit.rlim_cur = _memory * 1024; // 转化为KB
      mem_rlimit.rlim_max = RLIM_INFINITY;
      setrlimit(RLIMIT_AS, &mem_rlimit);
   }

   static int Run(const std::string &file_name, int cpu_limit, int mem_limit)
   {
      std::string _execute = PathUtility::Exe(file_name);
      std::string _stdin   = PathUtility::Stdin(file_name);
      std::string _stdout  = PathUtility::Stdout(file_name);
      std::string _stderr  = PathUtility::Stderr(file_name);

      umask(0);
      // 父进程打开的文件，fork之后子进程也是打开的！
      int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
      int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
      int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);

      if(_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
      {
         LOG(Error) << "运行时打开标准文件失败" << "\n";
         return -1; // 打开文件失败
      }

      // 创建子进程
      pid_t pid = fork();
      if(pid < 0)
      {
         LOG(Error) << "运行时创建子进程失败" << "\n";
         close(_stdin_fd);
         close(_stdout_fd);
         close(_stderr_fd); 
         return -2; // 创建子进程失败
      }
      else if(pid == 0)
      {
         // 把0 1 2重定向到文件中去
         dup2(_stdin_fd, stdin->_fileno);
         dup2(_stdout_fd, stdout->_fileno);
         dup2(_stderr_fd, stderr->_fileno);

         SetProcLimit(cpu_limit, mem_limit);

         // 执行程序
         execl(_execute.c_str()/*我要执行谁*/, _execute.c_str()/*命令行参数*/, nullptr);
         exit(1);
      }
      else
      {// 父进程
         close(_stdin_fd);
         close(_stdout_fd);
         close(_stderr_fd);

         int status = 0;
         waitpid(pid, &status, 0);
         LOG(Info) << "运行完毕, info: " << (status & 0x7F) << "\n"; 
         return status & 0x7F; // 返回错误信号值
      }
   }
};
```
参数说明：
   - `cpu_limit`: 运行时cpu时间限制
   - `mem_limit`: 运行时内存限制
   
返回值说明：
   - **返回值 > 0**：当返回值大于零时，说明子进程因接收到信号而退出。返回的值是对应的信号编号。这通常表明程序在运行期间出现了异常，例如内存访问违规、除零错误、超时等。
   - **返回值 = 0**：当返回值等于零时，说明子进程正常运行完毕。stdout文件存放在在temp目录下中。这意味着用户的代码正确执行，未出现异常。==**需要注意的是，这不代表代码的输出是否正确并满足测试用例要求，只表示代码运行完毕，没有异常**==。
   * **返回值 < 0**：服务器内部错误，我们不希望暴露给用户错误的具体信息，但是我们服务器会打印error日志，此时返回值的具体含义是：
      - -1: 表示在运行时打开标准输入、输出、错误文件失败。
      - -2: 表示在运行时创建子进程失败。

对三个标准文件中的内容做说明：
   - 标准输入: 我们暂时不处理，后期可以增加用户输入功能
   - 标准输出: 程序运行完成，输出结果就在里面
   - 标准错误: 运行时错误信息

## 5.3 测试资源限制功能 (test_resource_limitation.cc)

一般的OJ判题系统（如leetcode）会限制用户提交代码编译后运行时使用的cpu时间和内存，我们想测试一下在Linux服务器下如何实现这个功能：
```cpp
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    std::cout << "signo: " << signo << std::endl;
}

int main()
{
    for(int i = 0; i <= 31; i++)
    {
        signal(i, handler);
    }

    // 限制累计运行时长
    struct rlimit time_limit;
    time_limit.rlim_cur = 1; // 一秒钟
    time_limit.rlim_max = RLIM_INFINITY; 
    setrlimit(RLIMIT_CPU, &time_limit);

    // 限制内存
    struct rlimit mem_limit;
    mem_limit.rlim_cur = 1024 * 1024 * 40;
    mem_limit.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_AS, &mem_limit);


    int count = 0;
    while(1)
    {
        int *p = new int[1024 * 1024];
        count++;
        std::cout << "size: " << count << " MB" << std::endl;
        sleep(1);
    }
    return 0;
}
```

设置资源限制（包括内存和cpu时间）的系统调用叫setrlimit，它的函数原型如下：
```cpp
int setrlimit(int resource, const struct rlimit *rlim);
```
它的第一个参数是我们要设置限制的资源类型：
- 使用RLIMIT_AS来限制进程虚拟内存(地址空间)的最大字节数
- 使用RLIMIT_CPU来限制CPU时间限制(以秒为单位)。

而第二个参数需要一个rlimit结构体，我们需要设置好结构体对象再传给该函数，结构体声明如下：
```cpp
struct rlimit {
   rlim_t rlim_cur;  /* Soft limit */
   rlim_t rlim_max;  /* Hard limit (ceiling for rlim_cur) */
};
```

说明一下：
- 软限制是内核对对应资源实施的值。
- 硬限制相当于软限制的上限
   - 非特权进程只能将其软限制设置为0到硬限制之间的值，并(不可逆转地)降低其硬限制。
   - 特权进程(在Linux下:具有CAP_SYS_RESOURCE能力的进程)可以对任何一个限制值进行任意修改。值RLIM_INFINITY表示对资源没有限制(无论是在getrlimit()返回的结构中，还是在传递给setrlimit()的结构中)。



测试结果：
1. 内存申请失败，进程收到6号`SIGABRT`信号：![](./ReadMePics/内存资源限制.png)
2. CPU使用超时，收到24号`SIGXCPU`信号：![](./ReadMePics/CPU时间限制.png)



## 5.4 "编译并运行"模块 (Compile_And_Run.hpp)

上面将编译和运行模块分开了，正常流程则是先编译后运行。那么我们需要将这两个流程整合起来实现编译运行模块的后端工作。

想要编译，那么我们就需要源文件，源文件从哪里来？通过网络发送而来（来源其实是我们之后即将编写的OJ_Server服务器）。所以我们编译服务器收到的从网络发来的数据是一个json串，我们编译服务器对其进行反序列化后得到的数据写入源文件中，交给编译功能编译后形成可执行文件然后交给运行功能运行，最后整合结果也向对方返回结果json串完成工作。

>我们对Compile_Server服务器从网络中收发的json串做一个规定：

- Compile_Server收到的：
   - code：     用户提交的代码
   - input:     用户给自己提交的代码对应的输入，不做处理 - 后期扩展
   - cpu_limit: 时间要求
   - mem_limit: 空间要求

- Compile_Server返回的：
   - 必填：
      - status: 状态码
      - reason: 请求结果
   - 选填：
      - stdout: 我的程序运行完的结果
      - stderr: 我的程序运行完的错误结果

>编译服务收发的json串示范：
```json
in_json: {
   "code": "#include...", 
   "input": "",
   "cpu_limit":1, 
   "mem_limit":10240
}

out_json: {
   "status":"0",
   "reason":"",
   "stdout":"",
   "stderr":""
}
```
>注意:cpp的json库在linux下需要`sudo yum install jsoncpp-devel`安装json开发库，并且在编译选项中加上`-ljsoncpp`方可编译。
点击[这里](#section1)跳转如何安装jsoncpp。

### 生成唯一文件名 (Utility.hpp)

当一份用户提交代码后，我们为其生成的各种文件的前缀（包括`.cpp`、`.compile_error`、`.stderr`、...）需要具有唯一性。名字生成的唯一性我们可以利用 **毫秒级时间戳 + 原子性的增长计数器** 实现。

> 如何实现？
1. 毫秒级时间戳可以利用`gettimeofday`函数调用实现（返回的结构体存在微秒级的属性，简单转换就可以得到微秒）
   ```cpp
   // 获取毫秒级时间戳
   static std::string GetTimeMs()
   {
      struct timeval _time;
      gettimeofday(&_time, nullptr);
      return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000);
   }
   ```
2. 原子性的增长计数器（同一时刻不同执行流调用-利用static的变量）利用C++11的原子性操作库中的`atomic_uint`即可实现
   ```cpp
   static std::string GetUniqueFileName()
   {
      static std::atomic_uint id(0);
      id++;
      std::string ms = TimeUtility::GetTimeMs();

      // 通过毫秒级时间戳+原子性递增的唯一值
      std::string uniq_id = std::to_string(id);
      return ms + "_" + uniq_id;
   }
   ```


> 为什么要这样做？
1. **用毫秒级时间戳是为了一定程度上保证文件名的随机性。**
   
   但是如果仅仅使用毫秒级时间戳来命名文件，可能会碰到极端情况：服务器在一毫秒内被并发地多次请求编译时，生成的文件名都是一样的。

2. **原子性递增的计数器可以保证程序从开始运行到结束的序号不重复。**

   这里采用原子性变量的原因是为了保证线程安全，因为会存在多个并发的Post请求（请求编译服务），也就存在多条执行流（多个线程）去递增同一个计数器（这个计数器是static的，整个程序里只存在一份），如果不加锁会存在线程安全问题。

<br>

**效果：**

![](./ReadMePics/随机文件名.png)

### 写文件/读文件 (Utility.hpp)

对收到的json串反序列化得到数据后，我们需要将code部分写入`.cpp`源文件中去。 写入文件很简单（利用C++的`<ofstream>`简单IO即可），但是需要注意，之后此模块的功能是被打包为网络服务的。也就是说可能同时出现了很多用户提交的代码。如果此时名字冲突就会发生问题，不同用户之间执行的不同题或者编程内容就会出现问题，因此需要使用上面生成唯一文件名的方法。

```cpp
static bool WriteFile(const std::string &target_path, const std::string &content)
{
   std::ofstream out(target_path, std::ios::binary);
   if(!out.is_open())
   {
         return false;
   }

   out.write(content.c_str(), content.size());
   out.close();
   return true;
}

// 从指定文件读取文件
static bool ReadFile(const std::string &target_path, std::string *content, bool keep = false)
{
   std::ifstream in(target_path);
   if(!in.is_open())
   {
         std::cerr << "打开: " << target_path << "文件失败!" << std::endl;
         return false;
   }

   std::string line;
   while(std::getline(in, line))
   {
         (*content) += line;
         (*content) += (keep ? "\n" : "");
   }

   return true;
}
```
参数说明：
1. target_path 文件路径
2. content 文件内容（输出型参数）
3. keep 是否保留文件中的"\n"





### 统一处理错误码

对我们关心的错误（比如内存超限和cpu超时）进行过滤：
```cpp
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
```

### 开始编译 (Start方法)

我们的`Compile_And_Run.hpp`主要就包含了Start函数，它完成了编译并运行，一遍后面的主文件`Compile_Server.cc`调用Start方法。

```cpp
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
```

## 5.5 完整的编译服务 (Compile_Server.cc)

temp:
命名空间： ns_compiler,  类：class Compiler
命名空间： ns_runner  类：class Runner
命名空间： ns_log
命名空间： ns_utility,  类：class PathUtility FileUtility TimeUtility

> [!Tip] 我们希望使用一个主文件来包含上面所有编写完成的头文件，来拼接出一个完整的编译服务，编译服务预期的结构如下：
> ![](./ReadMePics/文件结构简图.png)

### I. 本地测试
我们可以对“CompileAndRun”进行本地测试，给run函数传递一个json串，包括试运行的代码，然后看看返回的json串是否符合预期。

>预期：
1. 能否返回各种错误信息
2. 能够在temp目录下，生成带有编译错误、标准输出、标准错误的文件，且文件名唯一。

```cpp
#include "Compile_And_Run.hpp"
using namespace ns_compile_and_run;

int main()
{
    // 通过http 让client 给我们上传一个json数据
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
```

>本地测试运行结果：
1. 编译错误：![](ReadMePics/编译错误.png)
2. 运行错误 - 内存超出限制：![](ReadMePics/运行出错_内存.png)
3. 运行错误 - 时间超出限制：![](ReadMePics/运行错误_超时.png)







### II. 包装成网络服务，进行网络测试

1. 让我们的项目接入cpp-httplib开源第三方库，它是一个***阻塞式多线程的一个网络http库***
2. cpp-httplib是**header-only**的，所以只需要将`.h`拷贝到项目中，即可直接使用，点击[这里](#section2)跳转到如何安装cpp-httplib。

3. 编写Compile_Server.cc，引入httplib头文件：
```cpp
#include "Compile_And_Run.hpp"
#include "../Common/httplib.h"

using namespace ns_compile_and_run;
using namespace httplib;

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

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", atoi(argv[1]));

    return 0;
}
```

>说明一下：
1. **svr.Get("/hello", lambda表达式)**

   这个调用设置了当服务器接收到一个 GET 请求到 "/hello" 路径时应该执行的回调函数。这里的回调函数非常简单，它只是设置响应的内容为 "hello httplib, 你好httplib!"，并设置内容类型为 "text/plain;charset=utf-8"。

2. **svr.Post("/compile_and_run", lambda表达式)**

   这个调用设置了当服务器接收到一个 POST 请求到 "/compile_and_run" 路径时应该执行的回调函数。这个回调函数会获取请求的主体（body）内容，它应该是一个 JSON 字符串。然后，它调用 `CompileAndRun::Start` 函数让服务器对post过来的JSON串中的代码编译处理后，将运行结果作为 JSON串返回给客户端。响应的内容类型被设置为 "application/json;charset=utf-8"。
   我们使用PostMan进行测试，可以通过[PostMan官网](https://web.postman.com/)下载安装。
   
   - POST一个json串可以返回它对应的json串：
   ![](./ReadMePics/postman_test.png)

   - 可以在终端中看到服务器打出的日志信息，服务器响应了我们的请求：![](/ReadMePics/serverecho_post.png)

3. **svr.set_base_dir("./wwwroot")**

   这个调用设置了服务器的基础目录为 "./wwwroot"。这意味着，当客户端请求一个静态文件（例如一个图片或 CSS 文件）时，服务器会在这个目录下查找这个文件：![](/ReadMePics/testwwwroot.png)

4. **svr.listen("0.0.0.0", atoi(argv[1]))**

   这个调用启动服务器并使其监听传入的连接。`"0.0.0.0"` 表示服务器应该监听所有可用的网络接口。`atoi(argv[1])` 将命令行参数 `argv[1]` 转换为整数，这个整数表示服务器应该监听的端口号。

5. 注意响应正文对应的响应报头中写的类型（ConnectType）可以参考此网站进行对照： [HTTP 响应类型 ContentType 对照表 - 爱码网](https://www.likecs.com/show-656319.html)

<br>
<br>
<br>
<br>


# 六、OJ_Server - 基于MVC结构的Online Judge服务器设计
**本质：** 建立一个小型网站

**网站功能：**
1. 获取首页，跳转到题目列表
2. 每个题目的编辑区域页面
3. 提交判题功能（编译并运行）

> **什么是MVC结构？**
> - M: Model，通常是和数据交互的模块，比如，对题库进行增删查改（文件版，MySQL版）
> - V：View，通常是拿到数据之后，要进行构建网页，渲染网页内容，展示给用户（通过浏览器）
> - C：Controller，控制器。控制器是MVC结构中的协调者，它负责接收用户的输入并处理用户的请求。

```mermaid
flowchart LR
    User[用户] -->|在网页上操作| View[View]
    View -->|发送用户请求（通过AJAX发送json串）| Controller[Controller]
    Controller -->|处理请求| Model[Model]
    Model -->|返回数据| Controller
    Controller -->|更新视图| View
    User[用户] -.->|间接地请求| Controller[Controller]
```

<br>

前面的`Compile_Server`我们是倒过来介绍的，结构呈现一个倒金字塔，先介绍组件，再合并，包装成一个统一的服务。接下来的`OJ_Server`，我们会先介绍它的主执行流，再去分别实现它所调用的方法。

## OJ服务器的主执行流 (OJ_Server.cc)

```cpp
using namespace httplib;
using namespace ns_controller;

// #define DEAMON_ON

static Controller *ctrl_ptr = nullptr;

void Recovery(int signo)
{
    ctrl_ptr->RestoreService();
}

int main()
{
    signal(SIGQUIT, Recovery); // ctrl + "\"

    cout << "pid: " << getpid() << endl;

    srand(time(nullptr));

#ifdef DEAMON_ON
    // 守护进程化
    daemon(true, false);
#endif


    // 用户请求的服务，我们要给它做服务路由
    Server svr; // 创建服务器对象
    Controller ctrl;
    ctrl_ptr = &ctrl;
    
    // 获取所有的题目列表
    svr.Get("/all_questions", [&ctrl](const Request &request, Response &response)
    {
        // 返回一张包含有所有题目的html网页
        std::string html;
        ctrl.AllQuestions(&html);

        response.set_content(html, "text/html; charset=utf-8");
    });

    // 用户要根据题目编号，获取题目的内容
    // /questions/100 -> 正则匹配
    // R"(...)"，语法特性：raw string，让“\”成为普通字符，不用做转义
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request &request, Response &response)
    {
        std::string number = request.matches[1]; // 下标1是正则匹配到的数字(\d+)
        std::string html;
        ctrl.Question(number, &html);
        response.set_content(html, "text/html; charset=utf-8");
    });

    // 用户提交代码，使用我们的判题功能： 1. 每道题的测试用例 2. compile_and_run

    // 前端给到的json串：
    // // in_json:
    // {
    //     "code" : "#include...", 
    //     "input" : "程序的标准输入的内容"
    // };

    // 我们服务器要返回给前端网页的json串：
    // out_json:
    // {
    //     "status" : "0", 
    //     "reason" : "", 
    //     "stdout" : "",
    //     "stderr" : ""
    // };
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &request, Response & response)
    {
        std::string number = request.matches[1];
        std::string result_json_str;
        ctrl.Judge(number, request.body, &result_json_str);
        response.set_content(result_json_str, "application/json; charset=utf-8");

        // response.set_content("指定题目的判题：" + number, "text/plain; charset=utf-8");
    });


    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8888);
    return 0;
} 
```

## Model模块 (OJ_Model.hpp)
> 用户需要的是题目数据，那么我们应当合理设计题目数据存放的位置（磁盘）和方式（文件/MySQL数据库）
> 
> **我们设计两个版本：**
> 1. 文件版本
> 2. MySQL数据库版本

对应不同的版本model数据交互细节不一样（文件操作/CPP-mysql connect），但是提供给 `Controller模块` 调用的接口一致。

### I. 文件版题目设计

> **文件存放的内容：**
> 1. 题目编号
> 2. 题目标题
> 3. 题目难度
> 4. 题目描述（题面）
> 5. 题目的时间限制
> 6. 题目的内存限制
> 7. 通过率

> **需要的文件：**
> 1. question.list：题目列表（不需要题目内容）
> 2. 题目描述、题目的预设置代码`default_template_code.cpp`和测试用例代码`test_cases.cpp`
>![](./ReadMePics/关联.png)
> 以上两者用题目编号关联


> **编写思路：**
> 1. 用户提交代码
> 2. OJ不是只把用户代码交给compile_and_run，而是要融合用户基于`default_template_code.cpp`的更改和`test_cases.cpp`
![](./ReadMePics/代码拼接.png)

实际上我们的编译服务启动的子进程程序替换g++时，传递了`"-D", "COMPILER_ONLINE"`选项，就去掉了那一段没有实际意义的`#include ...`代码：
```cpp
execlp("g++", "g++", "-o", PathUtility::Exe(file_name).c_str(), \
PathUtility::Src(file_name).c_str(), "-std=c++11", "-D", "COMPILER_ONLINE", nullptr); // 这里的nullptr表示结尾
```

---
<br>

### II. 编写文件版`OJ_Model.hpp`

> 方法：先描述，再组织

#### 1. 把题目描述起来 - struct Question

```cpp
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
```
#### 2. 把题目组织起来 - std::unordered_map<string,Question> 

>用哈希表（unordered_map）建立题号到Question对象的映射关系：

```cpp
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
```


<br>
<br>

### III. MySQL版题目设计

1. 在数据库中设计可以远程登陆的MySQL用户，取名`oj_client`
   ```sql
   create user oj_client@'%' identified by 'Cydia4384!';
   ```

2. 设计表结构
   - 数据库：oj
      ```sql
      mysql> create database oj;
      Query OK, 1 row affected (0.06 sec)

      mysql> show create database oj;
      +----------+---------------------------------------------------------------+
      | Database | Create Database                                               |
      +----------+---------------------------------------------------------------+
      | oj       | CREATE DATABASE `oj` /*!40100 DEFAULT CHARACTER SET latin1 */ |
      +----------+---------------------------------------------------------------+
      ```

   - 表：oj_questions
      ```sql
      create table if not exists 'oj_questions'(
         'number' int primary key auto_increment COMMENT '题目的编号',
         'title' varchar(128) NOT NULL comment '题目的标题',
         'star' varchar(8) NOT NULL comment '题目的难度',
         'description' text NOT NULL comment '对应题目预设给用户看的代码',
         'test_cases' text NOT NULL comment '对应题目的测试用例代码',
         'cpu_limit' int default 1 comment '对应题目的超时时间',
         'mem_limit' int default 50000 comment '对应题目的内存上限'
      )
      ```


3. 给`oj_client`赋权
   ```sql
   mysql> grant all on oj.* to oj_client@'%';
   ```


4. 让我们的服务器连接访问该数据库
   有可能你的系统中，已经默认安装了mysql的开发包，也有可能没有安装，为了保险起见可以下载解压mysql的开发包在服务器本地：
   ![](ReadMePics/PixPin_2024-05-02_20-07-11.png)
   然后在项目中创建mysql开发包的include和lib路径的软链接：
   ```bash
   ln -s ~/third_party/mysql-connector/include include
   ln -s ~/third_party/mysql-connector/lib lib
   ```
   效果：
   ```bash
   include -> /home/chen/third_party/mysql-connector/include
   lib -> /home/chen/third_party/mysql-connector/lib
   ```


### IV. 编写MySQL版`OJ_Model.hpp`
```cpp
const std::string question_table_name = "oj_questions";
const std::string host = "127.0.0.1";
const std::string user = "oj_client";
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
```

## Controller (OJ_Controller.hpp)

OJ_Controller逻辑控制是OJ_Controller中的核心。也是我们整个服务器的核心。`OJ_Server.cc主执行流` 中的 `http请求中的功能路由` 的lambda表达式会调用Controller模块中的方法完成对应的功能，比如：
1. 当前端网页向服务器发出 `/all_questions` 的 `Get` 请求，表明希望返回一张题目列表的网页：
   ```cpp
   svr.Get("/all_questions", [&ctrl](const Request &request, Response &response)
   {
      // 返回一张包含有所有题目的html网页
      std::string html;
      ctrl.AllQuestions(&html);

      response.set_content(html, "text/html; charset=utf-8");
   });
   ```

2. 当前端网页向服务器发出 `/questions/具体题号` 的 `Get` 请求，表明希望返回一张题目列表的网页：
   ```cpp
   svr.Get(R"(/question/(\d+))", [&ctrl](const Request &request, Response &response)
   {
      std::string number = request.matches[1]; // 下标1是正则匹配到的数字(\d+)
      std::string html;
      ctrl.Question(number, &html);
      response.set_content(html, "text/html; charset=utf-8");
   });
   ```
   > 说明一下：
   > - 用户要根据题目编号，获取题目的内容
   > - `/questions/100` -> 会被正则匹配
   > - R"(...)" -> C++的语法特性raw string，让“\”成为普通字符，不用做转> 义

3. 当前端网页向服务器发出 `/judge/具体题号` 的 `Post` 请求，表明希望判断某道题目的代码是否能通过测试用例：
   ```cpp
   svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &request, Response & response)
   {
      std::string number = request.matches[1];
      std::string result_json_str;
      ctrl.Judge(number, request.body, &result_json_str);
      response.set_content(result_json_str, "application/json; charset=utf-8");
   });
   ```

   **注意：**
   - 前端给到的json串：
      ```json
      // in_json:
      {
         "code" : "#include...", 
         "input" : "程序的标准输入的内容"
      };
      ```
   - 我们服务器要返回给前端网页的json串：
      ```json
      // out_json:
      {
         "status" : "0", 
         "reason" : "", 
         "stdout" : "",
         "stderr" : ""
      };
      ```   


根据上面的信息：
1. 我们首先需要能够获得题库并且构建渲染成网页的功能，由 `AllQuestions()` 提供；
2. 其次我们也需要获得单个题目并且构建渲染成网页的功能
3. 最后我们能够通过用户上传的json串完成判题的功能返回结果json串。



<br>

---
Model功能我们已经完成了，View功能基本属于前端功能，由于侧重后端，后续简单介绍即可。Controller更多的是将Model和View联合控制起来，完成业务的逻辑控制。

*开始编写Controller：*


### 获取题目列表的html网页
model中已经为我们提供了获取单个Question对象vector数组的GetAllQuestion()方法，用这个Question对象数组传递给View模块来生成网页：
```cpp
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
```
>注意：获取到的题目没有排序，我们利用 `std::sort()` 函数，**根据question中的number字符串转换为整型**进行排序即可。


### 获取单个题目的html网页
model中已经为我们提供了获取单个Question对象的GetOneQuestion()方法，用这个Question对象传递给View模块来生成网页：
```cpp
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
```

### 判题

**判题功能：**
用户通过json格式提交上来的代码数据需要获取上来，然后需要通过编译运行服务执行结果，得到结果后构建json串返回结果。

**实际的执行流程：**
1. 首先需要定义用户上传的json数据。因为判题路由存在题目编号，那么用户上传的也只有用户编辑后的代码code以及input数据。由于用户测试运行模块属于扩展内容，这里就先将input的内容忽视。我们将code和对应题目的tail文件进行凭借组成一份完整代码，在加上题目的空间以及时间限制（编译服务需要的四份属性）组成一份json数据准备发送给编译运行服务。

2. 我们要确定好发送给哪台编译服务主机。 因为业务众多，不可能存在一台编译运行服务主机（负载压力太大），我们设计为网络服务的原因也就是能在不同的主机上部署此服务，方便于oj_server进行选择。

3. 我们使用负载均衡的模式进行主机选择。那么我们首先得定义主机对象，并且根据主机的配置文件加载当前的所有主机信息，方便我们进行调用。

这里的JSON串的传递的内容有一些杂乱，我们绘制一张图来理解，这张图描绘了请求体和响应体在不同阶段的内容：
![](./ReadMePics/项目中JSON串的传递.jpg)


为了减轻单台服务器的压力，把编译服务部署在多台Linux服务器上（我们可以在本地起多个进程模拟），我们使用**负载均衡模式**进行主机选择。我们依然使用**"先描述，再组织"**的思想构建这套模式：
1. 首先得定义主机对象 `Machine`，把主机信息==**描述**==起来；
2. 负载均衡模块`LoadBalancingModule`再根据指定目录下存放的 `所有现存主机的配置文件（存放IP和端口号）` 加载当前的所有主机信息，方便我们进行调用。
3. `LoadBalancingModule`中将来会存放一个所有主机对象的数组`std::vector<Machine> machines`将所有主机==**组织**==起来


#### 定义主机对象

```cpp
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
```
我们设计了四个字段：
- IP
- 端口号
- 负载个数
- C++内置的互斥锁mutex的指针

> IP和端口号很好理解，因为需要网络通信，IP和端口号唯一标识一个网络进程，我们主要来解释为什么要这样设计一把锁的指针：
> 
> 因为同一时刻存在不同的执行流执行判题功能（http网络服务里由线程池，会并发执行一些代码，会访问临界区），**为了保证线程安全，我们需要一把互斥锁，保证负载数的访问和修改安全**。可以利用C++中的std::mutex进行定义，需要注意的是mutex在C++中无法进行拷贝，所以最好定义为指针类型，以便在需要时初始化它，默认构造中直接赋`nullptr`即可。


#### 负载均衡式地选择Compile_Server

因为controller中实例化了Model、View、LoadBalancingModule，所有我们可以调用它们类作用域中定义的方法：

```cpp
// 核心业务逻辑的控制器
class Controller
{
private:
   Model _model;                       // 提供后台数据
   View _view;                         // 提供html渲染功能
   LoadBalancingModule _load_balancer; // 核心负载均衡器

public:
   // 判题的接口，它会调用编译服务集群
   // number 题目编号
   // in_json 请求
   // out_json 结果
   void Judge(const std::string &number, const std::string in_json, std::string *out_json)
   {
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
      compile_value["code"] = customer_code + "\n" + q.test_cases; // 加一个换行符，以免两段代码粘在一起
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
}
```

它调用了LoadBalancingModule提供的SmartChoice方法：
```cpp
// 选择负载相对最低的主机，并返回其id和指向主机对象的指针
// id 输出型参数，主机id
// m 输出型参数，主机对象地址
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
   for(int i = 1; i < online_num; i++)
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

```


负载均衡效果：![](./ReadMePics/负载均衡.gif)

>每次`SmartChoice`方法都会选择负载最低的主机，调用其编译服务


注意：
1. Judge函数中发送判题请求由于会出现请求失败的情况，我们将选择设置为死循环，直到主机全部下线了或者发送成功再退出。
2. LoadBalanceModuel还需要向上层的Controller提供一键上线全部主机功能，对于我们的主机：
   - 上线的本质是将主机的号码push进在线的主机的vector中
   - 判断一台主机是否下线了是基于提交Post判题请求是否成功，也就是根据http给我们服务端返回的状态码是否为200

<br>
<br>
<br>
<br>


## View (OJ_View.hpp)

因为需要将后端数据融入（直接文本替换）进前端网页，然后进行渲染，我们需要利用到ctemplate库，点击[这里](#section4)跳转如何安装jsoncpp。

OJ_View模块中的 `AllExpandToHtml` 方法用于生成 HTML 代码，该 HTML 代码包含了从 `questions` 数据中获取的内容。它使用了 `ctemplate` 来进行模板渲染，通过替换 HTML 模板中的占位符来生成最终的 HTML 输出。

```cpp
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
```

参数列表说明：

- **输入型参数**：`AllExpandToHtml` 方法接受一个 `questions` 向量和一个指向 `std::string` 的指针 `html`。`questions` 包含了题目数据，每个题目有一个编号 (`number`)、标题 (`title`)、和难度 (`star`)。
- **输出型参数**：生成一个包含题目列表的 HTML 页面，将结果存储在 `html` 指向的字符串中。

逻辑流程：
1. **模板路径**：
   设定模板文件的路径。这里，模板文件是 `all_questions.html`，通常包含 HTML 代码和占位符。

2. **创建字典**：
   创建一个 `ctemplate::TemplateDictionary` 实例 `root`，用于存储替换模板中的占位符所需的变量。该字典将被用于生成 HTML 内容。

3. **填充字典**：
   使用 `questions` 数据来填充 `root` 字典的 `question_list` 部分。
   - 通过 `root.AddSectionDictionary("question_list")` 创建一个字典部分，这个部分对应于模板中的 `{{#question_list}}...{{/question_list}}` 块。
   - 对于 `questions` 向量中的每个 `Question` 对象，设置对应的 `number`、`title` 和 `star` 值，以便在模板中替换。

4. **获取模板**：
   使用 `ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP)` 获取模板对象 `tpl`。模板对象是一个预解析的模板文件，其占位符会在渲染时替换。

5. **渲染 HTML**：
   使用 `tpl->Expand(html, &root)` 来渲染模板。
   - `Expand` 方法将模板中的占位符用 `root` 中的值进行替换。
   - 在这个例子中，`{{#question_list}}...{{/question_list}}` 是一个循环块，`root` 的 `question_list` 中每个子字典代表一个题目，循环生成 HTML 中对应的 `<tr>` 列表。
   - `{{number}}`、`{{title}}`、`{{star}}` 被替换为相应的值。

渲染完成后，`html` 指向的字符串会包含生成的 HTML 代码，包含了从 `questions` 数据中提取的内容。最终结果是一个动态生成的题目列表，用户可以在网页上查看。

替换前后的对比：![](./ReadMePics/替换前后对比.png)





<br>
<br>
<br>
<br>


# 七、前端网页设计
>虽然我们是做后端的，但是仍然需要一个相对美观的前端界面来构成一个完整的项目。

## I. 首页
首页 `index.html` 被放在了服务器文件夹中的wwwroot下，当服务器运行时，使用浏览器访问即可，首页的html代码：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Online Judge</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            background-color: #f8f9fa;
        }
        .hero {
            text-align: center;
            padding: 100px 0;
        }
        .hero h1 {
            font-size: 3em;
            font-weight: bold;
        }
        .hero p {
            font-size: 1.5em;
            color: #6c757d;
        }
    </style>
</head>
<body>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
        <a class="navbar-brand" href="#">Online Judge</a>
        <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav">
                <li class="nav-item active">
                    <a class="nav-link" href="#">Home <span class="sr-only">(current)</span></a>
                </li>
                <li class="nav-item">
                    <a class="nav-link" href="/all_questions">Problem List</a>
                </li>
            </ul>
        </div>
    </nav>
    
    <div class="hero">
        <h1>Welcome to the Online Judge System</h1>
        <p>Test your coding skills with various programming problems.</p>
        <a class="btn btn-primary" href="/all_questions">Start Solving Problems</a>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>

```

首页效果：
![](./ReadMePics/frontpage.png)


## II. 题目列表模板页


```html
<!DOCTYPE html>
<html lang="zh">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>题目列表 - My_OJ</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #ffffff; /* 浅色背景 */
            color: #333333; /* 深色文本 */
        }

        header {
            background-color: #f5f5f5; /* 浅灰色背景 */
            padding: 20px;
            text-align: center;
            border-bottom: 1px solid #e0e0e0; /* 分隔线 */
        }

        header h1 {
            font-weight: bold;
            font-size: 2em;
            color: #333333; /* 深色标题 */
        }

        main {
            padding: 20px;
            margin-top: 20px;
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        table th, table td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #e0e0e0; /* 分隔线 */
        }

        table th {
            background-color: #f5f5f5; /* 浅灰色背景 */
            font-weight: bold;
            color: #333333; /* 深色标题 */
        }

        table tr:hover {
            background-color: #f0f0f0; /* 浅灰色悬停效果 */
        }

        a {
            color: #0056b3; /* 深蓝色链接 */
            text-decoration: none;
        }

        a:hover {
            text-decoration: underline; /* 链接悬停效果 */
        }

        footer {
            background-color: #f5f5f5; /* 浅灰色背景 */
            color: #333333; /* 深色文本 */
            text-align: center;
            padding: 10px;
            position: fixed;
            bottom: 0;
            width: 100%;
            border-top: 1px solid #e0e0e0; /* 分隔线 */
        }
    </style>
</head>
<body>
    <header>
        <h1>My_OJ - 题目列表</h1>
    </header>
    
    <main>
        <h2>题目列表</h2>
        <table>
            <tr>
                <th>题目编号</th>
                <th>题目标题</th>
                <th>题目难度</th>
            </tr>
            
            {{#question_list}}
            <tr>
                <td>{{number}}</td>
                <td><a href="/question/{{number}}">{{title}}</a></td>
                <td>{{star}}</td>
            </tr>
            {{/question_list}}
            
        </table>
    </main>
    
    <footer>
        <p>版权所有 &copy; 2024 My_OJ</p>
    </footer>

    <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
```

题目列表页效果：![](./ReadMePics/题目列表页效果.png)

## III. 单个题目页

```html

<!DOCTYPE html>
<html lang="zh">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{{number}}. {{title}}</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f8f9fa;
            margin: 0;
            padding: 0;
        }

        header {
            background-color: #333; /* 深色背景 */
            color: #fff; /* 浅色文本 */
            padding: 15px;
            text-align: center;
        }

        .split-container {
            display: flex;
            height: calc(100vh - 50px); /* 除去 header */
            position: relative;
        }

        .left-panel {
            background-color: #ffffff;
            padding: 20px;
            box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
            width: 60%; /* 左侧初始宽度 */
            overflow-y: auto;
            position: relative; /* 以便分割线使用 */
        }

        .right-panel {
            background-color: #ffffff;
            padding: 20px;
            box-shadow: -2px 0 5px rgba(0, 0, 0, 0.1);
            width: 40%; /* 右侧初始宽度 */
            overflow-y: auto;
        }

        .outer-divider {
            width: 5px;
            cursor: ew-resize;
            background-color: #ccc;
            position: absolute;
            height: 100%;
        }

        .inner-divider {
            height: 5px;
            cursor: ns-resize;
            background-color: #ccc;
            position: absolute;
            width: 100%;
        }

        .top-part {
            height: 60%; /* 左侧顶部的初始高度 */
            overflow-y: auto;
        }

        .bottom-part {
            height: 40%; /* 左侧底部的初始高度 */
            overflow-y: auto;
            padding-top: 10px; /* 留出空间给分割线 */
        }

        footer {
            background-color: #333; /* 深色背景 */
            color: #fff; /* 浅色文本 */
            text-align: center;
            padding: 10px;
            position: fixed;
            bottom: 0;
            width: 100%;
        }

        .code-editor {
            height: 80%; /* Ace 编辑器的高度 */
        }

        .result {
            padding: 10px;
            background-color: #f8f9fa; /* 浅色背景 */
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }

    </style>
</head>
<body>
    <header>
        <h1>My_OJ</h1>
    </header>

    <div class="split-container">
        <!-- 左侧模块 -->
        <div class="left-panel">
            <!-- 顶部题目信息 -->
            <div class="top-part" id="problem-info">
                <h2>{{number}}. {{title}}</h2>
                <h4>难度：{{star}}</h4>
                <pre>{{description}}</pre>
            </div>

            <!-- 分割线，分隔题目信息和结果 -->
            <div class="inner-divider" id="inner-divider" style="top: 60%;"></div>

            <!-- 底部代码提交结果 -->
            <div class="bottom-part" id="result-container">
                <div class="result">
                    <!-- 显示代码提交的结果 -->
                </div>
            </div>
        </div>

        <!-- 外部分割线 -->
        <div class="outer-divider" id="outer-divider" style="left: 60%;"></div>

        <!-- 右侧模块 -->
        <div class="right-panel">
            <div id="code-editor" class="code-editor">{{pre_code}}</div>
            <button class="btn btn-primary" onclick="submitCode()">提交代码</button>
        </div>
    </div>

    <footer>
        <p>版权所有 &copy; 2024 My_OJ 系统</p>
    </footer>

    <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.6/ace.js" charset="utf-8"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.6/ext-language_tools.js" charset="utf-8"></script>
    
    <script>
        // Ace 编辑器初始化
        var editor = ace.edit("code-editor");
        editor.setTheme("ace/theme/idle_fingers");
        editor.session.setMode("ace/mode/c_cpp");
        editor.setFontSize(16);
        editor.session.setTabSize(4);
        editor.setOptions({
            enableBasicAutocompletion: true,
            enableSnippets: true,
            enableLiveAutocompletion: true
        });

        function submitCode() {
            var code = editor.getValue(); // 获取编辑器中的代码
            var number = document.querySelector("#problem-info h2").textContent.split(".")[0].trim(); // 题号
            var judge_url = "/judge/" + number; // 构建请求 URL

            $.ajax({
                method: 'POST', // 请求方式
                url: judge_url, // 请求地址
                dataType: 'json', // 响应的数据类型
                contentType: 'application/json;charset=utf-8',  // 请求的内容类型
                data: JSON.stringify({
                    'code': code,
                    'input': ''
                }),
                success: function(data) {
                    showResult(data); // 处理返回的结果
                }
            });

            function showResult(data) {
                const resultDiv = document.querySelector("#result-container .result"); // 获取结果显示区域
                resultDiv.innerHTML = ""; // 清空之前的内容

                // 状态码标题
                const statusHeader = document.createElement("h4");
                statusHeader.textContent = "状态码:";
                resultDiv.appendChild(statusHeader);

                const statusText = document.createElement("p");
                statusText.textContent = `${data.status}`; // 状态码的值
                resultDiv.appendChild(statusText);

                // 原因标题
                const reasonHeader = document.createElement("h4");
                reasonHeader.textContent = "原因:";
                resultDiv.appendChild(reasonHeader);

                const reasonText = document.createElement("p");
                reasonText.textContent = `${data.reason}`; // 原因的值
                resultDiv.appendChild(reasonText);

                // 标准输出标题
                const stdoutHeader = document.createElement("h4");
                stdoutHeader.textContent = "标准输出:";
                resultDiv.appendChild(stdoutHeader);

                const stdoutText = document.createElement("pre");
                stdoutText.textContent = data.stdout || "无输出"; // 若没有标准输出，则显示“无输出”
                resultDiv.appendChild(stdoutText);

                // 标准错误标题
                const stderrHeader = document.createElement("h4");
                stderrHeader.textContent = "标准错误:";
                resultDiv.appendChild(stderrHeader);

                const stderrText = document.createElement("pre");
                stderrText.textContent = data.stderr || "无错误"; // 若没有标准错误，则显示“无错误”
                resultDiv.appendChild(stderrText);
            }
        }

        // 外部分割线拖动逻辑
        const outerDivider = document.getElementById("outer-divider");
        const leftPanel = document.querySelector(".left-panel");
        const rightPanel = document.querySelector(".right-panel");
        let isDraggingOuter = false;

        outerDivider.addEventListener("mousedown", function() {
            isDraggingOuter = true;
        });

        document.addEventListener("mouseup", function() {
            isDraggingOuter = false;
        });

        document.addEventListener("mousemove", function(e) {
            if (!isDraggingOuter) return;

            const offsetX = e.clientX; /* 当前鼠标X轴坐标 */
            const containerWidth = document.querySelector(".split-container").offsetWidth; /* 容器宽度 */
            const leftWidth = (offsetX / containerWidth) * 100; /* 计算左侧占比 */
            leftPanel.style.width = `${leftWidth}%`;
            rightPanel.style.width = `${100 - leftWidth}%`;
            outerDivider.style.left = `${leftWidth}%`;
        });

        // 内部分割线拖动逻辑
        const innerDivider = document.getElementById("inner-divider");
        const topPart = document.querySelector(".top-part");
        const bottomPart = document.querySelector(".bottom-part");
        let isDraggingInner = false;

        innerDivider.addEventListener("mousedown", function() {
            isDraggingInner = true;
        });

        document.addEventListener("mouseup", function() {
            isDraggingInner = false;
        });

        document.addEventListener("mousemove", function(e) {
            if (!isDraggingInner) return;

            const offsetY = e.clientY; /* 当前鼠标Y轴坐标 */
            const containerHeight = document.querySelector(".left-panel").offsetHeight; /* 左侧高度 */
            const topHeight = (offsetY / containerHeight) * 100; /* 计算顶部占比 */
            topPart.style.height = `${topHeight}%`;
            bottomPart.style.height = `${100 - topHeight}%`;
            innerDivider.style.top = `${topHeight}%`;
        });
    </script>
</body>
</html>

```

单个题目页效果：![](./ReadMePics/单个题目页效果.png)


<br>
<br>
<br>
<br>
<br>
<br>



# Final. 所有备注

## 1. 安装jsoncpp

<a id="section1"></a>

```bash
[chen@ali-centos-7 Load-balancing-online-judging-system]$ sudo yum install -y jsoncpp-devel
Loaded plugins: fastestmirror
Loading mirror speeds from cached hostfile
 * centos-sclo-rh: mirrors.163.com
 * centos-sclo-sclo: mirrors.nju.edu.cn
Package jsoncpp-devel-0.10.5-2.el7.x86_64 already installed and latest version
Nothing to do
[chen@ali-centos-7 Load-balancing-online-judging-system]$ 
```


## 2. 安装cpp-httplib (header-only的)
<a id="section2"></a>

cpp-httplib gitee链接：https://gitee.com/yuanfeng1897/cpp-httplib?_from=gitee_search

v0.7.15版本链接： https://gitee.com/yuanfeng1897/cpp-httplib/tree/v0.7.15

说明：
1. 接入cpp-httplib，只需要将.h拷贝到你的项目中，即可直接使用
2. 最新的cpp-httplib在使用的时候，如果gcc不是特别新的话有可能会有运行时错误的问题
   - 建议：cpp-httplib 0.7.15
   - 下载zip安装包，上传到服务器即可


```cpp
// 使用样例:
#include "httplib.h"
int main()
{
   httplib::Server svr;
   svr.Get("/hi", [](const httplib::Request &req, httplib::Response &rsp){
   rsp.set_content("你好,世界!", "text/plain; charset=utf-8");
   });
   svr.listen("0.0.0.0", 8080);
   return 0;
}
// 更多的细节可以看gitee上面的使用手册
```

## 3. 安装boost库
```bash
$ sudo yum install -y boost-devel //是boost 开发库
```

## 4. 安装与测试ctemplate
<a id="section4"></a>
我们的项目使用了google的开源项目ctemplate，ctemplate支持几种文字替换的形式（基本方式是使用“{{}}”来表示需要被替换的内容），下面介绍一下如何安装：
```
sudo yum groupinstall "Development Tools"
sudo yum install autoconf automake libtool

$ git clone https://github.com/OlafvdSpek/ctemplate.git
$ ./autogen.sh
$ ./configure
$ make //编译
$ make install //安装到系统中

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

>注意:
>1. 使用高版本gcc编译
>2. 如果安装报错，使用sudo
><br>

### 测试CTemplate

```cpp
#include <ctemplate/template.h>
#include <iostream>

int main()
{
    // 1. 形成数据字典
    std::string value = "hello haha";
    ctemplate::TemplateDictionary root("test"); // hash_map
    root.SetValue("key", value);

    // 2. 形成被渲染网页对象
    std::string in_html = "./test.html";
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html, ctemplate::DO_NOT_STRIP);
    
    // 3. 添加字典到网页中
    std::string out_html;
    tpl->Expand(&out_html, &root);

    // 4. 完成渲染
    std::cout << out_html << std::endl;

    return 0;
}
```
效果：

![Alt text](./ReadMePics/ctemplate的kv替换.png)

### CTemplate的几种文字替换的形式
CTemplate主要使用双花括号`{{}}`来标记需要替换的部分，提供多种文字替换的形式，以下是其中几种常用形式：

1. **简单变量替换**：
   这是CTemplate最基础的形式，用来将变量名替换为实际值。格式是`{{variable_name}}`。当渲染模板时，这部分会被传入的变量值替代。

   ```cpp
   std::map<std::string, std::string> variables = {{"name", "Alice"}};
   std::string template_text = "Hello, {{name}}!";
   // 渲染后结果是 "Hello, Alice!"
   ```

2. **部分替换（Section）**：
   这涉及对模板中的整个部分进行替换，通常用于处理条件逻辑、列表或重复数据的情况。使用`{{#section_name}}...{{/section_name}}`来定义一个部分。

   ```cpp
   std::map<std::string, bool> variables = {{"show_greeting", true}};
   std::string template_text = "{{#show_greeting}}Hello, world!{{/show_greeting}}";
   // 如果 'show_greeting' 为 true，结果是 "Hello, world!"
   ```

3. **列表（Repeated Sections）**：
   如果要在模板中处理列表或循环，这种形式会很有用。重复的部分定义在`{{#section_name}}...{{/section_name}}`中，变量是一个列表。

   ```cpp
   std::vector<std::map<std::string, std::string>> items = {
       {{"item", "Item 1"}},
       {{"item", "Item 2"}},
       {{"item", "Item 3"}}
   };
   std::string template_text = "{{#items}}- {{item}}\n{{/items}}";
   // 生成的结果是:
   // "- Item 1
   //  - Item 2
   //  - Item 3"
   ```

4. **反向部分（Inverted Sections）**：
   反向部分用于处理条件逻辑中的"否定"或"空值"的情况。格式是`{{^section_name}}...{{/section_name}}`。

   ```cpp
   std::map<std::string, bool> variables = {{"show_greeting", false}};
   std::string template_text = "{{^show_greeting}}No greeting{{/show_greeting}}";
   // 如果 'show_greeting' 为 false，结果是 "No greeting"
   ```

5. **特殊字符转义**：
   CTemplate会自动处理特殊字符的转义，防止出现脚本注入等问题。这样可以在输出中安全地显示用户输入。

   ```cpp
   std::map<std::string, std::string> variables = {{"unsafe_text", "<script>alert('hi');</script>"}};
   std::string template_text = "{{unsafe_text}}";
   // 渲染结果会自动转义特殊字符，输出 "&lt;script&gt;alert('hi');&lt;/script&gt;"
   ```

这些是CTemplate中几种常见的文字替换形式，提供了灵活的模板生成和处理方法。

### CTemplate的缓存机制

CTemplate的核心功能之一是缓存机制，这可以显著提高模板渲染的效率。在使用Expand函数生成网页内容时，CTemplate的缓存机制旨在***避免重复加载和解析***模板文件，这有助于加快渲染过程。以下是CTemplate缓存机制的概要和Expand函数是如何利用该机制来确保高效渲染的：

#### 缓存机制的基础
CTemplate的缓存机制主要用于缓存模板文件及其解析后的结构。这些缓存机制的关键点包括：

1. **模板文件的缓存**：
   CTemplate通常会缓存模板文件的内容。这意味着一旦模板文件被加载，它的内容就会被缓存起来，以便在后续请求中快速访问。

2. **解析后的模板结构的缓存**：
   当模板文件被加载后，它会被解析成内部的模板结构，这种结构用于快速渲染内容。CTemplate会将这个解析后的结构缓存起来，从而避免每次渲染时都重新解析模板文件。

#### Expand函数的缓存机制
Expand函数是CTemplate用于渲染模板的主要方法。该函数通过以下步骤利用缓存机制来保证高效渲染：

1. **检查模板缓存**：
   当调用Expand函数时，它首先会检查模板缓存中是否已有解析后的模板结构。如果有，则直接使用缓存的结构进行渲染。

2. **模板解析和缓存**：
   如果模板结构尚未在缓存中，则会加载并解析模板文件。解析后的结构将被存储在缓存中，以便下次使用。这一步可以避免对同一模板文件的重复解析。

3. **渲染过程**：
   一旦模板结构被获得，Expand函数将根据提供的变量和数据进行渲染。由于模板结构已经在缓存中，不需要重新解析，因此渲染过程相对快速。

4. **自动刷新缓存**：
   虽然缓存机制非常有用，但有时模板文件可能会发生变化。CTemplate提供了一种自动刷新机制，确保在模板文件被修改后，缓存中的结构会被刷新。这样可以确保在模板发生变化后，依然能够正确渲染。

#### 结论
CTemplate的缓存机制通过缓存模板文件的内容和解析后的结构，显著提高了模板渲染的效率。Expand函数通过利用这些缓存机制，确保了网页的高效渲染。通过避免重复加载和解析模板文件，CTemplate实现了快速、可靠的模板渲染体验。


<br>
<br>
<br>




# BugFix

## 1. 编辑器内不能正确渲染“<  >”的bug
>问题：

在编辑器中的“<  >”中的内容无法正确显示，我们猜测可能是因为ctemplate采用的是纯文本的替换，而尖括号又是html的特殊字符

>如何解决的：

从`view`模块入手，在这份预设代码交给ctemplate拼接成最终的html之前，我们先对预设代码的string中的尖括号进行转义，确保在设置 `{{pre_code}}` 变量的值之前，代码中已经不存在html的特殊字符转义，以防止在页面中解析时出现意外结果。

我们在Utility中的StringUtility中加入转义函数：
```cpp
// 用来转义特殊字符，解决如果ctemplate在html中插入“<  >”导致错误
static std::string EscapeHtml(const std::string& input) 
{
   std::string output;
   for (char c : input) 
   {
         switch (c) 
         {
         case '<':
            output += "&lt;";
            break;
         case '>':
            output += "&gt;";
            break;
         default:
            output += c;
         }
   }
   return output;
}
```

>效果：

![](./ReadMePics/无法显示include名称.png)




<br>
<br>
<br>

# 项目扩展思路

1. 限制临时文件的最大数量，定期检查并删除过期文件。
2. 改成守护进程，长期运行
3. 基于注册和登陆的录题功能
4. 业务扩展，自己写一个论坛，接入到在线OJ中
5. 即便是编译服务在其他机器上，也其实是不太安全的，可以将编译服务部署在docker上
6. 目前后端compiler的服务我们使用的是http方式请求(仅仅是因为简单)，但是也可以将我们的compiler服务，设计成为远程过程调用，推荐:rest_rpc,替换我们的httplib(建议，可以不做)

