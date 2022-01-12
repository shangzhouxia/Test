#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
typedef struct list_node list_single;
struct list_node {
    int data;
    struct list_node* next;
};


//创建节点
list_single* create_list_node(const int data)
{
    list_single* node = NULL;
    node = (list_single*)malloc(sizeof(list_single));
    if (NULL == node) {
        printf("malloc fail.\n");
        return NULL;
    }

    memset(node, 0, sizeof(list_single));

    node->data = data;
    node->next = NULL;

    return node;
}
#endif

typedef struct slist {
    int id;
    struct slist* next;
}L;

//创建一个节点
L* create_node(int data)
{
    //给每个节点分配空间
    L* p = malloc(sizeof(L));
    if (NULL==p) {
        printf("%s malloc fail.\n",__func__);
        return NULL;
    }

    //初始化结构体
    memset(p, 0, sizeof(L));
    //初始化第一个节点
    p->id = data;
    p->next = NULL;
}

//链表的尾插
void tail_insert(L* pH, L* new)
{
    //获取当前的位置
    L* p = pH;
    //如果当前位置的下一个节点不为空
    while (NULL != p->next) {
        //移动到下一个节点
        p = p->next;
    }
    //上面循环退出，说明已经到了List的尾端
    //新插入的节点赋值到NULL这个位置
    p->next = new;
}

//链表的头插
void top_insert(L* pH, L* new)
{
    L* p = pH;
    new->next = p->next;
    p->next = new;
}

//链表的遍历
void Print_node(L* pH)
{
    L* p = pH;

    p = p->next;    //获取第一个节点的位置
    while(NULL != p->next) {     //当前位置的下一个节点不为空
        printf("id:%d\n",p->id);
        p = p->next;            //移动到下一个节点
    }

    //如果当前位置的下一个节点为空，则打印数据，说明只有一个节点
    printf("id:%d\n",p->id);
}

//删除链表中的节点
int delete_list_node(L* pH, int data)
{
    L* p = pH;
    L* prev = NULL;
    while (NULL != p->next) {
        prev = p;       //保存当前节点的前一个节点的指针
        p = p->next;

        if (p->id == data) {
            if (p->next != NULL) {          //普通节点
                prev->next = p->next;
                free(p);
            } else {
                prev->next = NULL;
                free(p);
            }
            return 0;
        }
    }

    printf("no list to delete.\n");
    return -1;
}

//链表逆序
void trave_list(L* pH)
{
    L* p= pH->next;     //保存第一个节点的位置
    L* pBack;

    int i=0;
    
    if (p->next == NULL || p == NULL) {
        return;
    }

    while (NULL != p->next) {       //遍历链表
        pBack = p->next;        //保存第一个节点的下一个节点
        if (p == pH->next) {
            //第一个有效节点就是最后一个节点
            p->next = NULL;
        } else {
            p->next = pH->next;
        }

        pH->next = p;
        p = pBack;
    }
    //原先的最后一个有效节点头插
    top_insert(pH, p);
}

int main()
{
#if 0
    int data = 999;
    list_single* node_ptr = create_list_node(data);
    if (NULL != node_ptr) {
        printf("node data:%d\n", node_ptr->data);
        printf("node->next:%d\n",node_ptr->next);
    }
    free(node_ptr);
#endif

    int i;
    L* header = create_node(0);
    for (i=1; i<5; i++) {
        tail_insert(header, create_node(i));
    }

    Print_node(header);

    delete_list_node(header,2);
    putchar('\n');
    Print_node(header);

    putchar('\n');
    trave_list(header);
    Print_node(header);

    return 0;
}