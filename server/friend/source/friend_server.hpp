// 实现用户管理子服务
#pragma once

#include <brpc/server.h>
#include <butil/logging.h>

#include "data_es.hpp" // es数据管理客户端封装
#include "etcd.hpp"    // 服务注册模块封装
#include "mysql_chat_session_member.hpp"
#include "mysql_chat_session.hpp"
#include "mysql_relation.hpp"
#include "mysql_apply.hpp"
#include "logger.hpp"  // 日志模块封装
#include "utils.hpp"   // 基础工具接口
#include "channel.hpp" // 信道管理封装

#include "friend.pb.h"
#include "base.pb.h"
#include "user.pb.h"
#include "message.pb.h"

namespace zht_im
{
    class FriendServiceImpl : public FriendService
    {
    public:
        using ptr = std::shared_ptr<FriendServiceImpl>;

    private:
        ESUser::ptr _es_user;
        FriendApplyTable::ptr _mysql_apply;
        ChatSessionTable::ptr _mysql_chat_session;
        ChatSessionMemberTable::ptr _mysql_chat_session_member;
        RelationTable::ptr _mysql_relation;

        // rpc调用客户端相关对象
        std::string _user_service_name;
        std::string _message_service_name;
        ServiceManager::ptr _mm_channels;

    public:
        FriendServiceImpl(
            const std::shared_ptr<elasticlient::Client> &es_client,
            const std::shared_ptr<odb::core::database> &mysql_client,
            const std::shared_ptr<ServiceManager> &mm_channels,
            const std::string &user_service_name,
            const std::string &message_service_name)
            : _es_user(std::make_shared<ESUser>(es_client)),
              _mysql_apply(std::make_shared<FriendApplyTable>(mysql_client)),
              _mysql_chat_session(std::make_shared<ChatSessionTable>(mysql_client)),
              _mysql_chat_session_member(std::make_shared<ChatSessionMemberTable>(mysql_client)),
              _mysql_relation(std::make_shared<RelationTable>(mysql_client)),
              _user_service_name(user_service_name),
              _message_service_name(message_service_name),
              _mm_channels(mm_channels)
        {
        }
        ~FriendServiceImpl() {}
        void GetFriendList(::google::protobuf::RpcController *controller,
                                   const ::zht_im::GetFriendListReq *request,
                                   ::zht_im::GetFriendListRsp *response,
                                   ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 获取请求中的用户 ID
            std::string rid = request->request_id();
            std::string uid = request->user_id();
            // 2. 根据用户 ID，从数据库的好友关系表&用户表中取出该用户所有的好友简息
            auto friend_id_lists = _mysql_relation->friends(uid);
            std::unordered_set<std::string> user_id_list;
            for(auto &id : friend_id_lists)
            {
                user_id_list.insert(id);
            }
            // 3. 根据好友简息中的好友头像 ID，批量获取头像数据，组织完整用户信息结构
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = GetUserInfo(rid, user_id_list, user_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败", rid);
                return err_response(rid, "批量获取用户信息失败");
            }
            // 4. 组织响应，将好友列表返回给网关
            response->set_success(true);
            response->set_request_id(rid);
            for(const auto& user_it : user_list)
            {
                auto user_info = response->add_friend_list();
                user_info->CopyFrom(user_it.second);
            }
        }
        void FriendRemove(::google::protobuf::RpcController *controller,
                                  const ::zht_im::FriendRemoveReq *request,
                                  ::zht_im::FriendRemoveRsp *response,
                                  ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 取出请求中的删除者 ID 和被删除者 ID
            std::string uid = request->user_id();
            std::string pid = request->peer_id();
            // 2. 从用户好友关系表中删除相关关系数据
            bool ret = _mysql_relation->remove(uid, pid);
            if(ret == false)
            {
                LOG_ERROR("{} - 从数据库删除好友信息失败", rid);
                return err_response(rid, "从数据库删除好友信息失败");
            }
            // 3. 从会话表中删除单聊会话，从会话成员表中删除会话成员信息
            ret = _mysql_chat_session->remove(uid, pid);
            if(ret == false)
            {
                LOG_ERROR("{} - 从数据库删除好友会话信息失败", rid);
                return err_response(rid, "从数据库删除好友会话信息失败");
            }
            // 4. 组织响应，返回给网关
            response->set_request_id(rid);
            response->set_success(true);
        }
        void FriendAdd(::google::protobuf::RpcController *controller,
                               const ::zht_im::FriendAddReq *request,
                               ::zht_im::FriendAddRsp *response,
                               ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 获取申请人ID和被申请人ID
            std::string uid = request->user_id();
            std::string pid = request->respondent_id();
            std::string rid = request->request_id();
            // 2. 判断是否已经是好友
            bool ret = _mysql_relation->exists(uid, pid);
            if(ret == true)
            {
                LOG_ERROR("{} - 申请好友失败,两人已经是好友 {}-{}", rid, uid, pid);
                return err_response(rid, "申请好友失败,两人已经是好友");
            }
            // 3. 判断当前是否已经申请过了
            ret = _mysql_apply->exists(uid, pid);
            if(ret == true)
            {
                LOG_ERROR("{} - 申请好友失败,不要重复发送申请 {}-{}", rid, uid, pid);
                return err_response(rid, "申请好友失败,不要重复发送申请");
            }
            // 4. 向apply数据库中新增一条操作
            std::string eid = uuid();
            FriendApply ev(eid, uid, pid);
            ret = _mysql_apply->insert(ev);
            if(ret == false)
            {
                LOG_ERROR("{} - 向数据库中插入申请信息失败", rid);
                return err_response(rid, "向数据库中插入申请信息失败");
            }
            // 5. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            response->set_notify_event_id(eid);
        }
        void FriendAddProcess(::google::protobuf::RpcController *controller,
                                      const ::zht_im::FriendAddProcessReq *request,
                                      ::zht_im::FriendAddProcessRsp *response,
                                      ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 提取关键要素：通知事件ID，当前ID，申请者ID，是否同意申请，
            std::string eid = request->notify_event_id();
            std::string uid = request->apply_user_id(); // 申请人
            std::string pid = request->user_id(); // 被申请人
            bool agree = request->agree();
            // 2. 判断该事件是否存在  
            bool ret = _mysql_apply->exists(eid);
            if(ret == false)
            {
                LOG_ERROR("{} - 不存在该申请事件 {}-{}", rid, uid, pid);
                return err_response(rid, "不存在该申请事件");
            }
            // 3. 如果有，对应删除事件，事件处理完毕
            ret = _mysql_apply->remove(uid, pid); // 这里是不是出错了？虽然测试没问题，但是之后可以考虑是不是对的
            if(ret == false)
            {
                LOG_ERROR("{} - 从数据库中删除该事件失败 {}-{}", rid, uid, pid);
                return err_response(rid, "从数据库中删除该事件失败");
            }
            // 4. 处理结果如果是同意：relation中插入 向会话表中插入新的单聊会话，向会话成员表中新增成员
            std::string cssid = uuid();
            if(agree == true)
            {
                ret = _mysql_relation->insert(uid, pid);
                if(ret == false)
                {
                    LOG_ERROR("{} - 向数据库中插入两人的好友关系失败 {}-{}", rid, uid, pid);
                    return err_response(rid, "向数据库中插入两人的好友关系失败");
                }
                ChatSession cs(cssid, "", ChatSessionType::SINGLE);
                ret = _mysql_chat_session->insert(cs);
                if(ret == false)
                {
                    LOG_ERROR("{} - 新增会话信息失败 {}-{}", rid, uid, pid);
                    return err_response(rid, "新增会话信息失败");
                }
                ChatSessionMember csm1(cssid, uid);
                ChatSessionMember csm2(cssid, pid);
                std::vector<ChatSessionMember> csms = {csm1, csm2};
                ret = _mysql_chat_session_member->append(csms);
                if(ret == false)
                {
                    LOG_ERROR("{} - 新增会话成员信息失败 {}-{}", rid, uid, pid);
                    return err_response(rid, "新增会话成员信息失败");
                }
            }
            // 5. 组织响应
            response->set_success(true);
            response->set_request_id(rid);
            response->set_new_session_id(cssid);
        }
        void FriendSearch(::google::protobuf::RpcController *controller,
                                  const ::zht_im::FriendSearchReq *request,
                                  ::zht_im::FriendSearchRsp *response,
                                  ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 提取搜索关键字skey
            std::string skey = request->search_key();
            std::string uid = request->user_id();
            // 2. 从ES中通过skey搜索模糊匹配，拿到uid
            std::unordered_set<std::string> user_id_list;
            auto friend_id_list = _mysql_relation->friends(uid); // 这里的搜索是为了添加好友，所以屏蔽掉已经是好友的部分
            friend_id_list.push_back(uid); // 这里搜索是为了添加好友，所以需要把他自己屏蔽掉
            auto search_res = _es_user->search(skey, friend_id_list);
            for(auto& it : search_res)
            {
                user_id_list.insert(it.user_id());
            }
            // 3. 根据获取到的uid，通过用户管理子服务拿到对应的用户信息
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = GetUserInfo(rid, user_id_list, user_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败", rid);
                return err_response(rid, "批量获取用户信息失败");
            }
            // 4. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for(const auto& user_it : user_list)
            {
                auto user_info = response->add_user_info();
                user_info->CopyFrom(user_it.second);
            }
        }
        void GetPendingFriendEventList(::google::protobuf::RpcController *controller,
                                               const ::zht_im::GetPendingFriendEventListReq *request,
                                               ::zht_im::GetPendingFriendEventListRsp *response,
                                               ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 提取要素：当前用户ID
            std::string rid = request->request_id();
            std::string uid = request->user_id();
            // 2. 从数据库中获取待处理的申请信息
            auto user_id_list = _mysql_apply->applyUsers(uid);
            std::unordered_set<std::string> uid_list;
            for(auto &id : user_id_list)
            {
                uid_list.insert(id);
            }
            // 3. 批量获取申请人的信息
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = GetUserInfo(rid, uid_list, user_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败", rid);
                return err_response(rid, "批量获取用户信息失败");
            }
            // 4. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for(const auto& user_it : user_list)
            {
                auto ev = response->add_event();
                ev->mutable_sender()->CopyFrom(user_it.second);
            }
        }
        void GetChatSessionList(::google::protobuf::RpcController *controller,
                                        const ::zht_im::GetChatSessionListReq *request,
                                        ::zht_im::GetChatSessionListRsp *response,
                                        ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 用于用户登录之后展示自己的聊天历史
            // 1. 提取关键要素：请求的用户ID
            std::string rid = request->request_id();
            std::string uid = request->user_id();
            // 2. 从数据库中查询出用户的单聊会话列表
            auto sf_list = _mysql_chat_session->singleChatSession(uid);
            //   2.1 从单聊会话列表中获取所有的好友ID，从用户子服务中获取用户信息
            std::unordered_set<std::string> user_id_list;
            for(const auto& f : sf_list)
            {
                user_id_list.insert(f.friend_id);
            }
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = GetUserInfo(rid, user_id_list, user_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败", rid);
                return err_response(rid, "批量获取用户信息失败");
            }
            //   2.2 设置响应会话信息：会话名称就是好友名称，会话头像就是好友头像
            // 3. 从数据库中查询出用户的群聊会话列表
            auto gc_list = _mysql_chat_session->groupChatSession(uid);
            // 4. 根据所有的会话ID，从消息存储子服务中获取会话的最后一条消息
            // 5. 组织响应
            for(const auto& f : sf_list)
            {
                auto chat_session_info = response->add_chat_session_info_list();
                chat_session_info->set_single_chat_friend_id(f.friend_id);
                chat_session_info->set_chat_session_id(f.chat_session_id);
                chat_session_info->set_chat_session_name(user_list[f.friend_id].nickname());
                chat_session_info->set_avatar(user_list[f.friend_id].avatar());
                MessageInfo msg;
                bool ret = GetRecentMsg(rid, f.chat_session_id, msg);
                if(ret == false)
                {
                    continue;
                }
                chat_session_info->mutable_prev_message()->CopyFrom(msg);
            }
            for(const auto& f : gc_list)
            {
                auto chat_session_info = response->add_chat_session_info_list();
                chat_session_info->set_chat_session_id(f.chat_session_id);
                chat_session_info->set_chat_session_name(f.chat_session_name);
                MessageInfo msg;
                bool ret = GetRecentMsg(rid, f.chat_session_id, msg);
                if(ret == false)
                {
                    continue;
                }
                chat_session_info->mutable_prev_message()->CopyFrom(msg);
            }
            response->set_request_id(rid);
            response->set_success(true);
        }
        // 这里是在创建群聊会话，针对的是用户主动发起的请求
        void ChatSessionCreate(::google::protobuf::RpcController *controller,
                                       const ::zht_im::ChatSessionCreateReq *request,
                                       ::zht_im::ChatSessionCreateRsp *response,
                                       ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 提取请求关键要素：会话名称，会话成员
            std::string rid = request->request_id();
            std::string uid = request->user_id();
            std::string cssname = request->chat_session_name();
            // 2. 生成会话ID向数据库中添加会话信息和会话成员信息
            std::string cssid = uuid();
            ChatSession cs(cssid, cssname, ChatSessionType::GROUP);
            bool ret = _mysql_chat_session->insert(cs);
            if(ret == false)
            {
                LOG_ERROR("{} - 向数据库中插入会话信息失败 {}-{}", rid, cssid, cssname);
                return err_response(rid, "向数据库中插入会话信息失败");
            }
            std::vector<ChatSessionMember> member_list;
            for(int i = 0; i < request->member_id_list_size(); ++i)
            {
                ChatSessionMember csm(cssid, request->member_id_list(i));
                member_list.push_back(csm);
            }
            ret = _mysql_chat_session_member->append(member_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 向数据库中插入会话成员信息失败 {}", rid, cssname);
                return err_response(rid, "向数据库中插入会话成员信息失败");
            }
            // 3. 组织响应 -- 组织会话信息
            response->set_request_id(rid);
            response->set_success(true);
            response->mutable_chat_session_info()->set_chat_session_id(cssid);
            response->mutable_chat_session_info()->set_chat_session_name(cssname);
        }
        void GetChatSessionMember(::google::protobuf::RpcController *controller,
                                          const ::zht_im::GetChatSessionMemberReq *request,
                                          ::zht_im::GetChatSessionMemberRsp *response,
                                          ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 用户查看群聊成员信息的时候进行成员展示
            // 1. 提取关键要素：聊天会话ID
            std::string rid = request->request_id();
            std::string cssid = request->chat_session_id();
            // 2. 从数据库中获取会话成员ID列表
            auto member_id_list = _mysql_chat_session_member->members(cssid);
            std::unordered_set<std::string> uid_list;
            for(auto& id : member_id_list)
            {
                uid_list.insert(id);
            }
            // 3. 从用户子服务批量获取用户信息
            std::unordered_map<std::string, UserInfo> user_list;
            bool ret = GetUserInfo(rid, uid_list, user_list);
            if(ret == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败", rid);
                return err_response(rid, "批量获取用户信息失败");
            }
            // 4. 组织响应
            response->set_request_id(rid);
            response->set_success(true);
            for(auto user_it : user_list)
            {
                auto user_info = response->add_member_info_list();
                user_info->CopyFrom(user_it.second);
            }
        }
        
