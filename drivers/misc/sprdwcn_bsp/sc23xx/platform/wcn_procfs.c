/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * File:		wcn_procfs.c
 * Description:	Marlin Debug System main file. Module,device &
 * driver related defination.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the	1
 * GNU General Public License for more details.
 */

#include <linux/of.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/wait.h>
#include <soc/sprd/wcn_bus.h>

#include "wcn_glb.h"
#include "wcn_log.h"
#include "wcn_procfs.h"
#include "wcn_txrx.h"

static u32 g_dumpmem_switch =  1;
static u32 g_loopcheck_switch;

struct mdbg_proc_entry {
	char *name;
	struct proc_dir_entry *entry;
	struct completion completed;
	wait_queue_head_t	rxwait;
	unsigned int	rcv_len;
	void *buf;
};

struct mdbg_proc_t {
	char *dir_name;
	struct proc_dir_entry		*procdir;
	struct mdbg_proc_entry		assert;
	struct mdbg_proc_entry		loopcheck;
	struct mdbg_proc_entry		at_cmd;
	struct mdbg_proc_entry		snap_shoot;
	char write_buf[MDBG_WRITE_SIZE];
	int fail_count;
	bool first_boot;
};

static struct mdbg_proc_t *mdbg_proc;

void mdbg_assert_interface(char *str)
{
	int len = MDBG_ASSERT_SIZE;

	if (strlen(str) <= MDBG_ASSERT_SIZE)
		len = strlen(str);

	strncpy(mdbg_proc->assert.buf, str, len);
	WCN_INFO("mdbg_assert_interface:%s\n",
		(char *)(mdbg_proc->assert.buf));

	sprdwcn_bus_set_carddump_status(true);
	/* wcn_hold_cpu(); */
	wcnlog_clear_log();
	mdbg_proc->assert.rcv_len = 23;
	mdbg_proc->fail_count++;
	complete(&mdbg_proc->assert.completed);
	wake_up_interruptible(&mdbg_proc->assert.rxwait);
}
EXPORT_SYMBOL_GPL(mdbg_assert_interface);

static int mdbg_assert_read(int channel, struct mbuf_t *head,
		     struct mbuf_t *tail, int num)
{
	if (head->len > MDBG_ASSERT_SIZE) {
		MDBG_ERR("assert data len:%d,beyond max read:%d",
			head->len, MDBG_ASSERT_SIZE);
		sprdwcn_bus_push_list(channel, head, tail, num);
		return -1;
	}

	memcpy(mdbg_proc->assert.buf, head->buf + PUB_HEAD_RSV, head->len);
	mdbg_proc->assert.rcv_len = head->len;
	WCN_INFO("mdbg_assert_read:%s\n",
		(char *)(mdbg_proc->assert.buf));
	mdbg_proc->fail_count++;
	complete(&mdbg_proc->assert.completed);
	wake_up_interruptible(&mdbg_proc->assert.rxwait);
	sprdwcn_bus_push_list(channel, head, tail, num);

	return 0;
}
EXPORT_SYMBOL_GPL(mdbg_assert_read);

static int mdbg_loopcheck_read(int channel, struct mbuf_t *head,
			struct mbuf_t *tail, int num)
{
	if (head->len > MDBG_LOOPCHECK_SIZE) {
		MDBG_ERR("The loopcheck data len:%d,beyond max read:%d",
			head->len, MDBG_LOOPCHECK_SIZE);
		sprdwcn_bus_push_list(channel, head, tail, num);
		return -1;
	}

	memset(mdbg_proc->loopcheck.buf, 0, MDBG_LOOPCHECK_SIZE);
	memcpy(mdbg_proc->loopcheck.buf, head->buf + PUB_HEAD_RSV, head->len);
	mdbg_proc->loopcheck.rcv_len = head->len;
	WCN_INFO("mdbg_loopcheck_read:%s\n",
		(char *)(mdbg_proc->loopcheck.buf));
	mdbg_proc->fail_count = 0;
	complete(&mdbg_proc->loopcheck.completed);
	sprdwcn_bus_push_list(channel, head, tail, num);

