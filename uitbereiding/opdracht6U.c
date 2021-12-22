#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#include <linux/timer.h>
#include <linux/gpio.h>


#include "query_ioctl.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static int output = 0, level = 0, togglespeed = 0;
struct timer_list blink_timer;

static void blink_timer_func(struct timer_list *t)
{
    gpio_set_value(output, level);
    level = !level;
    printk(KERN_INFO "Setup timer to fire in %is for %d\n", togglespeed, output);
    blink_timer.expires = jiffies + (togglespeed * HZ); // 1 sec.
    add_timer(&blink_timer);
}

static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long 
(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    query_arg_t q;
    int ret = 0;

    switch (cmd)
    {
    case QUERY_GET_VARIABLES:
        q.output = output;
        q.level = level;
        q.togglespeed = togglespeed;
        if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
        {
            return -EACCES;
        }
        break;
    case QUERY_CLR_VARIABLES:
        gpio_set_value(output, 0);
        gpio_free(output);
        del_timer_sync(&blink_timer);
        printk(KERN_INFO "GPIO: %i level:%i", output, 0);
        output = 0;
        level = 0;
        togglespeed = 0;
        break;
    case QUERY_SET_VARIABLES:
        if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t)))
        {
            return -EACCES;
        }
        output = q.output;
        level = q.level;
        togglespeed = q.togglespeed;

        if (gpio_is_valid(output))
        {
            char buff[10];
            sprintf(buff, "GPIO_%i", output);
            ret = gpio_request(output, buff);
            if (ret)
            {
                printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
                return ret;
            }
            gpio_direction_output(output, 0);
            gpio_set_value(output, level);

            printk(KERN_INFO "GPIO: %i level:%i", output, level);

            if (togglespeed != 0)
            {
                timer_setup(&blink_timer, blink_timer_func, 0);
                blink_timer.function = blink_timer_func;
                blink_timer.expires = jiffies + (togglespeed * HZ); // 1 sec.
                add_timer(&blink_timer);
            }
        }
        else
            printk(KERN_INFO "%i is not a valid GPIO", output);

        break;
    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations query_fops =
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
        .ioctl = my_ioctl
#else
        .unlocked_ioctl = my_ioctl
#endif
};

static int __init query_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;

    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0)
    {
        return ret;
    }

    cdev_init(&c_dev, &query_fops);

    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit query_ioctl_exit(void)
{
    gpio_set_value(output, 0);
    gpio_free(output);
    del_timer_sync(&blink_timer);
    printk(KERN_INFO "GPIO: %i level:%i", output, 0);

    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(query_ioctl_init);
module_exit(query_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manu Dumolein");
MODULE_DESCRIPTION("Query ioctl() Char Driver");
