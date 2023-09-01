#include "virt_cic.h"


static ssize_t virt_cic_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct virt_cic_provider *devp = dev_get_drvdata(dev);


	return  sprintf(buf, "mask reg value:0x%x\nlevel reg value:0x%x \nedge reg value:0x%x\n",
		devp->irq_mask_reg, devp->irq_level_reg, devp->irq_edge_reg);
}

static ssize_t virt_cic_status_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct virt_cic_provider *devp = dev_get_drvdata(dev);
	uint32_t irq_status = simple_strtoul(buf, NULL, 10);


	spin_lock(&devp->lock);
	devp->irq_status = irq_status;
	spin_unlock(&devp->lock);

	printk("%s:%d irq status=%d\n", __FUNCTION__, __LINE__, irq_status);
	return count;
}

static DEVICE_ATTR(irq_status, S_IWUSR|S_IRUSR, NULL, virt_cic_status_store);
static DEVICE_ATTR(irq_info, S_IRUSR, virt_cic_info_show, NULL);


static struct attribute *virt_irq_chip_attrs[] =
{
	&dev_attr_irq_status.attr,
	&dev_attr_irq_info.attr,
	NULL
};
static const struct attribute_group virt_irq_chip_attr_group = 
{
	.attrs = virt_irq_chip_attrs,
};

void virt_cic_unmask(struct irq_data *data)
{
	struct virt_cic_provider *devp = data->chip_data;
	uint8_t irq_index = data->hwirq;
	
	printk("%s:%d irq_num=%d hw_irq=%ld\n", __FUNCTION__, __LINE__, data->irq, data->hwirq);

	spin_lock(&devp->lock);
	devp->irq_mask_reg &= ~(1<<irq_index);
	spin_unlock(&devp->lock);
}

void virt_cic_mask(struct irq_data *data)
{
	struct virt_cic_provider *devp = data->chip_data;
	uint8_t irq_index = data->hwirq;
	
	printk("%s:%d irq_num=%d hw_irq=%ld\n", __FUNCTION__, __LINE__, data->irq, data->hwirq);

	spin_lock(&devp->lock);
	devp->irq_mask_reg |= (1<<irq_index);
	spin_unlock(&devp->lock);

}

int virt_cic_set_affinity(struct irq_data *data, const struct cpumask *dest, bool force)
{
	/*set affinity*/
	return 0;
}