	return 0;
}
EXPORT_SYMBOL_GPL(mdbg_loopcheck_read);

static int mdbg_at_cmd_read(int channel, struct mbuf_t *head,
		     struct mbuf_t *tail, int num)
{
	if (head->len > MDBG_AT_CMD_SIZE) {
		MDBG_ERR("The at cmd data len:%d,beyond max read:%d",
			head->len, MDBG_AT_CMD_SIZE);
		sprdwcn_bus_push_list(channel, head, tail, num);
		return -1;
	}

	memset(mdbg_proc->at_cmd.buf, 0, MDBG_AT_CMD_SIZE);
	memcpy(mdbg_proc->at_cmd.buf, head->buf + PUB_HEAD_RSV, head->len);
	mdbg_proc->at_cmd.rcv_len = head->len;
	WCN_INFO("mdbg_at_cmd_read:%s\n",
		(char *)(mdbg_proc->at_cmd.buf));
	complete(&mdbg_proc->at_cmd.completed);
	sprdwcn_bus_push_list(channel, head, tail, num);

	return 0;
}
EXPORT_SYMBOL_GPL(mdbg_at_cmd_read);

static ssize_t mdbg_snap_shoot_seq_write(struct file *file,
						const char __user *buffer,
						size_t count, loff_t *ppos)
{
	/* nothing to do */
	return count;
}

static void *mdbg_snap_shoot_seq_start(struct seq_file *m, loff_t *pos)
{
	u8 *pdata;
	u8 *buf;
	s32 ret = 0;

	if (!*(u32 *)pos) {
		buf = mdbg_proc->snap_shoot.buf;
		memset(buf, 0, MDBG_SNAP_SHOOT_SIZE);
#ifdef CONFIG_SC2342_INTEG
		ret = mdbg_snap_shoot_iram(buf);
		if (ret < 0) {
			seq_puts(m, "==== IRAM DATA SNAP SHOOT FAIL ====\n");
			return NULL;
		}
		seq_puts(m, "==== IRAM DATA SNAP SHOOT START ====\n");
#else
		pr_info("not support iram snap shoot! ret %d\n", ret);
		seq_puts(m, "==== IRAM DATA SNAP SHOOT NOT SUPPORT ====\n");
		return NULL;
#endif
	}

	pdata = mdbg_proc->snap_shoot.buf + *(u32 *)pos * 16;
	(*(u32 *)pos)++;

	if (*(u32 *)pos > 2048) {
		seq_puts(m, "==== IRAM DATA SNAP SHOOT END    ====\n");
		return NULL;
	} else
		return pdata;
}

static void *mdbg_snap_shoot_seq_next(struct seq_file *m, void *p, loff_t *pos)

{
	return mdbg_snap_shoot_seq_start(m, pos);
}

static void mdbg_snap_shoot_seq_stop(struct seq_file *m, void *p)
{
	/* nothing to do */
}

static int mdbg_snap_shoot_seq_show(struct seq_file *m, void *p)
{
	u8 *pdata;
	u32 loop;

	if (p) {
		for (loop = 0; loop < 2; loop++) {
			pdata = p + 8*loop;
			seq_printf(m, "0x%02x%02x%02x%02x 0x%02x%02x%02x%02x ",
					pdata[3], pdata[2], pdata[1], pdata[0],
					pdata[7], pdata[6], pdata[5], pdata[4]);
		}
		seq_puts(m, "\n");
	}

	return 0;
}

static const struct seq_operations mdbg_snap_shoot_seq_ops = {
	.start = mdbg_snap_shoot_seq_start,
	.next = mdbg_snap_shoot_seq_next,
	.stop = mdbg_snap_shoot_seq_stop,
	.show = mdbg_snap_shoot_seq_show
};

