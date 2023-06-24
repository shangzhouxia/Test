#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void showMovies(Item item);
char* s_gets(char* st, int n);

int main(void)
{
    List movies;
    Item temp;

    //初始化一个链表并判断当前系统是否还有空间分配内存?
    InitializeList(&movies);
    if (ListIsFull(&movies)) {
        fprintf(stderr, "No memory avaiable!\n");
        exit(1);
    }


    puts("Enter first movie title:");
    while (s_gets(temp.title, TSIZE) != NULL && temp.title[0] != '\0') {
        puts("Enter rating <0-10>:");
        scanf("%d", &temp.rating);
        while (getchar() != '\n') {
            continue;
        }

        if (AddItem(temp, &movies) == false) {
            fprintf(stderr, "AddItem failed!\n");
            break;
        }

        if (ListIsFull(&movies)) {
            puts("The list is now full.");
            break;
        }

        puts("Next movie?(empty to stop):");
    }

    if (ListIsEmpty(&movies)) {
        printf("No data inputed!\n");
    } else {
        printf("Here is the movie list:\n");
        Traverse(&movies, showMovies);
    }

    printf("You entered %d movies.\n", ListCount(&movies));

    FreeList(&movies);
    printf("Bye\n");

    return 0;
}

void showMovies(Item item)
{
    printf("Movie: %s Rating: %d\n", item.title, item.rating);
}

char * s_gets(char * st, int n)
{
     char * ret_val;
     char * find;

     ret_val = fgets(st, n, stdin);
     if (ret_val)
     {
          find = strchr(st, '\n');    // 查找换行符
          if (find)                   // 如果地址不是 NULL，
               *find = '\0';          // 在此处放置一个空字符
          else
               while (getchar() != '\n')
                    continue;         // 处理剩余输入行
     }
     return ret_val;
}