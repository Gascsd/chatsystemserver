

// 实现文件存储子服务
// 1. 实现文件Rpc服务类 -- 实现rpc调用的业务处理接口
// 2. 实现文件存储子服务的服务器类
// 3. 实现文件存储子服务类的构造者

#include <brpc/server.h>
#include <gflags/gflags.h>
#include <butil/logging.h>

#include "etcd.hpp"   // 服务注册模块封装
#include "logger.hpp" // 日志模块封装
#include "utils.hpp"
#include "base.pb.h"
#include "file.pb.h"

namespace zht_im
{
    class FileServiceImpl : public zht_im::FileService
    {
    private:
        std::string _storage_path;
    public:
        FileServiceImpl(const std::string storage_path) 
            : _storage_path(storage_path)
        {
            umask(0);
            mkdir(_storage_path.c_str(), 0775);
            if(_storage_path.back() != '/') _storage_path += '/';
        }
        ~FileServiceImpl() {}
        void GetSingleFile(google::protobuf::RpcController *controller,
                           const ::zht_im::GetSingleFileReq *request,
                           ::zht_im::GetSingleFileRsp *response,
                           ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            // 1. 取出请求中的ID（就是文件名）
            std::string fileId = request->file_id();
            std::string filename = _storage_path + fileId;
            // 2. 讲文件ID作为文件名读取文件内容
            std::string body;
            bool ret = zht_im::readFile(filename, body);
            if (ret == false)
            {
                response->set_success(false);
                response->set_errmsg("读取文件数据失败");
                LOG_ERROR("{} 请求, 读取文件数据失败", request->request_id());
            }
            // 3. 组织响应
            response->set_success(true);
            response->mutable_file_data()->set_file_id(fileId);
            response->mutable_file_data()->set_file_content(body);
        }
        void GetMultiFile(google::protobuf::RpcController *controller,
                          const ::zht_im::GetMultiFileReq *request,
                          ::zht_im::GetMultiFileRsp *response,
                          ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            // 循环取出请求中的文件ID，读取文件数据进行填充
            for (int i = 0; i < request->file_id_list_size(); ++i)
            {
                std::string fileId = request->file_id_list(i);
                std::string filename = _storage_path + fileId;
                std::string body;
                bool ret = zht_im::readFile(filename, body);
                if (ret == false)
                {
                    response->set_success(false);
                    response->set_errmsg("读取文件数据失败");
                    LOG_ERROR("{} 请求, 读取文件数据失败", request->request_id());
                    return;
                }
                FileDownloadData data;
                data.set_file_id(fileId);
                data.set_file_content(body);
                response->mutable_file_data()->insert({fileId, data});
            }
            response->set_success(true);
        }
        void PutSingleFile(google::protobuf::RpcController *controller,
                           const ::zht_im::PutSingleFileReq *request,
                           ::zht_im::PutSingleFileRsp *response,
                           ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            // 1. 为文件生成一个唯一的uuid作为文件名
            std::string fileId = zht_im::uuid();
            std::string filename = _storage_path + fileId;
            // 2. 取出请求中的文件数据,进行文件写入
            bool ret = zht_im::writeFile(filename, request->file_data().file_content());
            if (ret == false)
            {
                response->set_success(false);
                response->set_errmsg("写入文件数据失败");
                LOG_ERROR("{} 请求, 写入文件数据失败", request->request_id());
                return;
            }
            // 3. 组织响应
            response->set_success(true);
            response->mutable_file_info()->set_file_id(fileId);
            response->mutable_file_info()->set_file_size(request->file_data().file_size());
            response->mutable_file_info()->set_file_name(request->file_data().file_name());
        }
        void PutMultiFile(google::protobuf::RpcController *controller,
                          const ::zht_im::PutMultiFileReq *request,
                          ::zht_im::PutMultiFileRsp *response,
                          ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            for (int i = 0; i < request->file_data_size(); ++i)
            {
                std::string fileId = zht_im::uuid();
                std::string filename = _storage_path + fileId;
                bool ret = zht_im::writeFile(filename, request->file_data(i).file_content());
                if (ret == false)
                {
                    response->set_success(false);
                    response->set_errmsg("写入文件数据失败");
                    LOG_ERROR("{} 请求, 写入文件数据失败", request->request_id());
                    return;
                }
                zht_im::FileMessageInfo *info = response->add_file_info();
                info->set_file_id(fileId);
                info->set_file_size(request->file_data(i).file_size());
                info->set_file_name(request->file_data(i).file_name());
            }
            response->set_success(true);
        }
    };

    class FileServer
    {
    public:
        using ptr = std::shared_ptr<FileServer>;

    private:
        Registrant::ptr _reg_client;
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        FileServer(const Registrant::ptr &reg_client,
                   const std::shared_ptr<brpc::Server> &rpc_server)
            : _reg_client(reg_client), _rpc_server(rpc_server) {}
        ~FileServer() {}

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
        }
    };

    class FileServerBuilder // 建造者模式
    {
    public:
        // 构造服务注册客户端对象
        void make_reg_object(const std::string &reg_host, const std::string &service_name, const std::string &access_host)
        {
            _reg_client = std::make_shared<Registrant>(reg_host);
            _reg_client->registry(service_name, access_host);
        }
        // 构造服务RPC服务器对象
        void make_rpc_server(uint16_t port, uint32_t timeout, uint8_t num_threads, const std::string &path = "./data/")
        {
            _rpc_server = std::make_shared<brpc::Server>();
            FileServiceImpl *file_service = new FileServiceImpl(path);
            int ret = _rpc_server->AddService(file_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
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
        FileServer::ptr build()
        {
            if (!_reg_client)
            {
                LOG_ERROR("还未初始化服务注册模块");
                abort();
            }
            if (!_rpc_server)
            {
                LOG_ERROR("还未初始化RPC服务器模块");
                abort();
            }
            FileServer::ptr server = std::make_shared<FileServer>(_reg_client, _rpc_server);
            return server;
        }

    private:
        Registrant::ptr _reg_client;
        std::shared_ptr<brpc::Server> _rpc_server;
    };
} // namespace zht_im
