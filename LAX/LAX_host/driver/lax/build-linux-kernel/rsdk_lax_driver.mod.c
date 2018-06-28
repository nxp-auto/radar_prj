#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa114a540, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x8df1f13d, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x9b4da327, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x1c516378, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x4a2f905d, __VMLINUX_SYMBOL_STR(irq_of_parse_and_map) },
	{ 0xa241c0a3, __VMLINUX_SYMBOL_STR(of_parse_phandle) },
	{ 0x2cf0202a, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x52add085, __VMLINUX_SYMBOL_STR(of_property_read_u32_array) },
	{ 0xffa3d45, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x32ec3763, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0x422a286d, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xdcb764ad, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xa5fdd5bd, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x97fdbab9, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0xe6f80844, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x35480d0f, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x7c1372e8, __VMLINUX_SYMBOL_STR(panic) },
	{ 0xfaeb5f7a, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x1d0709c2, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x1be1ba5, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x2724ba66, __VMLINUX_SYMBOL_STR(__ioremap) },
	{ 0x57148890, __VMLINUX_SYMBOL_STR(__task_pid_nr_ns) },
	{ 0x14e92101, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xc4063447, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xd3ee3db2, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x579cb3c1, __VMLINUX_SYMBOL_STR(of_property_read_u64) },
	{ 0xa916b694, __VMLINUX_SYMBOL_STR(strnlen) },
	{ 0x12a38747, __VMLINUX_SYMBOL_STR(usleep_range) },
	{ 0x211f68f1, __VMLINUX_SYMBOL_STR(getnstimeofday64) },
	{ 0xd463a05d, __VMLINUX_SYMBOL_STR(of_get_property) },
	{ 0x5cd885d5, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x96220280, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x66f4b4df, __VMLINUX_SYMBOL_STR(init_pid_ns) },
	{ 0x19ad29b0, __VMLINUX_SYMBOL_STR(find_get_pid) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xa46f2f1b, __VMLINUX_SYMBOL_STR(kstrtouint) },
	{ 0x4829a47e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x76e4b95d, __VMLINUX_SYMBOL_STR(send_sig_info) },
	{ 0x5c2fb31b, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x45a55ec8, __VMLINUX_SYMBOL_STR(__iounmap) },
	{ 0xadf67419, __VMLINUX_SYMBOL_STR(of_address_to_resource) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x5ac18a8f, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x4cb165b8, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x78dda92d, __VMLINUX_SYMBOL_STR(of_property_read_u64_array) },
	{ 0x9e7d6bd0, __VMLINUX_SYMBOL_STR(__udelay) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x85f5e2aa, __VMLINUX_SYMBOL_STR(krealloc) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

