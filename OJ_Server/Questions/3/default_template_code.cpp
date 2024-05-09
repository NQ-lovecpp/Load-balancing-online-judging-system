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
