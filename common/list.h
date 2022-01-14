#ifndef __LIST_H__
#define __LIST_H__
//#include <stdlib.h>


typedef struct list_head {
    struct list_head* prev;
    struct list_head* next;
};

#define LIST_FOR_EACH(pos, head) \
    for (pos=(head)->next; pos!=(head); pos=pos->next)

#define LIST_HEAD_INIT(name) {&(name), &(name)}


static inline void init_list_head(struct list_head* list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head* new_node, struct list_head* prev,
    struct list_head* next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

static inline void __list_del(struct list_head* prev, struct list_head* next)
{
    next->prev = prev;
    prev->next = next;
}

/*
new_node增加在head后面
如:
head->A_node
增加B_node
head->B_node->A_node
*/
static inline void list_add(struct list_head* new_node, struct list_head* head)
{
    __list_add(new_node, head, head->next);
}

static inline void list_add_tail(struct list_head* new_node, struct list_head* head)
{
    __list_add(new_node, head->prev, head);
}

static inline void list_del(struct list_head* entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

//根据一个结构体变量中的一个域成员变量的指针来获取指向整个结构体变量的指针
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif // !1