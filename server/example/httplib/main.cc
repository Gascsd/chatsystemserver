#include "../common/httplib.h"
#include <iostream>

int main()
{
    httplib::Server server;
    server.Get("/hello", [](const httplib::Request& req, httplib::Response& resp){
        std::cout << req.method << std::endl;
        std::cout << req.body << std::endl;
        for(auto it : req.headers)
        {
            std::cout << it.first << " : " << it.second << std::endl;
        }
        resp.status = 200;
        std::string body = "<html><body><h1>Hello World</h1></body></html>";
        resp.set_content(body, "text/html");
    });
    server.listen("0.0.0.0", 8080);
    return 0;
}