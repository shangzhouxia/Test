
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#define MAX_BUFFERS     10

#define LOGFILE     "/tmp/example.log"

#define SEM_MUTEX_NAME  "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME   "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME   "/sem-spool-signal"
#define SHARED_MEM_NAME         "/posix-shared-mem-example"


struct shared_memory {
    char buf[MAX_BUFFERS][256];
    int buffer_index;
    int buffer_print_index;
};

void error(char* msg);

int main(int argc, char** argv)
{
    struct shared_memory* shared_mem_ptr;
    sem_t* mutex_sem, *buffer_count_sem, * spool_signal_sem;
    int fd_shm, fd_log;
    char mybuf[256];

    //log文件: 用于存储来自client端的数据
    fd_log = open(LOGFILE, O_CREAT | O_WRONLY | O_APPEND | O_SYNC, 0666);
    if (-1 == fd_log) {
        error("fd_log open");
    }

    if (SEM_FAILED == (mutex_sem = sem_open(SEM_MUTEX_NAME, O_CREAT, 0660, 0))) {
        error("sem_open");
    }

    //获取共享内存
    if (-1 == (fd_shm = shm_open(SHARED_MEM_NAME, O_RDWR | O_CREAT, 0660))) {
        error("shm_open");
    }
    //设置共享内存的大小
    if (ftruncate(fd_shm, sizeof(struct shared_memory)) == -1) {
        error("ftruncate");
    }

    shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if (MAP_FAILED == shared_mem_ptr) {
        error("mmap");
    }
    //初始化共享内存的数据
    shared_mem_ptr->buffer_index = shared_mem_ptr->buffer_print_index = 0;

    if (SEM_FAILED == (buffer_count_sem = sem_open(SEM_BUFFER_COUNT_NAME, O_CREAT, 0660, MAX_BUFFERS))) {
        error("buffer_count_sem");
    }

    if (SEM_FAILED == (spool_signal_sem = sem_open(SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0))) {
        error("spool_signal_sem");
    }

    if (sem_post(mutex_sem) == -1) {
        error("sem_post(mutex_sem)");
    }

    while (1) {
        if (sem_wait(spool_signal_sem) == -1) {
            error("sem_wait(spool_signal_sem)");
        }

        strcpy(mybuf, shared_mem_ptr->buf[shared_mem_ptr->buffer_print_index]);

        (shared_mem_ptr->buffer_print_index) ++;

        if (shared_mem_ptr->buffer_print_index == MAX_BUFFERS) {
            shared_mem_ptr->buffer_print_index = 0;
        }

        if (sem_post(buffer_count_sem) == -1) {
            error("sem_post(buffer_count_sem)");
        }

        //将字符串写入文件
        if (write(fd_log, mybuf, strlen(mybuf)) != strlen(mybuf)) {
            error("write fd_log");
        }
    }
    

}

void error(char* msg)
{
    perror(msg);
    exit(1);
}