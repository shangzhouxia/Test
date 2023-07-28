#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


#define     PORT            4321
#define     BUFFER_SIZE     1024
#define     MAX_CONNECT     5

int main()
{
    struct sockaddr_in server_addr, client_addr;
    int sin_size, recvbytes;
    int sockfd, client_fd;
    char buf[BUFFER_SIZE];
    int option_val;

    //1. 建立socke连接
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //设置sockaddr_in 参数
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);         //主机字节序转换为网络字节序
    server_addr.sin_addr.s_addr = INADDR_ANY;   //用于绑定服务器套接字（socket）到所有可用的网络接口上
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    //SO_REUSEADDR：允许地址重用，允许多个套接字绑定到相同的地址和端口。
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_val, sizeof(option_val));

    //2. 绑定
    if (-1 == bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("Bind success!\n");

    //3. listen 创建未处理的请求队列
    if (-1 == listen(sockfd, MAX_CONNECT)) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listen ... ...\n");

    //4. 调用accept 等待客户端的连接
    sin_size = sizeof(client_addr);

    client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size);
    if (-1 == client_fd) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //5. 调用recv/send 处理数据
    memset(buf, 0, sizeof(buf));
    /* 
    注意 send 和 recv 的参数不能是sockfd，否则报错
    recv: Transport endpoint is not connected
     */
    recvbytes = recv(client_fd, buf, BUFFER_SIZE, 0);
    if (-1 == recvbytes) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    printf("Recevied message: %s\n", buf);

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "message send to client from server!");
    if (-1 == send(client_fd, buf, BUFFER_SIZE, 0)) {
        perror("send");
        exit(EXIT_FAILURE);
    }


    close(sockfd);
    exit(EXIT_SUCCESS);
}