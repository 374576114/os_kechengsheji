#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("CraboYang 2016/3/12");

#define MAJOR_NUM 777 
#define BUFF_SIZE 256

static ssize_t printer_read(struct file*, char*, size_t, loff_t*);
static ssize_t printer_write(struct file*, const char*, size_t, loff_t*);
static int printer_open(struct inode *inode, struct file *filp);
static int printer_release(struct inode *inode, struct file *filp);

/*入口函数*/
static struct file_operations printer_fops = 
{
	.open = printer_open,
	.release = printer_release,
	.read = printer_read,
	.write= printer_write,
};

/*各种变量*/
static char *kbuf = NULL;
static int kbuf_count = 0; /*表示kbuf不空时,已经写入的数据数*/
static int count = 0;/*正在使用的printer数目*/
static struct semaphore sem;
static DEFINE_SPINLOCK(spin);

/*init*/
static int __init printer_kernel_init(void)
{
	int ret;
	printk("*******Printer Driver!********\n");
	ret = register_chrdev(MAJOR_NUM, "printer", &printer_fops);

	if(ret){
		printk("printer register failure");
	}else{
		kbuf = (char *)kmalloc(sizeof(char) * BUFF_SIZE, GFP_KERNEL);
		if(kbuf == NULL){
			printk("memory error\n");
		}else {
			sema_init(&sem, 1);
			printk("init sucess");
		}
	}
	return ret;
}

/*exit 函数*/
static void __exit printer_kernel_exit(void)
{
	int ret = 0;
	printk("********Exit Printer*****\n");
	unregister_chrdev(MAJOR_NUM, "printer");
	if(ret){
		printk("unregister_chrdev failure");
	}else{
		kfree(kbuf);
		kbuf = NULL;
		printk("unregister_chrdev sucess");
	}
}
/*open*/
static int printer_open(struct inode* inode, struct file * filp)
{
	spin_lock(&spin);

	if(count == 1){/*已经有一个进程在使用该设备*/
		spin_unlock(&spin);
		printk("***error***More Than one printer device\n");
		return -1;
	}

	count ++;

	spin_unlock(&spin);
	return 0;
}

/*release*/
static int printer_release(struct inode* inode, struct file *filp)
{
	count --;
	return 0;
}
/*read*/
static ssize_t printer_read(struct file *filp, char *buf, size_t len, loff_t *off)
{
	if(down_interruptible(&sem)){
		return -ERESTARTSYS;
	}

	if(kbuf == NULL){
		printk("memory error \n");
		return -1;
	}

	if( len > kbuf_count || copy_to_user(buf, kbuf, len)){
		up(&sem);
		return -EFAULT;
	}

	/*读处理*/
	kbuf_count = 0;
	up(&sem);
	return len;
}

/*write*/
static ssize_t printer_write(struct file *filp, const char *buf, size_t len, loff_t* off)
{
	if(down_interruptible(&sem)){
		return -ERESTARTSYS;
	}
	if(kbuf == NULL){
		printk("memory error\n");
		return -1;
	}

	if(kbuf_count != 0){
		up(&sem);
		return -1;
	}

	if(copy_from_user(kbuf, buf, len)){
		kbuf_count = 0;
		up(&sem);
		return -EFAULT;
	}
	/*write 处理*/
	kbuf_count = len;
	up(&sem);
	return len; 
}

module_init(printer_kernel_init);
module_exit(printer_kernel_exit);



