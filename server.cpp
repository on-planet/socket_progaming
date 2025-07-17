#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    //1.创建socket   ::表示全局命名空间
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   //Socket类型为IPv4，流式套接字  protocol为TCP
    if (sockfd < 0){
        printf("create socket error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return 1;
    }else{
        printf("create socket success: sockfd=%d\n", sockfd);
    }

    //2.绑定地址
    std::string ip = "127.0.0.1";
    int port = 8888;

    struct sockaddr_in addr{};
    //初始化sockaddr_in结构体
    std::memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET; //地址族为IPv4
    addr.sin_addr.s_addr = inet_addr(ip.c_str()); //将IP地址转换为网络字节序
    addr.sin_port = htons(port);

    if (::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        printf("bind socket error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return 1;
    }

    //3.监听端口
    if (::listen(sockfd, SOMAXCONN) < 0){ //SOMAXCONN是一个常量，表示系统允许的最大连接数
        printf("listen socket error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return 1;
    }

    //4.等待连接
    while (true){
        int connfd = ::accept(sockfd, nullptr, nullptr); //接受连接
        if (connfd < 0){
            printf("accept socket error: errno=%d errmsg=%s", errno, std::strerror(errno));
            continue; //继续等待下一个连接
        }
        char buf[1024] = {0};

        //5.接收数据
        size_t len = ::recv(connfd, buf, sizeof(buf), 0);
        printf("recv sockfd=%d len=%lu\n", connfd, len);

        //6.发送数据
        ::send(connfd, buf, len, 0);

        //7.关闭连接
        ::close(sockfd);
        return 0; //这里可以选择退出循环，或者继续等待下一个连接
    }
    return 0;
}