static int mdbg_snap_shoot_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &mdbg_snap_shoot_seq_ops);
}

static const struct file_operations mdbg_snap_shoot_seq_fops = {
	.open = mdbg_snap_shoot_seq_open,
	.read = seq_read,
	.write = mdbg_snap_shoot_seq_write,
	.llseek = seq_lseek,
	.release = seq_release
};

static int mdbg_proc_open(struct inode *inode, struct file *filp)
{
	struct mdbg_proc_entry *entry =
		(struct mdbg_proc_entry *)PDE_DATA(inode);
	filp->private_data = entry;

	return 0;
}

static int mdbg_proc_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t mdbg_proc_read(struct file *filp,
		char __user *buf, size_t count, loff_t *ppos)
{
	struct mdbg_proc_entry *entry =
		(struct mdbg_proc_entry *)filp->private_data;
	char *type = entry->name;
	int timeout = -1;
	int len = 0;
	int ret;

	if (filp->f_flags & O_NONBLOCK)
		timeout = 0;

	if (strcmp(type, "assert") == 0) {
		if (timeout < 0) {
			while (1) {
				ret = wait_for_completion_timeout(
						&mdbg_proc->assert.completed,
						msecs_to_jiffies(1000));
				if (ret != -ERESTARTSYS)
					break;
			}
		}

		if (copy_to_user((void __user *)buf,
				mdbg_proc->assert.buf,
				min(count, (size_t)MDBG_ASSERT_SIZE)))
			MDBG_ERR("Read assert info error\n");
		len = mdbg_proc->assert.rcv_len;
		mdbg_proc->assert.rcv_len = 0;
		memset(mdbg_proc->assert.buf, 0, MDBG_ASSERT_SIZE);
	}

	if (strcmp(type, "loopcheck") == 0) {
		if (unlikely(g_loopcheck_switch != 0)) {
			if (marlin_get_module_status() == 1) {
				pr_info("fake loopcheck\n");
				if (copy_to_user((void __user *)buf,
							"loopcheck_ack", 13))
					MDBG_ERR("fake loopcheck reply err\n");
				len = 13;
			} else {
				if (copy_to_user((void __user *)buf,
							"poweroff", 8))
					pr_err("Read loopcheck poweroff error\n");
				len = 8;
				pr_info("mdbg loopcheck poweroff\n");
			}
			return len;
		}

		if (timeout < 0) {
			while (1) {
				ret = wait_for_completion_timeout(
						&mdbg_proc->loopcheck.completed,
						msecs_to_jiffies(1000));
				if (ret != -ERESTARTSYS)
					break;
			}
		}
		if (marlin_get_module_status() == 1) {
			if (!mdbg_proc->first_boot) {
				if (copy_to_user((void __user *)buf,
					"loopcheck_ack", 13))
					MDBG_ERR("loopcheck first error\n");
				mdbg_proc->first_boot = true;
				WCN_INFO("CP power on first time\n");
				len = 13;
			} else
			if (mdbg_rx_count_change()) {
			/* fix the error(ack slow),use rx count to verify CP */
				WCN_INFO("CP run well with rx_cnt change\n");
				if (copy_to_user((void __user *)buf,
							"loopcheck_ack", 13))
					MDBG_ERR("loopcheck gpio info error\n");
				len = 13;
			} else {
				if (copy_to_user((void __user *)buf,
					mdbg_proc->loopcheck.buf, min(count,
						(size_t)MDBG_LOOPCHECK_SIZE)))
					MDBG_ERR("loopcheck sdio info error\n");
				len = mdbg_proc->loopcheck.rcv_len;
				if (strncmp(mdbg_proc->loopcheck.buf,
					"loopcheck_ack", 13) != 0)
					mdbg_proc->fail_count++;
				WCN_INFO("loopcheck status:%d\n",
					mdbg_proc->fail_count);
			}
		} else {
			if (copy_to_user((void __user *)buf, "poweroff", 8))
				WCN_ERR("Read loopcheck poweroff error\n");
			len = 8;
			WCN_INFO("mdbg loopcheck poweroff\n");
		}
		memset(mdbg_proc->loopcheck.buf, 0, MDBG_LOOPCHECK_SIZE);
		mdbg_proc->loopcheck.rcv_len = 0;
	}

	if (strcmp(type, "at_cmd") == 0) {
		if (timeout < 0) {
			while (1) {
				ret = wait_for_completion_timeout(
						&mdbg_proc->at_cmd.completed,
						msecs_to_jiffies(1000));
				if (ret != -ERESTARTSYS)
					break;
			}
		}

		if (copy_to_user((void __user *)buf,
					mdbg_proc->at_cmd.buf,
					min(count, (size_t)MDBG_AT_CMD_SIZE)))
			MDBG_ERR("Read at cmd ack info error\n");

		len = mdbg_proc->at_cmd.rcv_len;
		mdbg_proc->at_cmd.rcv_len = 0;
		memset(mdbg_proc->at_cmd.buf, 0, MDBG_AT_CMD_SIZE);
	}

	return len;
}
/**************************************************
 * marlin2 crash
 *   |-user:       rebootmarlin
 *   `-userdebug : dumpmem for btwifi
 *
 * GNSS2 crash
 *   |-user:       rebootwcn
 *   `-userdebug : no action(libgps and gnss dbg will do)
 *
 * marlin3 crash
 *   |-user:       rebootmarlin
 *   `-userdebug : dumpmem for btwifi
 *
 * GNSS3 crash
 *   |-user:       rebootwcn
 *   `-userdebug : dumpmem for gnss
 *
 *  rebootmarlin: reset gpio enable
 *  rebootwcn :   chip_en gpio and reset gpio enable
 *****************************************************/
