#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CUSTOMER_SIZE       10
#define BANK_STAFF_SIZE         3

typedef struct {
    sem_t my_sem;
    pthread_t tid[MAX_CUSTOMER_SIZE];
} bank;

bank local_bank;


void* back_service(void* arg);

int main()
{
    int ret;

    //信号量初始化为BANK_STAFF_SIZE，最多只有BANK_STAFF_SIZE个线程可以访问临界区数据
    sem_init(&local_bank.my_sem, 0, BANK_STAFF_SIZE);

    for (int i=0; i<MAX_CUSTOMER_SIZE; i++) {
        ret = pthread_create(&local_bank.tid[i], NULL, back_service, &i);
        if (ret != 0) {
            perror("pthread_create");
            return ret;
        }

        printf("Customers %d come to handle business.\n", i+1);

        sleep(1);
    }

    for (int i=0; i<MAX_CUSTOMER_SIZE; i++) {
        ret = pthread_join(local_bank.tid[i], NULL);
        if (ret != 0) {
            perror("pthread_join");
            return ret;
        }
    }

    sem_destroy(&local_bank.my_sem);

    return 0;
}

void* back_service(void* arg)
{
    int id = *((int*)arg) + 1;

    sem_wait(&local_bank.my_sem);

    printf("Customer %d is doing business.\n", id);
    sleep(2);
    printf("Customer %d has completed the business.\n", id);

    sem_post(&local_bank.my_sem);

    return 0;
}