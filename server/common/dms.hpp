#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>
#include "logger.hpp"

namespace zht_im
{
    class DMSClient
    {
    private:
        std::string _access_key_id;
        std::string _access_key_secret;
        std::unique_ptr<AlibabaCloud::CommonClient> _client;

    public:
        DMSClient(const std::string &access_key_id, const std::string access_key_secret)
            : _access_key_id(access_key_id), _access_key_secret(access_key_secret)
        {
            AlibabaCloud::InitializeSdk();
            AlibabaCloud::ClientConfiguration configuration("cn-qingdao");
            configuration.setConnectTimeout(1500);
            configuration.setReadTimeout(4000);
            AlibabaCloud::Credentials credential(_access_key_id, _access_key_secret);
            _client = std::make_unique<AlibabaCloud::CommonClient>(credential, configuration);
        }
        ~DMSClient() { AlibabaCloud::ShutdownSdk(); }

        bool Send(const std::string &phone, const std::string code)
        {
            AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
            request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
            request.setDomain("dysmsapi.aliyuncs.com");
            request.setVersion("2017-05-25");
            request.setQueryParameter("Action", "SendSms");
            request.setQueryParameter("SignName", "即时通信平台");
            request.setQueryParameter("TemplateCode", "SMS_475885878");
            request.setQueryParameter("PhoneNumbers", phone);
            std::string code_json = "{\"code\":\"" + code + "\"}";
            request.setQueryParameter("TemplateParam", code_json);

            auto response = _client->commonResponse(request);
            if (!response.isSuccess())
            {
                LOG_ERROR("短信验证码请求失败: {}, request id : {}", response.error().errorMessage(), response.error().requestId());
                return false;
            }
            LOG_INFO("短信验证码发送成功,发送日志: {}", response.result().payload().c_str());
            return true;
        }
    };
}