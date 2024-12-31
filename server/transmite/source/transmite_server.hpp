// 实现语音识别子服务
#pragma once

#include <brpc/server.h>
#include <gflags/gflags.h>
#include <butil/logging.h>

#include "channel.hpp"
#include "etcd.hpp"   // 服务注册模块封装
#include "logger.hpp" // 日志模块封装
#include "rabbitmq.hpp"
#include "data_mysql.hpp"
#include "utils.hpp"

#include "base.pb.h"
#include "user.pb.h"
#include "transmite.pb.h"

namespace zht_im
{
    class MsgTransmitServiceImpl : public MsgTransmitService
    {
    private:
        // 用户子服务调用相关信息
        std::string _user_service_name;
        ServiceManager::ptr _mm_channels;

        // 聊天会话成员表的操作句柄
        ChatSessionMemberTable::ptr _mysql_session_member_table;

        // 消息队列客户端的句柄
        std::string _exchange_name;
        MQClient::ptr _mq_client;

    public:
        MsgTransmitServiceImpl(const std::string &user_service_name,
                               const ServiceManager::ptr &mm_channels,
                               const std::shared_ptr<odb::core::database> &mysql_client,
                               const std::string exchange_name,
                               MQClient::ptr &mq_client)
            : _user_service_name(user_service_name),
              _mm_channels(mm_channels),
              _mysql_session_member_table(std::make_shared<ChatSessionMemberTable>(mysql_client)),
              _exchange_name(exchange_name),
              _mq_client(mq_client)
        {
        }
        ~MsgTransmitServiceImpl() {}

        void GetTransmitTarget(google::protobuf::RpcController *controller,
                               const ::zht_im::NewMessageReq *request,
                               ::zht_im::GetTransmitTargetRsp *response,
                               ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void
            {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();

            // 1. 从请求中取出消息内容，会话 ID， 用户 ID
            std::string uid = request->user_id();
            std::string chat_ssid = request->chat_session_id();
            const MessageContent &content = request->message();
            // 2. 根据用户 ID 从用户子服务获取当前发送者用户信息
            auto channel = _mm_channels->choose(_user_service_name);
            if (!channel)
            {
                LOG_ERROR("{}-{}没有可供访问的用户子服务节点", rid, _user_service_name);
                return err_response(rid, "没有可供访问的用户子服务节点");
            }
            UserService_Stub stub(channel.get());
            GetUserInfoReq req;
            GetUserInfoRsp rsp;
            req.set_request_id(rid);
            req.set_user_id(uid);
            brpc::Controller cntl;
            stub.GetUserInfo(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("{} 用户管理子服务调用失败 {}", rid, cntl.ErrorText());
                return err_response(rid, "用户管理子服务调用失败");
            }
            // 3. 根据消息内容构造完成的消息结构（分配消息 ID，填充发送者信息，填充消息产生时间）
            MessageInfo message;
            message.set_message_id(uuid());
            message.set_chat_session_id(chat_ssid);
            message.set_timestamp(time(nullptr));
            message.mutable_sender()->CopyFrom(rsp.user_info());
            message.mutable_message()->CopyFrom(content);
            // 4. 将消息序列化后发布到 MQ 消息队列中，让消息存储子服务对消息进行持久化存储
            bool ret = _mq_client->publish(_exchange_name, message.SerializeAsString());
            if(ret == false)
            {
                LOG_ERROR("{} 持久化消息发布失败 ", rid);
                return err_response(rid, "持久化消息发布失败");
            }
            // 5. 从数据库获取目标会话所有成员 ID
            auto target_list = _mysql_session_member_table->members(chat_ssid);
            // 6. 组织响应（完整消息+目标用户 ID），发送给网关，告知网关该将消息发送给谁。
            response->set_request_id(rid);
            response->set_success(true);
            response->mutable_message()->CopyFrom(message);
            for (const auto &id : target_list)
            {
                response->add_target_id_list(id);
            }
        }
    };

    class TransmitServer
    {
    public:
        using ptr = std::shared_ptr<TransmitServer>;

