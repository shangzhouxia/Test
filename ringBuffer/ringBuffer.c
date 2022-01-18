#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LEN         64

//环形队列结构体
typedef struct ring_buff {
    int array[LEN];
    int w;
    int r;
} *ring;

//环形队列初始化
struct ring_buff* fifo_init(void)
{
    struct ring_buff* p = NULL;
    p = (struct ring_buff*)malloc(sizeof(struct ring_buff));
    if (p == NULL) {
        printf("%s failed.\n",__func__);
        return NULL;
    }

    memset(p, 0, sizeof(struct ring_buff));

    return p;
}

int get_ring_buff_fullstate(struct ring_buff* p_ring_buff)
{
    if ((p_ring_buff->w - p_ring_buff->r) == LEN) {
        return 1;
    } else {
        return 0;
    }
}

int get_ringbuff_emptystate(struct ring_buff* p_ring_buff)
{
    if (p_ring_buff->w == p_ring_buff->r) {
        return 1;
    } else {
        return 0;
    }
}

int ring_buff_insert(struct ring_buff* p_ring_buff, int data)
{
    int i = 0;
    if (p_ring_buff == NULL) {
        printf("p null.\n");
        return -1;
    }

    if (get_ring_buff_fullstate(p_ring_buff) == 1) {
        printf("buff is full.\n");
        return -2;
    }

    //p_ring_buff->array[p_ring_buff->w % LEN] = data;
    i = p_ring_buff->w & (LEN-1);
    p_ring_buff->array[i] = data;
    p_ring_buff->w  ++;
    return 0;
}

int ring_buff_get(struct ring_buff* p_ring_buff)
{
    int data = 0;

    if (p_ring_buff == NULL) {
        printf("p null.\n");
        return -1;
    }

    if (get_ringbuff_emptystate(p_ring_buff) == 1) {
        printf("buff is full.\n");
        return -2;
    }

    //data = p_ring_buff->array[p_ring_buff->r % LEN];
    data = p_ring_buff->array[p_ring_buff->r & (LEN-1)];
    p_ring_buff->r ++;
    return data;
}

int ring_buff_destory(struct ring_buff* p_ring_buff)
{
    if (p_ring_buff == NULL) {
        printf("p null.\n");
        return -1;
    }

    free(p_ring_buff);
    return 0;
}


int main()
{
    int i=0;

    ring pt_ring_buff = fifo_init();

    for (i=0; i<LEN; i++) {
        ring_buff_insert(pt_ring_buff, i+2);
    }

    for (i=0; i<LEN; i++) {
        printf("%d ", ring_buff_get(pt_ring_buff));
    }

    ring_buff_destory(pt_ring_buff);

    return 0;
}
