#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        // 处理边界情况
        if (nums.size() == 0) return {-1, -1};
        int begin = 0;

        // 1. 二分查找左端点
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        // 判断是否有结果
        if (nums[left] != target) {
            return {-1, -1};
        } else {
            begin = left; // 标记左端点
        }

        // 2. 二分查找右端点
        left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left + 1) / 2;
            if (nums[mid] <= target) {
                left = mid;
            } else {
                right = mid - 1;
            }
        }

        return {begin, right};
    }
};
