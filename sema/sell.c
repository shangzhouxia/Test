#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_TICKET_SIZE     10
#define MAX_TID_SIZE        4

typedef struct {
    sem_t mySem;
    int ticket_sum;
    pthread_t tids[MAX_TID_SIZE];
} ticket;

static ticket my_item;

void* sell(void* arg);

int main()
{
    int ret;
    void* arg;

    sem_init(&my_item.mySem, 0, 1);
    my_item.ticket_sum = MAX_TICKET_SIZE;


    for (int i=0; i<MAX_TID_SIZE; i++) {
        ret = pthread_create(&my_item.tids[i], NULL, &sell, NULL);
        if (ret != 0) {
            perror("pthread_create");
            return ret;
        }
    }

    sleep(10);

    for (int i=0; i<MAX_TID_SIZE; i++) {
        ret = pthread_join(my_item.tids[i], &arg);
        if (ret != 0) {
            printf("tid=%ld waiting failed\n", my_item.tids[i]);
            return ret;
        }
    }

    sem_destroy(&my_item.mySem);
}

void* sell(void* arg)
{
    printf("Current thread ID: %lu\n", pthread_self());

    int ret;

    for (int i=0; i<MAX_TICKET_SIZE; i++) {
        sem_wait(&my_item.mySem);

        if (my_item.ticket_sum > 0) {
            sleep(1);
            printf("%lu sold %d ticket\n", pthread_self(), 10-my_item.ticket_sum+1);

            my_item.ticket_sum --;
        }

        sem_post(&my_item.mySem);
        sleep(1);
    }

    return 0;
}

