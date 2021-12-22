#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbd97005b, "module_layout" },
	{ 0xc65b2c7a, "device_destroy" },
	{ 0xafe18e20, "class_destroy" },
	{ 0xfada38fe, "device_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xf72f2e16, "cdev_del" },
	{ 0xd9379699, "__class_create" },
	{ 0x4a91e5dd, "cdev_add" },
	{ 0xc72c11da, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0xfe990052, "gpio_free" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x4ccd9305, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x24d273d1, "add_timer" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xc5850110, "printk" },
	{ 0xba4c293, "gpiod_set_raw_value" },
	{ 0xe30324cf, "gpio_to_desc" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "309A0F88DA35A25C9B2F374");
