#include "irq_driver.h"
/*virt_irq_provider_t*/

static irqreturn_t chained_handler(int irq, void *data)
{
	printk("%s:%d irq=%d\n", __FUNCTION__, __LINE__, irq);	

	return IRQ_HANDLED;
}

static irqreturn_t root_handler(int irq, void *data)
{
	printk("%s:%d irq=%d\n", __FUNCTION__, __LINE__, irq);	

	return IRQ_HANDLED;
}

static int irq_test_probe(struct platform_device *platform_dev)
{
	//为了获取platform device的irq 信息
	struct irq_consumer_info *infop  = platform_dev->dev.platform_data;
	int ret = 0;
	struct irq_consumer_dev *devp = NULL;

	
	if(infop == NULL)
		return -EINVAL;

	devp = devm_kzalloc(&platform_dev->dev, sizeof(struct irq_consumer_dev), GFP_KERNEL);
	if(devp == NULL)
		return -ENOMEM;
	
	devp->irq_index_for_virt_ric = infop->irq_index_for_virt_ric;
	devp->irq_index_for_virt_cic = infop->irq_index_for_virt_cic;

	//申请base 80的7号中断（从0开始，index是6）
	ret = devm_request_irq(&platform_dev->dev, devp->irq_index_for_virt_ric, root_handler, 0,
							platform_dev->name, devp);
	if (ret < 0) 
	{
		printk("request_irq() for %d failed: %d", devp->irq_index_for_virt_ric, ret);
		return ret;
	}
	irq_set_irq_type(devp->irq_index_for_virt_ric, IRQ_TYPE_EDGE_RISING);

	//申请 base96的1号中断（从0开始，index是0）
	ret = devm_request_irq(&platform_dev->dev, devp->irq_index_for_virt_cic, chained_handler, 0,
							platform_dev->name, devp);
	if (ret < 0) 
	{
		printk("request_irq() for %d failed: %d", devp->irq_index_for_virt_cic, ret);
		return ret;
	}
	irq_set_irq_type(devp->irq_index_for_virt_cic, IRQ_TYPE_EDGE_RISING);

	platform_set_drvdata(platform_dev, devp);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
} 

static int irq_test_remove(struct platform_device *platform_dev)
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	platform_set_drvdata(platform_dev, NULL);

	return 0;
}


static struct platform_driver irq_test_driver = {
	.driver = {
		.name = "virt_irq_test",
		.owner = THIS_MODULE,
	},
	.probe = irq_test_probe,
	.remove = irq_test_remove,
};


static int __init irq_test_driver_init(void)
{
	
	return platform_driver_register(&irq_test_driver);
}

static void __exit irq_test_driver_exit(void)
{
	platform_driver_unregister(&irq_test_driver);
}

module_init(irq_test_driver_init);
module_exit(irq_test_driver_exit);
MODULE_DESCRIPTION("Irq Test Driver");
MODULE_LICENSE("GPL");
