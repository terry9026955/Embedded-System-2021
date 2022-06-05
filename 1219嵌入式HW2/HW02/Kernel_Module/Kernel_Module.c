#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define mem_size 1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t *off);



static struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.read    = my_read,
	.write   = my_write,
	.open    = my_open,
	.release = my_release,
};

static int my_open(struct inode *inode, struct file *file){
	/* Creating physical memory */
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0) {
		printk(KERN_INFO"Cannot allocate the memory to the kernel...\n");
		return -1;
	}
	printk(KERN_INFO "Device File opened...\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file){
	kfree(kernel_buffer);
	printk(KERN_INFO"Device File closed...\n");
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
	//copy_to_user(buf, kernel_buffer, mem_size);
	printk(KERN_INFO"Data read: DONE...\n");
	return mem_size;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
	//copy_from_user(kernel_buffer, buf, len);
	printk(KERN_INFO"Data is written successfully...\n");
	return len;
}



static int __init chr_driver_init(void){
	/* Allocating major number */
	if((alloc_chrdev_region(&dev,0,1,"demo")) < 0){
		printk(KERN_INFO"Cannot allocate the major number...\n");
		return -1;
	}

	printk(KERN_INFO"Major = %d Minor = %d...\n", MAJOR(dev), MINOR(dev));

	/* Creating cdev structure */

		cdev_init(&my_cdev, &fops);
		
	/* Adding character device to the system */
		if((cdev_add(&my_cdev,dev,1)) < 0) {
			printk(KERN_INFO"Cannot add the device to the system...\n");
			goto r_class;
		}

	/* Creating struct class */
		if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
			printk(KERN_INFO"Cannot create the struct class...\n");
			goto r_class;
		}
	
	/* Creating device */
		if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL) {
			printk(KERN_INFO"Cannot create the device...\n");
			goto r_device;
		}

	printk(KERN_INFO"Device driver insert...done properly...\n");
	return 0;


r_device:
		class_destroy(dev_class);


r_class: 
		unregister_chrdev_region(dev,1);
		return -1;
}


void __exit chr_driver_exit(void){
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO"Device driver is removed successfully...\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Terry Chen");
MODULE_DESCRIPTION("The character device driver");


