#include <iostream>
#include <vector>

using namespace std;

class Solution 
{
public:
    int tribonacci(int n) 
    {
        if (n == 0 || n == 1) return n;
        vector<int> dp(n + 1); // dp[i] 表示：第 i 个泰波那契数的值。
        dp[0] = 0, dp[1] = 1, dp[2] = 1; // 初始化
        // 从左往右填表
        for (int i = 3; i <= n; i++) 
        {
            dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
        }
        // 返回结果
        return dp[n];
    }
};

#ifndef COMPILER_ONLINE
#include "./default_template_code.cpp"
#endif


#include <iostream>
#include <vector>
#include <algorithm>

void Test1()
{
    // 输入
    int _x = 121;
    // 预期结果
    bool _result = true;


    // 定义一个临时对象，调用成员函数
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
    {
        std::cout << "通过测试用例1，OK！" << std::endl;
    }
    else
    {
        std::cerr << "没有通过用例1，Falied！" << std::endl; 
    }
}

void Test2()
{
    // 输入
    int _x = 10;
    // 预期结果
    bool _result = false;


    // 定义一个临时对象，调用成员函数
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
    {
        std::cout << "通过测试用例1，OK！" << std::endl;
    }
    else
    {
        std::cerr << "没有通过用例1，Falied！" << std::endl; 
    }
}


int main()
{
    Test1();
    Test2();
    
    return 0;
}
