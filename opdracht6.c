/*
 * Basic kernel module using a timer and GPIOs to flash a LED.
 *
 * Author:
 * 	Stefan Wendler (devnull@kaltpost.de)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * <sudo insmod opdracht6.ko outputs=4,17 levels=1,1>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>

struct timer_info
{
    int togglespeed;
    int output;
    int data;
    struct timer_list blink_timer;
    void (*blink_timer_func)(struct timer_list *);
};

static struct timer_info timer_arr[5];

static int outputs[5] = {0};
static int levels[5] = {1};
static int togglespeeds[5] = {0};

static int outputs_argc = 0;
static int levels_argc = 0;
static int togglespeeds_argc = 0;

module_param_array(outputs, int, &outputs_argc, 0000);
MODULE_PARM_DESC(outputs, "An array of integers");

module_param_array(levels, int, &levels_argc, 0000);
MODULE_PARM_DESC(levels, "An array of integers");

module_param_array(togglespeeds, int, &togglespeeds_argc, 0000);
MODULE_PARM_DESC(togglespeeds, "An array of integers");

/*
 * Timer function called periodically
 */
static void blink_timer_func0(struct timer_list *t)
{
    int i = 0;
    gpio_set_value(timer_arr[i].output, timer_arr[i].data);
    timer_arr[i].data = !timer_arr[i].data;
    printk(KERN_INFO "Setup timer to fire in %is for gpio %d\n", timer_arr[i].togglespeed, timer_arr[i].output);
    timer_arr[i].blink_timer.expires = jiffies + (timer_arr[i].togglespeed * HZ);
    add_timer(&timer_arr[i].blink_timer);
}

static void blink_timer_func1(struct timer_list *t)
{
    int i = 1;
    gpio_set_value(timer_arr[i].output, timer_arr[i].data);
    timer_arr[i].data = !timer_arr[i].data;
    printk(KERN_INFO "Setup timer to fire in %is for gpio %d\n", timer_arr[i].togglespeed, timer_arr[i].output);
    timer_arr[i].blink_timer.expires = jiffies + (timer_arr[i].togglespeed * HZ);
    add_timer(&timer_arr[i].blink_timer);
}
static void blink_timer_func2(struct timer_list *t)
{
    int i = 2;
    gpio_set_value(timer_arr[i].output, timer_arr[i].data);
    timer_arr[i].data = !timer_arr[i].data;
    printk(KERN_INFO "Setup timer to fire in %is for gpio %d\n", timer_arr[i].togglespeed, timer_arr[i].output);
    timer_arr[i].blink_timer.expires = jiffies + (timer_arr[i].togglespeed * HZ);
    add_timer(&timer_arr[i].blink_timer);
}
static void blink_timer_func3(struct timer_list *t)
{
    int i = 3;
    gpio_set_value(timer_arr[i].output, timer_arr[i].data);
    timer_arr[i].data = !timer_arr[i].data;
    printk(KERN_INFO "Setup timer to fire in %is for gpio %d\n", timer_arr[i].togglespeed, timer_arr[i].output);
    timer_arr[i].blink_timer.expires = jiffies + (timer_arr[i].togglespeed * HZ);
    add_timer(&timer_arr[i].blink_timer);
}
static void blink_timer_func4(struct timer_list *t)
{
    int i = 4;
    gpio_set_value(timer_arr[i].output, timer_arr[i].data);
    timer_arr[i].data = !timer_arr[i].data;
    printk(KERN_INFO "Setup timer to fire in %is for gpio %d\n", timer_arr[i].togglespeed, timer_arr[i].output);
    timer_arr[i].blink_timer.expires = jiffies + (timer_arr[i].togglespeed * HZ);
    add_timer(&timer_arr[i].blink_timer);
}

void (*blink_timer_funcs[5])(struct timer_list *) = {blink_timer_func0, blink_timer_func1, blink_timer_func2, blink_timer_func3, blink_timer_func4};

/*
 * Module init function
 */
static int __init gpiomod_init(void)
{
    int i;
    int ret = 0;
    printk(KERN_INFO "%s\n", __func__);

    if (outputs_argc > 5 || outputs_argc == 0)
    {
        printk(KERN_ERR "parameters zijn niet juist ingevuld");
        return -1;
    }

    for (i = 0; i < outputs_argc; i++)
    {
        if (gpio_is_valid(outputs[i]))
        {
            char buff[10];
            if (levels[i] != 0 || levels[i] != 1)
                levels[i] = 1;

            sprintf(buff, "GPIO_%i", outputs[i]);
            ret = gpio_request(outputs[i], buff);
            if (ret)
            {
                printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
                return ret;
            }
            gpio_direction_output(outputs[i], 0);
            gpio_set_value(outputs[i], levels[i]);

            printk(KERN_INFO "GPIO: %i level:%i", outputs[i], levels[i]);

            if (togglespeeds[i] != 0)
            {
                timer_arr[i].data = levels[i];
                timer_arr[i].output = outputs[i];
                timer_arr[i].togglespeed = togglespeeds[i];
                timer_arr[i].blink_timer_func = blink_timer_funcs[i];

                timer_setup(&timer_arr[i].blink_timer, timer_arr[i].blink_timer_func, 0);
                timer_arr[i].blink_timer.function = timer_arr[i].blink_timer_func;
                timer_arr[i].blink_timer.expires = jiffies + (togglespeeds[i] * HZ); // 1 sec.
                add_timer(&timer_arr[i].blink_timer);
            }
        }
        else
            printk(KERN_INFO "%i is not a valid GPIO", outputs[i]);
    }

    return ret;
}

/*
 * Module exit function
 */
static void __exit gpiomod_exit(void)
{
    int i;
    printk(KERN_INFO "%s\n", __func__);

    for (i = 0; i < outputs_argc; i++)
    {
        gpio_set_value(outputs[i], 0);
        gpio_free(outputs[i]);
        del_timer_sync(&timer_arr[i].blink_timer);
        printk(KERN_INFO "GPIO: %i level:%i", outputs[i], 0);
    }
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manu Dumolein");
MODULE_DESCRIPTION("Basic kernel module using a timer and GPIOs to flash a multiple GPIOS.");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
