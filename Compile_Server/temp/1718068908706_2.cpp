#include <iostream>
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
};

#ifndef COMPILER_ONLINE

#include "default_template_code.cpp"

#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1()
{
    // 输入
    vector<int> _arr = {2, 7, 11, 15};
    int _target = 9;
    // 预期结果
    vector<int> _result = {0, 1};


    // 定义一个临时对象，调用成员函数
    vector<int> _ret_vector = Solution().twoSum(_arr, _target);
    std::sort(_ret_vector.begin(), _ret_vector.end());
    if(std::equal(_ret_vector.begin(), _ret_vector.end(), _result.begin()))
    {
        std::cout << "通过测试用例1，OK！" << std::endl;
    }
    else
    {
        std::cerr << "没有通过用例1，falied！" << std::endl; 
    }
}

void Test2()
{
    // 输入
    vector<int> _arr = {3, 2, 4};
    int _target = 6;
    // 预期结果
    vector<int> _result = {1, 2};


    // 定义一个临时对象，调用成员函数
    vector<int> _ret_vector = Solution().twoSum(_arr, _target);
    std::sort(_ret_vector.begin(), _ret_vector.end());
    if(std::equal(_ret_vector.begin(), _ret_vector.end(), _result.begin()))
    {
        std::cout << "通过测试用例2，OK！" << std::endl;
    }
    else
    {
        std::cerr << "没有通过用例2，falied！" << std::endl; 
    }
}


int main()
{
    Test1();
    Test2();
    
    return 0;
}
