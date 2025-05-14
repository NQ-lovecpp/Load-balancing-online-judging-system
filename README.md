# 负载均衡在线评测系统

## 系统架构

本系统由以下几个部分组成：

- **OJ服务器(OJ_Server)**: 提供Web界面和OJ业务逻辑
- **编译服务器(Compile_Server)**: 提供代码编译和执行服务
- **Nginx负载均衡器**: 提供请求的负载均衡分发
- **Docker容器化**: 所有服务都运行在Docker容器中

### 负载均衡方案

- 外部请求通过Nginx反向代理负载均衡到多个OJ服务器实例
- OJ服务器的请求通过Nginx反向代理负载均衡到多个编译服务器实例
- 默认配置：2个OJ服务器实例，6个编译服务器实例

## 部署说明

### 前提条件

- 安装Docker和Docker Compose
- 公网IP地址可访问

### 快速启动

```bash
# 启动整个系统
./start.sh
```

### 调整实例数量

```bash
# 调整OJ服务器实例数量
./scale.sh oj_server <实例数量>

# 调整编译服务器实例数量
./scale.sh compile_server <实例数量>
```

## 访问方式

- **OJ系统**: `http://<服务器公网IP>:13000`
- **性能测试页面**: `http://<服务器公网IP>:13000/benchmark`

## 性能测试

系统提供了内置的性能测试页面，可以测试以下指标：

- **QPS(每秒查询数)**: 系统每秒处理的请求数
- **响应时间**: 请求的处理延迟
- **成功率**: 请求处理的成功比例

测试步骤：
1. 访问性能测试页面
2. 设置并发用户数和请求数
3. 输入测试代码
4. 点击"开始测试"按钮
5. 查看测试结果和图表

## 系统扩展

如需进一步扩展系统，可以：

1. 修改`docker-compose.yml`文件中的服务定义
2. 使用`scale.sh`脚本调整服务实例数量
3. 编辑Nginx配置文件修改负载均衡策略

## 排障指南

如果系统出现问题：

```bash
# 查看系统状态
docker-compose ps

# 查看服务日志
docker-compose logs oj_server
docker-compose logs compile_server
docker-compose logs nginx
``` 