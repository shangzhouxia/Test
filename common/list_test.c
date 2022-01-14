//#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include "list.h"

#define MAX_NAME_LEN 32
#define MAX_ID_LEN 10

typedef struct stud {
    struct list_head list;
    char name[MAX_NAME_LEN];
    char stu_number[MAX_ID_LEN];
} num_n_stu;
 
int main()
{
    struct list_head head;
    num_n_stu stu_1;
    num_n_stu stu_2;
    num_n_stu* entry;
 
    struct list_head* p;
    init_list_head(&head);

    strncpy(stu_1.name, "lisi", MAX_NAME_LEN);
    strncpy(stu_1.stu_number, "1000000", MAX_ID_LEN);
    strncpy(stu_2.name, "zhangsan", MAX_NAME_LEN);
    strncpy(stu_2.stu_number, "10000001", MAX_ID_LEN);

    list_add(&stu_1.list, &head);
    list_add(&stu_2.list, &head);

    printf("add stu_1,stu_2.\n");
    //LIST_FOR_EACH(p, &head) {
    for (p=(&head)->next; p != (&head); p=p->next) {
        entry = list_entry(p, struct stud, list);
        printf("name: %s\n", entry->name);
        printf("stu_number: %s\n", entry->stu_number);
    }
    list_del(&stu_2.list);
    printf("delete stu_2.\n");

    LIST_FOR_EACH(p, &head) {
        entry = list_entry(p, struct stud, list);
        printf("name: %s\n", entry->name);
        printf("stu_number: %s\n", entry->stu_number);
    }
    list_del(&stu_1.list);

    return 0;
}
 