static ssize_t mdbg_proc_write(struct file *filp,
		const char __user *buf, size_t count, loff_t *ppos)
{
	char x;
#ifdef MDBG_PROC_CMD_DEBUG
	char *tempbuf = NULL;
	int ret = -1, i;
#endif

	if (count < 1)
		return -EINVAL;
	if (copy_from_user(&x, buf, 1))
		return -EFAULT;
#ifdef MDBG_PROC_CMD_DEBUG
/* for test boot */

	if (x == '0')
		dump_arm_reg();

	if (x == 'B') {
		WCN_INFO("wsh proc write =%c\n", x);
		tempbuf = kzalloc(10, GFP_KERNEL);
		memset(tempbuf, 0, 10);
		ret = sprdwcn_bus_direct_read(CP_START_ADDR, tempbuf, 10);
		if (ret < 0)
			WCN_ERR("wsh debug CP_START_ADDR error:%d\n", ret);
		WCN_INFO("\nwsh debug CP_START_ADDR(10) :\n");
		for (i = 0; i < 10; i++)
			WCN_INFO("0x%x\n", tempbuf[i]);

		memset(tempbuf, 0, 10);
		ret = sprdwcn_bus_reg_read(CP_RESET_REG, tempbuf, 4);
		if (ret < 0)
			WCN_ERR("wsh debug CP_RESET_REG error:%d\n", ret);
		WCN_INFO("\nwsh debug CP_RESET_REG(4) :\n");
		for (i = 0; i < 4; i++)
			WCN_INFO(":0x%x\n", tempbuf[i]);

		memset(tempbuf, 0, 10);
		ret = sprdwcn_bus_direct_read(GNSS_CP_START_ADDR, tempbuf, 10);
		if (ret < 0)
			WCN_ERR("wsh debug GNSS_CP_START_ADDR error:%d\n", ret);
		WCN_INFO("\nwsh debug GNSS_CP_START_ADDR(10) :\n");
		for (i = 0; i < 10; i++)
			WCN_INFO(":0x%x\n", tempbuf[i]);

		memset(tempbuf, 0, 10);
		ret = sprdwcn_bus_reg_read(GNSS_CP_RESET_REG, tempbuf, 4);
		if (ret < 0)
			WCN_ERR("wsh debug GNSS_CP_RESET_REG error:%d\n", ret);
		WCN_INFO("\nwsh debug GNSS_CP_RESET_REG(4) :\n");
		for (i = 0; i < 4; i++)
			WCN_INFO(":0x%x\n", tempbuf[i]);

		kfree(tempbuf);
	}

#ifdef MDBG_PROC_CMD_DEBUG
/* for test cdev */
	if (x == '1') {
		WCN_INFO("wsh proc write =%c\n", x);
		WCN_INFO("start char release test\n");
		log_cdev_exit();
	}
	if (x == '2') {
		WCN_INFO("wsh proc write =%c\n", x);
		WCN_INFO("start char register test\n");
		log_cdev_init();
	}
#endif
/* for test power on/off frequently */
#ifdef MDBG_PROC_CMD_DEBUG
	if (x == '0')
		start_marlin(MARLIN_BLUETOOTH);
	if (x == '1')
		start_marlin(MARLIN_FM);
	if (x == '2')
		start_marlin(MARLIN_WIFI);
	if (x == '3')
		start_marlin(MARLIN_MDBG);
	if (x == '4')
		start_marlin(MARLIN_GNSS);

	if (x == '5')
		stop_marlin(MARLIN_BLUETOOTH);
	if (x == '6')
		stop_marlin(MARLIN_FM);
	if (x == '7')
		stop_marlin(MARLIN_WIFI);
	if (x == '8')
		stop_marlin(MARLIN_MDBG);
	if (x == '9')
		stop_marlin(MARLIN_GNSS);
	if (x == 'x')
		open_power_ctl();
#endif
	if (x == 'Z')
		slp_mgr_drv_sleep(MARLIN_GNSS, FALSE);
	if (x == 'Y')
		slp_mgr_wakeup(MARLIN_GNSS);
	if (x == 'W')
		slp_mgr_drv_sleep(MARLIN_GNSS, TRUE);
	if (x == 'V')
		inform_cp_wifi_download();
	if (x == 'U')
		sprdwcn_bus_aon_writeb(0X1B0, 0X10);
	if (x == 'T')
		mem_pd_mgr(MARLIN_WIFI, 0X1);
	if (x == 'Q')
		mem_pd_save_bin();
	if (x == 'N')
		start_marlin(MARLIN_WIFI);
	if (x == 'R')
		stop_marlin(MARLIN_WIFI);

#endif
	if (count > MDBG_WRITE_SIZE) {
		MDBG_ERR("mdbg_proc_write count > MDBG_WRITE_SIZE\n");
		return -ENOMEM;
	}
	memset(mdbg_proc->write_buf, 0, MDBG_WRITE_SIZE);

	if (copy_from_user(mdbg_proc->write_buf, buf, count))
		return -EFAULT;

	WCN_INFO("mdbg_proc->write_buf:%s\n", mdbg_proc->write_buf);

	if (strncmp(mdbg_proc->write_buf, "startwcn", 8) == 0) {
		if (start_marlin(MARLIN_MDBG)) {
			WCN_ERR("%s power on failed\n", __func__);
			return -EIO;
		}
		return count;
	}

	if (strncmp(mdbg_proc->write_buf, "stopwcn", 7) == 0) {
		if (stop_marlin(MARLIN_MDBG)) {
			WCN_ERR("%s power off failed\n", __func__);
			return -EIO;
		}
		return count;
	}

	if (strncmp(mdbg_proc->write_buf, "disabledumpmem",
		strlen("disabledumpmem")) == 0) {
		g_dumpmem_switch = 0;
		WCN_INFO("hold mdbg dumpmem function:switch(%d)\n",
				g_dumpmem_switch);
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "enabledumpmem",
		strlen("enabledumpmem")) == 0) {
		g_dumpmem_switch = 1;
		WCN_INFO("release mdbg dumpmem function:switch(%d)\n",
				g_dumpmem_switch);
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "debugloopcheckon",
		strlen("debugloopcheckon")) == 0) {
		g_loopcheck_switch = 1;
		pr_info("loopcheck debug:switch(%d)\n",
				g_loopcheck_switch);
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "debugloopcheckoff",
		strlen("debugloopcheckoff")) == 0) {
		g_loopcheck_switch = 0;
		pr_info("loopcheck debug:switch(%d)\n",
				g_loopcheck_switch);
		return count;
	}

