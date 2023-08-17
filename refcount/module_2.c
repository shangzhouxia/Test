#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

extern void custom_get(const char* name);
extern void custom_put(const char* name);

static int __init module2_init(void)
{
    custom_get("moudle_2");
    return 0;
}

static void __exit module2_exit(void)
{
    custom_put("moudle_2");
}


module_init(module2_init);
module_exit(module2_exit);
MODULE_LICENSE("GPL");