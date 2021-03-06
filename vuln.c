#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>

#define DEVNAME "vuln"
#define MAX_RW (PAGE_SIZE*2)

MODULE_AUTHOR("Mohamed Ghannam");
MODULE_LICENSE("GPL v2");

static struct cdev *cdev;
static char *ramdisk;
static int vuln_major = 700,vuln_minor = 3;
static dev_t first;
static int count = 1;

static int vuln_open_dev(struct inode *inode, struct file *file) {
static int counter=0;
char *ramdisk;

printk(KERN_INFO"opening device : %s \n",DEVNAME);
ramdisk = kzalloc(MAX_RW,GFP_KERNEL);
if(!ramdisk)
return -ENOMEM;
//file->private_data = ramdisk;
printk(KERN_INFO"MAJOR no = %d and MINOR no = %d\n",imajor(inode),iminor(inode));
printk(KERN_INFO"Opened device : %s\n",DEVNAME);
counter++;
printk(KERN_INFO"opened : %d\n",counter);
return 0;
}

static int vuln_release_dev(struct inode *inode,struct file *file)
{
printk(KERN_INFO"closing device : %s \n",DEVNAME);
return 0;
}

static ssize_t vuln_write_dev(struct file *file,const char __user *buf,size_t lbuf,loff_t *ppos)
{
int nbytes,i;
char *copy;

char *ramdisk = kzalloc(lbuf,GFP_KERNEL);
if(!ramdisk)
return -ENOMEM;

copy = kmalloc(256 , GFP_KERNEL);
if(!copy)
return -ENOMEM;

if ((lbuf+*ppos) > MAX_RW) {
printk(KERN_WARNING"Write Abbort \n");
return 0;
}

nbytes = lbuf-copy_from_user(ramdisk+*ppos,buf,lbuf);
ppos += nbytes;

for(i=0;i<0x40;i++)
copy[i]=0xCC;
memcpy(copy,ramdisk,lbuf);
printk("ramdisk : %s \n",ramdisk);
printk("Writing : bytes = %d\n",(int)lbuf);
return nbytes; }
static ssize_t vuln_read_dev(struct file *file ,char __user*buf,size_t lbuf ,loff_t*ppos)
{
int nbytes;
char *ramdisk = file->private_data;
if((lbuf + *ppos) > MAX_RW)
{
printk(KERN_WARNING"Read Abort \n");
return 0;
}

nbytes = lbuf - copy_to_user(buf,ramdisk + *ppos , lbuf);
*ppos += nbytes;
return nbytes;
}

static struct file_operations fps = {
.owner = THIS_MODULE,
.open = vuln_open_dev,
.release = vuln_release_dev,
.write = vuln_write_dev,
.read = vuln_read_dev,
};

static int __init vuln_init(void)
{
ramdisk = kmalloc(MAX_RW,GFP_KERNEL);
first = MKDEV(vuln_major,vuln_minor);
register_chrdev_region(first,count,DEVNAME);
cdev = cdev_alloc();
cdev_init(cdev,&fps);
cdev_add(cdev,first,count);
printk(KERN_INFO"Registring device %s\n",DEVNAME);
return 0;
}

static void __exit vuln_exit(void)
{
cdev_del(cdev);
unregister_chrdev_region(first,count);
kfree(ramdisk);

}

module_init(vuln_init);
module_exit(vuln_exit)
