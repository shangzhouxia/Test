#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>



#define     PORT            4321
#define     BUFFER_SIZE     1024
#define     MAX_CONNECT     5


int main()
{
    int sockfd, sendbytes;
    char buf[BUFFER_SIZE];
    struct hostent* host;
    char host_name[64];
    struct sockaddr_in server_addr;

    //获得主机名
    if (-1 == gethostname(host_name, 64)) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    host = gethostbyname(host_name);
    if (!host) {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "message send to server from client!");

    //1. 创建socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        perror("sockfd");
        exit(EXIT_FAILURE);
    }

    //设置sockaddr_in 参数
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);            //在本机通信
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    //2. 调用connect 连接服务器
    if (-1 == connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    //3. 调用recv/send 处理数据
    sendbytes = send(sockfd, buf, BUFFER_SIZE, 0);
    if (-1 == sendbytes) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    memset(buf, 0, sizeof(buf));
    if (-1 == recv(sockfd, buf, BUFFER_SIZE, 0)) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    printf("Recevied message: %s\n", buf);

    close(sockfd);

    exit(EXIT_SUCCESS);
}