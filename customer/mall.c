#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

//一小时包含多少分钟
#define     MIN_PER_HR      60.0

bool newcustomer(double x);         //是否有新顾客到来？
Item customertime(long when);       //设置顾客参数

int main(void)
{
    Queue line;
    Item temp;
    int hours;                  //IN: 模拟小时数
    int perhour;                //IN: 模拟每小时平均多少顾客
    long cycle, cyclelimit;     //循环计数器及其上限
    long turnaways = 0;         //队列满被拒绝的客户数量
    long customers = 0;         //加入队列的客户数量
    long served = 0;            //已经服务的顾客数量
    long sum_line = 0;          //累计的队列总长
    int wait_time = 0;          //当前到空闲所需的时间
    double minutes_per_cust;    //顾客到来的平均时间,单位: 分钟
    long line_wait = 0;         //队列累计等待时间


    InitializeQueue(&line);
    srand((unsigned int) time(0));

    puts("Case study:");
    puts("Enter the number of simulation hours:");
    scanf("%d", &hours);

    cyclelimit = MIN_PER_HR * hours;        //咨询的最低时限是1min

    puts("Enter the average number of customers per hour:");
    scanf("%d", &perhour);

    minutes_per_cust = MIN_PER_HR / perhour;

    //以分钟为单位
    for (cycle=0; cycle<cyclelimit; cycle++) {
        if (newcustomer(minutes_per_cust)) {    //这一分钟，是否可能有新客户加入?
            if (QueueIsFull(&line)) {
                turnaways ++;                   //队列满，可能的新客户被拒绝
            } else {
                customers ++;                   //更新加入的客户数量
                temp = customertime(cycle);
                AddQueue(temp, &line);
            }
        }

        if (wait_time <= 0 && !QueueIsEmpty(&line)) {
            //这里的del意思是排在队首的人可以去咨询了，而不是单纯的剔出队伍的意思
            DelQueue(&temp, &line);
            wait_time = temp.processtime;           //有人已经进了咨询室，这里设置等待时间
            line_wait += cycle - temp.arrive;       //这个人等待的时间 = 当前时间 - 加入排队队伍的时间
            served ++;                              //更新已经服务的总人数
        }

        if (wait_time > 0) {
            wait_time --;                           //咨询室里在消耗时间
        }

        sum_line += QueueItemCount(&line);          //更新队列的总长度
    }

    if (customers > 0) {
        printf("customers accepted: %ld\n", customers);
        printf("customers served: %ld\n", served);
        printf("reject customers: %ld\n", turnaways);

        printf("\n\nAverage queue size: %.2f\n", (double)sum_line/ cyclelimit);
         printf("\n\nAverage wait time: %.2f minutes\n", (double)line_wait/ served);
    } else {
        puts("No customers!");
    }

    EmptyQueue(&line);
    puts("Done!");

    return 0;
}


/* 
顾客到来的平均时间: x分钟
一分钟内有客户到来，返回true 

这个函数的基本思想是:
    如果客户的到来是一个随机事件，且平均每x分钟有一个客户到来，那么在任何给定的分钟内，有客户到来的概率应该是1/x。
因此，这个函数生成一个在0到x之间的随机数，如果这个数小于1，那么就认为有客户到来。

当x = 1.0时，函数newcustomer(x)的返回值几乎总是true（即1），因为rand() * x / RAND_MAX生成的随机数在0到1之间，大部分情况下都会小于1。
当x = 2.0时，rand() * x / RAND_MAX生成的随机数在0到2之间，有一半的概率会小于1。
当x = 3.0时，rand() * x / RAND_MAX生成的随机数在0到3之间，有大约三分之一的概率会小于1。
*/
bool newcustomer(double x)
{
    if (rand() * x / RAND_MAX < 1) {
        return true;
    } else {
        return false;
    }
}

Item customertime(long when)
{
    Item cust;

    //rand() % 3 结果在[0,2]之间
    cust.processtime = rand() % 3 + 1;      //[1,3]之间的数
    cust.arrive = when;

    return cust;
}