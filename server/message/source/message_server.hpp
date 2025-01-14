// 实现用户管理子服务
#pragma once

#include <brpc/server.h>
#include <butil/logging.h>

#include "data_es.hpp"       // es数据管理客户端封装
#include "mysql_message.hpp" // es数据管理客户端封装
#include "etcd.hpp"          // 服务注册模块封装
#include "logger.hpp"        // 日志模块封装
#include "utils.hpp"         // 基础工具接口
#include "channel.hpp"       // 信道管理封装
#include "rabbitmq.hpp"      // 消息队列封装

#include "message.pb.h"
#include "user.pb.h"
#include "base.pb.h"
#include "file.pb.h"

namespace zht_im
{
    class MessageServiceImpl : public MsgStorageService
    {
    public:
        using ptr = std::shared_ptr<MessageServiceImpl>;

    private:
        ESMessage::ptr _es_message;
        MessageTable::ptr _mysql_message;

        // rpc调用客户端相关对象
        std::string _file_service_name;
        std::string _user_service_name;
        ServiceManager::ptr _mm_channels;

    public:
        MessageServiceImpl(const std::string &file_service_name, const std::string &user_service_name,
                           const std::shared_ptr<elasticlient::Client> &es_client,
                           const std::shared_ptr<odb::core::database> &mysql_client,
                           const std::shared_ptr<ServiceManager> &mm_channels)
            : _es_message(std::make_shared<ESMessage>(es_client)),
              _mysql_message(std::make_shared<MessageTable>(mysql_client)),
              _file_service_name(file_service_name),
              _user_service_name(user_service_name),
              _mm_channels(mm_channels)
        {
            _es_message->createIndex();
        }
        ~MessageServiceImpl() {}

