#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <future>
#include <memory>
#include <stdexcept>
#include <functional>
#include "./Common/httplib.h"

// 定义请求信息结构体
struct RequestInfo {
    std::string path;
    std::string body;
    std::string content_type;
    int timeout_seconds;
};

// 定义响应结果结构体
struct ResponseResult {
    bool success;
    int status_code;
    std::string response_body;
    std::string error_message;
    std::chrono::milliseconds response_time;
};

class ConcurrentHttpClient {
public:
    // 构造函数，初始化HTTP客户端配置
    ConcurrentHttpClient(const std::string& host, int port, 
                         size_t thread_count = std::thread::hardware_concurrency())
        : host_(host), port_(port), thread_pool_(thread_count) {
        std::cout << "初始化并发HTTP客户端: " << host << ":" << port 
                  << "，线程数: " << thread_count << std::endl;
    }
    
    // 执行批量并发POST请求
    std::vector<ResponseResult> post_concurrent(const std::vector<RequestInfo>& requests) {
        std::vector<std::future<ResponseResult>> futures;
        futures.reserve(requests.size());
        
        // 提交所有请求到线程池
        for (const auto& req : requests) {
            futures.push_back(post_async(req));
        }
        
        // 等待所有请求完成
        std::vector<ResponseResult> results;
        results.reserve(futures.size());
        
        for (auto& future : futures) {
            try {
                results.push_back(future.get());
            } catch (const std::exception& e) {
                // 捕获任何异常并记录为失败的响应
                ResponseResult error_result;
                error_result.success = false;
                error_result.status_code = -1;
                error_result.error_message = std::string("未捕获的异常: ") + e.what();
                results.push_back(error_result);
            }
        }
        
        return results;
    }
    
    // 异步发送单个POST请求
    std::future<ResponseResult> post_async(const RequestInfo& req) {
        auto task = std::make_shared<std::packaged_task<ResponseResult()>>(
            [this, req]() {
                return this->perform_post_request(req);
            }
        );
        
        std::future<ResponseResult> future = task->get_future();
        
        // 提交到线程池
        thread_pool_.enqueue([task]() {
            (*task)();
        });
        
        return future;
    }
    
    // 关闭客户端，停止线程池
    void shutdown() {
        thread_pool_.shutdown();
    }
    
private:
    std::string host_;
    int port_;
    httplib::ThreadPool thread_pool_;
    
    // 执行实际的POST请求
    ResponseResult perform_post_request(const RequestInfo& req) {
        ResponseResult result;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        try {
            // 创建客户端
            httplib::Client client(host_, port_);
            
            // 设置超时
            int timeout = (req.timeout_seconds > 0) ? req.timeout_seconds : 10;
            client.set_connection_timeout(timeout, 0);
            client.set_read_timeout(timeout, 0);
            client.set_write_timeout(timeout, 0);
            
            // 执行POST请求
            auto http_result = client.Post(
                req.path.c_str(), 
                req.body, 
                req.content_type.c_str()
            );
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.response_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time
            );
            
            // 处理结果
            if (http_result) {
                // 请求成功
                result.success = true;
                result.status_code = http_result->status;
                result.response_body = http_result->body;
                
                // 检查HTTP状态码是否表示成功
                if (http_result->status < 200 || http_result->status >= 300) {
                    result.error_message = "HTTP错误状态码: " + std::to_string(http_result->status);
                }
            } else {
                // 请求失败
                result.success = false;
                result.status_code = -1;
                result.error_message = "HTTP请求失败: " + 
                    httplib::to_string(http_result.error());
            }
        } catch (const std::exception& e) {
            // 捕获任何异常
            auto end_time = std::chrono::high_resolution_clock::now();
            result.response_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time
            );
            
            result.success = false;
            result.status_code = -1;
            result.error_message = std::string("请求异常: ") + e.what();
        }
        
        return result;
    }
};

// 打印结果统计信息
void print_results_summary(const std::vector<ResponseResult>& results) {
    int success_count = 0;
    int failure_count = 0;
    long total_time = 0;
    
    for (const auto& result : results) {
        if (result.success && result.status_code >= 200 && result.status_code < 300) {
            success_count++;
        } else {
            failure_count++;
        }
        total_time += result.response_time.count();
    }
    
    std::cout << "请求结果统计:" << std::endl;
    std::cout << "  总请求数: " << results.size() << std::endl;
    std::cout << "  成功请求: " << success_count << std::endl;
    std::cout << "  失败请求: " << failure_count << std::endl;
    
    if (!results.empty()) {
        std::cout << "  平均响应时间: " << (total_time / results.size()) << "ms" << std::endl;
    }
}

// 示例：如何使用ConcurrentHttpClient
int main() {
    try {
        // 配置HTTP客户端
        std::string host = "localhost";
        int port = 8080;
        ConcurrentHttpClient client(host, port, 10);  // 10线程
        
        // 准备一批请求
        std::vector<RequestInfo> requests;
        
        // 添加一些示例请求
        for (int i = 1; i <= 20; i++) {
            RequestInfo req;
            req.path = "/api/items/" + std::to_string(i);
            req.body = "{\"id\": " + std::to_string(i) + ", \"action\": \"update\"}";
            req.content_type = "application/json";
            req.timeout_seconds = 5;  // 5秒超时
            requests.push_back(req);
        }
        
        std::cout << "开始执行 " << requests.size() << " 个并发POST请求..." << std::endl;
        
        // 记录开始时间
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // 执行并发请求
        auto results = client.post_concurrent(requests);
        
        // 记录结束时间
        auto end_time = std::chrono::high_resolution_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time
        ).count();
        
        std::cout << "所有请求已完成，总耗时: " << total_time << "ms" << std::endl;
        
        // 打印结果统计
        print_results_summary(results);
        
        // 打印各个请求的详细信息
        std::cout << "\n各请求详情:" << std::endl;
        for (size_t i = 0; i < results.size(); i++) {
            const auto& result = results[i];
            std::cout << "请求 #" << (i + 1) << " (" << requests[i].path << "):" << std::endl;
            std::cout << "  状态: " << (result.success ? "成功" : "失败") << std::endl;
            std::cout << "  状态码: " << result.status_code << std::endl;
            std::cout << "  响应时间: " << result.response_time.count() << "ms" << std::endl;
            
            if (!result.error_message.empty()) {
                std::cout << "  错误信息: " << result.error_message << std::endl;
            }
            
            // 如果有响应体，只打印一部分
            if (!result.response_body.empty()) {
                std::string preview = result.response_body.substr(0, 100);
                if (result.response_body.length() > 100) {
                    preview += "...";
                }
                std::cout << "  响应预览: " << preview << std::endl;
            }
            
            std::cout << std::endl;
        }
        
        // 关闭客户端
        client.shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "程序错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 