    private:
        bool GetRecentMsg(const std::string& rid, const std::string &cssid, MessageInfo& msg)
        {
            auto channel = _mm_channels->choose(_message_service_name);
            if(!channel)
            {
                LOG_ERROR("{} - 获取消息存储子服务信道失败", rid);
                return false;
            }
            GetRecentMsgReq req;
            GetRecentMsgRsp rsp;
            req.set_request_id(rid);
            req.set_chat_session_id(cssid);
            req.set_msg_count(1);
            brpc::Controller cntl;
            MsgStorageService_Stub stub(channel.get());
            stub.GetRecentMsg(&cntl, &req, &rsp, nullptr);
            if(cntl.Failed() == true)
            {
                LOG_ERROR("{} - 消息存储子服务调用失败 - {}", rid, cntl.ErrorText());
                return false;
            }
            if(rsp.success() == false)
            {
                LOG_ERROR("{} - 获取会话 {} 最后一条消息失败 - {}", rid, cssid, rsp.errmsg());
                return false;
            }
            if(rsp.msg_list_size() > 0)
            {
                msg.CopyFrom(rsp.msg_list(0));
                return true;
            }
            return false;
        }
        bool GetUserInfo(
            const std::string &rid, const std::unordered_set<std::string> &uid_list, std::unordered_map<std::string, UserInfo>& res)
        {
            auto channel = _mm_channels->choose(_user_service_name);
            if(!channel)
            {
                LOG_ERROR("{} - 获取用户子服务信道失败", rid);
                return false;
            }
            GetMultiUserInfoReq req;
            GetMultiUserInfoRsp rsp;
            req.set_request_id(rid);
            for(auto& id : uid_list)
            {
                req.add_users_id(id);
            }
            brpc::Controller cntl;
            UserService_Stub stub(channel.get());
            stub.GetMultiUserInfo(&cntl, &req, &rsp, nullptr);
            if(cntl.Failed() == true)
            {
                LOG_ERROR("{} - 用户子服务调用失败 - {}", rid, cntl.ErrorText());
                return false;
            }
            if(rsp.success() == false)
            {
                LOG_ERROR("{} - 批量获取用户信息失败 - {}", rid, rsp.errmsg());
                return false;
            }
            for(const auto& user_it : rsp.users_info())
            {
                res.insert(std::make_pair(user_it.first, user_it.second));
            }
            return true;
        }
    };

