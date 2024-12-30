// 实现用户管理子服务
#pragma once

#include <brpc/server.h>
#include <butil/logging.h>

#include "data_es.hpp"          // es数据管理客户端封装
#include "data_mysql.hpp"       // es数据管理客户端封装
#include "data_redis.hpp"       // es数据管理客户端封装
#include "etcd.hpp"             // 服务注册模块封装
#include "logger.hpp"           // 日志模块封装
#include "utils.hpp"            // 基础工具接口
#include "dms.hpp"              // 短信平台SDK的封装
#include "channel.hpp"          // 信道管理封装

#include "user-odb.hxx"
#include "user.hxx"

#include "user.pb.h"
#include "base.pb.h"
#include "file.pb.h"

namespace zht_im
{
    class UserServiceImpl : public UserService
    {
    public:
        using ptr = std::shared_ptr<DMSClient>;

    private:
        ESUser::ptr _es_user;
        UserTable::ptr _mysql_user;
        Session::ptr _redis_session;
        Status::ptr _redis_status;
        Codes::ptr _redis_codes;
        DMSClient::ptr _dms_client;

        // rpc调用客户端相关对象  
        std::string _file_service_name;
        ServiceManager::ptr _mm_channels; 

    public:
        UserServiceImpl(const DMSClient::ptr &dms_client,
            const std::shared_ptr<elasticlient::Client> &es_client,
            const std::shared_ptr<odb::core::database> &mysql_client,
            const std::shared_ptr<sw::redis::Redis> &redis_client,
            const std::shared_ptr<ServiceManager> &mm_channels,
            const std::string &file_service_name)
              : _es_user(std::make_shared<ESUser>(es_client)),
                _mysql_user(std::make_shared<UserTable>(mysql_client)),
                _redis_session(std::make_shared<Session>(redis_client)),
                _redis_status(std::make_shared<Status>(redis_client)),
                _redis_codes(std::make_shared<Codes>(redis_client)),
                _file_service_name(file_service_name),
                _dms_client(dms_client),
                _mm_channels(mm_channels)
            {}
        ~UserServiceImpl() {}
        bool nickname_check(const std::string &nickname)
        {
            auto nicknameLen = nickname.size();
            if(nicknameLen >= 22)
            {
                LOG_ERROR("用户名长度不合法: {} - {}", nickname, nicknameLen);
                return false;
            }
            return true;
        }
        bool password_check(const std::string &password)
        {
            auto passwordLen = password.size();
            if(passwordLen < 6 || passwordLen > 15)
            {
                LOG_ERROR("密码长度不合法: {} - {}", password, passwordLen);
                return false;
            }
            for(const auto& ch : password)
            {
                if(!(isupper(ch) || islower(ch) || isdigit(ch) || ch == '_' || ch == '-'))
                {
                    LOG_ERROR("密码字符不合法: {}", password);
                    return false;
                }
            }
            return true;
        }
        void UserRegister(google::protobuf::RpcController *controller,
                                  const ::zht_im::UserRegisterReq *request,
                                  ::zht_im::UserRegisterRsp *response,
                                  ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到用户注册请求");
            brpc::ClosureGuard rpc_guard(done);
            // 定义一个错误处理函数，当出错的时候被调用
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            // 1. 从请求中取出昵称和密码
            std::string rid = request->request_id();
            std::string nickname = request->nickname();
            std::string password = request->password();
            LOG_DEBUG("要注册的用户名: {} 密码： {}", nickname, password);
            // 2. 检查昵称是否合法（只能包含字母，数字，连字符-，下划线_，长度限制 3~15 之间）
            bool ret = nickname_check(nickname);
            if(ret == false)
            {
                return err_response(rid, "用户名不合法");
            }
            // 3. 检查密码是否合法（只能包含字母，数字，长度限制 6~15 之间）
            ret = password_check(password);
            if(ret == false)
            {
                return err_response(rid, "密码格式不合法");
            }
            // 4. 根据昵称在数据库进行判断是否昵称已存在
            std::shared_ptr<User> user = _mysql_user->select_by_nickname(nickname);
            if(user)
            {
                LOG_ERROR("{} - 用户名被占用- {}", rid, nickname);
                return err_response(rid, "用户名被占用");
            }
            LOG_DEBUG("检测完成，可以新增数据");
            // 5. 向数据库新增数据
            std::string uid = uuid();
            user = std::make_shared<User>(uid, nickname, password);
            ret = _mysql_user->insert(user);
            if(ret == false)
            {
                LOG_ERROR("{} - mysql数据库新增数据失败", rid);
                return err_response(rid, "mysql数据库新增数据失败");
            }
            LOG_DEBUG("向MySQL数据库中新增数据成功");
            // 6. 向 ES 服务器中新增用户信息
            _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - es搜索引擎新增数据失败", rid);
                return err_response(rid, "es搜索引擎新增数据失败");
            }
            LOG_DEBUG("向es搜索引擎中新增数据成功");
            // 7. 组织响应，进行成功与否的响应即可
            response->set_request_id(rid);
            response->set_success(true);
        }
        void UserLogin(google::protobuf::RpcController *controller,
                               const ::zht_im::UserLoginReq *request,
                               ::zht_im::UserLoginRsp *response,
                               ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到用户登录请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出昵称和密码
            std::string request_nickname = request->nickname();
            std::string request_password = request->password();
            // 2. 通过昵称获取用户信息，进行密码是否一致的判断
            std::shared_ptr<User> user = _mysql_user->select_by_nickname(request_nickname);
            if(user.get() == nullptr)
            {
                LOG_ERROR("{} - 用户名不存在 {}", rid, request_nickname);
                return err_response(rid, "用户名不存在或者密码错误");
            }
            if(request_password != *user->password())
            {
                LOG_ERROR("{} - 密码错误- 收到的用户名和密码-{}-{}-实际的用户名和密码 -{}-{}", rid, request_nickname, request_password, *user->nickname(), *user->password());
                return err_response(rid, "用户名不存在或者密码错误");
            }
            // 3. 根据 redis 中的登录标记信息是否存在判断用户是否已经登录。
            bool ret = _redis_status->exists(user->user_id());
            if(ret == true)
            {
                LOG_ERROR("{} - 用户已经在其他地方登录", rid);
                return err_response(rid, "用户已经在其他地方登录");
            }
            // 4. 构造会话 ID，生成会话键值对，向 redis 中添加会话信息以及登录标记信息
            std::string ssid = uuid();
            _redis_session->append(ssid, user->user_id());
            // 5. 组织响应，返回生成的会话 ID
            response->set_request_id(rid);
            response->set_login_session_id(ssid);
            response->set_success(true);
        }
        bool phone_check(const std::string &phone)
        {
            if(phone.size() != 11) return false;
            if(phone[0] != '1') return false;
            if(phone[1] < '3' || phone[1] > '9') return false;
            for(int i = 2; i < 11; ++i)
                if(!isdigit(phone[i])) 
                    return false;
            return true;
        }
        void GetPhoneVerifyCode(google::protobuf::RpcController *controller,
                                        const ::zht_im::PhoneVerifyCodeReq *request,
                                        ::zht_im::PhoneVerifyCodeRsp *response,
                                        ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到获取验证码请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出手机号码
            std::string phone = request->phone_number();
            // 2. 验证手机号码格式是否正确（必须以 1 开始，第二位 3~9 之间，后边 9 个数字字符）
            bool ret = phone_check(phone);
            if(ret == false)
            {
                LOG_ERROR("{} - 手机号格式错误 - {}", rid, phone);
                return err_response(rid, "手机号格式错误");
            }
            // 3. 生成 4 位随机验证码
            std::string code_id = uuid();
            std::string code = vcode();
            // 4. 基于短信平台 SDK 发送验证码
            ret = _dms_client->Send(phone, code);
            if(ret == false)
            {
                LOG_ERROR("{} - 短信验证码发送失败 - {}", rid, phone);
                return err_response(rid, "短信验证码发送失败");
            }
            // 5. 构造验证码 ID，添加到 redis 验证码映射键值索引中
            ret = _redis_codes->append(code_id, code);
            if(ret == false)
            {
                LOG_ERROR("{} - 验证码添加管理失败 - {}-{}", rid, code_id, code);
                return err_response(rid, "验证码添加管理失败");
            }
            // 6. 组织响应，返回生成的验证码 ID
            response->set_request_id(rid);
            response->set_verify_code_id(code_id);
            response->set_success(true);
        }
        void PhoneRegister(google::protobuf::RpcController *controller,
                                   const ::zht_im::PhoneRegisterReq *request,
                                   ::zht_im::PhoneRegisterRsp *response,
                                   ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到用户手机号注册请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出手机号码和验证码
            std::string phone = request->phone_number();
            std::string code_id = request->verify_code_id();
            std::string code = request->verify_code();
            // 2. 检查注册手机号码是否合法
            bool ret = phone_check(phone);
            if(ret == false)
            {
                LOG_ERROR("{} - 手机号格式错误 - {}", rid, phone);
                return err_response(rid, "手机号格式错误");
            }
            // 3. 从 redis 数据库中进行验证码 ID-验证码一致性匹配
            ret = _redis_codes->equal(code_id, code);
            if(ret == false)
            {
                LOG_ERROR("{} - 验证码错误 - {}-{}", rid, code_id, code);
                return err_response(rid, "验证码错误");
            }
            // 4. 通过数据库查询判断手机号是否已经注册过
            auto user = _mysql_user->select_by_phone(phone);
            if(user)
            {
                LOG_ERROR("{} - 该手机号已经被注册 - {}", rid, phone);
                return err_response(rid, "该手机号已经被注册");
            }
            // 5. 向数据库新增用户信息
            std::string uid = uuid();
            user = std::make_shared<User>(uid, phone);
            ret = _mysql_user->insert(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 向数据库中添加用户信息失败 - {}", rid, phone);
                return err_response(rid, "向数据库中添加用户信息失败");
            }
            // 6. 向 ES 服务器中新增用户信息
            ret = _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - es搜索引擎新增数据失败", rid);
                return err_response(rid, "es搜索引擎新增数据失败");
            }
            // 7. 组织响应，返回注册成功与否
            response->set_request_id(rid);
            response->set_success(true);
        }
        void PhoneLogin(google::protobuf::RpcController *controller,
                                const ::zht_im::PhoneLoginReq *request,
                                ::zht_im::PhoneLoginRsp *response,
                                ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到手机号登录请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出手机号码和验证码 ID，以及验证码。
            std::string phone = request->phone_number();
            std::string code_id = request->verify_code_id();
            std::string code = request->verify_code();
            // 2. 检查注册手机号码是否合法
            bool ret = phone_check(phone);
            if(ret == false)
            {
                LOG_ERROR("{} - 手机号格式错误 - {}", rid, phone);
                return err_response(rid, "手机号格式错误");
            }
            // 3. 根据手机号从数据数据进行用户信息查询，判断用用户是否存在
            auto user = _mysql_user->select_by_phone(phone);
            if(!user)
            {
                LOG_ERROR("{} - 用户不存在 - {}", rid, phone);
                return err_response(rid, "用户不存在");
            }
            // 4. 从 redis 数据库中进行验证码 ID-验证码一致性匹配
            ret = _redis_codes->equal(code_id, code);
            if(ret == false)
            {
                LOG_ERROR("{} - 验证码错误 - {}-{}", rid, code_id, code);
                return err_response(rid, "验证码错误");
            }
            // 5. 根据 redis 中的登录标记信息是否存在判断用户是否已经登录。
            ret = _redis_status->exists(user->user_id());
            if(ret == true)
            {
                LOG_ERROR("{} - 用户已经在其他地方登录", rid);
                return err_response(rid, "用户已经在其他地方登录");
            }
            // 6. 构造会话 ID，生成会话键值对，向 redis 中添加会话信息以及登录标记信息
            std::string ssid = uuid();
            _redis_session->append(ssid, user->user_id());
            // 7. 组织响应，返回生成的会话 ID
            response->set_request_id(rid);
            response->set_login_session_id(ssid);
            response->set_success(true);
        }

        // 从这一步开始之后就会带上ssid了
        void GetUserInfo(google::protobuf::RpcController *controller,
                                 const ::zht_im::GetUserInfoReq *request,
                                 ::zht_im::GetUserInfoRsp *response,
                                 ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到获取用户信息请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出用户 ID
            std::string uid = request->user_id();
            // 2. 通过用户 ID，从数据库中查询用户信息
            auto user = _mysql_user->select_by_id(uid);
            if(!user)
            {
                LOG_ERROR("{} - 未找到用户信息 - {}", rid, uid);
                return err_response(rid, "未找到用户信息");
            }
            // 3. 根据用户信息中的头像 ID，从文件服务器获取头像文件数据，组织完整用户信息
            UserInfo *userInfo = response->mutable_user_info();
            userInfo->set_user_id(user->user_id());
            if(user->nickname()) userInfo->set_nickname(*user->nickname());
            if(user->description()) userInfo->set_description(*user->description());
            if(user->phone()) userInfo->set_phone(*user->phone());

            if(user->avatar_id())
            {
                // 从信道管理对象中，获取到连接了文件管理子服务的channel
                auto channel = _mm_channels->choose(_file_service_name);
                if(!channel)
                {
                    LOG_ERROR("{} - 未找到文件管理子服务节点 - {}", rid, _file_service_name);
                    return err_response(rid, "未找到文件管理子服务节点");
                }
                // 进行文件子服务的rpc请求，进行头像文件下载
                FileService_Stub stub(channel.get());
                GetSingleFileReq req;
                GetSingleFileRsp rsp;
                req.set_request_id(rid);
                req.set_file_id(*user->avatar_id());

                brpc::Controller cntl;
                stub.GetSingleFile(&cntl, &req, &rsp, nullptr);
                if (cntl.Failed() == true || rsp.success() == false)
                {
                    LOG_ERROR("{} - 文件管理子服务调用失败 {}", rid, cntl.ErrorText());
                    return err_response(rid, "文件管理子服务调用失败");
                }
                userInfo->set_avatar(rsp.file_data().file_content());
            }
            // 4. 组织响应，返回用户信息
            response->set_request_id(rid);
            // response->set_allocated_user_info(userInfo);
            response->set_success(true);
        }
        void GetMultiUserInfo(google::protobuf::RpcController *controller,
                                      const ::zht_im::GetMultiUserInfoReq *request,
                                      ::zht_im::GetMultiUserInfoRsp *response,
                                      ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到获取多个用户信息请求");
            brpc::ClosureGuard rpc_guard(done);
             // 1. 调用错误回调
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 2. 从请求中取出用户ID -- 列表
            std::vector<std::string> uid_lists;
            for(int i = 0; i < request->users_id_size(); ++i)
            {
                uid_lists.push_back(request->users_id(i));
            }
            // 3. 从数据库中进行批量用户查询
            auto users = _mysql_user->select_multi_users(uid_lists);
            if(users.size() != request->users_id_size())  
            {
                LOG_ERROR("{} - 从数据库中查找的用户信息数量不一致 {}-{}", rid, request->users_id_size(), users.size());
                return err_response(rid, "从数据库中查找的用户信息数量不一致");
            }
            // 4. 批量从文件管理子服务进行文件下载
            auto channel = _mm_channels->choose(_file_service_name);
            if(!channel)
            {
                LOG_ERROR("{} - 未找到文件管理子服务节点 - {}", rid, _file_service_name);
                return err_response(rid, "未找到文件管理子服务节点");
            }
            FileService_Stub stub(channel.get());
            GetMultiFileReq req;
            GetMultiFileRsp rsp;
            req.set_request_id(rid);
            for(auto& user : users)
            {
                req.add_file_id_list(*user.avatar_id());
            }

            brpc::Controller cntl;
            stub.GetMultiFile(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("{} - 文件管理子服务调用失败 {}", rid, cntl.ErrorText());
                return err_response(rid, "文件管理子服务调用失败");
            }
            // 5. 组织响应
            for(auto& user : users)
            {
                auto user_map = response->mutable_users_info(); // 本次请求要响应的用户信息map
                auto file_map = rsp.mutable_file_data();       // 批量文件请求响应中的map
                UserInfo user_info;
                user_info.set_user_id(user.user_id());
                if(user.nickname()) user_info.set_nickname(*user.nickname());
                if(user.description()) user_info.set_description(*user.description());
                if(user.phone()) user_info.set_phone(*user.phone());
                if(user.avatar_id()) user_info.set_avatar((*file_map)[*user.avatar_id()].file_content());
                (*user_map)[user_info.user_id()] = user_info;
            }
            response->set_request_id(rid);
            response->set_success(true);
        }
        void SetUserAvatar(google::protobuf::RpcController *controller,
                                   const ::zht_im::SetUserAvatarReq *request,
                                   ::zht_im::SetUserAvatarRsp *response,
                                   ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到设置头像请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出用户 ID 与头像数据
            std::string uid = request->user_id();
            // 2. 从数据库通过用户 ID 进行用户信息查询，判断用户是否存在
            auto user = _mysql_user->select_by_id(uid);
            if(!user)
            {
                LOG_ERROR("{} - 未找到用户信息 - {}", rid, uid);
                return err_response(rid, "未找到用户信息");
            }
            // 3. 上传头像文件到文件子服务
            auto channel = _mm_channels->choose(_file_service_name);
            if(!channel)
            {
                LOG_ERROR("{} - 未找到文件管理子服务节点 - {}", rid, _file_service_name);
                return err_response(rid, "未找到文件管理子服务节点");
            }

            zht_im::FileService_Stub stub(channel.get());
            zht_im::PutSingleFileReq req;
            zht_im::PutSingleFileRsp rsp;
            
            req.set_request_id(rid); 
            req.mutable_file_data()->set_file_name("");
            req.mutable_file_data()->set_file_size(request->avatar().size());
            req.mutable_file_data()->set_file_content(request->avatar());

            brpc::Controller cntl;
            stub.PutSingleFile(&cntl, &req, &rsp, nullptr);
            if (cntl.Failed() == true || rsp.success() == false)
            {
                LOG_ERROR("{} - 文件管理子服务调用失败 {}", rid, cntl.ErrorText());
                return err_response(rid, "文件管理子服务调用失败");
            }
            std::string avatar_id = rsp.file_info().file_id();
            // 4. 将返回的头像文件 ID 更新到数据库中
            user->avatar_id(avatar_id);
            bool ret = _mysql_user->update(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新数据库头像ID失败: {}", rid, cntl.ErrorText());
                return err_response(rid, "更新数据库头像ID失败");
            }
            // 5. 更新 ES 服务器中用户信息
            ret = _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新es搜索引擎头像ID失败: {}", rid, cntl.ErrorText());
                return err_response(rid, "更新es搜索引擎头像ID失败");
            }
            // 6. 组织响应，返回更新成功与否
            response->set_request_id(rid);
            response->set_success(true);
        }
        void SetUserNickname(google::protobuf::RpcController *controller,
                                     const ::zht_im::SetUserNicknameReq *request,
                                     ::zht_im::SetUserNicknameRsp *response,
                                     ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到设置昵称请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出用户 ID 与新的昵称
            std::string uid = request->user_id();
            std::string new_nickname = request->nickname();
            // 2. 判断昵称格式是否正确
            bool ret = nickname_check(new_nickname);
            if(ret == false)
            {
                return err_response(rid, "用户名不合法");
            }
            // 3. 从数据库通过用户 ID 进行用户信息查询，判断用户是否存在
            auto user = _mysql_user->select_by_id(uid);
            if(!user)
            {
                LOG_ERROR("{} - 未找到用户信息 - {}", rid, uid);
                return err_response(rid, "未找到用户信息");
            }
            // 4. 将新的昵称更新到数据库中
            user->nickname(new_nickname);
            ret = _mysql_user->update(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新数据库用户昵称失败 - {}", rid, new_nickname);
                return err_response(rid, "更新数据库用户昵称失败");
            }
            // 5. 更新 ES 服务器中用户信息
            ret = _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新es搜索引擎用户昵称失败: {}", rid, new_nickname);
                return err_response(rid, "更新es搜索引擎用户昵称失败");
            }
            // 6. 组织响应，返回更新成功与否
            response->set_request_id(rid);
            response->set_success(true);
        }
        void SetUserDescription(google::protobuf::RpcController *controller,
                                        const ::zht_im::SetUserDescriptionReq *request,
                                        ::zht_im::SetUserDescriptionRsp *response,
                                        ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到设置签名请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出用户 ID 与新的签名
            std::string uid = request->user_id();
            std::string new_description = request->description();
            // 2. 从数据库通过用户 ID 进行用户信息查询，判断用户是否存在
            auto user = _mysql_user->select_by_id(uid);
            if(!user)
            {
                LOG_ERROR("{} - 未找到用户信息 - {}", rid, uid);
                return err_response(rid, "未找到用户信息");
            }
            // 3. 将新的签名更新到数据库中
            user->description(new_description);
            bool ret = _mysql_user->update(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新数据库用户签名失败 - {}", rid, new_description);
                return err_response(rid, "更新数据库用户签名失败");
            }
            // 5. 更新 ES 服务器中用户信息
            ret = _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新es搜索引擎用户签名失败: {}", rid, new_description);
                return err_response(rid, "更新es搜索引擎用户签名失败");
            }
            // 6. 组织响应，返回更新成功与否
            response->set_request_id(rid);
            response->set_success(true);
        }
        void SetUserPhoneNumber(google::protobuf::RpcController *controller,
                                        const ::zht_im::SetUserPhoneNumberReq *request,
                                        ::zht_im::SetUserPhoneNumberRsp *response,
                                        ::google::protobuf::Closure *done)
        {
            LOG_DEBUG("收到设置手机号请求");
            brpc::ClosureGuard rpc_guard(done);
            auto err_response = [this, response](const std::string &rid, const std::string &errmsg) -> void {
                response->set_request_id(rid);
                response->set_success(false);
                response->set_errmsg(errmsg);
                return;
            };
            std::string rid = request->request_id();
            // 1. 从请求中取出手机号码和验证码 ID，以及验证码
            std::string uid = request->user_id();
            std::string new_phone = request->phone_number();
            std::string code_id = request->phone_verify_code_id();
            std::string code = request->phone_verify_code();
            // 2. 检查注册手机号码是否合法
            bool ret = phone_check(new_phone);
            if(ret == false)
            {
                LOG_ERROR("{} - 手机号格式错误 - {}", rid, new_phone);
                return err_response(rid, "手机号格式错误");
            }
            // 3. 从 redis 数据库中进行验证码 ID-验证码一致性匹配
            ret = _redis_codes->equal(code_id, code);
            if(ret == false)
            {
                LOG_ERROR("{} - 验证码错误 - {}-{}", rid, code_id, code);
                return err_response(rid, "验证码错误");
            }
            // 4. 从数据库通过用户 ID 进行用户信息查询，判断用户是否存在
            auto user = _mysql_user->select_by_id(uid);
            if(!user)
            {
                LOG_ERROR("{} - 未找到用户信息 - {}", rid, uid);
                return err_response(rid, "未找到用户信息");
            }
            // 5. 将新的手机号更新到数据库中
            user->phone(new_phone);
            ret = _mysql_user->update(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新数据库用户手机号失败 - {}", rid, new_phone);
                return err_response(rid, "更新数据库用户手机号失败");
            }
            // 5. 更新 ES 服务器中用户信息
            ret = _es_user->appendData(user);
            if(ret == false)
            {
                LOG_ERROR("{} - 更新es搜索引擎用户手机号失败: {}", rid, new_phone);
                return err_response(rid, "更新es搜索引擎用户手机号失败");
            }
            // 6. 组织响应，返回更新成功与否
            response->set_request_id(rid);
            response->set_success(true);
        }
    };

    class UserServer
    {
    public:
        using ptr = std::shared_ptr<UserServer>;

    private:
        Registrant::ptr _registry_client;
        Discovery::ptr _service_discovery;
        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;
        std::shared_ptr<sw::redis::Redis> _redis_client;
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        UserServer(const Discovery::ptr &service_discovery,
                   const Registrant::ptr &registry_client,
                   const std::shared_ptr<elasticlient::Client> &es_client,
                   const std::shared_ptr<odb::core::database> &mysql_client,
                   const std::shared_ptr<sw::redis::Redis> &redis_client,
                   const std::shared_ptr<brpc::Server> &rpc_server)
            : _service_discovery(service_discovery), _registry_client(registry_client)
            , _es_client(es_client),_mysql_client(mysql_client) ,_redis_client(redis_client)
            ,_rpc_server(rpc_server) 
        {}
        ~UserServer() {}

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
        }
    };
    class UserServerBuilder // 建造者模式
    {
    public:
        // 构造短信平台客户端对象
        void make_dms_object(const std::string &access_key_id, const std::string &access_key_secret)
        {
            _dms_client = std::make_shared<DMSClient>(access_key_id, access_key_secret);
        }
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
        // 构造redis客户端对象
        void make_redis_object(const std::string &host, int port, int db, bool keep_alive)
        {
            _redis_client = RedisClientFactory::create(host, port, db, keep_alive);
        }
        // 构造服务发现客户端&信道管理对象
        void make_discovery_object(const std::string &reg_host,
            const std::string &base_service_name,
            const std::string &file_service_name)
        {
            _file_service_name = file_service_name;
            _mm_channels = std::make_shared<ServiceManager>();
            _mm_channels->declared(_file_service_name);
            LOG_DEBUG("设置文件子服务为需添加管理的子服务: {}", _file_service_name);
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
            if (!_dms_client)
            {
                LOG_ERROR("还未初始化短信平台模块");
                abort();
            }
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
            if (!_redis_client)
            {
                LOG_ERROR("还未初始化Redis数据库模块");
                abort();
            }
            if (!_mm_channels)
            {
                LOG_ERROR("还未初始化信道管理模块");
                abort();
            }
            UserServiceImpl *user_service = new UserServiceImpl(
                _dms_client, _es_client, _mysql_client, _redis_client, _mm_channels, _file_service_name
            );
            int ret = _rpc_server->AddService(user_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
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
        UserServer::ptr build()
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
            
            UserServer::ptr server = std::make_shared<UserServer>(
                _service_discovery, _registry_client, _es_client, _mysql_client, _redis_client, _rpc_server
            );
            return server;
        }

    private:
        Registrant::ptr _registry_client;

        std::shared_ptr<elasticlient::Client> _es_client;
        std::shared_ptr<odb::core::database> _mysql_client;
        std::shared_ptr<sw::redis::Redis> _redis_client;

        std::shared_ptr<DMSClient> _dms_client;

        std::string _file_service_name;
        ServiceManager::ptr _mm_channels;
        Discovery::ptr _service_discovery;

        std::shared_ptr<brpc::Server> _rpc_server;
    };
}