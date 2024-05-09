#ifndef COMPILER_ONLINE
#include "default_template_code.cpp"
#endif

#include <iostream>
#include <vector>

void Test1() {
    // 输入
    vector<int> _input = {3, 4, 5, 1, 2};
    // 预期结果
    int _expected = 1;

    // 定义一个临时对象，调用成员函数
    int _result = Solution().findMin(_input);

    if (_result == _expected) {
        std::cout << "通过测试用例1，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例1，failed！" << std::endl;
    }
}

void Test2() {
    // 输入
    vector<int> _input = {4, 5, 6, 7, 0, 1, 2};
    // 预期结果
    int _expected = 0;

    // 定义一个临时对象，调用成员函数
    int _result = Solution().findMin(_input);

    if (_result == _expected) {
        std::cout << "通过测试用例2，OK！" << std::endl;
    } else {
        std::cerr << "没有通过用例2，failed！" << std::endl;
    }
}

void Test3() {
    // 输入
    vector<int> _input = {11, 13, 15, 17};
    // 预期结果
    int _expected = 11;

    // 定义一个临时对象，调用成员函数
    int _result = Solution().findMin(_input);

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