    private:
        Registrant::ptr _registry_client;                   // 服务注册客户端
        Discovery::ptr _service_discovery;                  // 服务发现客户端
        std::shared_ptr<odb::core::database> _mysql_client; // MySQL数据客户端
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        TransmitServer(
            const std::shared_ptr<odb::core::database> &mysql_client, const Discovery::ptr &service_discovery,
            const Registrant::ptr &reg_client, const std::shared_ptr<brpc::Server> &rpc_server)
            : _mysql_client(mysql_client), _service_discovery(service_discovery),
              _registry_client(reg_client), _rpc_server(rpc_server)
        {
        }
        ~TransmitServer() {}

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
        }
    };
    class TransmitServerBuilder // 建造者模式
    {
    public:
        // 构造服务发现客户端&信道管理对象
        void make_discovery_object(const std::string &reg_host,
                                   const std::string &base_service_name,
                                   const std::string &user_service_name)
        {
            _user_service_name = user_service_name;
            _mm_channels = std::make_shared<ServiceManager>();
            _mm_channels->declared(_user_service_name);
            LOG_DEBUG("设置用户子服务为需添加管理的子服务: {}", _user_service_name);
            auto put_cb = std::bind(&ServiceManager::onServiceOnline, _mm_channels.get(), std::placeholders::_1, std::placeholders::_2);
            auto del_cb = std::bind(&ServiceManager::onServiceOffline, _mm_channels.get(), std::placeholders::_1, std::placeholders::_2);
            _service_discovery = std::make_shared<Discovery>(reg_host, base_service_name, put_cb, del_cb);
        }
        // 构造服务注册客户端对象
        void make_registry_object(const std::string &reg_host, const std::string &service_name, const std::string &access_host)
        {
            _registry_client = std::make_shared<Registrant>(reg_host);
            _registry_client->registry(service_name, access_host);
        }
        // 构造mysql客户端对象
        void make_mysql_object(const std::string &user, const std::string &passwd,
                               const std::string &host, const std::string &db,
                               const std::string &cset, int port, int conn_pool_count)
        {
            _mysql_client = ODBFactory::create(user, passwd, host, db, cset, port, conn_pool_count);
        }
        // 构造rabbitmq客户端对象
        void make_mq_object(const std::string &user, const std::string &passwd, const std::string &host,
                            const std::string &exchange_name, const std::string &queue_name, const std::string &binding_key)
        {
            _exchange_name = exchange_name;
            _mq_client = std::make_shared<MQClient>(user, passwd, host);
            _mq_client->declareComponents(exchange_name, queue_name, binding_key);
        }
        // 构造服务RPC服务器对象
        void make_rpc_server(uint16_t port, uint32_t timeout, uint8_t num_threads)
        {
            if (!_mq_client)
            {
                LOG_ERROR("还未初始化消息队列客户端模块");
                abort();
            }
            if (!_mm_channels)
            {
                LOG_ERROR("还未初始化信道管理模块");
                abort();
            }
            if (!_mysql_client)
            {
                LOG_ERROR("还未初始化MySQL数据库模块");
                abort();
            }

            _rpc_server = std::make_shared<brpc::Server>();

            MsgTransmitServiceImpl *transmit_service = new MsgTransmitServiceImpl(_user_service_name, _mm_channels, _mysql_client,_exchange_name, _mq_client);
            int ret = _rpc_server->AddService(transmit_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
            if (ret == -1)
            {
                LOG_ERROR("添加RPC服务失败");
                abort();
            }
            brpc::ServerOptions options;
            options.idle_timeout_sec = timeout;
            options.num_threads = num_threads;
            ret = _rpc_server->Start(port, &options);
            if (ret == -1)
            {
                LOG_ERROR("启动 rpc 服务器失败");
                abort();
            }
        }
        TransmitServer::ptr build()
        {
            if (!_service_discovery)
            {
                LOG_ERROR("还未初始化服务发现模块");
                abort();
            }
            if (!_registry_client)
            {
                LOG_ERROR("还未初始化服务注册模块");
                abort();
            }
            if (!_rpc_server)
            {
                LOG_ERROR("还未初始化RPC服务器模块");
                abort();
            }
            TransmitServer::ptr server = std::make_shared<TransmitServer>(_mysql_client, _service_discovery, _registry_client, _rpc_server);
            return server;
        }

    private:
        std::string _user_service_name;
        ServiceManager::ptr _mm_channels;

        std::string _exchange_name;
        MQClient::ptr _mq_client;

        Registrant::ptr _registry_client;                   // 服务注册客户端
        Discovery::ptr _service_discovery;                  // 服务发现客户端
        std::shared_ptr<odb::core::database> _mysql_client; // MySQL数据客户端
        std::shared_ptr<brpc::Server> _rpc_server;
    };
}