#include "rpcprovider.h"
#include <string>
#include "mprpcapplication.h"
#include "mprpcconfig.h"
#include <functional>
#include <google/protobuf/descriptor.h>
#include "rpcheader.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service *service){

    ServiceInfo service_info;
    const google::protobuf::ServiceDescriptor *pserviceDesc = service -> GetDescriptor();
    std::string service_name = pserviceDesc -> name();
    int methodCnt = pserviceDesc -> method_count();
    std::cout << "service_name:" << service_name << std::endl;

    for (int i = 0; i < methodCnt; i ++) {
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc -> method(i);
        std::string method_name = pserviceDesc -> name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}
void RpcProvider::Run(){
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, 
            std::placeholders::_2, std::placeholders::_3));
    server.setThreadNum(4);
    std::cout << "RpcProvider start service at ip:" << ip << " port:" << port << std::endl;

    server.start();
    m_eventLoop.loop();
}


void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){
    if (!conn -> connected()) {
        conn -> shutdown();
    }

}


void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, 
                            muduo::net::Buffer *buffer, 
                            muduo::Timestamp){
    std::string recv_buf = buffer -> retrieveAllAsString();
    uint32_t header_size = 0;

    recv_buf.copy((char*)&header_size, 4, 0);

    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;

    uint32_t args_size;

    if (rpcHeader.ParseFromString(rpc_header_str)) {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }else{
        std::cout << "rpc_header_str:" << rpc_header_str << " parse error!" << std::endl;
        return;
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    // 打印调试信息
    std::cout << "============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl; 
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl; 
    std::cout << "service_name: " << service_name << std::endl; 
    std::cout << "method_name: " << method_name << std::endl; 
    std::cout << "args_str: " << args_str << std::endl; 
    std::cout << "============================================" << std::endl;


    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end()) {
        std::cout << service_name << " is not exist!" << std::endl;
        return;
    }
}