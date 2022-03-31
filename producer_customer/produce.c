#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "mylock.h"

#define MAXLEN                  10          //缓冲区大小最大值
#define ALPHABET                1
#define ALPHABET_START          'a'
#define COUNT_OF_ALPHABET       26
#define DIGTAL                  2
#define DIGTAL_START            '0'
#define COUNT_OF_DIGTAL         10          //数字字符的个数
#define SIGN_TYPE               ALPHABET

const char* fifo_file = "./myfifo";
char buff[MAXLEN];

//产生一个字符并写入仿真FIFO文件中
int product(void)
{
    int fd;
    unsigned int sign_type, sign_start, sign_count, size;
    static unsigned int counter = 0;

    fd = open(fifo_file, O_CREAT | O_RDWR | O_APPEND, 0644);
    if (fd < 0) {
        perror("open fifo file error");
        exit(1);
    }

    sign_type = SIGN_TYPE;
    switch (sign_type)
    {
    case ALPHABET:
        sign_start = ALPHABET_START;
        sign_count = COUNT_OF_ALPHABET;
        break;
    case DIGTAL:
        sign_start = DIGTAL_START;
        sign_count = COUNT_OF_DIGTAL;
        break;

    default:
        return -1;
    }

    sprintf(buff, "%c", (sign_start + counter));
    counter = (counter+1) % sign_count;

    lock_set(fd, F_WRLCK);
    size = write(fd, buff, strlen(buff));
    if (size < 0) {
        perror("Producer write err");
        return -1;
    }

    lock_set(fd,F_UNLCK);

    close(fd);

    return 0;
}

int main(int argc, char* argv[])
{
    int time_step = 1;          //生产周期
    int time_life = 10;         //资源总数

    if (argc > 1) {
        sscanf(argv[1], "%d", &time_step);
    }

    if (argc > 2) {
        sscanf(argv[2], "%d", &time_life);
    }

    while(time_life--) {
        if (product() < 0)
            break;
        
        sleep(time_step);
    }

    exit(EXIT_SUCCESS);
}

