INSERT INTO `oj_questions`
  (`number`, `title`, `star`, `description`, `test_cases`, `cpu_limit`, `mem_limit`)
VALUES
(1,
 '【入门】两数之和',
 '简单',
 '#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution 
{
public:
    vector<int> twoSum(vector<int>& nums, int target) 
    {
        // 哈希表可以很快的知道：
        // 1. 有没有存（key）
        // 2. 存了多少（value）
        unordered_map<int, int> HashTable; // <nums[i], i>
        for(int i = 0; i < nums.size(); i++)
        {
            if(HashTable.count(target - nums[i]) != 0)
            {
                return {HashTable[target - nums[i]], i};
            }
            HashTable[nums[i]] = i; 
        }
        return {-1, -1};
    }
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1()
{
    // 输入
    int n = 3;
    int V = 5;
    vector<int> v = {2, 4, 1};
    vector<int> w = {10, 5, 4};
    // 预期结果
    pair<int, int> _expected = {14, 9};

    pair<int, int> _result = Solution().knapsack(n, V, v, w);
    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2()
{
    // 输入
    int n = 3;
    int V = 8;
    vector<int> v = {12, 11, 6};
    vector<int> w = {6, 8, 8};
    // 预期结果
    pair<int, int> _expected = {8, 0};

    pair<int, int> _result = Solution().knapsack(n, V, v, w);
    if (_result == _expected) {
        std::cout << "通过测试用例2，OK！" << std::endl;
    } else {
        std::cerr << "没有通过测试用例2，failed！" << std::endl;
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}',
 1, 30000),

(2,
 '【入门】回文数',
 '简单',
 '#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution 
{
public:
    bool isPalindrome(int x) 
    {

    }
};',
 '#ifndef COMPILER_ONLINE
#include "./default_template_code.cpp"
#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1()
{
    int _x = 121;
    bool _result = true;
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
        std::cout << "通过测试用例1，OK！" << std::endl;
    else
        std::cerr << "没有通过用例1，Falied！" << std::endl; 
}

void Test2()
{
    int _x = 10;
    bool _result = false;
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
        std::cout << "通过测试用例2，OK！" << std::endl;
    else
        std::cerr << "没有通过用例2，Falied！" << std::endl; 
}

int main()
{
    Test1();
    Test2();
    return 0;
}',
 1, 30000),

(3,
 '【动态规划】第N个泰波那契数',
 '简单',
 '#include <iostream>
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
        for (int i = 3; i <= n; i++) 
        {
            dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
        }
        return dp[n];
    }
};',
 '#ifndef COMPILER_ONLINE
#include "./default_template_code.cpp"
#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1()
{
    int _x = 121;
    bool _result = true;
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
        std::cout << "通过测试用例1，OK！" << std::endl;
    else
        std::cerr << "没有通过用例1，Falied！" << std::endl; 
}

void Test2()
{
    int _x = 10;
    bool _result = false;
    bool _ret = Solution().isPalindrome(_x);
    if(_ret == _result)
        std::cout << "通过测试用例2，OK！" << std::endl;
    else
        std::cerr << "没有通过用例2，Falied！" << std::endl; 
}

int main()
{
    Test1();
    Test2();
    return 0;
}',
 1, 30000),

(4,
 '【动态规划】使用最小花费爬楼梯',
 '简单',
 '#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution 
{
public:
    int minCostClimbingStairs(vector<int>& cost) 
    {
        int n = cost.size();
        vector<int> dp(n + 1, 0);
        dp[0] = dp[1] = 0;
        for (int i = 2; i <= n; i++) 
        {
            dp[i] = min(cost[i - 1] + dp[i - 1], cost[i - 2] + dp[i - 2]);
        }
        return dp[n];
    }
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    vector<int> _input = {10, 15, 20};
    int _expected = 15;
    int _result = Solution().minCostClimbingStairs(_input);
    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    vector<int> _input = {1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
    int _expected = 6;
    int _result = Solution().minCostClimbingStairs(_input);
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
}',
 1, 30000),

(5,
 '【双指针】在排序数组中查找元素的第一个和最后一个位置',
 '中等',
 '#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.size() == 0) return {-1, -1};
        int begin = 0;
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) left = mid + 1;
            else right = mid;
        }
        if (nums[left] != target) return {-1, -1};
        begin = left;
        left = 0; right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left + 1) / 2;
            if (nums[mid] <= target) left = mid;
            else right = mid - 1;
        }
        return {begin, right};
    }
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    vector<int> _input = {5, 7, 7, 8, 8, 10};
    int _target = 8;
    vector<int> _expected = {3, 4};
    vector<int> _result = Solution().searchRange(_input, _target);
    if (_result == _expected) std::cout << "通过测试用例1，OK！" << std::endl;
    else std::cerr << "没有通过用例1，failed！" << std::endl;
}

void Test2() {
    vector<int> _input = {5, 7, 7, 8, 8, 10};
    int _target = 6;
    vector<int> _expected = {-1, -1};
    vector<int> _result = Solution().searchRange(_input, _target);
    if (_result == _expected) std::cout << "通过测试用例2，OK！" << std::endl;
    else std::cerr << "没有通过用例2，failed！" << std::endl;
}

