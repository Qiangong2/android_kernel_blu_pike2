#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/wait.h>

#include "rdc_debug.h"
#include "wcn_glb.h"

#define MDBG_WRITE_SIZE			(64)
#define MDBG_ASSERT_SIZE		(1024)
#define MDBG_LOOPCHECK_SIZE		(128)
#define MDBG_AT_CMD_SIZE		(128)
#define WCN_LOG_MAJOR 255
#define WCN_LOG_MAX_MINOR 2
static int wcn_log_major = WCN_LOG_MAJOR;
static struct class		*wcnlog_class;

struct wcnlog_dev {
	struct cdev cdev;
	int			major;
	int			minor;
};

struct mdbg_device_t *mdbg_dev;
wait_queue_head_t	mdbg_wait;

int wake_up_log_wait(void)
{
	wake_up_interruptible(&mdbg_dev->rxwait);
	wake_up_interruptible(&mdbg_wait);

	return 0;
}

void wcnlog_clear_log(void)
{
	if (mdbg_dev->ring_dev->ring->rp
		!= mdbg_dev->ring_dev->ring->wp) {
		WCN_INFO("log:%ld left in ringbuf not read\n",
			mdbg_dev->ring_dev->ring->wp -
			mdbg_dev->ring_dev->ring->rp);
		mdbg_ring_clear(mdbg_dev->ring_dev->ring);
	}
}

static int wcnlog_open(struct inode *inode, struct file *filp)
{
	struct wcnlog_dev *dev;

	int minor = iminor(filp->f_path.dentry->d_inode);
	int minor1 = iminor(inode);
	int major = imajor(inode);

	WCN_INFO("minor=%d,minor1=%d,major=%d\n", minor, minor1, major);
	MDBG_LOG("wcnlog_open\n");
	dev = container_of(inode->i_cdev, struct wcnlog_dev, cdev);
	filp->private_data = dev;

	MDBG_LOG("wcnlog_open z=%d,major=%d,minor = %d\n",
		dev->cdev.dev, MAJOR(dev->cdev.dev), MINOR(dev->cdev.dev));
	MDBG_LOG("minor= %d\n", minor);

	if (mdbg_dev->open_count != 0)
		MDBG_ERR("mdbg_open %d\n", mdbg_dev->open_count);

	mdbg_dev->open_count++;

	return 0;
}

static int wcnlog_release(struct inode *inode, struct file *filp)
{
	struct wcnlog_dev *dev = filp->private_data;

	WCN_INFO("wcnlog_release z=%d,major=%d,minor = %d\n",
		dev->cdev.dev, MAJOR(dev->cdev.dev), MINOR(dev->cdev.dev));
	mdbg_dev->open_count--;

	return 0;
}

static ssize_t wcnlog_read(struct file *filp,
		char __user *buf, size_t count, loff_t *ppos)
{
	long int read_size;
	int timeout = -1;
	int rval = 0;
	static unsigned int dum_send_size;
	struct wcnlog_dev *dev = filp->private_data;

	if (filp->f_flags & O_NONBLOCK)
		timeout = 0;

	MDBG_LOG("read timeout=%d,major=%d, minor=%d\n",
		timeout, dev->major, dev->minor);

	MDBG_LOG("wcnlog_read z=%d,major=%d,minor = %d\n", dev->cdev.dev,
		MAJOR(dev->cdev.dev), MINOR(dev->cdev.dev));
	/* count :100K-log, 32K-mem ;cat :4096 */
	MDBG_LOG("wcnlog_read len = %ld\n", count);
	if ((functionmask[7] & CP2_FLAG_YLOG) == 1)
		return -EIO;

	if (MINOR(dev->cdev.dev) == 1) {
		WCN_INFO("unsupported to read slog_wcn1: BT audio data\n");
		return -EPERM;
	}

	if (filp->f_flags & O_NONBLOCK)
		timeout = 0;

	if (count > MDBG_RX_RING_SIZE)
		count = MDBG_RX_RING_SIZE;

	if (timeout < 0) {
		/* when kernel go to sleep, it return -512 */
		rval = wait_event_interruptible(mdbg_wait,
				(mdbg_content_len() > 0));
		if (rval < 0)
			WCN_INFO("mdbg_read wait interrupted=%d\n", rval);
	}

	mutex_lock(&mdbg_dev->mdbg_lock);
	read_size = mdbg_receive((void *)buf, (long int)count);
	if (sprdwcn_bus_get_carddump_status() == 1) {
		dum_send_size += read_size;
		WCN_INFO("read_size = %ld dum_total_size= %d,remainder =%ld\n",
				read_size, dum_send_size, mdbg_content_len());
	} else
		WCN_INFO("%s : %ld remainder:0x%lx\n",
			__func__, read_size, mdbg_content_len());
	/* read_size = log1040 or mem32K, 1024 */
	if (read_size > 0) {
		MDBG_LOG("Show %ld bytes data.", read_size);
		rval = read_size;
	} else if (read_size == 0)
		rval = -EAGAIN;
	else {
		rval = read_size;
		MDBG_ERR("log read error %d\n", rval);
	}
	mutex_unlock(&mdbg_dev->mdbg_lock);

	return rval;

}

