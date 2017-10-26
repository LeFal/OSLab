#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static int my_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "open\n");
	return 0;
}
static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "write\n");
	return count;
}

static const struct file_operations myproc_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.write = my_write,
};

static int __init my_init(void)
{
	printk(KERN_INFO "init\n");
	proc_dir = proc_mkdir(PROC_DIRNAME,NULL);
	proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);
	
	return 0;	
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "exit\n");

	return;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