#ifdef CONFIG_SC2342_INTEG
	if (strncmp(mdbg_proc->write_buf, "dumpmem", 7) == 0) {
		if (g_dumpmem_switch == 0)
			return count;
		WCN_INFO("start mdbg dumpmem");
		sprdwcn_bus_set_carddump_status(true);
		mdbg_dump_mem();
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "holdcp2cpu",
		strlen("holdcp2cpu")) == 0) {
		mdbg_hold_cpu();
		WCN_INFO("hold cp cpu\n");
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "rebootwcn", 9) == 0 ||
		strncmp(mdbg_proc->write_buf, "rebootmarlin", 12) == 0) {
		wcn_set_reset_flag_status(1);
		WCN_INFO("marlin gnss need reset\n");
		WCN_INFO("fail_count is value %d\n", mdbg_proc->fail_count);
		mdbg_proc->fail_count = 0;
		wcn_sys_reset();
		if (marlin_reset_func != NULL)
			marlin_reset_func(marlin_callback_para);
		wcn_set_reset_flag_status(0);
		WCN_INFO("marlin gnss  reset finish!\n");
		return count;
	}
#else
	if (strncmp(mdbg_proc->write_buf, "dumpmem", 7) == 0) {
		sprdwcn_bus_set_carddump_status(true);
		marlin_set_sleep(MARLIN_MDBG, FALSE);
		marlin_set_wakeup(MARLIN_MDBG);
		mdbg_dump_mem();
		marlin_set_sleep(MARLIN_MDBG, TRUE);
		return count;
	}

	if (strncmp(mdbg_proc->write_buf, "poweroff_wcn", 12) == 0) {
		marlin_power_off(MARLIN_ALL);
		return count;
	}

	if (strncmp(mdbg_proc->write_buf, "rebootmarlin", 12) == 0) {
		flag_reset = 1;
		WCN_INFO("marlin need reset\n");
		WCN_INFO("fail_count is value %d\n", mdbg_proc->fail_count);
		WCN_INFO("fail_reset is value %d\n", flag_reset);
		mdbg_proc->fail_count = 0;
		if (marlin_reset_func != NULL)
			marlin_reset_func(marlin_callback_para);
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "rebootwcn", 9) == 0) {
		flag_reset = 1;
		WCN_INFO("marlin gnss need reset\n");
		WCN_INFO("fail_count is value %d\n", mdbg_proc->fail_count);
		mdbg_proc->fail_count = 0;
		marlin_chip_en(false, true);
		if (marlin_reset_func != NULL)
			marlin_reset_func(marlin_callback_para);
		return count;
	}
	if (strncmp(mdbg_proc->write_buf, "at+getchipversion", 17) == 0) {
		struct device_node *np_marlin2 = NULL;

		WCN_INFO("marlin get chip version\n");
		np_marlin2 = of_find_node_by_name(NULL, "sprd-marlin2");
		if (np_marlin2) {
			if (of_get_property(np_marlin2,
				"common_chip_en", NULL)) {
				WCN_INFO("marlin common_chip_en\n");
				memcpy(mdbg_proc->at_cmd.buf,
					"2342B", strlen("2342B"));
				mdbg_proc->at_cmd.rcv_len = strlen("2342B");
			}
		}
		return count;
	}