static ssize_t wcnlog_write(struct file *filp,
		const char __user *buf, size_t count, loff_t *ppos)
{
	long int sent_size = 0;
	char *p_data = NULL;

	MDBG_LOG("wcnlog_write\n");
	if (count > MDBG_WRITE_SIZE) {
		WCN_ERR("mdbg_write count > MDBG_WRITE_SIZE\n");
		return -ENOMEM;
	}

	if (NULL == buf || 0 == count) {
		MDBG_ERR("Param Error!");
		return count;
	}

	p_data = memdup_user(buf, count);
	mutex_lock(&mdbg_dev->mdbg_lock);
	sent_size = mdbg_send(p_data, count, MDBG_SUBTYPE_AT);
	mutex_unlock(&mdbg_dev->mdbg_lock);
	kfree(p_data);

	MDBG_LOG("sent_size = %ld", sent_size);

	return sent_size;
}

static unsigned int wcnlog_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;

	MDBG_LOG("wcnlog_poll\n");
	poll_wait(filp, &mdbg_dev->rxwait, wait);
	if (mdbg_content_len() > 0)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static long wcnlog_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static const struct file_operations wcnlog_fops = {
	.open		= wcnlog_open,
	.release	= wcnlog_release,
	.read		= wcnlog_read,
	.write		= wcnlog_write,
	.poll		= wcnlog_poll,
	.unlocked_ioctl	= wcnlog_ioctl,
	.owner		= THIS_MODULE,
	.llseek		= default_llseek,
};

static int wcnlog_register_device(struct wcnlog_dev *dev, int index)
{
	dev_t devno;
	int ret;

	devno = MKDEV(wcn_log_major, index);
	dev->cdev.owner = THIS_MODULE;
	cdev_init(&dev->cdev, &wcnlog_fops);
	ret = cdev_add(&dev->cdev, devno, 1);
	if (ret != 0) {
		kfree(dev);
		unregister_chrdev_region(devno, 1);
		WCN_ERR("Failed to add wcn log cdev\n");
		return ret;
	}
	dev->major = MAJOR(devno);
	dev->minor = MINOR(devno);
	WCN_INFO("log dev major=%d,minor=%d\n", dev->major, dev->minor);
	if (index == 0)
		device_create(wcnlog_class, NULL,
				MKDEV(MAJOR(devno), MINOR(devno)),
				NULL, "%s", "slog_wcn");
	else
		device_create(wcnlog_class, NULL,
				MKDEV(MAJOR(devno), MINOR(devno)),
				NULL, "%s%d", "slog_wcn", index);

	return 0;
}

int log_cdev_init(void)
{
	int ret = -1;
	int	i;
	dev_t devno;

	struct wcnlog_dev *dev = NULL;
	int malloc_size = WCN_LOG_MAX_MINOR * sizeof(struct wcnlog_dev);

	WCN_INFO("log_cdev_init\n");
	wcnlog_class = class_create(THIS_MODULE, "slog_wcn");
	if (IS_ERR(wcnlog_class))
		return PTR_ERR(wcnlog_class);

	devno = MKDEV(wcn_log_major, 0);
	if (wcn_log_major)
		ret = register_chrdev_region(devno,
				WCN_LOG_MAX_MINOR, "wcnlog");
	if (ret < 0) {
		WCN_INFO("failed to apply for static device number");
		ret = alloc_chrdev_region(&devno, 0,
				WCN_LOG_MAX_MINOR, "wcnlog");
		wcn_log_major = MAJOR(devno);
	}

	if (ret < 0) {
		WCN_ERR("failed to apply for device number");
		return ret;
	}

	dev = kmalloc(malloc_size, GFP_KERNEL);
	if (!dev) {
		ret = -ENOMEM;
		goto fail_malloc;
	}

	for (i = 0; i < WCN_LOG_MAX_MINOR; i++)
		wcnlog_register_device(&dev[i], i);
	mdbg_dev->dev = dev;

	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

int log_cdev_exit(void)
{
	struct wcnlog_dev *dev;
	int i;

	if (!mdbg_dev->dev)
		return 0;

	dev = mdbg_dev->dev;

	for (i = 0; i < WCN_LOG_MAX_MINOR; i++) {
		device_destroy(wcnlog_class, (&(dev[i].cdev))->dev);
		cdev_del(&(dev[i].cdev));
	}
	kfree(dev);

	class_destroy(wcnlog_class);

	unregister_chrdev_region(MKDEV(wcn_log_major, 0), WCN_LOG_MAX_MINOR);

	return 0;
}

int log_dev_init(void)
{
	int err;

	MDBG_FUNC_ENTERY;
	mdbg_dev = kzalloc(sizeof(struct mdbg_device_t), GFP_KERNEL);
	if (!mdbg_dev)
		return -ENOMEM;

	mdbg_dev->open_count = 0;
	mutex_init(&mdbg_dev->mdbg_lock);
	init_waitqueue_head(&mdbg_dev->rxwait);
	init_waitqueue_head(&mdbg_wait);
	err = mdbg_ring_init();
	if (err < 0)
		return -ENOMEM;

	log_cdev_init();

	return 0;
}

int log_dev_exit(void)
{
	log_cdev_exit();

	/* free for old mdbg_dev */
	mdbg_ring_remove();
	mutex_destroy(&mdbg_dev->mdbg_lock);
	kfree(mdbg_dev);

	return 0;
}
