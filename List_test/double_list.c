#if 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//创建一个双链表的数据结构
typedef struct __double_list {
    int data;
    struct __double_list* prev;
    struct __double_list* next;
} DL;

//创建双向链表并插入一个节点
DL* create_dl_list(int data)
{
    DL* p = malloc(sizeof(DL));
    if (NULL == p) {
        printf("%s failed.\n",__func__);
        return NULL;
    }

    p->data = data;
    p->next = NULL;
    p->prev = NULL;
}

//双向链表的尾插
void double_list_tail_insert(DL* header, DL* new)
{
    DL* p = header;

    //找到链表的尾节点
    while (NULL != p->next) {
        p = p->next;
    }

    //找到了尾节点，指向新节点的首地址
    p->next = new;
    //新节点的prev指针指向原来的尾节点的首地址
    new->prev = p;
}

//双链表的头插
void double_list_top_insert(DL* header, DL* new)
{
    //新节点的next指针指向原来的节点1的地址
    new->next = header->next;
    //当前节点的下一个节点的地址是否为空
    if (NULL != header->next) {
        header->next->prev = new;
    }

    header->next = new;
    new->prev = header;
}

//双向链表的正向遍历
void double_list_for_each(DL* header)
{
    DL* p = header;
    while(NULL != p->next) {
        p = p->next;
        printf("%s data:%d.\n",__func__,p->data);
    }
}

//双向链表的反向遍历
void double_list_for_each_reverse(DL* header)
{
    DL* p = header;

    //先找到尾节点
    while(NULL != p->next) {
        p = p->next;
    }

    //已经找到了尾节点开始向前遍历
    while (NULL != p->prev) {
        printf("%s data:%d.\n",__func__,p->data);
        p = p->prev;
    }
}

//双向链表删除节点
int double_list_delete_node(DL* header, int data)
{
    DL* p = header;

    //遍历所有的节点
    while(NULL != p->next) {
        p = p->next;

        if(data == p->data) {
            if (NULL != p->next) {
                p->next->prev = p->prev;
                p->prev->next = p->next;
                free(p);
            } else {    //当前节点的下一个节点为空
                p->prev->next = NULL;
                free(p);
            }
            return 0;
        }
    }
    printf("No node to delete found.\n");
    return -1;
}

int main(void)
{
    int i;
    DL* header = create_dl_list(0);
    for (i=0; i<10; i++) {
        //double_list_tail_insert(header, create_dl_list(i));
        double_list_top_insert(header, create_dl_list(i));
    }

    double_list_delete_node(header,5);
    double_list_for_each(header);

    double_list_delete_node(header,5);

    double_list_for_each_reverse(header);

    return 0;
}

#else 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//创建一个双链表的数据结构
typedef struct __double_list
{
	int data ;
	struct __double_list *prev ;
	struct __double_list *next ;
}DL ; 
 
//创建双向链表并插入一个节点 
DL *create_dl_node(int data)
{
	DL *p = malloc(sizeof(DL));
	if(NULL == p)
	{
		printf("create dl node fair!\n");
		return NULL ;
	}
	//初始化数据 
	p->data = data ;
	//初始化指针 
	p->next = NULL ;
	p->prev = NULL ;
}
 
//双向链表的尾插 
void double_list_tail_insert(DL *header , DL *new)
{
	//取得当前节点的地址 
	DL *p = header ;
	//找到链表的尾节点 
	while(NULL != p->next)
	{
		//找不到接着找 
		p = p->next ;
	}
	//找到了尾节点，指向新节点的首地址 
	p->next = new ;
	//新节点的prev指针指向原来的尾节点的首地址。
	new->prev = p;
}
 
//双向链表的头插(也就是插在两个节点之间的插入方式)
void double_list_top_insert(DL *header , DL *new)
{
	//新节点的next指针指向原来的节点一的地址
	new->next = header->next ; 
	//判断当前节点的下一个节点的地址是否为空
	if(NULL != header->next) 
		header->next->prev = new ; //节点1的prev指针指向新节点的地址 
	header->next = new ;
	new->prev = header ;
}
 
//双向链表的正向遍历 
void double_list_for_each(DL *header)
{
	DL *p = header ;
	while(NULL != p->next)
	{
		p = p->next ;
		printf("%d ",p->data);
	}
}
 
//双向链表的反向遍历 
void double_list_for_each_nx(DL *header)
{
	DL *p = header ;
	//先找到尾节点
	while(NULL != p->next)
	{
		p = p->next ;	
	} 
	//已经找到了尾节点，向前遍历，注意，遍历到头节点之前
	//限制条件: != 头结点 
	while(NULL != p->prev)
	{
		printf("%d ",p->data);
		p = p->prev ;
	}
}
 
//双向链表节点的删除
int double_list_delete_node(DL *header , int data)
{
	//取得当前节点 
	DL *p = header;
	//遍历所有的节点 
	while(NULL != p->next)
	{
		p = p->next ;
		//找到了对应要删除的数据了 
		if(p->data == data)
		{
			//一样存在两种情况
			//(1)当前节点的下一个节点不为空
			if(p->next != NULL)
			{
				//那就把当前节点的prev节点指向要删除的这个节点的prev
				//因为当前的prev节点保存的是要删除的上一个节点的指针 
				p->next->prev = p->prev ;
				//还要指定它的next节点 
				p->prev->next = p->next ;
				free(p);
			}
			//(2)当前节点的下一个节点为空 
			else
			{
				//把 
				p->prev->next = NULL ;
				free(p); 
			}
			return 0 ;
		}
	}
	printf("\n没有找到对应要删除的节点，或者节点已经被删除！\n");
	return -1 ;	
} 
 
int main(void)
{
	int i ;
	DL *header = create_dl_node(0);
	for(i = 0 ; i < 10 ; i++)
	{
		//双向链表的尾插 
		//double_list_tail_insert(header,create_dl_node(i));
		//双向链表的头插 
		double_list_top_insert(header,create_dl_node(i));
	}
	//双向链表的正向遍历 
	printf("双向链表的正向遍历:");
	double_list_delete_node(header,5);
	double_list_for_each(header);
//	double_list_delete_node(header,9);
	double_list_delete_node(header,5);
	putchar('\n');
	printf("双向链表的反向遍历:");
	double_list_for_each_nx(header);
	return 0 ;
}

#endif