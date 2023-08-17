#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

extern void custom_get(const char* name);
extern void custom_put(const char* name);

static int __init module1_init(void)
{
    custom_get("moudle_1");
    return 0;
}

static void __exit module1_exit(void)
{
    custom_put("moudle_1");
}


module_init(module1_init);
module_exit(module1_exit);
MODULE_LICENSE("GPL");