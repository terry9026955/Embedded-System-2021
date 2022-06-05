#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static char buf1[261];
static char buf2[261];
static int IO_init(void)
{
    int i = 0, j = 0, k = 0;
    struct file *fp;
    struct file *output;
    loff_t pos = 0;
    //Read the textfile and check file
    printk("readfile enter!\n");
    fp = filp_open("/home/terry9026955/Desktop/fileIO/input.txt", O_RDWR, 0);
    if (IS_ERR(fp)) {
        printk("create file error\n");
        return -1;
    }
    //Use kernel_read() instead of vfs_write or vfs_read
    kernel_read(fp, buf1, sizeof(buf1), &pos); //read fp's content into buf1
    printk("read: %s\n", buf1);
    //Reverse buf1's content and write into buf2
    while(buf1[i] != '\0'){
	i++;
    }
    for(j = i-1; j >= 0; j--){
	buf2[k] = buf1[j];
	k++;
    }	 
    printk("output: %s\n", buf2);
    
    //Write buf2 into a new textfile
    output = filp_open("/home/terry9026955/Desktop/fileIO/output.txt", O_RDWR, 0);
    pos = 0; //pos表示當前文件讀寫所處於的偏移位置
    kernel_write(output, buf2, sizeof(buf2), &pos);
    filp_close(fp, NULL);
    return 0;
}
static void IO_exit(void)
{
    printk(KERN_INFO "readfile  exit!\n");
}
module_init(IO_init);
module_exit(IO_exit);
MODULE_LICENSE("GPL");

