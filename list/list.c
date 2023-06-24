#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static void CopyToNode(Item item, Node* pnode);

//这里应该是头节点初始化为NULL而不是存储头节点的指针初始化为NULL，否则在AddItem中判断就会出错
void InitializeList(List* pHead)
{
    //pHead = NULL;
    *pHead = NULL;
}

bool ListIsEmpty(const List* pHead)
{
    if (*pHead == NULL) {
        return true;
    } else {
        return false;
    }
}

//这是看系统还有没有空间分配???
bool ListIsFull(const List* pHead)
{
    Node* pNode;
    bool full;

    pNode = (Node*)malloc(sizeof(Node));
    if (pNode == NULL) {
        full = true;
    } else {
        full = false;
    }

    free(pNode);

    return full;
}

unsigned int ListCount(const List* pHead)
{
    unsigned int count;
    Node* pNode = *pHead;

    while (pNode != NULL) {
        count ++;
        pNode = pNode->next;
    }

    return count;
}

bool AddItem(Item item, List* pHead)
{
    Node* pNew;
    Node* scan = *pHead;

    pNew = (Node*)malloc(sizeof(Node));
    if (pNew == NULL) {
        return false;
    }

    CopyToNode(item, pNew);
    pNew->next = NULL;

    if (scan == NULL) {             //空链表
        *pHead = pNew;
    } else {
        while (scan->next != NULL) {
            scan = scan->next;      //遍历到链表尾部
        }

        scan->next = pNew;          //把pNew添加到链表的末尾
    }

    return true;
}

void Traverse(const List* pHead, void (*pfunc) (Item item))
{
    Node* pNode = *pHead;

    while (pNode != NULL) {
        pfunc(pNode->item);         //对链表中的每个节点执行形参传递进来的函数指针
        pNode = pNode->next;
    }
}

void FreeList(List* pHead)
{
    Node* pSave;

    while (*pHead != NULL) {
        pSave = (*pHead)->next;     //1)保存下一个节点
        free(*pHead);               //2)释放当前节点
        *pHead = pSave;             //3)执行第一步保存的next节点
    }
}

static void CopyToNode(Item item, Node* pnode)
{
    pnode->item = item;
}