#endif
	mdbg_send(mdbg_proc->write_buf, count, MDBG_SUBTYPE_AT);

	return count;
}

static unsigned int mdbg_proc_poll(struct file *filp, poll_table *wait)
{
	struct mdbg_proc_entry *entry =
		(struct mdbg_proc_entry *)filp->private_data;
	char *type = entry->name;
	unsigned int mask = 0;

	if (strcmp(type, "assert") == 0) {
		poll_wait(filp, &mdbg_proc->assert.rxwait, wait);
		if (mdbg_proc->assert.rcv_len > 0)
			mask |= POLLIN | POLLRDNORM;
	}

	if (strcmp(type, "loopcheck") == 0) {
		poll_wait(filp, &mdbg_proc->loopcheck.rxwait, wait);
#ifdef CONFIG_SC2342_INTEG
		WCN_INFO("loopcheck:power_state_changed:%d\n",
					wcn_get_module_status_changed());
		if (wcn_get_module_status_changed()) {
			wcn_set_module_status_changed(false);
			mask |= POLLIN | POLLRDNORM;
		}
#else
		if (marlin_get_module_status() == 1)
			mask |= POLLIN | POLLRDNORM;
#endif
	}

	return mask;
}

static const struct file_operations mdbg_proc_fops = {
	.open		= mdbg_proc_open,
	.release	= mdbg_proc_release,
	.read		= mdbg_proc_read,
	.write		= mdbg_proc_write,
	.poll		= mdbg_proc_poll,
};

