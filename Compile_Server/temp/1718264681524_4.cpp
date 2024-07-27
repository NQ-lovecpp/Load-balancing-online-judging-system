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

#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    // 输入
    vector<int> _input = {5, 7, 7, 8, 8, 10};
    int _target = 8;
    // 预期结果
    vector<int> _expected = {3, 4};

    // 定义一个临时对象，调用成员函数
    vector<int> _result = Solution().searchRange(_input, _target);

    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    // 输入
    vector<int> _input = {5, 7, 7, 8, 8, 10};
    int _target = 6;
    // 预期结果
    vector<int> _expected = {-1, -1};

    // 定义一个临时对象，调用成员函数
    vector<int> _result = Solution().searchRange(_input, _target);

    if (_result == _expected) {
        std::cout << "通过测试用例2，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例2，failed！" << std::endl;
    }
}

void Test3() {
    // 输入
    vector<int> _input = {};
    int _target = 0;
    // 预期结果
    vector<int> _expected = {-1, -1};

    // 定义一个临时对象，调用成员函数
    vector<int> _result = Solution().searchRange(_input, _target);

    if (_result == _expected) {
        std::cout << "通过测试用例3，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例3，failed！" << std::endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();

    return 0;
}
