#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/bio.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/bio.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

// run when opened
static int my_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "open\n");
	return 0;
}

// run when writed
static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "write\n");
	if (cir_q.q_count == QUESIZE) {
		int i = 0;
		while(cir_q.q_count > 0){
			char *filename = cir_q.que[i].name;
			long sector_num = cir_q.que[i].sector_num;
			long time = cir_q.que[i].time.tv_sec;

			


			cir_q.q_count--;
			i++;
		} 
	}
	
	return count;
}

//proc operations
static const struct file_operations myproc_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.write = my_write,
};


// run when init
static int __init my_init(void)
{
	printk(KERN_INFO "init\n");
	// when modules initiated, make dir and file
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