int mdbg_memory_alloc(void)
{
	mdbg_proc->assert.buf =  kzalloc(MDBG_ASSERT_SIZE, GFP_KERNEL);
	if (!mdbg_proc->assert.buf)
		return -ENOMEM;

	mdbg_proc->loopcheck.buf =  kzalloc(MDBG_LOOPCHECK_SIZE, GFP_KERNEL);
	if (!mdbg_proc->loopcheck.buf) {
		kfree(mdbg_proc->assert.buf);
		return -ENOMEM;
	}
	mdbg_proc->at_cmd.buf =  kzalloc(MDBG_AT_CMD_SIZE, GFP_KERNEL);
	if (!mdbg_proc->at_cmd.buf) {
		kfree(mdbg_proc->assert.buf);
		kfree(mdbg_proc->loopcheck.buf);
		return -ENOMEM;
	}

	mdbg_proc->snap_shoot.buf =  kzalloc(MDBG_SNAP_SHOOT_SIZE, GFP_KERNEL);
	if (!mdbg_proc->snap_shoot.buf) {
		kfree(mdbg_proc->assert.buf);
		kfree(mdbg_proc->loopcheck.buf);
		kfree(mdbg_proc->at_cmd.buf);
		return -ENOMEM;
	}

	return 0;
}

struct mchn_ops_t mdbg_proc_ops[MDBG_ASSERT_RX_OPS + 1] = {
	{
		.channel = WCN_AT_TX,
		.inout = WCNBUS_TX,
		.pool_size = 5,
		.pop_link = mdbg_tx_cb,
	},
	{
		.channel = WCN_LOOPCHECK_RX,
		.inout = WCNBUS_RX,
		.pool_size = 1,
		.pop_link = mdbg_loopcheck_read,
	},
	{
		.channel = WCN_AT_RX,
		.inout = WCNBUS_RX,
		.pool_size = 1,
		.pop_link = mdbg_at_cmd_read,
	},
	{
		.channel = WCN_ASSERT_RX,
		.inout = WCNBUS_RX,
		.pool_size = 1,
		.pop_link = mdbg_assert_read,
	},
};

static void mdbg_fs_channel_destroy(void)
{
	int i;

	for (i = 0; i <= MDBG_ASSERT_RX_OPS; i++)
		sprdwcn_bus_chn_deinit(&mdbg_proc_ops[i]);
}

