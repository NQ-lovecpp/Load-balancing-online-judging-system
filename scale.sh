#!/bin/bash

# 使用说明
if [ "$#" -lt 2 ]; then
    echo "用法: $0 <服务名称> <实例数量>"
    echo "例如: $0 oj_server 3"
    echo "例如: $0 compile_server 8"
    exit 1
fi

# 获取参数
SERVICE_NAME=$1
INSTANCES=$2

# 检查服务名称是否有效
if [[ "$SERVICE_NAME" != "oj_server" && "$SERVICE_NAME" != "compile_server" ]]; then
    echo "错误: 服务名称必须是 'oj_server' 或 'compile_server'"
    exit 1
fi

# 检查实例数量是否为正整数
if ! [[ "$INSTANCES" =~ ^[1-9][0-9]*$ ]]; then
    echo "错误: 实例数量必须是正整数"
    exit 1
fi

# 项目名称前缀
PREFIX="load-balancing-online-judging-system"

# 停止现有容器
echo "停止现有 $SERVICE_NAME 容器..."
docker-compose stop $SERVICE_NAME

# 调整实例数量
echo "调整 $SERVICE_NAME 实例数量为 $INSTANCES..."
docker-compose scale $SERVICE_NAME=$INSTANCES

# 启动服务
echo "重新启动 $SERVICE_NAME 服务..."
docker-compose start $SERVICE_NAME

# 更新Nginx配置
if [ "$SERVICE_NAME" == "oj_server" ]; then
    echo "更新 OJ 服务器负载均衡配置..."
    # 生成新的server配置
    SERVERS=""
    for i in $(seq 1 $INSTANCES); do
        SERVERS="${SERVERS}    server ${PREFIX}-${SERVICE_NAME}-${i}:8888;\n"
    done

    # 更新配置文件
    sed -i "/upstream oj_servers {/,/}/c\upstream oj_servers {\n    ip_hash;\n${SERVERS}}" nginx/conf.d/default.conf
elif [ "$SERVICE_NAME" == "compile_server" ]; then
    echo "更新编译服务器负载均衡配置..."
    # 生成新的server配置
    SERVERS=""
    for i in $(seq 1 $INSTANCES); do
        SERVERS="${SERVERS}    server ${PREFIX}-${SERVICE_NAME}-${i}:8081;\n"
    done

    # 更新配置文件
    sed -i "/upstream compile_servers {/,/}/c\upstream compile_servers {\n    least_conn;\n${SERVERS}}" nginx/conf.d/compile_servers.conf
fi

# 重新加载Nginx配置
echo "重新加载Nginx配置..."
docker-compose exec nginx nginx -s reload

echo "完成! $SERVICE_NAME 已扩展到 $INSTANCES 个实例" 