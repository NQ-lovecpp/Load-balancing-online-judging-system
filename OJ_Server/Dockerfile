FROM ubuntu:20.04

# 设置时区避免安装过程中交互
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

# 安装基本运行时依赖
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libjsoncpp1 \
    libmysqlclient21 \
    libpthread-stubs0-dev \
    && rm -rf /var/lib/apt/lists/*

# 创建工作目录
WORKDIR /app

# 创建lib目录
RUN mkdir -p /app/lib

# 复制已编译好的可执行文件
COPY ./oj_server /app/

# 复制必要的库文件
COPY ./lib/libctemplate.so.3 /usr/local/lib/
RUN ldconfig

# 复制必要的目录
COPY ./conf /app/conf
COPY ./Questions /app/Questions
COPY ./template_html /app/template_html
COPY ./wwwroot /app/wwwroot

# 设置可执行权限
RUN chmod +x /app/oj_server

# 暴露服务端口
EXPOSE 8888

# 启动OJ服务器
CMD ["./oj_server"] 