#include <cstdint>
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>


class FriendService : public fixbug::FriendServiceRpc{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid) {
        std::cout << "do GetFriendsList service! userid:" << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("liu hong");
        vec.push_back("wang shuo");
        return vec;
    }
    // 重写基类方法
    void GetFriendsList(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetFriendListRequest* request,
                        ::fixbug::GetFriendListResponse* response,
                        ::google::protobuf::Closure* done){
        uint32_t userid = request -> userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);

        response -> mutable_result() -> set_errcode(0);
        response -> mutable_result() -> set_errmsg("");

        for (std::string & name : friendsList) {
            std::string *p = response -> add_friends();
            *p = name;
        }
        done->Run();
    }
private:
};