#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mylock.h"

#define MAX_FILE_LEN            100*1024*1024       //100MB
const char* fifo_file = "./myfifo";                 //仿真FIFO文件名
const char* tmp_file  = "./tmp";                    //临时文件名

int customing(const char* myfifo, int need)
{
    int fd;
    char buff;
    int counter = 0;

    fd = open(myfifo, O_RDONLY);
    if (fd < 0) {
        perror("cutoming open error");
        return -1;
    }

    printf("Enjoy:");
    lseek(fd, SEEK_SET, 0);
    while (counter < need) {
        while ( (read(fd, &buff, 1) == 1) && (counter < need) ) {
            fputc(buff, stdout);
            counter ++;
        }
    }

    fputs("\n", stdout);
    close(fd);
    return 0;
}

//将count字节复制到dest file
int myfilecopy(const char* sour_file, const char* dest_file, int offset, int count, int copy_mode)
{
    int in_file, out_file;
    int counter;
    char buff_unit;
    int ret = 0;


    in_file = open(sour_file, O_RDONLY | O_NONBLOCK);
    if (in_file < 0) {
        perror("open sour_file fail");
        return -1;
    }

    out_file = open(dest_file, O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 0644);
    if (out_file < 0) {
        perror("open dest_file fail");
        return -1;
    }

    lseek(in_file, offset, SEEK_SET);
    while ( (read(in_file, &buff_unit, 1) == 1) && (counter < count) ) {
        //printf("buff_unit:%c\n",buff_unit);
        ret = write(out_file, &buff_unit, 1);
        if (ret < 0) {
            perror("write to dest file failed");
            //return -1;
        }
        counter ++;
    }

    close(in_file);
    close(out_file);

    return 0;
}

int custom(int need)
{
    int fd;

    customing(fifo_file, need);
    fd = open(fifo_file, O_RDWR);
    if (fd < 0) {
        printf("%s open failed.\n",__func__);
        return -1;
    }

    lock_set(fd, F_WRLCK);
    myfilecopy(fifo_file, tmp_file, need, MAX_FILE_LEN, 0);     //读取玩之后的字符复制到临时文件
    myfilecopy(tmp_file, fifo_file, 0, MAX_FILE_LEN, 0);        //再把临时文件拷贝到原始文件中（形成消费之后就没有的现象）
    lock_set(fd, F_UNLCK);

    unlink(tmp_file);
    close(fd);

    return 0;
}

int main(int argc, char* argv[])
{
    int customer_capacity = 10;

    if (argc > 1) {
        sscanf(argv[1], "%d", &customer_capacity);
    }

    if (customer_capacity > 0) {
        custom(customer_capacity);
    }

    exit(EXIT_SUCCESS);
}