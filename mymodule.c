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

#define PROCSIZE 1000
#define ASCIENTER 0x0A
#define ASCISPACE 0x20

// access blk-core.c variance
extern struct io_cir_q cir_q;
extern int q_boot;

static char proc_buf[PROCSIZE] = "";
//static unsigned long proc_buf_size = 0;

int buff_count = 0;

// run when opened
static int my_open(struct inode *inode, struct file *file)
{
	buff_count = 0;
	printk(KERN_INFO "open\n");
	return 0;
}


static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *off)
{
	printk(KERN_INFO "write\n");
	int i = 0;
	if (len > 0)
		return len;
	if (buff_count > PROCSIZE-100)
		return len;



	if (cir_q.q_count == Q_SIZE) {

		while (--cir_q.q_count){
			if (i == Q_SIZE) {
				i = 0;
			}
			buff_count += sprintf(&proc_buf[buff_count], "%s", cir_q.q[i].filename);
			proc_buf[buff_count++] = ASCISPACE;
			buff_count += sprintf(&proc_buf[buff_count], "%lu", cir_q.q[i].sector_num);
			proc_buf[buff_count++] = ASCISPACE;
			buff_count += sprintf(&proc_buf[buff_count], "%ld", cir_q.q[i].time.tv_sec);
			proc_buf[buff_count++] = ASCIENTER;
			i++;
		}
		memset(&cir_q, 0, sizeof(struct io_cir_q));
		}
	printk("write end\n");
	return len;
}


static ssize_t my_read(struct file *f, char __user *userArray, size_t s, loff_t * l){
	printk("read\n");
	//printk("%d\n",PROCSIZE);
	printk("%s\n",proc_buf);
	if (s >= PROCSIZE){
		memcpy(userArray, proc_buf, s);
		printk("my read\n");
		return s;
	}else{
		printk("buffer is too small\n");
		return -1;
	}
}

/*
static ssize_t my_read(struct file *f, char __user *buffer, size_t len, loff_t *off){
	int i = 0;
	printk("read\n");
	if (cir_q.q_count >= Q_SIZE) {
		for(i = 0; i< Q_SIZE; i++){
			char *filename = cir_q.q[i].filename;
			long sector_num = cir_q.q[i].sector_num;
			long time = cir_q.q[i].time.tv_sec;

			// write data to buffer
			buff_count += sprintf(&proc_buf + buff_count, "%s", filename);
			buff_count += sprintf(&proc_buf + buff_count, "%lu", sector_num);
			buff_count += sprintf(&proc_buf + buff_count, "%ld", time);
		}
		memset(&cir_q, 0, sizeof(struct io_cir_q));
		cir_q.q_count = 0;
	}

	if (len <= PROCSIZE){
		memcpy(buffer, proc_buf, len);
		return len;
	}
	else if (len > PROCSIZE){
		printk("error : size of userArray is smaller than buffer\n");
		return -1;
	}
}
*/

//proc operations
static const struct file_operations myproc_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
};


// run when init
static int __init my_init(void)
{
	int time = 1;
	memset(proc_buf, 0, sizeof(proc_buf));
	q_boot = 1;
	printk(KERN_INFO "init\n");
	// when modules initiated, make dir and file
	proc_dir = proc_mkdir(PROC_DIRNAME,NULL);
	proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);

	while(time < 10000000)
	{
		my_write(NULL,NULL,0,NULL);
		udelay(1000);
		time++;
//		printk("%d",time);
	}

	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "exit\n");

	return;
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("OHLEE");
MODULE_DESCRIPTION("f*ck this shit");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
