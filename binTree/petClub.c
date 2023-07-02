#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"


char menu(void);
void addPet(Tree* pt);
void dropPet(Tree* pt);
void showPets(const Tree* pt);
void findPet(const Tree* pt);
void printItem(Item item);
void upperCase(char* str);

char* s_gets(char* str, int n);


int main(void)
{
    Tree pets;
    char choice;

    InitializeTree(&pets);

    while ((choice = menu()) != 'q') {
        switch (choice) {
        case 'a': addPet(&pets);
            break;
        case 'l': showPets(&pets);
            break;
        case 'f': findPet(&pets);
            break;
        case 'n':
            printf("%d pets in club\n", TreeItemCount(&pets));
            break;
        case 'd': dropPet(&pets);
            break;

        default:
            puts("Switching error");
        }
    }

    DeinitTree(&pets);
    puts("Done!");

    return 0;
}

char menu(void)
{
    int ch;

    puts("Pet club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a) add a pet          l) show list of pets");
    puts("n) numbers of pets    f) find pets");
    puts("d) delete a pet       q) Quit");

    while ((ch = getchar()) != EOF) {
        while (getchar() != '\n') {
            continue;
        }

        ch = tolower(ch);
        if (strchr("alfndq", ch) == NULL) {
            puts("Please enter an a, l, f, n, d, or q:");
        } else {
            break;
        }
    }

/* 
在这个代码的上下文中，`EOF`（End of File）是一个特殊的标记，代表输入流结束。在大多数系统中，
`EOF`的值是-1。在标准输入（通常是键盘输入）中，我们可以通过某些特殊的键盘组合来生成`EOF`。

在Unix或Linux系统中，我们可以通过按`Ctrl + D`生成`EOF`。在Windows系统中，我们可以通过按`Ctrl + Z`然后按回车键来生成`EOF`。

这段代码的这一部分逻辑主要是为了处理用户可能输入的`EOF`。
如果用户输入了`EOF`
（在Unix/Linux系统中，按下`Ctrl + D`；
在Windows系统中，按下`Ctrl + Z`后回车），那么程序将`EOF`视为用户想要退出（即选项'q'），因此将`ch`设置为字符'q'，然后返回。
 */
    if (ch == EOF) {
        ch = 'q';
    }

    return ch;
}

void addPet(Tree* pt)
{
    Item temp;

    if (TreeIsFull(pt)) {
        puts("No room in the club!");
    } else {
        puts("Name of the pet?");
        s_gets(temp.petname, SLEN);

        puts("Kind of the pet?");
        s_gets(temp.petkind, SLEN);

        upperCase(temp.petname);
        upperCase(temp.petkind);

        AddItem(&temp, pt);
    }
}

void showPets(const Tree* pt)
{
    if (TreeIsEmpty(pt)) {
        puts("No pets!");
    } else {
        Traverse(pt, printItem);
    }
}

void printItem(Item item)
{
    printf("Pet: %-19s Kind: %-19s\n", item.petname, item.petkind);
}

void findPet(const Tree* pt)
{
    Item temp;

    if (TreeIsEmpty(pt)) {
        puts("No pets!");
        return;
    }

    puts("name of pet you wish to find?");
    s_gets(temp.petname, SLEN);

    puts("kind of pet you wish to find?");
    s_gets(temp.petkind, SLEN);

    upperCase(temp.petkind);
    upperCase(temp.petname);

    printf("%s the %s ", temp.petname, temp.petkind);

    if (InTree(&temp, pt)) {
        printf("is a member.\n");
    } else {
        printf("is not a member.\n");
    }
}

void dropPet(Tree* pt)
{
    Item temp;

    if (TreeIsEmpty(pt)) {
        puts("No pets!");
        return;
    }

    puts("name of pet you wish to delete?");
    s_gets(temp.petname, SLEN);

    puts("kind of pet you wish to delete?");
    s_gets(temp.petkind, SLEN);

    upperCase(temp.petkind);
    upperCase(temp.petname);

    printf("%s the %s ", temp.petname, temp.petkind);
    if (DeleteItem(&temp, pt)) {
        printf("is dropped from the club.\n");
    } else {
        printf("is not a member.\n");
    }
}

void upperCase(char* str)
{
    while (*str) {
        *str = toupper(*str);
        str ++;
    }
}

char* s_gets(char* str, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(str, n, stdin);
    if (ret_val)
    {
         find = strchr(str, '\n');    // 查找换行符
         if (find)                   // 如果地址不是 NULL，
              *find = '\0';          // 在此处放置一个空字符
         else
              while (getchar() != '\n')
                   continue;         // 处理剩余输入行
    }
    return ret_val;
}