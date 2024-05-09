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
