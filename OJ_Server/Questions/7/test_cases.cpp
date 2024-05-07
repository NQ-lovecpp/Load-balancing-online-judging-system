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
