#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    // 输入
    vector<int> _input = {10, 15, 20};
    // 预期结果
    int _expected = 15;

    // 定义一个临时对象，调用成员函数
    int _result = Solution().minCostClimbingStairs(_input);

    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    // 输入
    vector<int> _input = {1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
    // 预期结果
    int _expected = 6;

    // 定义一个临时对象，调用成员函数
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
}
