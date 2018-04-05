#include <linux/module.h> /* for modules */
#include <linux/fs.h> /* file_operations */
#include <linux/uaccess.h> /* copy_(to,from)_user */
#include <linux/init.h> /* module_init, module_exit */
#include <linux/slab.h> /* kmalloc */
#include <linux/cdev.h> /* cdev utilities */



#define MAJOR_NUMBER 500
#define MINOR_NUMBER 0
const char* tuxdrv;
char* buffer;
const int buff_size=100;

// void *tux_open(void){

// }

static ssize_t tux_read(struct file *flip, char *buffer, size_t len, loff_t *offset);
static ssize_t tux_write(struct file *flip, const char *buffer, size_t len, loff_t *offset);
static int tux_open(struct inode *inode, struct file *file);
static int tux_release(struct inode *inode, struct file *file);

struct file_operations fops = {

	.owner = THIS_MODULE,
	.open = tux_open,
	.release = tux_release,
	.write = tux_write,
	.read = tux_read,
};


struct cdev *cdev_ptr;



static int tux_init(void)
{
	dev_t device_no = MKDEV(MAJOR_NUMBER,MINOR_NUMBER);
	buffer = kmalloc(buff_size,GFP_KERNEL);
	register_chrdev_region(device_no, 1, "tux");

	cdev_ptr = cdev_alloc();
	cdev_init(cdev_ptr,&fops);
	cdev_add(cdev_ptr,device_no,1);

	printk(KERN_INFO "Tux driver loaded!\n");

	return 0;
}

static void tux_exit(void)
{
	cdev_del(cdev_ptr);

	unregister_chrdev_region(MKDEV(MAJOR_NUMBER,MINOR_NUMBER),1);
	kfree(buffer);
	printk(KERN_INFO "Tux driver unloaded!\n");

}


static int tux_open(struct inode *inode, struct file *file) {

	printk("Device file opened! Device No=%d\n",MKDEV(MAJOR_NUMBER,MINOR_NUMBER));
	
 return 0;
}

static int tux_release(struct inode *inode, struct file *file) {

	printk("Device file closed! Device No=%d\n",MKDEV(MAJOR_NUMBER,MINOR_NUMBER));
	
 return 0;
}


static ssize_t tux_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
 
 char temp[1024];
 if(len>1023)
 	len = 1023;

 copy_from_user(temp, buffer, len);
 temp[len] = '\0';
 printk("%s",temp);

 return len;
}


static ssize_t tux_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
 int i;
 char *buff = "Hello from kernel space ";
 buff[23] = '\0';
 

 for(i=0;i<23;i++ )
 	put_user(*(buff++), buffer++);
 
 return 23;
}


MODULE_LICENSE("GPL");
module_init(tux_init);
module_exit(tux_exit);