static void mdbg_fs_channel_init(void)
{
	int i;

	for (i = 0; i <= MDBG_ASSERT_RX_OPS; i++)
		sprdwcn_bus_chn_init(&mdbg_proc_ops[i]);
}

static  void mdbg_memory_free(void)
{
	kfree(mdbg_proc->snap_shoot.buf);
	mdbg_proc->snap_shoot.buf = NULL;

	kfree(mdbg_proc->assert.buf);
	mdbg_proc->assert.buf = NULL;

	kfree(mdbg_proc->loopcheck.buf);
	mdbg_proc->loopcheck.buf = NULL;

	kfree(mdbg_proc->at_cmd.buf);
	mdbg_proc->at_cmd.buf = NULL;
}

int proc_fs_init(void)
{
	mdbg_proc = kzalloc(sizeof(struct mdbg_proc_t), GFP_KERNEL);
	if (!mdbg_proc)
		return -ENOMEM;

	mdbg_proc->dir_name = "mdbg";
	mdbg_proc->procdir = proc_mkdir(mdbg_proc->dir_name, NULL);

	mdbg_proc->assert.name = "assert";
	mdbg_proc->assert.entry = proc_create_data(mdbg_proc->assert.name,
						S_IRUSR | S_IWUSR,
						mdbg_proc->procdir,
						&mdbg_proc_fops,
						&(mdbg_proc->assert));

	mdbg_proc->loopcheck.name = "loopcheck";
	mdbg_proc->loopcheck.entry = proc_create_data(mdbg_proc->loopcheck.name,
						S_IRUSR | S_IWUSR,
						mdbg_proc->procdir,
						&mdbg_proc_fops,
						&(mdbg_proc->loopcheck));

	mdbg_proc->at_cmd.name = "at_cmd";
	mdbg_proc->at_cmd.entry = proc_create_data(mdbg_proc->at_cmd.name,
						S_IRUSR | S_IWUSR,
						mdbg_proc->procdir,
						&mdbg_proc_fops,
						&(mdbg_proc->at_cmd));

	mdbg_proc->snap_shoot.name = "snap_shoot";
	mdbg_proc->snap_shoot.entry = proc_create_data(
						mdbg_proc->snap_shoot.name,
						S_IRUSR | S_IWUSR,
						mdbg_proc->procdir,
						&mdbg_snap_shoot_seq_fops,
						&(mdbg_proc->snap_shoot));

	mdbg_fs_channel_init();

	init_completion(&mdbg_proc->assert.completed);
	init_completion(&mdbg_proc->loopcheck.completed);
	init_completion(&mdbg_proc->at_cmd.completed);
	init_waitqueue_head(&mdbg_proc->assert.rxwait);
	init_waitqueue_head(&mdbg_proc->loopcheck.rxwait);

	if (mdbg_memory_alloc() < 0)
		return -ENOMEM;

	return 0;
}

void proc_fs_exit(void)
{
	mdbg_memory_free();
	mdbg_fs_channel_destroy();
	remove_proc_entry(mdbg_proc->snap_shoot.name, mdbg_proc->procdir);
	remove_proc_entry(mdbg_proc->assert.name, mdbg_proc->procdir);
	remove_proc_entry(mdbg_proc->loopcheck.name, mdbg_proc->procdir);
	remove_proc_entry(mdbg_proc->at_cmd.name, mdbg_proc->procdir);
	remove_proc_entry(mdbg_proc->dir_name, NULL);

	kfree(mdbg_proc);
	mdbg_proc = NULL;
}

int get_loopcheck_status(void)
{
	WCN_INFO("loopcheck fail_count %d\n", mdbg_proc->fail_count);
	return mdbg_proc->fail_count;
}

void wakeup_loopcheck_int(void)
{
	wake_up_interruptible(&mdbg_proc->loopcheck.rxwait);
}

void loopcheck_first_boot_clear(void)
{
	mdbg_proc->first_boot = false;
}
