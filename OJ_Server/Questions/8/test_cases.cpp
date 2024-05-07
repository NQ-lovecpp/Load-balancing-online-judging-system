#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>
#include <algorithm>

void Test1() {
    // 输入
    vector<int> _input = {1, 2, 3};
    // 预期结果
    vector<vector<int>> _expected = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};

    // 定义一个临时对象，调用成员函数
    vector<vector<int>> _result = Solution().permute(_input);

    // 比较结果
    std::sort(_expected.begin(), _expected.end());
    std::sort(_result.begin(), _result.end());

    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    // 输入
    vector<int> _input = {0, 1};
    // 预期结果
    vector<vector<int>> _expected = {{0, 1}, {1, 0}};

    // 定义一个临时对象，调用成员函数
    vector<vector<int>> _result = Solution().permute(_input);

    // 比较结果
    std::sort(_expected.begin(), _expected.end());
    std::sort(_result.begin(), _result.end());

    if (_result == _expected) {
        std::cout << "通过测试用例2，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例2，failed！" << std::endl;
    }
}

void Test3() {
    // 输入
    vector<int> _input = {1};
    // 预期结果
    vector<vector<int>> _expected = {{1}};

    // 定义一个临时对象，调用成员函数
    vector<vector<int>> _result = Solution().permute(_input);

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
