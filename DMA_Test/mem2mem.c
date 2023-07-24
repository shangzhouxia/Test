#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/jiffies.h>


#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>


#define MEMCPY_NO_DMA   0
#define MEMCPY_DMA      1
#define BUFFER_SIZE     (10 * 1024)


struct cdev my_cdev;
static int major_ret;
static struct class* pdma_class;
static struct device* pdma_device;


static dma_addr_t* src = NULL;
static dma_addr_t src_phys;
static dma_addr_t* dst = NULL;
static dma_addr_t dst_phys;

static volatile int dma_finished = 0;
static DECLARE_WAIT_QUEUE_HEAD(wq);


static int do_memcpy_with_dma(void);
static void do_memcpy_no_dma(void);
void demo_dma_callback(void *dma_async_param);


static long demo_ioctl(struct file * file, unsigned int cmd, unsigned long data)
{
    switch (cmd) {
        case MEMCPY_NO_DMA:
            do_memcpy_no_dma();
            break;
        case MEMCPY_DMA:
            do_memcpy_with_dma();
            break;
        default:
            printk("%s undefined command.\n", __func__);
            break;
    }

    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = demo_ioctl,
};


static int __init demo_init(void)
{
    int ret;
    dev_t devno = 0;

    printk("%s Start.\n", __func__);

    ret = alloc_chrdev_region(&devno, 0, 1, "dma_test");
    if (0 != ret) {
        printk("alloc_chrdev_region error\n");
        return ret;
    }

    major_ret = MAJOR(devno);
    cdev_init(&my_cdev, &fops);
    ret = cdev_add(&my_cdev, devno, 1);
    if (0 != ret) {
        printk("cdev_add error\n");
        return ret;
    }

    pdma_class = class_create(THIS_MODULE, "dma_test_class");
    pdma_device = device_create(pdma_class, NULL, MKDEV(major_ret, 0), NULL, "dma_test");

    src = dma_alloc_coherent(NULL, BUFFER_SIZE, &src_phys, GFP_ATOMIC);
    if (NULL == src) {
        printk("dma_alloc_coherent src error\n");
        goto failed_alloc_src;
    }

    dst = dma_alloc_coherent(NULL, BUFFER_SIZE, &dst_phys, GFP_ATOMIC);
    if (NULL == dst) {
        printk("dma_alloc_coherent src error\n");
        goto failed_alloc_dst;
    }

    printk("%s Done.\n", __func__);
    return 0;

failed_alloc_dst:
    dma_free_coherent(NULL, BUFFER_SIZE, src, src_phys);

failed_alloc_src:
    device_destroy(pdma_class, MKDEV(major_ret, 0));
    class_destroy(pdma_class);

    cdev_del(&my_cdev);
    unregister_chrdev_region(MKDEV(major_ret, 0), 1);

    return -1;
}


static void __exit demo_exit(void)
{
    printk("%s Start.\n", __func__);

    device_destroy(pdma_class, MKDEV(major_ret, 0));
    class_destroy(pdma_class);

    cdev_del(&my_cdev);
    unregister_chrdev_region(MKDEV(major_ret, 0), 1);

    dma_free_coherent(NULL, BUFFER_SIZE, src, src_phys);
    dma_free_coherent(NULL, BUFFER_SIZE, dst, dst_phys);

    printk("%s Done.\n", __func__);
}

void demo_dma_callback(void *dma_async_param)
{
    printk("%s.\n", __func__);

    dma_finished = 1;
    wake_up_interruptible(&wq);
}

static int do_memcpy_with_dma(void)
{
    struct dma_chan* chan = NULL;
    dma_cap_mask_t mask;
    struct dma_async_tx_descriptor* tx_desc = NULL;
    dma_cookie_t dma_cooike;

    unsigned long t1, t2, diff, msec;
    int i;

    printk("%s Start.\n", __func__);

    memset(src, 0xAA, BUFFER_SIZE);
    memset(dst, 0x55, BUFFER_SIZE);

    dma_cap_zero(mask);
    dma_cap_set(DMA_MEMCPY, mask);

    chan = dma_request_channel(mask, NULL, NULL);
    if (NULL == chan) {
        printk("%s dma_request_channel failed.\n", __func__);
        return -1;
    }

    t1 = jiffies;
    for (i=0; i<1000; i++) {
        dma_finished = 0;

        tx_desc = dmaengine_prep_dma_memcpy(chan, dst_phys, src_phys, BUFFER_SIZE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
        if (NULL == tx_desc) {
            printk("%s get tx descriptor failed.\n", __func__);
            dma_release_channel(chan);
            return -1;
        }

        tx_desc->callback = demo_dma_callback;

        dma_cooike = dmaengine_submit(tx_desc);
        if (dma_submit_error(dma_cooike)) {
            printk("%s dmaengine_submit error.\n", __func__);
            dma_release_channel(chan);
            return -2;
        }

        dma_async_issue_pending(chan);
        wait_event_interruptible(wq, dma_finished);

    }

    t2 = jiffies;
    diff = (long)t2 - (long)t1;
    msec = diff * 1000/HZ;
    printk("%s used %ld ms\n", __func__, msec);


    if (memcmp(src, dst, BUFFER_SIZE) == 0) {
        printk("%s success.\n", __func__);
    } else {
        printk("%s failed.\n", __func__);
    }

    printk("%s Done.\n", __func__);

    dma_release_channel(chan);

    return 0;
}

static void do_memcpy_no_dma(void)
{
    unsigned long t1, t2, diff, msec;
    int i;
    t1 = jiffies;

    for (i=0; i<1000; i++) {
        memcpy(dst, src, BUFFER_SIZE);
    }

    t2 = jiffies;
    diff = (long)t2 - (long)t1;
    msec = diff * 1000/HZ;
    printk("%s used %ld ms\n", __func__, msec);

}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
