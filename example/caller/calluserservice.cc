#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include ""
int main(int argc, char** argv) {
    MprpcApplication::Init(argc, argv);

    MprpcApplication::Init(argc, argv);

    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    stub.Login(, const ::fixbug::LoginRequest *request, ::fixbug::LoginResponse *response, ::google::protobuf::Closure *done);
    return 0;
}