#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
 

static dev_t first;
static struct class *cl;
static struct cdev char_dev;

static int driverdrlr_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static int driverdrlr_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver DrLr: open()\n");
    return 0;
}
 

static int driverdrlr_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver DrLr: close()\n");
    return 0;
}

static ssize_t driverdrlr_read(struct file *f, char __user *buf, size_t  len, loff_t *off)
{
    int retval = 0;
    char charArray[255] = "Hola SO !\0";

    if (copy_to_user(buf, charArray, 15)) {         
        printk(KERN_INFO "Driver DrLr: read() - copy_to_user() fail\n"); 
            retval = -ENODEV;
        } else {
            printk(KERN_INFO "Driver DrLr: read() - copy_to_user() success\n");
            printk(KERN_INFO "Driver DrLr: read() - %s\n",charArray);
            retval = 12;
        };

    if(off > 0) {
        printk(KERN_INFO "Driver DrLr: read() - Finished Reading\n");
        return 0;   
    };

    return retval;
}

static ssize_t driverdrlr_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver DrLr: write() - %s\n", buf);
    return len;
}
 

static struct file_operations driverdrlr_fops =
{
  .owner = THIS_MODULE,
  .open = driverdrlr_open,
  .release = driverdrlr_close,
  .read = driverdrlr_read,
  .write = driverdrlr_write
};

 
static int __init driverdrlr_init(void)
{
    printk("Driver DrLr Initialized\n");

    if (alloc_chrdev_region(&first, 0, 3, "Driver DrLr") < 0)
    {
            return -1;
    }
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));

    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        printk(KERN_INFO "class_create() failed");

            unregister_chrdev_region(first, 1);
            return -1;
    }

    cl->dev_uevent = driverdrlr_uevent;

    if (device_create(cl, NULL, first, NULL, "driverdrlr") == NULL)
    {
        printk(KERN_INFO "device_create() failed"); 
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
 
    cdev_init(&char_dev, &driverdrlr_fops);

    if (cdev_add(&char_dev, first, 1) == -1)
    {
            device_destroy(cl, first);
            class_destroy(cl);
            unregister_chrdev_region(first, 1);
            return -1;
    }

    return 0;
}

static void __exit driverdrlr_exit(void)
{
    unregister_chrdev_region(first, 3);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk("Driver DrLr Being Removed\n");
}

module_init(driverdrlr_init);
module_exit(driverdrlr_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dairo Rivas - Luis Rios");
MODULE_DESCRIPTION("Driver Dairo Rivas - Luis Rios");
MODULE_VERSION("1.0.0");