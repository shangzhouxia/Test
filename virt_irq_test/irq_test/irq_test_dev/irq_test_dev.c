#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mdio.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/kdev_t.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/device.h>
#include "../irq_test_driver/irq_driver.h"

static void irq_test_dev_release(struct device *dev)
{

}


static struct irq_consumer_info consumer_info =
{
	.irq_index_for_virt_cic = 96,
	.irq_index_for_virt_ric = 86,
};
static struct platform_device irq_test_device = {
	.name = "virt_irq_test",
	.id = 0,
	.dev =
	{
		.release = irq_test_dev_release, 
		.platform_data = &consumer_info,
	}
};

static int __init irq_test_dev_init(void)
{
	platform_device_register(&irq_test_device);

	return 0;
}

static void __exit irq_test_dev_exit(void)
{
	platform_device_unregister(&irq_test_device);
}


module_init(irq_test_dev_init);
module_exit(irq_test_dev_exit);

MODULE_DESCRIPTION("Irq Test Devs");
MODULE_LICENSE("GPL");
