FROM ubuntu:20.04

# 设置时区避免安装过程中交互
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

# 安装运行时依赖和编译工具
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libjsoncpp1 \
    libpthread-stubs0-dev \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# 创建工作目录和临时目录
WORKDIR /app
RUN mkdir -p /app/temp

# 复制已编译好的可执行文件
COPY compile_server /app/

# 复制必要的目录
COPY ./wwwroot /app/wwwroot

# 设置可执行权限和权限
RUN chmod +x /app/compile_server
RUN chmod 777 -R /app/temp

# 暴露服务端口
EXPOSE 8081

# 启动编译服务器，端口号会作为参数传入
CMD ["./compile_server", "8081"] 