#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/rculist.h>
#include <linux/slab.h>
#include <linux/sched.h>


struct my_node {
    int value;
    struct list_head list;
};


static LIST_HEAD(my_list);              //初始化链表头
static DEFINE_SPINLOCK(list_lock);

static struct task_struct *reader_thread, * writer_thread;

static int write_thr_func(void* data);
static int read_thr_func(void* data);

int write_thr_func(void* data)
{
    struct my_node* node;

    //return test_bit(KTHREAD_SHOULD_STOP, &to_kthread(current)->flags);
    while (!kthread_should_stop()) {
        node = kmalloc(sizeof(struct my_node), GFP_KERNEL);

        if (node) {
            node->value = jiffies;                  //节点值是当前时间

            spin_lock(&list_lock);                  //读线程中对链表修改需要加锁
            list_add_rcu(&node->list, &my_list);    //添加链表项
            spin_unlock(&list_lock);

            printk(KERN_INFO"Write: %d\b", node->value);

            msleep(1000);                           //每秒钟添加一个新的节点
        }
    }

    return 0;
}

/* 
list_for_each_entry_rcu 是 Linux 内核中的一个宏，用于在 Read-Copy-Update (RCU) 受保护的链表中遍历每一个元素。
这个宏提供了一种安全地在没有锁的情况下读取 RCU 链表的方法。

在 list_for_each_entry_rcu 循环中，你可以安全地读取每一个节点，即使在其他线程中有新的节点被添加到链表中。你应该在调用 list_for_each_entry_rcu 之前获取 RCU 读锁，然后在循环结束后释放读锁。

list_for_each_entry_rcu 宏的
第一个参数是用于存储当前节点的指针，
第二个参数是链表的头，
第三个参数是链表元素在结构体中的名称。

请注意，你不应该在 list_for_each_entry_rcu 循环中修改链表。如果你需要修改链表，你应该使用相应的 RCU 函数，比如 list_add_rcu、list_del_rcu 等，并确保有适当的锁来保护写操作。
 */

int read_thr_func(void* data)
{
    struct my_node* node;

    while (!kthread_should_stop()) {
        rcu_read_lock();                //标记RCU读过程的开始

        list_for_each_entry_rcu(node, &my_list, list) {
            printk(KERN_INFO"Read: %d\n", node->value);
            msleep(500);
        }

        rcu_read_unlock();              //标记RCU读线程的结束
        msleep(1000);
    }

    return 0;
}


static int __init rcu_test_init(void)
{
    reader_thread = kthread_run(read_thr_func, NULL, "rcu_reader");
    writer_thread = kthread_run(write_thr_func, NULL, "rcu_write");

    return 0;
}


/* 
synchronize_rcu 是 Linux 内核中的一个函数，它会阻塞当前的线程，直到所有正在进行的 RCU 读取临界区的操作都完成。
这是一种等待所有已经开始但还未结束的 RCU 读取操作完成的方法。

在使用 Read-Copy-Update (RCU) 机制时，你可以在没有锁的情况下读取数据。然而，当你需要更新数据时，你需要确保没有其他线程正在读取旧的数据。
synchronize_rcu 函数就是用于这个目的的。

在调用 list_del_rcu 之后，节点从链表中移除，但可能还有其他线程在读取这个节点。
调用 synchronize_rcu 会等待所有正在读取这个节点的线程完成读取。
然后，当 synchronize_rcu 返回时，你就可以安全地释放这个节点。

需要注意的是，synchronize_rcu 可能会阻塞一段时间，你应该避免在持有锁的情况下调用它，否则可能会造成死锁。
 */
static void __exit rcu_test_exit(void)
{
    struct my_node* node;
    struct list_head *pos, *q;

    //set_bit(KTHREAD_SHOULD_STOP, &kthread->flags);
    kthread_stop(reader_thread);
    kthread_stop(writer_thread);

    spin_lock(&list_lock);

    list_for_each_safe(pos, q, &my_list) {
        node = list_entry(pos, struct my_node, list);

        list_del_rcu(pos);          //删除链表项
        synchronize_rcu();          //阻塞等待读取RCU完成

        kfree(node);                //synchronize_rcu 返回之后，才能真正释放节点内存
    }

    spin_unlock(&list_lock);

}


module_init(rcu_test_init);
module_exit(rcu_test_exit);

MODULE_LICENSE("GPL");