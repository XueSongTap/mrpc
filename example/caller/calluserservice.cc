#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char** argv) {
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    
    //rpc方法请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;

    std::cout << "finish initialize request and response" << std::endl;
    stub.Login(nullptr, &request, &response, nullptr);
    std::cout << "finish rpc call" << std::endl;
    //rpc调用完成，返回结果
    if (0 == response.result().errcode()){
        std::cout << "rpc login response success:" << response.success() << std::endl;
    }else {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("6666666666666");
    fixbug::RegisterResponse rsp;
    // 以同步的方式发起rpc调用请求，等待返回结果
    stub.Register(nullptr, &req, &rsp, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (0 == rsp.result().errcode())
    {
        std::cout << "rpc register response success:" << rsp.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error : " << rsp.result().errmsg() << std::endl;
    }
    return 0;
}