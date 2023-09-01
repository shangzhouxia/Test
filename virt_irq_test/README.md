#### 安装教程

1.  make
2.  make install
3.  在images目录下即为该测试驱动相关的ko文件。

、

在Linux内核中,irq_alloc_descs() 函数用于为中断控制器 allocate 一批连续的硬中断描述符。

原型:

```c
/* use macros to avoid needing export.h for THIS_MODULE */
#define irq_alloc_descs(irq, from, cnt, node)	\
	__irq_alloc_descs(irq, from, cnt, node, THIS_MODULE, NULL)

/**
 * __irq_alloc_descs - allocate and initialize a range of irq descriptors
 * @irq:	Allocate for specific irq number if irq >= 0
 * @from:	Start the search from this irq number
 * @cnt:	Number of consecutive irqs to allocate.
 * @node:	Preferred node on which the irq descriptor should be allocated
 * @owner:	Owning module (can be NULL)
 * @affinity:	Optional pointer to an affinity mask array of size @cnt which
 *		hints where the irq descriptors should be allocated and which
 *		default affinities to use
 *
 * Returns the first irq number or error code
 */
int __ref
__irq_alloc_descs(int irq, unsigned int from, unsigned int cnt, int node,
		  struct module *owner, const struct irq_affinity_desc *affinity)
```

参数说明:

- irq - 中断控制器所使用的中断号
- from - 分配中断描述符的起始编号 
- cnt - 需要分配的描述符数量
- node - 从哪个 NUMA node 分配内存


这个函数主要用于中断控制器初始化时批量申请中断描述符,这些描述符之后可以映射到实际的中断源。

申请得到的描述符已经确定了中断号范围,之后可以直接使用这些中断号来处理中断,无需再动态分配中断号。

所以irq_alloc_descs() 可以提前批量分配中断描述符,使中断处理简单高效。


在Linux内核中,irq_set_chip_data() 函数的作用是为一个中断线路设置自定义的私有数据。

原型:

```c
/**
 *	irq_set_chip_data - set irq chip data for an irq
 *	@irq:	Interrupt number
 *	@data:	Pointer to chip specific data
 *
 *	Set the hardware irq chip data for an irq
 */
int irq_set_chip_data(unsigned int irq, void *data)
{
	unsigned long flags;
	struct irq_desc *desc = irq_get_desc_lock(irq, &flags, 0);

	if (!desc)
		return -EINVAL;
	desc->irq_data.chip_data = data;
	irq_put_desc_unlock(desc, flags);
	return 0;
}
```

这个函数可以为irq号所对应的中断设置一个void*类型的自定义数据指针。

设置的私有数据可以在中断处理回调函数中通过irq_get_chip_data(irq)获取。
```c
static inline void *irq_get_chip_data(unsigned int irq)
{
	struct irq_data *d = irq_get_irq_data(irq);
	return d ? d->chip_data : NULL;
}
```

主要用途有:

- 将中断和中断处理函数相关的数据绑定在一起。

- 通过中断号irq可以获取自定义的数据。

- 例如可以将设备结构体指针等信息绑定到中断。

- 这样中断处理函数可以轻松获取自己需要的上下文数据。

- 不需要依赖全局变量或静态数据。

所以 irq_set_chip_data 提供了一个可以与中断处理直接关联的数据绑定接口, 使用它可以避免全局变量,也方便了中断回调函数的编写。

类似的函数还有irq_set_handler_data()。


在Linux内核中,irq_set_chip_and_handler_name() 函数用于为一个中断号指定中断控制器(chip)和中断处理函数。

原型:

```c
void
irq_set_chip_and_handler_name(unsigned int irq, struct irq_chip *chip,
			      irq_flow_handler_t handle, const char *name)
{
	irq_set_chip(irq, chip);
	__irq_set_handler(irq, handle, 0, name);
}
```

它主要完成了以下两件事:

1. 设置irq对应的中断控制器:

- 指定irq会使用哪个中断控制器chip来处理。

- 中断控制器负责底层的中断控制与硬件交互。

2. 注册中断处理函数handler:

- handler会在中断触发时作为回调函数被调用。

- name参数打印调试信息用。

通过这两个设置,可以配置一个中断线路的硬件控制和处理逻辑。

一般在设备驱动初始化时会调用该函数,完成指定中断的初始化和配置。

这样可以通过一个函数同时设置中断的硬件控制器和处理函数,简化中断管理。


```c
/**
* irq_domain_add_legacy() - 分配并注册旧版 revmap irq_domain。
  * @of_node：指向中断控制器的设备树节点的指针。
  * @size：传统映射中的 irq 总数
  * @first_irq: 分配给域的第一个irq块编号
  * @first_hwirq：用于翻译的第一个 hwirq 编号。 通常应为“0”，但如果有效 hwirqs 编号不从零开始，则可以使用正整数。
  * @ops：map/unmap domain callbacks
  * @host_data: 控制器私有数据指针
  *
  * 注意：在该函数返回除 0 之外的所有传统中断（对于传统控制器而言始终是无效的 irq）之前，将调用 map() 回调。
 */
struct irq_domain *irq_domain_add_legacy(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
					 irq_hw_number_t first_hwirq,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	struct irq_domain *domain;

	domain = __irq_domain_add(of_node_to_fwnode(of_node), first_hwirq + size,
				  first_hwirq + size, 0, ops, host_data);
	if (domain)
		irq_domain_associate_many(domain, first_irq, first_hwirq, size);

	return domain;
}
EXPORT_SYMBOL_GPL(irq_domain_add_legacy);
```

在 Linux 内核中,irq_domain_xlate_onetwocell() 函数用于将二元组的硬件中断信息翻译成通用的 IRQ 号。

irq 域(domain)使用二元组(onecell 或 twocell)表示硬件中断:

- onecell - 一个单元格,只包含中断号
- twocell - 两个单元格,包含中断控制器 ID + 中断号

而 irq_alloc_descs() 分配的是标准连续的 IRQ 号。

那么 irq_domain_xlate_onetwocell() 就是连接二者的翻译函数:

- 输入:二元组的硬件中断信息,如 <0, 10>(控制器0,中断10)

- 输出:映射后的标准 IRQ 号,如 20

- 通过域内数据结构映射和转换

这样,底层的不连续硬件中断就被转换成了连续的 IRQ 号。

上层只需要使用标准中断 API,简化中断处理。

所以它起到连接硬件中断和通用 IRQ 的翻译作用。