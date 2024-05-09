#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution 
{
public:
    int minCostClimbingStairs(vector<int>& cost) 
    {
        int n = cost.size();
        // 初始化一个 dp表
        vector<int> dp(n + 1, 0);
        // 初始化
        dp[0] = dp[1] = 0;
        // 填表
        for (int i = 2; i <= n; i++) 
        {
            dp[i] = min(cost[i - 1] + dp[i - 1], cost[i - 2] + dp[i - 2]);
        }
        // 返回结果
        return dp[n];
    }
};
