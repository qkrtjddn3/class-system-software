#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DELAY_TIME_MSEC(msec) ((msec) * HZ / 10)

#define CHR_DEV_NAME "chr_dev"
#define CHR_DEV_MAJOR 240 

MODULE_LICENSE("Dual BSD/GPL");

/* Timer var, func */
static struct timer_list timer;
static void timer_func(unsigned long data)
{
	static int i = 0;
	printk(KERN_ALERT "%d\n", ++i);
	timer.expires = get_jiffies_64() + DELAY_TIME_MSEC(10);
	timer.function = timer_func;
	add_timer(&timer);
}

/* Device driver operation func, operations */
int chr_open(struct inode *inode, struct file *file)
{
	int number = MINOR(inode->i_rdev); 	/* Get minor number */
	printk("Virtual Character Device Open: Minor Number is %d\n", number);
	return 0;
}
ssize_t chr_read(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	printk("read data: %s\n", buf);
	return count;
}
ssize_t chr_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	printk("write data: %s\n", buf); 
	return count;
}
int chr_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case 0:
			printk("cmd value is %d\n", cmd);
			break;
		case 4:
			printk("cmd value is %d\n", cmd);
			break;
	}
	return 0;
}
int chr_release(struct inode *inode, struct file *file)
{
	printk("Virtual Character Device Release\n");
	return 0;
}
struct file_operations chr_fops =
{
	owner: THIS_MODULE,
	unlocked_ioctl: chr_ioctl,
	write: chr_write,
	read: chr_read,
	open: chr_open,
	release: chr_release
};

/* Module init, exit func */
static int battery_module_init(void)
{
	int registration;
	printk(KERN_ALERT "Hello World!\n");

	/* Timer set up */
	init_timer(&timer);
	//timer.expires = jiffies + DELAY_TIME_MSEC(10);
	timer.expires = get_jiffies_64() + DELAY_TIME_MSEC(10);
	timer.function = timer_func;
	add_timer(&timer);

	/* Register device */
	registration = register_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME, &chr_fops);
	if(registration < 0)
	{
		printk("Registration failed\n");
		return registration;
	}
	printk("Major Number:%d\n", registration);

	return 0; 					/* return zero on successful loading */
}
static void battery_module_exit(void)
{
	/* Timer remove */
	del_timer(&timer);

	/* Device remove */
	unregister_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME);

	printk(KERN_ALERT "Good bye!\n");
}

module_init(battery_module_init);
module_exit(battery_module_exit);
