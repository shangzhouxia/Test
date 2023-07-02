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
    struct rcu_head rcu;            //for rcu callback
};

static LIST_HEAD(my_list);              //初始化链表头
static DEFINE_SPINLOCK(list_lock);

static struct task_struct *reader_thread, *writer_thread;

static int write_thr_func(void* data);
static int read_thr_func(void* data);


static void rcu_cb(struct rcu_head *head)
{
    //根据结构成员rcu找到结构体地址
    struct my_node* node = container_of(head, struct my_node, rcu);
    kfree(node);
}

static int write_thr_func(void* data)
{
    struct my_node* node;

    while (!kthread_should_stop()) {
        node = kmalloc(sizeof(struct my_node), GFP_KERNEL);
        if (node) {
            node->value = jiffies;

            spin_lock(&list_lock);
            list_add_rcu(&node->list, &my_list);
            spin_unlock(&list_lock);

            printk(KERN_INFO "Write: %d\n", node->value);
            msleep(2000);
        }
    }

    return 0;
}

static int read_thr_func(void* data)
{
    struct my_node* node;

    while (!kthread_should_stop()) {
        rcu_read_lock();

        //list_entry_rcu——>container_of(rcu_dereference(ptr), type, member)
        //本质上还是container_of，通过成员变量list找到结构体的地址struct my_node
        list_for_each_entry_rcu(node, &my_list, list) {
            printk(KERN_INFO "Read: %d\n", node->value);
            msleep(500);
        }
        rcu_read_unlock();
        msleep(1000);

    }

    return 0;
}

static int __init call_run_init(void)
{
    reader_thread = kthread_run(read_thr_func, NULL, "call_read");
    writer_thread = kthread_run(write_thr_func, NULL, "call_write");

    return 0;
}

static void __exit call_run_exit(void)
{
    struct my_node* node;
    struct list_head *pos, *q;

    kthread_stop(reader_thread);
    kthread_stop(writer_thread);

    spin_lock(&list_lock);

    list_for_each_safe(pos, q, &my_list) {
        node = list_entry(pos, struct my_node, list);

        list_del_rcu(pos);

        call_rcu(&node->rcu, rcu_cb);
    }

    spin_unlock(&list_lock);
}


module_init(call_run_init);
module_exit(call_run_exit);
MODULE_LICENSE("GPL");