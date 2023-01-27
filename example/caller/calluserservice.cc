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
    stub.Login(nullptr, &request, &response, nullptr);

    //rpc调用完成，返回结果
    if (0 == response.result().errcode()){
        std::cout << "rpc login response success:" << response.success() << std::endl;
    }else {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
    return 0;
}