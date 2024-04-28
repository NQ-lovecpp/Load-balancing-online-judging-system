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