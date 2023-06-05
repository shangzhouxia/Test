#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>

//自定义一个结构，包含struct kobject
struct test_kobj {
    int value;
    struct kobject kobj;
};

struct test_kobj_attribute {
    struct attribute attr;
    ssize_t (*show) (struct test_kobj* obj, struct test_kobj_attribute* attr, char* buf);
    ssize_t (*store) (struct test_kobj* obj, struct test_kobj_attribute* attr, const char* buf, size_t count);
};



struct test_kobj* obj;

static ssize_t test_kobj_attr_show(struct kobject* kobj, struct attribute* attr, char* buf)
{
    struct test_kobj_attribute* test_kobj_attr;

    ssize_t ret = -EIO;

    test_kobj_attr = container_of(attr, struct test_kobj_attribute, attr);

    if (test_kobj_attr->show) {
        ret = test_kobj_attr->show(container_of(kobj, struct test_kobj, kobj), 
                    test_kobj_attr, buf);
    }

    return ret;
}

static ssize_t test_kobj_attr_store(struct kobject* kobj, struct attribute* attr,const char* buf,  size_t count)
{
    struct test_kobj_attribute* test_kobj_attr;

    ssize_t ret = -EIO;

    test_kobj_attr = container_of(attr, struct test_kobj_attribute, attr);

    if (test_kobj_attr->store) {
        ret = test_kobj_attr->store(container_of(kobj, struct test_kobj, kobj), 
                    test_kobj_attr, buf, count);
    }

    return ret;
}

const struct sysfs_ops test_kobj_sysfs_ops = {
    .show = test_kobj_attr_show,
    .store = test_kobj_attr_store,
};

void obj_release(struct kobject* kobj)
{
    struct test_kobj* obj = container_of(kobj, struct test_kobj, kobj);

    printk(KERN_INFO"%s %s\n", __FUNCTION__, kobject_name(&obj->kobj));

    kfree(obj);
}

static struct kobj_type test_kobj_ktype = {
    .release = obj_release,
    .sysfs_ops = &test_kobj_sysfs_ops,
};

ssize_t name_show(struct test_kobj* obj, struct test_kobj_attribute* attr, char* buf)
{
    return sprintf(buf, "%s\n", kobject_name(&obj->kobj));
}

ssize_t value_show(struct test_kobj* obj, struct test_kobj_attribute* attr, char* buf)
{
    return sprintf(buf, "%d\n", obj->value);
}

ssize_t value_store(struct test_kobj* obj, struct test_kobj_attribute* attr, const char* buf,  size_t size)
{
    sscanf(buf, "%d", &obj->value);
    return size;
}

//定义属性，最终注册进sysfs
struct test_kobj_attribute name_attribute = __ATTR(name, 0664, name_show, NULL);
struct test_kobj_attribute value_attribute = __ATTR(value, 0664, value_show, value_store);


struct attribute* test_kobj_attrs[] = {
    &name_attribute.attr,
    &value_attribute.attr,
    NULL,
};

struct attribute_group test_kobj_group = {
    .name = "test_obj_group",
    .attrs = test_kobj_attrs,
};


static int __init test_kobj_init(void)
{
    int ret_val;
    printk(KERN_INFO"%s\n", __FUNCTION__);

    obj = kmalloc(sizeof(struct test_kobj), GFP_KERNEL);
    if (!obj) {
        printk(KERN_ERR"kmalloc failed!\n");
        return -ENOMEM;
    }

    obj->value = 1;

    memset(&obj->kobj, 0, sizeof(struct kobject));

    //添加进sysfs系统
    kobject_init_and_add(&obj->kobj, &test_kobj_ktype, NULL, "test_kobj");

    //在sys文件夹下创建文件
    ret_val = sysfs_create_files(&obj->kobj, (const struct attribute **)test_kobj_attrs);
    if (ret_val) {
        kobject_put(&obj->kobj);
        return ret_val;
    }

    //在sysfs下创建group
    ret_val = sysfs_create_group(&obj->kobj, &test_kobj_group);
    if (ret_val) {
        kobject_put(&obj->kobj);
        return ret_val;
    }

    return 0;

}

static void __exit test_kobj_exit(void)
{
    printk(KERN_INFO"%s\n", __FUNCTION__);
    kobject_del(&obj->kobj);
    kobject_put(&obj->kobj);
}


module_init(test_kobj_init);
module_exit(test_kobj_exit);

MODULE_AUTHOR("Meng");
MODULE_LICENSE("GPL");

/* 
zgl@ZenBook:~$ ls /sys/ -l
total 0
drwxr-xr-x   2 root root 0 Jun  5 22:37 block
drwxr-xr-x  23 root root 0 Jun  5 21:15 bus
drwxr-xr-x  31 root root 0 Jun  5 21:15 class
drwxr-xr-x   4 root root 0 Jun  5 22:37 dev
drwxr-xr-x  13 root root 0 Jun  5 21:15 devices
drwxr-xr-x   4 root root 0 Jun  5 22:37 firmware
drwxr-xr-x  10 root root 0 Jun  5 21:15 fs
drwxr-xr-x  12 root root 0 Jun  5 21:16 kernel
drwxr-xr-x 106 root root 0 Jun  5 22:37 module
drwxr-xr-x   3 root root 0 Jun  5 22:37 test_kobj
zgl@ZenBook:~$ cd test_doc/
zgl@ZenBook:~/test_doc$ cd ../
zgl@ZenBook:~$ cd /sys/test_kobj/
zgl@ZenBook:/sys/test_kobj$ ls -l
total 0
-rw-rw-r-- 1 root root 4096 Jun  5 22:37 name
drwxr-xr-x 2 root root    0 Jun  5 22:37 test_obj_group
-rw-rw-r-- 1 root root 4096 Jun  5 22:37 value
zgl@ZenBook:/sys/test_kobj$ cat name
test_kobj
zgl@ZenBook:/sys/test_kobj$ cat value
1
zgl@ZenBook:/sys/test_kobj$ echo 2 > value
-bash: value: Permission denied
zgl@ZenBook:/sys/test_kobj$ sudo echo 2 > value
-bash: value: Permission denied
zgl@ZenBook:/sys/test_kobj$ su
Password:
root@ZenBook:/sys/test_kobj# echo 2 > value
root@ZenBook:/sys/test_kobj# cat value
2
root@ZenBook:/sys/test_kobj#

dmesg:
[ 4870.530712] test_kobj_init
[ 5011.667289] test_kobj_exit
[ 5011.667771] obj_release test_kobj
 */