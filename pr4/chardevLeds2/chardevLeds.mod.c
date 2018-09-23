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
	{ 0xdca08a4c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6900ca31, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x537143a2, __VMLINUX_SYMBOL_STR(kobject_put) },
	{ 0x9e959575, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x88cdaa46, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x169f48da, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xb5419b40, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x48e6e34d, __VMLINUX_SYMBOL_STR(vc_cons) },
	{ 0x4e6e8ea7, __VMLINUX_SYMBOL_STR(fg_console) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xc671e369, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x811e254f, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0xc2b8ffd0, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