        void GetHistoryMsg(google::protobuf::RpcController *controller,
                           const ::zht_im::GetHistoryMsgReq *request,
                           ::zht_im::GetHistoryMsgRsp *response,
                           ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void
            {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 从请求中，获取会话 ID， 要获取的消息的起始时间与结束时间。
            std::string rid = request->request_id();
            std::string chat_ssid = request->chat_session_id();
            boost::posix_time::ptime stime = boost::posix_time::from_time_t(request->start_time());
            boost::posix_time::ptime etime = boost::posix_time::from_time_t(request->over_time());
            // 2. 访问数据库，从数据库中按时间进行范围查询，获取消息简略信息（消息 ID，会话 ID，消息类型，产生时间，发送者用户 ID，文本消息内容，文件消息元信息）
            auto msg_list = _mysql_message->range(chat_ssid, stime, etime);
            // 3. 组织消息中所有文件消息的文件ID，并从文件子服务中进行下载
            std::unordered_set<std::string> file_id_lists;
            for (const auto &msg : msg_list)
            {
                if (!msg.file_id())
                    continue;
                if (msg.file_id())
                    file_id_lists.insert(*msg.file_id());
            }
            std::unordered_map<std::string, std::string> file_data_list;
            bool ret = _GetFile(rid, file_id_lists, file_data_list);
            if (ret == false)
            {
                LOG_ERROR("{} 批量文件数据下载失败", rid);
                return err_response(rid, "批量文件数据下载失败");
            }
            // 4. 组织消息中所有消息的的用户ID，并从用户子服务中下载用户信息
            std::unordered_set<std::string> user_id_lists;
            for (const auto &msg : msg_list)
            {
                user_id_lists.insert(msg.user_id());
            }
            std::unordered_map<std::string, UserInfo> user_list;
            ret = _GetUser(rid, user_id_lists, user_list);
            if (ret == false)
            {
                LOG_ERROR("{} 批量用户数据获取失败", rid);
                return err_response(rid, "批量用户数据获取失败");
            }
            // 5. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for (const auto &msg : msg_list)
            {
                auto message_info = response->add_msg_list();
                message_info->set_message_id(msg.message_id());
                message_info->set_chat_session_id(msg.session_id());
                message_info->set_timestamp(boost::posix_time::to_time_t(msg.create_time()));
                message_info->mutable_sender()->CopyFrom(user_list[msg.user_id()]);
                if (msg.message_type() == MessageType::STRING)
                {
                    message_info->mutable_message()->set_message_type(MessageType::STRING);
                    if (msg.content())
                        message_info->mutable_message()->mutable_string_message()->set_content(*msg.content());
                }
                else if (msg.message_type() == MessageType::IMAGE)
                {
                    message_info->mutable_message()->set_message_type(MessageType::IMAGE);
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_image_message()->set_file_id(*msg.file_id());
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_image_message()->set_image_content(file_data_list[*msg.file_id()]);
                }
                else if (msg.message_type() == MessageType::FILE)
                {
                    message_info->mutable_message()->set_message_type(MessageType::FILE);
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_file_message()->set_file_id(*msg.file_id());
                    if (msg.file_size())
                        message_info->mutable_message()->mutable_file_message()->set_file_size(*msg.file_size());
                    if (msg.file_name())
                        message_info->mutable_message()->mutable_file_message()->set_file_name(*msg.file_name());
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_file_message()->set_file_contents(file_data_list[*msg.file_id()]);
                }
                else if (msg.message_type() == MessageType::SPEECH)
                {
                    message_info->mutable_message()->set_message_type(MessageType::SPEECH);
                    if (msg.file_id())
                    {
                        message_info->mutable_message()->mutable_speech_message()->set_file_id(*msg.file_id());
                        message_info->mutable_message()->mutable_speech_message()->set_file_contents(file_data_list[*msg.file_id()]);
                    }
                }
            }
            return;
        }
        void GetRecentMsg(google::protobuf::RpcController *controller,
                          const ::zht_im::GetRecentMsgReq *request,
                          ::zht_im::GetRecentMsgRsp *response,
                          ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void
            {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 提取请求中的关键要素：请求ID，会话ID，要获取的消息数量
            std::string rid = request->request_id();
            std::string chat_ssid = request->chat_session_id();
            int count = request->msg_count();
            // 2. 从数据库中获取最近的消息元信息
            auto msg_list = _mysql_message->recent(chat_ssid, count);
            // 3. 组织消息中所有文件消息的文件ID，并从文件子服务中进行下载
            std::unordered_set<std::string> file_id_lists;
            for (const auto &msg : msg_list)
            {
                // if (!msg.file_id())
                //     continue;
                if (msg.file_id())
                    file_id_lists.insert(*msg.file_id());
            }
            std::unordered_map<std::string, std::string> file_data_list;
            bool ret = _GetFile(rid, file_id_lists, file_data_list);
            if (ret == false)
            {
                LOG_ERROR("{} 批量文件数据下载失败", rid);
                return err_response(rid, "批量文件数据下载失败");
            }
            // 4. 组织消息中所有消息的的用户ID，并从用户子服务中下载用户信息
            std::unordered_set<std::string> user_id_lists;
            for (const auto &msg : msg_list)
            {
                user_id_lists.insert(msg.user_id());
            }
            std::unordered_map<std::string, UserInfo> user_list;
            ret = _GetUser(rid, user_id_lists, user_list);
            if (ret == false)
            {
                LOG_ERROR("{} 批量用户数据下载失败", rid);
                return err_response(rid, "批量用户数据下载失败");
            }
            // 5. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for (const auto &msg : msg_list)
            {
                auto message_info = response->add_msg_list();
                message_info->set_message_id(msg.message_id());
                message_info->set_chat_session_id(msg.session_id());
                message_info->set_timestamp(boost::posix_time::to_time_t(msg.create_time()));
                message_info->mutable_sender()->CopyFrom(user_list[msg.user_id()]);
                if (msg.message_type() == MessageType::STRING)
                {
                    message_info->mutable_message()->set_message_type(MessageType::STRING);
                    if (msg.content())
                        message_info->mutable_message()->mutable_string_message()->set_content(*msg.content());
                }
                else if (msg.message_type() == MessageType::IMAGE)
                {
                    message_info->mutable_message()->set_message_type(MessageType::IMAGE);
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_image_message()->set_file_id(*msg.file_id());
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_image_message()->set_image_content(file_data_list[*msg.file_id()]);
                }
                else if (msg.message_type() == MessageType::FILE)
                {
                    message_info->mutable_message()->set_message_type(MessageType::FILE);
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_file_message()->set_file_id(*msg.file_id());
                    if (msg.file_size())
                        message_info->mutable_message()->mutable_file_message()->set_file_size(*msg.file_size());
                    if (msg.file_name())
                        message_info->mutable_message()->mutable_file_message()->set_file_name(*msg.file_name());
                    if (msg.file_id())
                        message_info->mutable_message()->mutable_file_message()->set_file_contents(file_data_list[*msg.file_id()]);
                }
                else if (msg.message_type() == MessageType::SPEECH)
                {
                    message_info->mutable_message()->set_message_type(MessageType::SPEECH);
                    if (msg.file_id())
                    {
                        message_info->mutable_message()->mutable_speech_message()->set_file_id(*msg.file_id());
                        message_info->mutable_message()->mutable_speech_message()->set_file_contents(file_data_list[*msg.file_id()]);
                    }
                }
            }
            return;
        }
        void MsgSearch(google::protobuf::RpcController *controller,
                       const ::zht_im::MsgSearchReq *request,
                       ::zht_im::MsgSearchRsp *response,
                       ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void
            {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 关键字的消息搜索--只针对文本消息
            // 1. 从请求中提取关键要素：请求ID，会话ID，关键字
            std::string rid = request->request_id();
            std::string chat_ssid = request->chat_session_id();
            std::string skey = request->search_key();
            // 2. 从ES搜索引擎中进行关键字消息搜索，得到消息列表
            auto msg_list = _es_message->search(skey, chat_ssid);
            // 3. 组织所有消息的用户ID，从用户子服务中获取用户信息
            std::unordered_set<std::string> user_id_lists;
            for (const auto &msg : msg_list)
            {
                user_id_lists.insert(msg.user_id());
            }
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = _GetUser(rid, user_id_lists, user_list);
            if (ret == false)
            {
                LOG_ERROR("{} 批量用户数据下载失败", rid);
                return err_response(rid, "批量用户数据下载失败");
            }
            // 4. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for (const auto &msg : msg_list)
            {
                auto message_info = response->add_msg_list();
                message_info->set_message_id(msg.message_id());
                message_info->set_chat_session_id(msg.session_id());
                message_info->set_timestamp(boost::posix_time::to_time_t(msg.create_time()));
                message_info->mutable_sender()->CopyFrom(user_list[msg.user_id()]);
                message_info->mutable_message()->set_message_type(MessageType::STRING);
                if (msg.content())
                    message_info->mutable_message()->mutable_string_message()->set_content(*msg.content());
            }
            return;
        }
        void onMessage(const char *body, size_t size)
        {
            LOG_DEBUG("收到新消息，进行存储处理");
            // 1. 取出序列化的内容反序列化
            zht_im::MessageInfo message;
            bool ret = message.ParseFromArray(body, size);
            if (ret == false)
            {
                LOG_ERROR("对消费的消息进行反序列化失败");
                return;
            }
            // 2. 根据不同的消息进行不同处理
            std::string file_id, file_name, file_content;
            int64_t file_size = 0;
            if (message.message().message_type() == MessageType::STRING)
            {
                //   1. 文本信息，取元数据存储到es中
                file_content = message.message().string_message().content();
                ret = _es_message->appendData(message.sender().user_id(), message.message_id(),
                                              message.timestamp(), message.chat_session_id(), file_content);
                if (ret == false)
                {
                    LOG_ERROR("文本消息向存储引擎进行存储失败");
                    return;
                }
            }
            //   2. 如果是图片/文件/语音，取出数据存储到文件子服务中，并获取文件ID
            else if (message.message().message_type() == MessageType::IMAGE)
            {
                const auto &msg = message.message().image_message();
                ret = _PutFile("", msg.image_content(), msg.image_content().size(), file_id);
                if (ret == false)
                {
                    LOG_ERROR("上传图片到文件子服务失败");
                    return;
                }
            }
            else if (message.message().message_type() == MessageType::FILE)
            {
                const auto &msg = message.message().file_message();
                file_name = msg.file_name();
                file_size = msg.file_size();
                ret = _PutFile(file_name, msg.file_contents(), file_size, file_id);
                if (ret == false)
                {
                    LOG_ERROR("上传文件到文件子服务失败");
                    return;
                }
            }
            else if (message.message().message_type() == MessageType::SPEECH)
            {
                const auto &msg = message.message().speech_message();
                ret = _PutFile("", msg.file_contents(), msg.file_contents().size(), file_id);
                if (ret == false)
                {
                    LOG_ERROR("上传语音到文件子服务失败");
                    return;
                }
            }
            else
            {
                LOG_ERROR("消息类型错误");
                return;
            }
            // 3. 提取消息的元信息，放到mysql中
            zht_im::Message msg(message.message_id(), message.chat_session_id(),
                                message.sender().user_id(), message.message().message_type(),
                                boost::posix_time::from_time_t(message.timestamp()));
            msg.content(file_content);
            msg.file_id(file_id);
            msg.file_name(file_name);
            msg.file_size(file_size);
            LOG_DEBUG("插入的数据中文件长度为 {} ", file_size);
            ret = _mysql_message->insert(msg);
            if (ret = false)
            {
                LOG_ERROR("向数据库中插入新消息失败");
            }
        }

    private:
        bool _GetUser(const std::string &rid,
                      std::unordered_set<std::string> &user_id_list,
                      std::unordered_map<std::string, UserInfo> &user_data_list)
        {
            auto channel = _mm_channels->choose(_user_service_name);
            if (!channel)
            {
                LOG_ERROR("{}没有可供访问的用户子服务节点", _user_service_name);
                return false;
            }
            UserService_Stub stub(channel.get());
            GetMultiUserInfoReq req;
            GetMultiUserInfoRsp rsp;
            req.set_request_id(rid);
            for (const auto &id : user_id_list)
            {
                req.add_users_id(id);
            }
            brpc::Controller cntl;
            stub.GetMultiUserInfo(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("用户管理子服务调用失败 {}", cntl.ErrorText());
                return false;
            }
            const auto &umap = rsp.users_info();
            for (auto it = umap.cbegin(); it != umap.cend(); ++it)
            {
                user_data_list.insert(std::make_pair(it->first, it->second));
            }
            return true;
        }
        bool _GetFile(const std::string &rid, std::unordered_set<std::string> &file_id_list,
                      std::unordered_map<std::string, std::string> &file_data_list)
        {
            auto channel = _mm_channels->choose(_file_service_name);
            if (!channel)
            {
                LOG_ERROR("{}没有可供访问的文件子服务节点", _file_service_name);
                return false;
            }
            FileService_Stub stub(channel.get());
            GetMultiFileReq req;
            GetMultiFileRsp rsp;
            req.set_request_id(rid);
            for (const auto &id : file_id_list)
            {
                req.add_file_id_list(id);
            }
            brpc::Controller cntl;
            stub.GetMultiFile(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("文件管理子服务调用失败 {}", cntl.ErrorText());
                return false;
            }
            const auto &fmap = rsp.file_data();
            for (auto it = fmap.begin(); it != fmap.end(); ++it)
            {
                file_data_list.insert(std::make_pair(it->first, it->second.file_content()));
            }
            return true;
        }
        bool _PutFile(const std::string &filename, const std::string &body, const int64_t fsize, std::string &file_id) // 实现文件数据的上传
        {
            auto channel = _mm_channels->choose(_file_service_name);
            if (!channel)
            {
                LOG_ERROR("{}没有可供访问的文件子服务节点", _file_service_name);
                return false;
            }
            FileService_Stub stub(channel.get());
            PutSingleFileReq req;
            PutSingleFileRsp rsp;
            // req.set_request_id(rid); // 这里可以不用上传，因为用不上
            // req.set_user_id(uid);
            req.mutable_file_data()->set_file_name(filename);
            req.mutable_file_data()->set_file_size(fsize);
            req.mutable_file_data()->set_file_content(body);
            brpc::Controller cntl;
            stub.PutSingleFile(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("文件管理子服务调用失败 {}", cntl.ErrorText());
                return false;
            }
            file_id = rsp.file_info().file_id();
            return true;
        }
    };

    class MessageServer
    {
    public:
        using ptr = std::shared_ptr<MessageServer>;

    private:
        Registrant::ptr _registry_client;
        Discovery::ptr _service_discovery;
        MQClient::ptr _mq_client;
        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        MessageServer(const MQClient::ptr &mq_client,
                      const Discovery::ptr &service_discovery,
                      const Registrant::ptr &registry_client,
                      const std::shared_ptr<elasticlient::Client> &es_client,
                      const std::shared_ptr<odb::core::database> &mysql_client,
                      const std::shared_ptr<brpc::Server> &rpc_server)
            : _service_discovery(service_discovery), _registry_client(registry_client),
              _es_client(es_client), _mysql_client(mysql_client), _mq_client(mq_client), _rpc_server(rpc_server)
        {
        }
        ~MessageServer() {}

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
        }
    };
    class MessageServerBuilder // 建造者模式
    {
    public:
        // 构造es客户端对象
        void make_es_object(const std::vector<std::string> &host_list)
        {
            _es_client = ESClientFactory::create(host_list);
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
            _queue_name = queue_name;
            _mq_client = std::make_shared<MQClient>(user, passwd, host);
            _mq_client->declareComponents(exchange_name, queue_name, binding_key);
        }
        // 构造服务发现客户端&信道管理对象
        void make_discovery_object(const std::string &reg_host,
                                   const std::string &base_service_name,
                                   const std::string &file_service_name,
                                   const std::string &user_service_name)
        {
            _file_service_name = file_service_name;
            _user_service_name = user_service_name;
            _mm_channels = std::make_shared<ServiceManager>();
            _mm_channels->declared(_file_service_name);
            _mm_channels->declared(_user_service_name);
            // LOG_DEBUG("设置文件子服务为需添加管理的子服务: {}", _file_service_name);
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
        // 构造服务RPC服务器对象
        void make_rpc_server(uint16_t port, uint32_t timeout, uint8_t num_threads)
        {
            _rpc_server = std::make_shared<brpc::Server>();
            if (!_es_client)
            {
                LOG_ERROR("还未初始化ES搜索引擎模块");
                abort();
            }
            if (!_mysql_client)
            {
                LOG_ERROR("还未初始化MySQL数据库模块");
                abort();
            }
            if (!_mm_channels)
            {
                LOG_ERROR("还未初始化信道管理模块");
                abort();
            }
            MessageServiceImpl *message_service = new MessageServiceImpl(_file_service_name, _user_service_name,
                                                                         _es_client, _mysql_client, _mm_channels);
            int ret = _rpc_server->AddService(message_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
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
            auto callback = std::bind(&MessageServiceImpl::onMessage, message_service, std::placeholders::_1, std::placeholders::_2);
            _mq_client->consume(_queue_name, callback);
        }
        MessageServer::ptr build()
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
            MessageServer::ptr server = std::make_shared<MessageServer>(
                _mq_client, _service_discovery, _registry_client, _es_client, _mysql_client, _rpc_server);

            return server;
        }

    private:
        Registrant::ptr _registry_client;

        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;

        std::string _file_service_name;
        std::string _user_service_name;
        ServiceManager::ptr _mm_channels;
        Discovery::ptr _service_discovery;

        std::string _exchange_name;
        std::string _queue_name;
        MQClient::ptr _mq_client;

        std::shared_ptr<brpc::Server> _rpc_server;
    };
}