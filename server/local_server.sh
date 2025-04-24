
show_help() {
    echo "用法: $0 [选项]"
    echo "选项:"
    echo "  -h, --help     显示帮助信息"
    echo "  -start,        开启本地相关服务"
    echo "  -stop,         关闭本地相关服务"
}
# 定义要启动的程序列表
programs=("elasticsearch" "mysql" "etcd" "redis-server" "rabbitmq-server" )

start() {
    echo "开启本地相关服务..."
    # 遍历程序列表，启动每个程序
    for program in "${programs[@]}"; do
        echo "启动 $program"
        sudo systemctl start $program
        echo "启动 $program 成功"
    done
    echo "启动本地相关服务成功"
}

stop() {
    echo "关闭本地相关服务..."
    # 遍历程序列表，启动每个程序
    for program in "${programs[@]}"; do
        echo "关闭 $program"
        sudo systemctl stop $program
        echo "关闭 $program 成功"
    done
    echo "关闭本地相关服务成功"
}


# 根据命令行参数执行相应操作
case "$1" in
    -h|--help)
        show_help
        ;;
    --stop)
        stop
        ;;
    --start)
        start
        ;;
    *)
        echo "错误: 未知选项 $1"
        show_help
        exit 1
        ;;
esac