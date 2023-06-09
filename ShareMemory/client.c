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

#define logfile     "/tmp/example.log"

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
    int fd_shm;
    char mybuf[256];

    if (SEM_FAILED == (mutex_sem = sem_open(SEM_MUTEX_NAME, 0, 0, 0))) {
        error("sem_open");
    }

    //获取共享内存
    if (-1 == (fd_shm = shm_open(SHARED_MEM_NAME, O_RDWR, 0))) {
        error("shm_open");
    }

    //内存映射
    shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if (MAP_FAILED == shared_mem_ptr) {
        error("mmap");
    }

    if (SEM_FAILED == (buffer_count_sem = sem_open(SEM_BUFFER_COUNT_NAME, 0, 0, 0))) {
        error("buffer_count_sem");
    }

    if (SEM_FAILED == (spool_signal_sem = sem_open(SEM_SPOOL_SIGNAL_NAME, 0, 0, 0))) {
        error("buffer_count_sem");
    }

    char buf[200], *cp;

    printf("Please type a message: ");

    while (fgets(buf, 198, stdin)) {
        int length = strlen(buf);
        if (buf[length-1] == '\n') {
            buf[length-1] = '\0';
        }

        //printf("%s\n", buf);
        if (!strcmp("quit", buf)) {
            printf("\nJob Done!\n");
            break;
        }

/* 
sem_wait函数也是一个 原子操作，它的作用是从 信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。
也就是说，如果你对一个值为2的 信号量调用sem_wait(), 线程将会继续执行，这信号量的值将减到1。
如果对一个值为0的 信号量调用sem_wait()，这个函数就 会地等待直到有其它 线程增加了这个值使它不再是0为止。
 */
        //buffer_count_sem 初始化为256,所以通常情况下这里不会阻塞
        if (sem_wait(buffer_count_sem) == -1) {
            error("sem_wait(buffer_count_sem)");
        }

        if (sem_wait(mutex_sem) == -1) {
            error("sem_wait(mutex_sem)");
        }

        time_t now = time(NULL);
        cp = ctime(&now);

        int len = strlen(cp);
        if (*(cp + len-1) == '\n') {
            *(cp + len-1) = '\0';
        }

        //格式化输出: 29086（pid）: Fri Jun  9 15:55:24 2023(时间) XXX(来自输入)
        sprintf(shared_mem_ptr->buf[shared_mem_ptr->buffer_index],"%d: %s %s\n",
                getpid(), cp, buf);

        (shared_mem_ptr->buffer_index)++;

        if (shared_mem_ptr->buffer_index == MAX_BUFFERS) {
            shared_mem_ptr->buffer_index = 0;
        }


        if (sem_post(mutex_sem) == -1) {
            error("sem_post(mutex_sem)");
        }

        //通知服务端已经有数据ready
        if (sem_post(spool_signal_sem) == -1) {
            error("sem_post(spool_signal_sem)");
        }

        printf("Please type a message: ");

    }


    if (munmap(shared_mem_ptr, sizeof(struct shared_memory)) == -1) {
        error("munmap");
    }

    exit(0);
}

void error(char* msg)
{
    perror(msg);
    exit(1);
}