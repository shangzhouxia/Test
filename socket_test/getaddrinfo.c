#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_NAME_LEN        256

int main()
{
    struct addrinfo hints, *res=NULL;
    char host_name[MAX_NAME_LEN], addr_str[INET_ADDRSTRLEN], *addr_strl;
    int rc;
    struct in_addr addr;

    memset(&hints, 0, sizeof(hints));

    hints.ai_flags = AI_CANONNAME;          //通知getaddrinfo返回主机名称
    hints.ai_family = AF_UNSPEC;            //IPv4 或者 IPv6
    hints.ai_socktype = SOCK_DGRAM;         //数据报套接字
    hints.ai_protocol = IPPROTO_UDP;        //User Datagram Protocol.

    //获得主机名
    if (-1 == gethostname(host_name, MAX_NAME_LEN)) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    printf("[%d] host_name: %s\n",__LINE__, host_name);

    rc = getaddrinfo(host_name, NULL, &hints, &res);
    if (rc != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    } else {
        addr = ((struct sockaddr_in*)(res->ai_addr))->sin_addr;
        inet_ntop(res->ai_family, &(addr.s_addr), addr_str, INET_ADDRSTRLEN);

        printf("Host name: %s\nIP address: %s\n", res->ai_canonname, addr_str);
    }


/* 
由于getaddrinfo函数涉及网络查询，它可能会导致网络I/O，因此在调用该函数时应该注意避免阻塞其他重要任务。
另外，在使用getaddrinfo函数后，不再需要使用返回的addrinfo结构后，应该调用freeaddrinfo函数来释放相关资源。
 */
    freeaddrinfo(res);

    exit(EXIT_SUCCESS);
}