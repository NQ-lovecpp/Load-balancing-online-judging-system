#!/bin/bash

# 构建和启动整个系统
echo "构建和启动OJ系统..."
docker-compose up -d --build

# 等待所有服务启动完成
echo "等待服务启动..."
sleep 10

# 显示系统状态
echo "系统状态:"
docker-compose ps

# 访问说明
PUBLIC_IP=$(curl -s ifconfig.me)
echo ""
echo "系统已启动! 可以通过以下地址访问:"
echo "OJ系统: http://$PUBLIC_IP:13000"
echo "性能测试页面: http://$PUBLIC_IP:13000/benchmark"
echo ""
echo "若要调整实例数量，请使用 './scale.sh' 脚本"
echo "例如: ./scale.sh oj_server 3  # 调整OJ服务器实例数为3"
echo "例如: ./scale.sh compile_server 8  # 调整编译服务器实例数为8" 