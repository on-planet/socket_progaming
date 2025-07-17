#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 1. 创建socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        std::cerr << "create socket error: errno=" << errno << " errmsg=" << std::strerror(errno) << std::endl;
        return 1;
    } else {
        std::cout << "create socket success: sockfd=" << sockfd << std::endl;
    }
    // 2. 连接服务器
    std::string ip = "127.0.0.1";
    int port = 8888;

    struct sockaddr_in addr;
    // 初始化sockaddr_in结构体
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // 地址族为IPv4
    addr.sin_addr.s_addr = inet_addr(ip.c_str()); // 将IP地址转换为网络字节序
    addr.sin_port = htons(port); // 将端口转换为网络字节序

    if (::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        std::cerr << "connect error: " << std::strerror(errno) << std::endl;
    }
    // 3. 发送数据
    std::string message = "Hello, Server!";
    ::send(sockfd, message.c_str(), message.size(), 0);

    // 4. 接收数据
    char buf[1024] = {0};
    size_t len = ::recv(sockfd, buf, sizeof(buf), 0);
    if (len < 0) {
        std::cerr << "recv error: " << std::strerror(errno) << std::endl;
    } else {
        std::cout << "recv from server: " << std::string(buf, len) << std::endl;
    }

    // 5. 关闭连接
    ::close(sockfd);
    std::cout << "connection closed." << std::endl;
    return 0;
}
