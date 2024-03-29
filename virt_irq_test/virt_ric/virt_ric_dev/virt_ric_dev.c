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
#include "../virt_ric_driver/virt_ric.h"
static struct virt_irq_controller_data controller_data =
{
	.irq_base = 80,
	.irq_num = 16
};
static void virt_irq_dev_release(struct device *dev)
{

}

static struct platform_device virt_irq_platform_device = {
	.name = "virt_ric_dev",
	.id = 0,
	.dev =
	{
		.release = virt_irq_dev_release, 
		.platform_data = &controller_data,
	}
};

static int __init virt_irq_platform_dev_init(void)
{
	platform_device_register(&virt_irq_platform_device);

	return 0;
}

static void __exit virt_irq_platform_dev_exit(void)
{
	platform_device_unregister(&virt_irq_platform_device);
}


module_init(virt_irq_platform_dev_init);
module_exit(virt_irq_platform_dev_exit);

MODULE_DESCRIPTION("Virt Irq Controller Platform Devs");
MODULE_LICENSE("GPL");
