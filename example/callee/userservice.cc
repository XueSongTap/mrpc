#include <cstdint>
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"


class UserService : public fixbug::UserServiceRpc{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;  
        return false;
    }
    bool Register(uint32_t id, std::string name, std::string pwd){
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id:" << id << "name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }

    /*
    重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    1. caller   ===>   Login(LoginRequest)  => muduo =>   callee 
    2. callee   ===>    Login(LoginRequest)  => 交到下面重写的这个Login方法上了
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done){
        std::string name = request -> name();
        std::string pwd = request -> pwd();

        bool login_result = Login(name, pwd);

        fixbug::ResultCode *code = response -> mutable_result();
        code -> set_errcode(0);
        code -> set_errmsg("");
        response-> set_success(login_result);
        done -> Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                    const ::fixbug::RegisterRequest* request,
                    ::fixbug::RegisterResponse* response,
                    ::google::protobuf::Closure* done){
        uint32_t id = request -> id();
        std::string name = request -> name();
        std::string pwd = request -> pwd();

        bool ret = Register(id, name, pwd);
        response -> mutable_result() -> set_errcode(0);
        response -> mutable_result() -> set_errmsg("");
        response -> set_success(ret);

        done -> Run();
    }
};

int main(int argc, char **argv)
{
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象。把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点   Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}