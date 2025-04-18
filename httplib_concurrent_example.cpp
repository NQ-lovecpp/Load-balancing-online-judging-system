#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include "./Common/httplib.h"

// 演示如何使用httplib实现POST请求的并发执行

class HttpClientPool {
public:
    // 构造函数，初始化线程池大小
    HttpClientPool(size_t thread_count = std::thread::hardware_concurrency()) 
        : thread_pool_(thread_count) {
        std::cout << "创建线程池，线程数: " << thread_count << std::endl;
    }

    // 使用线程池提交并发POST请求
    std::vector<httplib::Result> post_concurrent(
        const std::string& host, 
        int port, 
        const std::vector<std::string>& paths,
        const std::vector<std::string>& bodies,
        const std::string& content_type) {
        
        // 确保paths和bodies大小一致
        if (paths.size() != bodies.size()) {
            throw std::runtime_error("paths和bodies数量不匹配");
        }

        // 存储所有请求的future
        std::vector<std::future<httplib::Result>> futures;
        futures.reserve(paths.size());

        // 提交所有请求到线程池
        for (size_t i = 0; i < paths.size(); ++i) {
            auto future = post_async(host, port, paths[i], bodies[i], content_type);
            futures.push_back(std::move(future));
        }

        // 等待所有请求完成并收集结果
        std::vector<httplib::Result> results;
        results.reserve(futures.size());
        
        for (auto& future : futures) {
            results.push_back(future.get());
        }

        return results;
    }

    // 异步提交单个POST请求
    std::future<httplib::Result> post_async(
        const std::string& host,
        int port,
        const std::string& path,
        const std::string& body,
        const std::string& content_type) {
        
        // 使用packaged_task包装HTTP请求
        auto task = std::make_shared<std::packaged_task<httplib::Result()>>(
            [host, port, path, body, content_type]() {
                // 创建客户端
                httplib::Client client(host, port);
                
                // 设置超时时间，避免请求卡住
                client.set_connection_timeout(5, 0); // 5秒连接超时
                client.set_read_timeout(5, 0);      // 5秒读取超时
                client.set_write_timeout(5, 0);     // 5秒写入超时
                
                // 发送POST请求
                return client.Post(path.c_str(), body, content_type.c_str());
            }
        );

        // 获取future
        std::future<httplib::Result> future = task->get_future();
        
        // 将任务提交到线程池
        thread_pool_.enqueue([task]() {
            (*task)();
        });
        
        return future;
    }

    // 另一种方法：直接使用std::thread创建多个线程执行并发请求
    static std::vector<httplib::Result> post_concurrent_with_threads(
        const std::string& host, 
        int port, 
        const std::vector<std::string>& paths,
        const std::vector<std::string>& bodies,
        const std::string& content_type) {
        
        // 确保paths和bodies大小一致
        if (paths.size() != bodies.size()) {
            throw std::runtime_error("paths和bodies数量不匹配");
        }

        std::vector<httplib::Result> results(paths.size());
        std::vector<std::thread> threads;
        
        // 创建多个线程执行请求
        for (size_t i = 0; i < paths.size(); ++i) {
            threads.emplace_back([&host, port, &paths, &bodies, &content_type, &results, i]() {
                httplib::Client client(host, port);
                client.set_connection_timeout(5, 0);
                client.set_read_timeout(5, 0);
                client.set_write_timeout(5, 0);
                
                results[i] = client.Post(paths[i].c_str(), bodies[i], content_type.c_str());
            });
        }
        
        // 等待所有线程完成
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        
        return results;
    }

private:
    httplib::ThreadPool thread_pool_;
};

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
        
        // 方法1：使用线程池处理并发请求
        std::cout << "使用线程池处理并发请求..." << std::endl;
        HttpClientPool client_pool(8); // 创建8线程的线程池
        
        auto results = client_pool.post_concurrent(
            host, port, paths, bodies, content_type
        );
        
        // 输出结果
        std::cout << "线程池并发请求结果:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            if (results[i]) {
                std::cout << "请求 " << i + 1 << " 成功: " 
                          << results[i]->status << std::endl;
            } else {
                std::cout << "请求 " << i + 1 << " 失败!" << std::endl;
            }
        }
        
        // 方法2：使用std::thread处理并发请求
        std::cout << "\n使用std::thread处理并发请求..." << std::endl;
        auto thread_results = HttpClientPool::post_concurrent_with_threads(
            host, port, paths, bodies, content_type
        );
        
        // 输出结果
        std::cout << "线程并发请求结果:" << std::endl;
        for (size_t i = 0; i < thread_results.size(); ++i) {
            if (thread_results[i]) {
                std::cout << "请求 " << i + 1 << " 成功: " 
                          << thread_results[i]->status << std::endl;
            } else {
                std::cout << "请求 " << i + 1 << " 失败!" << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 