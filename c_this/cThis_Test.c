#include <stdio.h>
#include <stdlib.h>
#include "c_this.h"


Demo* Creat(int i, int j)      //模拟构造函数，返回值为模拟出来的this指针
{
    Test* t = (Test*)malloc(sizeof(Test));
    if (NULL != t) {
        t->mi = i;
        t->mj = j;
    }
    return t;
}

int GetI(Demo* pThis)
{
    Test* obj = (Test*)pThis;
    return obj->mi;
}

int GetJ(Demo* pThis)
{
    Test* obj = (Test*)pThis;
    return obj->mj;
}

int Add(Demo* pThis, int k)
{
    Test* obj = (Test*)pThis;
    return obj->mi + obj->mj;
}

void Free(Demo* pThis)         //模拟析构函数
{
    if (NULL != pThis) {
        free(pThis);
    }
}

void main()
{
    Test* t = Creat(3,4);
    printf("GetI=%d.\n",GetI(t));
    printf("GetJ=%d.\n",GetJ(t));
    printf("Add=%d.\n",Add(t,3));

    Free(t);
}