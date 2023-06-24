#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

#define     TSIZE       45

struct film {
    char title[TSIZE];          //电影标题
    int rating;                 //电影分级
};

typedef struct film Item;

typedef struct node {
    Item item;
    struct node* next;
} Node;

typedef Node* List;


/* 
初始化一个链表
IN:     pHead是一个指针，指向一个链表
OUT:    pHead指向的链表初始化为NULL
 */
void InitializeList(List* pHead);

/* 
判断一个链表是否为空
即判断头节点是否为空
 */
bool ListIsEmpty(const List* pHead);

/* 
判断链表是否满了
这里不是判断节点的数量，而是判断系统是否还能有内存可以让malloc执行成功
 */
bool ListIsFull(const List* pHead);

/* 
现有链表中节点的数量
 */
unsigned int ListCount(const List* pHead);


/* 
在链表尾部添加新的节点
IN: 新的节点item
IN: pHead指向头节点
 */
bool AddItem(Item item, List* pHead);

/* 
遍历链表节点，并对每个节点执行函数指针pfunc
 */
void Traverse(const List* pHead, void (*pfunc) (Item item));


/* 
释放每个节点malloc的资源
 */
void FreeList(List* pHead);


#endif