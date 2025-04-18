#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include "./Common/httplib.h"

// 演示如何直接使用httplib内置的ThreadPool实现POST请求的并发执行

int main() {
    try {
        const std::string host = "localhost";
        const int port = 8080;
        const std::string content_type = "application/json";
        
        // 准备多个请求
        std::vector<std::string> paths = {
            "/api/test1",
            "/api/test2",
            "/api/test3",
            "/api/test4",
            "/api/test5"
        };
        
        std::vector<std::string> bodies = {
            "{\"id\": 1, \"name\": \"测试1\"}",
            "{\"id\": 2, \"name\": \"测试2\"}",
            "{\"id\": 3, \"name\": \"测试3\"}",
            "{\"id\": 4, \"name\": \"测试4\"}",
            "{\"id\": 5, \"name\": \"测试5\"}"
        };
        
        // 创建httplib的ThreadPool
        // CPPHTTPLIB_THREAD_POOL_COUNT宏定义了默认线程数量
        // 通常是系统线程数-1或至少8个线程
        const int thread_count = 8;
        httplib::ThreadPool thread_pool(thread_count);
        std::cout << "创建线程池，线程数: " << thread_count << std::endl;
        
        // 用于存储任务完成状态的promise和future
        std::vector<std::promise<httplib::Result>> promises(paths.size());
        std::vector<std::future<httplib::Result>> futures;
        
        for (size_t i = 0; i < paths.size(); ++i) {
            futures.push_back(promises[i].get_future());
            
            // 提交任务到线程池
            thread_pool.enqueue([host, port, path=paths[i], body=bodies[i], 
                                content_type, &promises, i] {
                // 在线程池中执行HTTP请求
                httplib::Client client(host, port);
                
                // 设置超时
                client.set_connection_timeout(5, 0);
                client.set_read_timeout(5, 0);
                client.set_write_timeout(5, 0);
                
                // 执行POST请求
                auto result = client.Post(path.c_str(), body, content_type.c_str());
                
                // 设置结果
                promises[i].set_value(result);
            });
        }
        
        // 收集所有结果
        std::vector<httplib::Result> results;
        for (auto& future : futures) {
            results.push_back(future.get());
        }
        
        // 输出结果
        std::cout << "并发请求结果:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            if (results[i]) {
                std::cout << "请求 " << i + 1 << " 成功: " 
                          << results[i]->status << std::endl;
            } else {
                std::cout << "请求 " << i + 1 << " 失败!" << std::endl;
            }
        }
        
        // 关闭线程池
        thread_pool.shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 