void Test3() {
    vector<int> _input = {};
    int _target = 0;
    vector<int> _expected = {-1, -1};
    vector<int> _result = Solution().searchRange(_input, _target);
    if (_result == _expected) std::cout << "通过测试用例3，OK！" << std::endl;
    else std::cerr << "没有通过用例3，failed！" << std::endl;
}

int main() {
    Test1(); Test2(); Test3();
    return 0;
}',
 1, 30000),

(6,
 '【双指针】搜索旋转排序数组中的最小值',
 '中等',
 '#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        int x = nums[right];
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > x) left = mid + 1;
            else right = mid;
        }
        return nums[left];
    }
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    vector<int> _input = {3, 4, 5, 1, 2};
    int _expected = 1;
    int _result = Solution().findMin(_input);
    if (_result == _expected) std::cout << "通过测试用例1，OK！" << std::endl;
    else std::cerr << "没有通过用例1，failed！" << std::endl;
}

void Test2() {
    vector<int> _input = {4, 5, 6, 7, 0, 1, 2};
    int _expected = 0;
    int _result = Solution().findMin(_input);
    if (_result == _expected) std::cout << "通过测试用例2，OK！" << std::endl;
    else std::cerr << "没有通过用例2，failed！" << std::endl;
}

void Test3() {
    vector<int> _input = {11, 13, 15, 17};
    int _expected = 11;
    int _result = Solution().findMin(_input);
    if (_result == _expected) std::cout << "通过测试用例3，OK！" << std::endl;
    else std::cerr << "没有通过用例3，failed！" << std::endl;
}

int main() {
    Test1(); Test2(); Test3();
    return 0;
}',
 1, 30000),

(7,
 '【背包问题】01背包',
 '中等',
 '#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
public:
    pair<int, int> knapsack(int n, int V, vector<int>& v, vector<int>& w) {
        const int N = 1010;
        int dp[N][N];
        
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
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    int n = 3;
    int V = 5;
    vector<int> v = {2, 4, 1};
    vector<int> w = {10, 5, 4};
    pair<int, int> _expected = {14, 9};
    pair<int, int> _result = Solution().knapsack(n, V, v, w);
    if (_result == _expected) std::cout << "通过测试用例1，OK！" << std::endl;
    else std::cerr << "没有通过测试用例1，failed！" << std::endl;
}

void Test2() {
    int n = 3;
    int V = 8;
    vector<int> v = {12, 11, 6};
    vector<int> w = {6, 8, 8};
    pair<int, int> _expected = {8, 0};
    pair<int, int> _result = Solution().knapsack(n, V, v, w);
    if (_result == _expected) std::cout << "通过测试用例2，OK！" << std::endl;
    else std::cerr << "没有通过测试用例2，failed！" << std::endl;
}

int main() {
    Test1(); Test2();
    return 0;
}',
 2, 100000),

(8,
 '【DFS】全排列',
 '中等',
 '#include <iostream>
#include <vector>

using namespace std;

class Solution {
    vector<vector<int>> ret;
    vector<int> path;
    bool check[7] = {false};
    
public:
    vector<vector<int>> permute(vector<int>& nums) {
        dfs(nums);
        return ret;
    }

    void dfs(vector<int>& nums) {
        if (path.size() == nums.size()) {
            ret.push_back(path);
            return;
        }

        for (int i = 0; i < nums.size(); i++) {
            if (!check[i]) {
                path.push_back(nums[i]);
                check[i] = true;
                dfs(nums);
                path.pop_back();
                check[i] = false;
            }
        }
    }
};',
 '#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1() {
    vector<int> _input = {1, 2, 3};
    vector<vector<int>> _expected = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};
    vector<vector<int>> _result = Solution().permute(_input);
    std::sort(_expected.begin(), _expected.end());
    std::sort(_result.begin(), _result.end());
    if (_result == _expected) std::cout << "通过测试用例1，OK！" << std::endl;
    else std::cerr << "没有通过用例1，failed！" << std::endl;
}

void Test2() {
    vector<int> _input = {0, 1};
    vector<vector<int>> _expected = {{0, 1}, {1, 0}};
    vector<vector<int>> _result = Solution().permute(_input);
    std::sort(_expected.begin(), _expected.end());
    std::sort(_result.begin(), _result.end());
    if (_result == _expected) std::cout << "通过测试用例2，OK！" << std::endl;
    else std::cerr << "没有通过用例2，failed！" << std::endl;
}

void Test3() {
    vector<int> _input = {1};
    vector<vector<int>> _expected = {{1}};
    vector<vector<int>> _result = Solution().permute(_input);
    if (_result == _expected) std::cout << "通过测试用例3，OK！" << std::endl;
    else std::cerr << "没有通过用例3，failed！" << std::endl;
}

int main() {
    Test1(); Test2(); Test3();
    return 0;
};',
 3, 100000);
