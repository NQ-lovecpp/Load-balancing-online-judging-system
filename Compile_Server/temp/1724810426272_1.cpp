#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
public:
    pair<int, int> knapsack(int n, int V, vector<int>& v, vector<int>& w) {
        const int N = 1010;
        int dp[N][N];
        
        // 解决第一问：背包至多能装多大价值的物品
        memset(dp, 0, sizeof(dp));
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= V; j++) {
                dp[i][j] = dp[i - 1][j];
                if (j >= v[i - 1]) {
                    dp[i][j] = max(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
                }
            }
        }
        int max_value = dp[n][V];

        // 解决第二问：背包恰好装满，至多能装多大价值的物品
        memset(dp, 0, sizeof(dp));
        for (int j = 1; j <= V; j++) {
            dp[0][j] = -1;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= V; j++) {
                dp[i][j] = dp[i - 1][j];
                if (j >= v[i - 1] && dp[i - 1][j - v[i - 1]] != -1) {
                    dp[i][j] = max(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
                }
            }
        }
        int exact_value = dp[n][V] == -1 ? 0 : dp[n][V];

        return {max_value, exact_value};
    }
};

#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    // 输入
    int n = 3;
    int V = 5;
    vector<int> v = {2, 4, 1};
    vector<int> w = {10, 5, 4};
    // 预期结果
    pair<int, int> _expected = {14, 9};

    // 定义一个临时对象，调用成员函数
    pair<int, int> _result = Solution().knapsack(n, V, v, w);

    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    // 输入
    int n = 3;
    int V = 8;
    vector<int> v = {12, 11, 6};
    vector<int> w = {6, 8, 8};
    // 预期结果
    pair<int, int> _expected = {8, 0};

    // 定义一个临时对象，调用成员函数
    pair<int, int> _result = Solution().knapsack(n, V, v, w);

    if (_result == _expected) {
        std::cout << "通过测试用例2，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例2，failed！" << std::endl;
    }
}

int main() {
    Test1();
    Test2();

    return 0;
}
