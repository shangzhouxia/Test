#ifndef _C_THIS_H_
#define _C_THIS_H_

typedef void Demo;      //隐藏对外属性，模拟private限定符
typedef struct test {
    int mi;
    int mj;
} Test;

Demo* Creat(int i, int j);      //模拟构造函数，返回值为模拟出来的this指针
int GetI(Demo* pThis);
int GetJ(Demo* pThis);
int Add(Demo* pThis, int k);
void Free(Demo* pThis);         //模拟析构函数

#endif