    class FriendServer
    {
    public:
        using ptr = std::shared_ptr<FriendServer>;

    private:
        Registrant::ptr _registry_client;
        Discovery::ptr _service_discovery;
        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        FriendServer(const Discovery::ptr &service_discovery,
                     const Registrant::ptr &registry_client,
                     const std::shared_ptr<elasticlient::Client> &es_client,
                     const std::shared_ptr<odb::core::database> &mysql_client,
                     const std::shared_ptr<brpc::Server> &rpc_server)
            : _service_discovery(service_discovery), _registry_client(registry_client), _es_client(es_client), _mysql_client(mysql_client), _rpc_server(rpc_server)
        {
        }
        ~FriendServer() {}

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
        }
    };
    class FriendServerBuilder // 建造者模式
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
        // 构造服务发现客户端&信道管理对象
        void make_discovery_object(const std::string &reg_host,
                                   const std::string &base_service_name,
                                   const std::string &user_service_name,
                                   const std::string &message_service_name)
        {
            _user_service_name = user_service_name;
            _message_service_name = message_service_name;
            _mm_channels = std::make_shared<ServiceManager>();
            _mm_channels->declared(_user_service_name);
            _mm_channels->declared(_message_service_name);
            LOG_DEBUG("设置用户子服务为需添加管理的子服务: {}", _user_service_name);
            LOG_DEBUG("设置消息子服务为需添加管理的子服务: {}", _message_service_name);
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
            FriendServiceImpl *friend_service = new FriendServiceImpl(
                _es_client, _mysql_client, _mm_channels, _user_service_name, _message_service_name);
            int ret = _rpc_server->AddService(friend_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
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
        FriendServer::ptr build()
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

            FriendServer::ptr server = std::make_shared<FriendServer>(
                _service_discovery, _registry_client, _es_client, _mysql_client, _rpc_server);
            return server;
        }

    private:
        Registrant::ptr _registry_client;

        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;

        std::string _user_service_name;
        std::string _message_service_name;
        ServiceManager::ptr _mm_channels;
        Discovery::ptr _service_discovery;

        std::shared_ptr<brpc::Server> _rpc_server;
    };
}