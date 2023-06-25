#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

#define MAXQUEUE        10


//typedef int Item;
typedef struct item {
    long arrive;            //加入队列的时间
    int processtime;        //咨询花费的时间
} Item;


//队列中的元素,由链表组成
typedef struct node {
    Item item;
    struct node* next;
} Node;


//环形队列
typedef struct queue {
    Node* front;            //指向队列首项的指针
    Node* rear;             //指向队列尾项的指针
    int count;              //队列中的项目计数
} Queue;

//初始化队列为空
void InitializeQueue(Queue* pq);

//队列是否满了
bool QueueIsFull(const Queue* pq);
//队列是否为空?
bool QueueIsEmpty(const Queue* pq);

//队列的项数
int QueueItemCount(const Queue* pq);

//将Item添加到队列
bool AddQueue(Item item, Queue* pq);
//删除项
bool DelQueue(Item* pItem, Queue* pq);

//清空队列
void EmptyQueue(Queue* pq);

#endif