#!/bin/bash

# 显示帮助信息
show_help() {
    echo "用法: $0 [选项]"
    echo "选项:"
    echo "  -h, --help     显示帮助信息"
    echo "  -s, --stop     仅停止所有容器"
    echo "  -c, --clean    停止并清理所有容器和镜像"
    echo "  -b, --build    构建所有服务"
    echo "  -r, --restart  重新启动所有服务"
    echo "如果不指定选项，默认执行清理操作"
}

# 停止所有容器
stop_containers() {
    echo "停止所有运行中的容器..."
    docker-compose down
}

# 清理容器和镜像
clean_all() {
    stop_containers
    
    echo "删除所有server相关的容器..."
    docker ps -a | grep 'server' | awk '{print $1}' | xargs -r docker rm -f

    echo "删除所有server相关的镜像..."
    docker images | grep 'server' | awk '{print $1":"$2}' | xargs -r docker rmi -f

    echo "清理完成！"
}



# 构建所有服务
build_all() {
    clean_all
    
    echo "开始重新构建所有服务..."
    cd /home/ubuntu/workspace/chatsystemserver/server

    # 创建一个列表，用于存储要生成的程序所在目录
    programs=("file" "friend" "gateway" "message" "user" "transmite" "speech")

    # 遍历列表，为每个程序生成可执行文件
    for program in "${programs[@]}"; do
        echo "正在构建 ${program} 服务..."
        cd "${program}"
        pwd
        rm -rf build
        mkdir build
        cd build
        cmake ..
        make
        cd /home/ubuntu/workspace/chatsystemserver/server
    done

    echo "所有服务构建完成！"
    echo "重建完成！"
}

# 重新启动所有服务
restart_all() {
    clean_all
    echo "开始重新启动所有服务..."
    cd /home/ubuntu/workspace/chatsystemserver/server
    docker-compose up -d
    echo "所有服务重新启动完成！"
}

# 根据命令行参数执行相应操作
case "$1" in
    -h|--help)
        show_help
        ;;
    -s|--stop)
        stop_containers
        ;;
    -c|--clean)
        clean_all
        ;;
    -r|--restart)
        restart_all
        ;;
    -b|--build)
        build_all
        ;;
    "")
        # 默认执行清理操作
        clean_all
        ;;
    *)
        echo "错误: 未知选项 $1"
        show_help
        exit 1
        ;;
esac