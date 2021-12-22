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
	{ 0x9e9fbfdf, "param_array_ops" },
	{ 0x7b66b44c, "param_ops_int" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0xfe990052, "gpio_free" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x4ccd9305, "gpiod_direction_output_raw" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x24d273d1, "add_timer" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xc5850110, "printk" },
	{ 0xba4c293, "gpiod_set_raw_value" },
	{ 0xe30324cf, "gpio_to_desc" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B35CB88198693A8A250E97B");
