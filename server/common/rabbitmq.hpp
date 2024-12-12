#pragma once
#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>

#include "logger.hpp"

namespace zht_im
{
    class MQClient
    {
#define DEFAULT_ROUTING_KEY "routing_key"
    private:
        struct ev_async _async_watcher;
        struct ev_loop *_loop;
        std::unique_ptr<AMQP::LibEvHandler> _handler;
        std::unique_ptr<AMQP::TcpConnection> _connection;
        std::unique_ptr<AMQP::TcpChannel> _channel;
        std::thread _loop_thread;

    private:
        static void watcher_callback(struct ev_loop *loop, ev_async *watcher, int32_t revents)
        {
            ev_break(loop, EVBREAK_ALL);
        }

    public:
        using MessageCallback = std::function<void(const char *, size_t)>;
        MQClient(const std::string &user, const std::string passwd, const std::string host)
        {
            _loop = EV_DEFAULT;
            _handler = std::make_unique<AMQP::LibEvHandler>(_loop);
            std::string url = "amqp://" + user + ":" + passwd + "@" + host + "/";
            AMQP::Address address(url);
            _connection = std::make_unique<AMQP::TcpConnection>(_handler.get(), address);
            _channel = std::make_unique<AMQP::TcpChannel>(_connection.get());
            _loop_thread = std::thread([this]()
                                       { ev_run(_loop, 0); });
        }
        ~MQClient()
        {
            ev_async_init(&_async_watcher, watcher_callback);
            ev_async_start(_loop, &_async_watcher);
            ev_async_send(_loop, &_async_watcher);
            _loop_thread.join();
            _loop = nullptr;
        }
        void declareComponents(const std::string &exchange, const std::string &queue,
                               const std::string &routing_key = DEFAULT_ROUTING_KEY, int32_t exchage_type = AMQP::ExchangeType::direct)
        {
            _channel->declareExchange(exchange, AMQP::ExchangeType::direct)
                .onError([](const char *message)
                         {
                LOG_ERROR("声明交换机失败: {}" , message);
                exit(0); })
                .onSuccess([exchange]()
                           { LOG_INFO("{} 交换机创建成功", exchange); });
            // 5. 声明队列
            _channel->declareQueue(queue)
                .onError([](const char *message)
                         {
                LOG_ERROR("声明队列失败: {}" , message);
                exit(0); })
                .onSuccess([queue]()
                           { LOG_INFO("{} 队列创建成功", queue); });
            // 6. 针对交换机和队列进行绑定
            _channel->bindQueue(exchange, queue, routing_key)
                .onError([exchange, queue](const char *message)
                         {
                LOG_ERROR("{} - {}绑定失败:" , exchange, queue);
                exit(0); })
                .onSuccess([exchange, queue]()
                           { LOG_INFO("{} - {}绑定成功", exchange, queue); });
        }
        bool publish(const std::string &exchage, const std::string &msg, const std::string &routing_key = DEFAULT_ROUTING_KEY)
        {
            bool ret = _channel->publish(exchage, routing_key, msg);
            if (ret == false)
            {
                LOG_ERROR("{} 发布消息失败", exchage);
                return false;
            }
            return true;
        }
        void consume(const std::string &queue, const MessageCallback &cb, const std::string &tag = "consume-tag")
        {
            _channel->consume(queue, tag)
                .onReceived([this, &cb](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
                            {
                cb(message.body(), message.bodySize());
                _channel->ack(deliveryTag); })
                .onError([queue](const char *message)
                         {
                LOG_ERROR("订阅 {} 队列消息失败: {}", queue, message);
                exit(0); });
        }
    };
}