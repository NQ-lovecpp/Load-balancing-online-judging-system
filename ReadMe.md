# 二、所用技术与开发环境
## 所用技术
1. C++ STL 标准库
2. Boost 准标准库(字符串切割)
3. cpp-httplib 第三方开源网络库
4. ctemplate 第三方开源前端网页渲染库
5. jsoncpp 第三方开源序列化、反序列化库
6. 负载均衡设计
7. 多进程、多线程
8. MySQL C connect
9. Ace前端在线编辑器(了解)
10. html/css/js/jquery/ajax (了解)

## 开发环境
- CentOS 7 云服务器
- vscode
- DataGrip

# 三、项目宏观结构
我们的项目核心是三个模块：
1. Common : 公共模块
2. Compile_Server : 编译与运行模块
3. OJ_Server : 获取题目列表，查看题目编写题目界面，负载均衡，其他功能

## I. 风格：仿leetcode

只实现类似leetcode 的题目列表+在线编程功能

## II. 结构：Browser-Server模式
![](/ReadMePics/BS模式.png)

## III. 编写思路
1. 先编写compile_server
2. oj_server
3. version1 基于文件版的在线OJ
4. 前端的页面设计
5. version2 基于MySQL 版的在线OJ

<br>
<br>
<br>

# 四、关于Git分支管理

设计了一个 git 分支管理结构和分支命名风格的建议。这套结构以明确的分支策略、基于不同阶段的版本管理和团队协作的流程为基础。 

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


# 4. Compiler_Server - 编译服务设计
提供的服务：编译并运行代码，得到格式化的相关的结果：
![编译服务](/ReadMePics/编译服务.png)

## 编译功能 (Compiler.hpp)

## 日志功能 (Log.hpp)

## 运行功能 (Runner.hpp)

## 测试资源限制功能

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
1. 内存申请失败，进程收到6号SIGABRT信号：![](./ReadMePics/内存资源限制.png)
2. CPU使用超时，收到24号SIGXCPU信号：![](./ReadMePics/CPU时间限制.png)



## 编译并运行 (Compile_And_Run.hpp)