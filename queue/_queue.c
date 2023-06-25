#include <stdio.h>
#include <stdlib.h>
#include "_queue.h"


static void CopyToNode(Item item, Node* PNode);
static void CopyToItem(Node* pNode, Item* pItem);

//指向队列首尾的指针设置为NULL，且将项数设置为0
void InitializeQueue(Queue* pq)
{
    pq->front = NULL;
    pq->rear = NULL;
    pq->count = 0;
}

bool QueueIsFull(const Queue* pq)
{
    return pq->count == MAXQUEUE;
}

bool QueueIsEmpty(const Queue* pq)
{
    return pq->count == 0;
}

int QueueItemCount(const Queue* pq)
{
    return pq->count;
}

bool AddQueue(Item item, Queue* pq)
{
    Node* pNew;

    if (QueueIsFull(pq)) {
        printf("QueueIsFull, you can't add!\n");
        return false;
    }

    pNew = (Node*)malloc(sizeof(Node));
    if (!pNew) {
        fprintf(stderr, "malloc failed!\n");
        exit(1);
    }
    CopyToNode(item, pNew);
    pNew->next = NULL;

    if (QueueIsEmpty(pq)) {         //队列为空
        pq->front = pNew;           //只会跑进来一次
    } else {
        pq->rear->next = pNew;      //链接到队列尾端
    }

    pq->rear = pNew;                //前进一步，更新rear
    pq->count ++;                   //队列项数+1


    return true;
}

//先进先出，返回队首元素并在队列中删除它
bool DelQueue(Item* pItem, Queue* pq)
{
    Node* pNode;

    if (QueueIsEmpty(pq)) {
        printf("QueueIsEmpty, you can't delete!\n");
        return false;
    }

    //这里等同于pop，在删除队首元素后，可以额外得到队首元素(就看实际情况下是否关心)
    CopyToItem(pq->front, pItem);

    pNode = pq->front;              //保存待删除的节点
    pq->front = pq->front->next;    //front指向next
    free(pNode);                    //释放内存

    pq->count --;

    //如果删除的是最后一项，将尾指针重置为NULL(头指针在前面步骤中已经指向了 pq->front->next)
    if (pq->count == 0) {
        pq->rear = NULL;
    }

    return true;
}

void EmptyQueue(Queue* pq)
{
    Item dummy;
    while (!QueueIsEmpty(pq)) {
        DelQueue(&dummy, pq);
    }
}

void CopyToNode(Item item, Node* PNode)
{
    PNode->item = item;
}

void CopyToItem(Node* pNode, Item* pItem)
{
    *pItem = pNode->item;
}