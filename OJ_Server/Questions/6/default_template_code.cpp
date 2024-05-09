#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        int x = nums[right]; // 标记最后一个位置的值
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > x) {
                left = mid + 1; // 旋转点在右侧
            } else {
                right = mid; // 旋转点在左侧或是mid
            }
        }
        return nums[left]; // 旋转点即是最小值
    }
};
