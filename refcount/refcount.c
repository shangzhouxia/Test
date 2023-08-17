#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kref.h>
#include <linux/slab.h>

typedef struct {
    struct kref refcount;
} CUSTOM_OBJ_T;

static CUSTOM_OBJ_T *my_obj;

void custom_get(const char* name);
void custom_put(const char* name);

static void custom_release(struct kref *ref)
{
    CUSTOM_OBJ_T * obj = container_of(ref, CUSTOM_OBJ_T, refcount);
    printk("%s \n", __func__);
    kfree(obj);
}


void custom_get(const char* name)
{
    printk("module name: %s, %s\n", name, __func__);
    kref_get(&my_obj->refcount);
}
EXPORT_SYMBOL_GPL(custom_get);

void custom_put(const char* name)
{
    printk("module name: %s, %s\n", name, __func__);
    kref_put(&my_obj->refcount, custom_release);
}
EXPORT_SYMBOL_GPL(custom_put);

static int __init ref_init(void)
{
    my_obj = kzalloc(sizeof(CUSTOM_OBJ_T), GFP_KERNEL);
    if (!my_obj) {
        printk("kzalloc faild.\n");
        return -ENOMEM;
    }

    kref_init(&my_obj->refcount);

    return 0;
}

static void __exit ref_exit(void)
{
    printk("%s\n", __func__);
    kref_put(&my_obj->refcount, custom_release);
}



module_init(ref_init);
module_exit(ref_exit);
MODULE_LICENSE("GPL");