int  virt_cic_set_type(struct irq_data *data, unsigned int flow_type)
{
	struct virt_cic_provider *devp = data->chip_data;
	uint8_t bit_shift = data->hwirq;
	uint32_t level = 0;
	uint32_t edge = 0;
	int ret = 0;

	printk("%s:%d irq_num=%d flow_type=%d\n", __FUNCTION__, __LINE__, bit_shift, flow_type);
	
	spin_lock(&devp->lock);
	level = devp->irq_level_reg;
	edge = devp->irq_edge_reg;
	if (flow_type == IRQ_TYPE_EDGE_RISING) {
		level |= (1<<bit_shift);
		edge |= (1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_EDGE_FALLING) {
		level &= ~(1<<bit_shift);
		edge |= (1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_LEVEL_LOW) {
		level &= ~(1<<bit_shift);
		edge &= ~(1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_LEVEL_HIGH) {
		level |= (1<<bit_shift);
		edge &= ~(1<<bit_shift);
	} else if(flow_type == IRQ_TYPE_NONE)
	{
		//low type
		level &= ~(1<<bit_shift);
		edge &= ~(1<<bit_shift);
	}
	else
	{
		ret = -1;
		goto unlock;
	}

	
	devp->irq_level_reg = level;
	devp->irq_edge_reg = edge;
unlock:
	spin_unlock(&devp->lock);

	return ret;
}


static int virt_cic_map(struct irq_domain *d, unsigned int irq,
		     irq_hw_number_t hw)
{
	struct virt_cic_provider *devp = (struct virt_cic_provider *)(d->host_data);

	irq_set_chip_data(irq, (void *)devp);
	irq_set_chip_and_handler_name(irq, &devp->irq_chip, handle_level_irq, devp->platform_dev->name);
	irq_set_irq_type(irq, IRQ_TYPE_NONE);

	return 0;
}

static const struct irq_domain_ops virt_irq_ops = {
	.map = virt_cic_map,
	.xlate = irq_domain_xlate_onetwocell,
};


static int virt_irq_init(struct virt_cic_provider *devp)
{
	int ret = 0;


	ret = irq_alloc_descs(devp->irq_base, 0, devp->irq_num, 0);
	if(ret < 0)
		return ret;

	//创建并注册一个irq_domain, 这其中完成了hw irq到virtual irq num的映射，此外这里将会为范围内的irq都执行一次map
	devp->irq_domain = irq_domain_add_legacy(NULL, devp->irq_num, devp->irq_base, 0, &virt_irq_ops, devp);
	if (!devp->irq_domain)
	{
		return -ENODEV;
	}
	
	return 0;
}

static void virt_cic_irq_handler(struct irq_desc *desc)
{
	int handled = 0;
	uint32_t irq_status = 0;
	unsigned int trigger_irq = 0;
	int i = 0;
	struct virt_cic_provider *devp = irq_desc_get_handler_data(desc);
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	spin_lock(&devp->lock);
	irq_status = devp->irq_status;
	devp->irq_status = 0;
	spin_unlock(&devp->lock);

	printk("irq_status=%d, irq_num: %d\n", irq_status, devp->irq_num);

	for(i = 0; i < devp->irq_num; i++)
	{
		if(irq_status&(1<<i))
		{
			//在Linux内核中,irq_find_mapping() 函数的作用是根据hw irq查找对应的中断号。
			trigger_irq = irq_find_mapping(devp->irq_domain, i);
			printk("trigger_irq=%d\n", trigger_irq);
			generic_handle_irq(trigger_irq);
			handled ++;
		}
	}

	if (0 == handled) {
		handle_bad_irq(desc);
	}
}

static int virt_cic_platform_probe(struct platform_device *platform_dev)
{
	struct virt_cic_data *datap = platform_dev->dev.platform_data;
	
	struct virt_cic_provider *devp = devm_kzalloc(&platform_dev->dev, sizeof(struct virt_cic_provider), GFP_KERNEL);
	int ret = 0;
	
	if(devp == NULL || datap == NULL)
		return -ENOMEM;
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	devp->irq_chip.irq_unmask= virt_cic_unmask;
	devp->irq_chip.irq_mask = virt_cic_mask;
	devp->irq_chip.irq_set_type = virt_cic_set_type;
	devp->irq_chip.irq_set_affinity = virt_cic_set_affinity;
	devp->irq_base = datap->irq_base;
	devp->irq_num = datap->irq_num;
	devp->platform_dev = platform_dev;
	
	spin_lock_init(&devp->lock);
	ret = virt_irq_init(devp);
	if(ret < 0)
		return ret;

	//.parent_irq = 95
/* 
irq_set_chained_handler_and_data
	——>__irq_do_set_handler
	这里有一个参数: is_chained
		1: 这是一个级联中断处理器，不能真正处理用户的中断函数
 */
	irq_set_chained_handler_and_data(datap->parent_irq,virt_cic_irq_handler, devp);

	printk("%s:%d\n", __FUNCTION__, __LINE__);
	sysfs_create_group(&(platform_dev->dev.kobj), &virt_irq_chip_attr_group);
	platform_set_drvdata(platform_dev, devp);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
} 

static int virt_cic_platform_remove(struct platform_device *platform_dev)
{

	struct virt_cic_provider *devp = platform_get_drvdata(platform_dev);

	printk("%s:%d\n", __FUNCTION__, __LINE__);
	irq_domain_remove(devp->irq_domain);

	irq_free_descs(devp->irq_base, devp->irq_num);
	sysfs_remove_group(&(platform_dev->dev.kobj), &virt_irq_chip_attr_group);
	platform_set_drvdata(platform_dev, NULL);

	return 0;
}


static struct platform_driver virt_cic_platform_driver = {
	.driver = {
		.name = "virt_cic_dev",
		.owner = THIS_MODULE,
	},
	.probe = virt_cic_platform_probe,
	.remove = virt_cic_platform_remove,
};


static int __init virt_cic_device_init(void)
{
	int ret = 0;
	
	ret = platform_driver_register(&virt_cic_platform_driver);

	return ret;
}

static void __exit virt_cic_device_exit(void)
{
	platform_driver_unregister(&virt_cic_platform_driver);
}

module_init(virt_cic_device_init);
module_exit(virt_cic_device_exit);
MODULE_DESCRIPTION("Virtual IRQ Device");
MODULE_LICENSE("GPL");
