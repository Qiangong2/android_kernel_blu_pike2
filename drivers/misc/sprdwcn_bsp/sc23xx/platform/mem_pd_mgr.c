/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 * Filename : slp_mgr.c
 * Abstract : This file is a implementation for  sleep manager
 *
 * Authors	: QI.SUN
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/marlin_platform.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/wakelock.h>
#include <soc/sprd/wcn_bus.h>

#include "mem_pd_mgr.h"
#include "../include/wcn_glb_reg.h"
#include "../sleep/sdio_int.h"

#define CP_NO_MEM_PD_TIMEROUT 50
#define CP_CREAT_THREAD_TIMEROUT 3000000000
/* time out in waiting wifi to come up */
#define MEM_SAVE_BIN_TIMEROUT 30000000
#define MEM_PD_WIFI_BEGIN_ADD 0X40568000
#define MEM_PD_WIFI_SIZE 0X7fA00	/* 32k*15 */
#define MEM_PD_BT_BEGIN_ADD 0X40528000
#define MEM_PD_BT_SIZE 0X39400
#define MEM_PD_UNIT_SIZE 0X8000	/* 32k */
#define SDIO_CP_BASE_ADD 0X40400000	/* 32k */
#define CP_MEM_OFFSET 0X00100000	/* 32k */
#define DRAM_ADD 0X40580000

struct mem_pd_t mem_pd;

/* bit_start FORCE SHUTDOWN IRAM [16...31]*32K=512K
 * and bit_start++ bit_cnt how many 32k
 */
int mem_pd_power_switch(int subsys, int val)
{
	int ret;
	unsigned int reg_val = 0;

	unsigned int wif_bt_mem_cfg;
	/* unsigned int mem_pd_power_delay; */
	MEM_PD_MGR_INFO("mem_pd_power_switch CHIP MODE!\n");
	switch (subsys) {
	case MARLIN_WIFI:
		if (val) {
			/* wifi iram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG1;
			ret = sprdwcn_bus_direct_read(
						wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO(" sdiohal_dt_read error !\n");
					return ret;
			}
			if (reg_val & (0xE0000000)) {
				/* val =1 ,powerdown */
				reg_val &= ~(0xE0000000);
				/* set bit_start ,mem power down */
				ret = sprdwcn_bus_direct_write(
				wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dt_write error !\n");
					return ret;
				}
			} else {
			}
			/* wifi dram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG3;
			ret = sprdwcn_bus_direct_read(
						wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO("sdiohal_dt_read error !\n");
				return ret;
			}
			if (reg_val & (0xF0000)) {
				/* val =1 ,powerdown */
				reg_val &= ~(0xF0000);
				/* set bit_start ,mem power down */
				ret = sprdwcn_bus_direct_write(
						wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dt_write error !\n");
					return ret;
				}
			}
		} else {
			/* wifi iram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG1;
			ret = sprdwcn_bus_direct_read(
						wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO("dt read error !\n");
				return ret;
			}
			if (reg_val & (0xE0000000)) {/* val =1 ,powerdown */
			} else {
				reg_val |= (0xE0000000);
				/* clear bit_start ,mem power on */
				ret = sprdwcn_bus_direct_write(
				wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dt write error !\n");
					return ret;
				}
			}
			/* wifi dram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG3;
			ret = sprdwcn_bus_direct_read(
						wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO(" sdio read error !\n");
				return ret;
			}
			if (reg_val&(0xF0000)) {/* val =1 ,powerdown */
			} else{
				reg_val |= (0xF0000);
				/* clear bit_start ,mem power on */
				ret = sprdwcn_bus_direct_write(
					wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dt write error !\n");
					return ret;
				}
			}
		}
	break;
	case MARLIN_BLUETOOTH:
		if (val) {
			/* bt iram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG1;
			ret = sprdwcn_bus_direct_read(
						wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO("dt read error !\n");
				return ret;
			}
			if (reg_val & (0x0FE00000)) {/* val =1 ,powerdown */
				reg_val &= ~(0x0FE00000);
				/* set bit_start ,mem power down */
				ret = sprdwcn_bus_direct_write(
					wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dt write error!\n");
					return ret;
				}
			}
		} else {
			/* wifi iram mem pd range */
			wif_bt_mem_cfg = REG_AON_APB_BTWF_MEM_CGG1;
			ret = sprdwcn_bus_direct_read(
					wif_bt_mem_cfg, &reg_val, 4);
			if (!(ret == 0)) {
				MEM_PD_MGR_INFO("sdiohal_dt_read error!\n");
				return ret;
			}
			if (reg_val&(0x0FE00000)) {/* val =1 ,powerdown */
			} else{
				reg_val |= (0x0FE00000);
				/* clear bit_start ,mem power on */
				ret = sprdwcn_bus_direct_write(
					wif_bt_mem_cfg, &reg_val, 4);
				if (!(ret == 0)) {
					MEM_PD_MGR_INFO("dtwrite error!\n");
					return ret;
				}
			}
		}
		break;
	default:
	break;
	}

	return 0;
}

int inform_cp_wifi_download(void)
{
	union AP_INT_CP0_REG mem_pd_reg_int_cp0 = {0};

	union AP_INT_CP0_REG reg_tem = {0};

	sprdwcn_bus_aon_readb(sdio_int.pub_int_en0, &(reg_tem.reg));
	MEM_PD_MGR_INFO("qi.sun reg sido pub-0x%x\n",
	reg_tem.reg);
	mem_pd_reg_int_cp0.bit.wifi_bin_download = 1;
	sprdwcn_bus_aon_writeb(sdio_int.ap_int_cp0, mem_pd_reg_int_cp0.reg);
	return 0;
}
int inform_cp_bt_download(void)
{
	union AP_INT_CP0_REG mem_pd_reg_int_cp0 = {0};

	union AP_INT_CP0_REG reg_tem = {0};

	sprdwcn_bus_aon_readb(sdio_int.pub_int_en0, &(reg_tem.reg));
	MEM_PD_MGR_INFO("qi.sun reg sido pub-0x%x\n",
	reg_tem.reg);
	mem_pd_reg_int_cp0.bit.bt_bin_download = 1;
	sprdwcn_bus_aon_writeb(sdio_int.ap_int_cp0, mem_pd_reg_int_cp0.reg);
	return 0;
}

static void wifi_cp_open(void)
{
	MEM_PD_MGR_INFO("qi.sun wifi_open int");
	complete(&(mem_pd.wifi_open_completion));
}
static void wifi_cp_close(void)
{
	complete(&(mem_pd.wifi_close_completion));
	MEM_PD_MGR_INFO("qi.sun wifi_thread_delete int");
}

static void bt_cp_open(void)
{
	MEM_PD_MGR_INFO("qi.sun bt_open int");
	complete(&(mem_pd.bt_open_completion));
}
static void bt_cp_close(void)
{
	complete(&(mem_pd.bt_close_completion));
	MEM_PD_MGR_INFO("qi.sun bt_thread_delete int");
}
static void save_bin_cp_ready(void)
{
	if (mem_pd.cp_mem_all_off == 0) {
		complete(&(mem_pd.save_bin_completion));
		MEM_PD_MGR_INFO("qi.sun mem_save_bin_ready int");
		mem_pd.cp_mem_all_off = 1;
		return;
	}
	mem_pd_power_switch(MARLIN_WIFI, FALSE);
	mem_pd_power_switch(MARLIN_BLUETOOTH, FALSE);
	MEM_PD_MGR_INFO("qi.sun clear all");
}
int mem_pd_pub_int_RegCb(void)
{
	sdio_pub_int_RegCb(WIFI_OPEN, (PUB_INT_ISR)wifi_cp_open);
	sdio_pub_int_RegCb(WIFI_CLOSE, (PUB_INT_ISR)wifi_cp_close);
	sdio_pub_int_RegCb(BT_OPEN, (PUB_INT_ISR)bt_cp_open);
	sdio_pub_int_RegCb(BT_CLOSE, (PUB_INT_ISR)bt_cp_close);
	sdio_pub_int_RegCb(MEM_SAVE_BIN, (PUB_INT_ISR)save_bin_cp_ready);
	return 0;
}

int sdio_read_mem_from_cp(void)
{
	int err = 0;
#ifdef MEM_PD_PASS
		MEM_PD_MGR_INFO("save mem bin do nothing, return\n");
		return 0;
#endif
	err = sprdwcn_bus_direct_read(MEM_PD_WIFI_BEGIN_ADD, mem_pd.wifi_mem,
		MEM_PD_WIFI_SIZE);
	if (err < 0) {
		pr_err("%s wifi save mem bin error:%d\n", __func__, err);
		return -1;
	}
	MEM_PD_MGR_INFO("qi.sun sdiohal_dt_read ok\n");
	err = sprdwcn_bus_direct_read(MEM_PD_BT_BEGIN_ADD, mem_pd.bt_mem,
		MEM_PD_BT_SIZE);
	if (err < 0) {
		pr_err("%s bt save mem bin error:%d\n", __func__, err);
		return -1;
	}
	return 0;
}
static int sdio_ap_int_cp_save_cp_mem(void)
{
	union AP_INT_CP0_REG mem_pd_reg_int_cp0 = {0};

	mem_pd_reg_int_cp0.bit.save_cp_mem = 1;

	sprdwcn_bus_aon_writeb(sdio_int.ap_int_cp0, mem_pd_reg_int_cp0.reg);
	return 0;
}
int mem_pd_save_bin(void)
{
#ifdef MEM_PD_PASS
	return 0;
#endif
	/* mutex_lock(&(mem_pd.mem_pd_lock)); */
	MEM_PD_MGR_INFO("%s entry", __func__);
	if (wait_for_completion_timeout(
		&(mem_pd.save_bin_completion),
		msecs_to_jiffies(CP_NO_MEM_PD_TIMEROUT)) <= 0) {
		MEM_PD_MGR_INFO("cp version is wcn_trunk ,cp_version =1");
		/* mutex_unlock(&(mem_pd.mem_pd_lock)); */
		mem_pd.cp_version = 1;
		return 0;
	}
		sdio_read_mem_from_cp();/* save to char[] */
		sdio_ap_int_cp_save_cp_mem();
		/* save done , AP inform cp by INT. */
	/* mutex_unlock(&(mem_pd.mem_pd_lock)); */
	return 0;
}

int ap_int_cp_wifi_bin_done(int subsys)
{
	switch (subsys) {
	case MARLIN_WIFI:
		inform_cp_wifi_download();
	break;
	case MARLIN_BLUETOOTH:
		inform_cp_bt_download();
	break;
	default:
	return -1;
	}
	return 0;
}

int test_mem_clrear(int subsys)
{
int err;
#ifdef MEM_PD_PASS
	MEM_PD_MGR_INFO("mem_pd_download_mem_bin do nothing\n");
	return 0;
#endif
	switch (subsys) {
	case MARLIN_WIFI:
		err = sprdwcn_bus_direct_write(MEM_PD_WIFI_BEGIN_ADD,
			(mem_pd.wifi_clear), MEM_PD_WIFI_SIZE);
		if (err < 0) {
			pr_err("%s wifi down bin error:%d\n", __func__, err);
			return -1;
		}
	break;
	case MARLIN_BLUETOOTH:
		err = sprdwcn_bus_direct_write(MEM_PD_BT_BEGIN_ADD,
			(mem_pd.bt_clear), MEM_PD_BT_SIZE);
		if (err < 0) {
			pr_err("%s bt down mem bin error:%d\n", __func__, err);
			return -1;
		}
	break;
	default:
	return -1;
	}
	return 0;
}

int mem_pd_download_mem_bin(int subsys)
{
	union PUB_INT_EN0_REG reg_tem = {0};
	int err;
	unsigned int addr = 0;
	char *mem;
	unsigned int len = 0;

	sprdwcn_bus_aon_readb(sdio_int.pub_int_en0, &(reg_tem.reg));
	MEM_PD_MGR_INFO("qi.sun reg sido pub-0x%x\n",
		reg_tem.reg);
	/* MEM_PD_MGR_INFO("mem_pd_download_mem_bin do nothing\n"); */
	/* return 0; */
	MEM_PD_MGR_INFO("mem_pd_download_mem_bin do\n");
	switch (subsys) {
	case MARLIN_WIFI:
		addr = MEM_PD_WIFI_BEGIN_ADD;
		mem = mem_pd.wifi_mem;
		len = MEM_PD_WIFI_SIZE;
	break;
	case MARLIN_BLUETOOTH:
		addr = MEM_PD_BT_BEGIN_ADD;
		mem = mem_pd.bt_mem;
		len = MEM_PD_BT_SIZE;
	break;
	default:
		return -1;
	}
	err = sprdwcn_bus_direct_write(addr, mem, len);
	if (err < 0) {
		pr_err("%s download mem bin error:%d\n", __func__, err);
		return -1;
	}
	return 0;
}
int mem_pd_mgr(int subsys, int val)
{
#ifdef MEM_PD_PASS
	return 0;
#endif
	if (mem_pd.after_init != 0x5E)
		return 0;
	if (mem_pd.cp_version)
		return 0;
	switch (subsys) {
	case MARLIN_WIFI:
		MEM_PD_MGR_INFO("qi.sun sentrt mem_pd_mgr cnt");
		mutex_lock(&(mem_pd.mem_pd_lock));
		MEM_PD_MGR_INFO("marlin wifi state:%d, subsys %d power %d\n",
				mem_pd.wifi_state, subsys, val);
		if (val) {
			if (mem_pd.wifi_state != THREAD_DELETE) {
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return 0;
			}
			mem_pd_power_switch(subsys, val);
			mem_pd_download_mem_bin(subsys);
			ap_int_cp_wifi_bin_done(subsys);
			if (wait_for_completion_timeout(
				&(mem_pd.wifi_open_completion),
			msecs_to_jiffies(CP_CREAT_THREAD_TIMEROUT))
			<= 0) {
				MEM_PD_MGR_INFO("wifi creat fail");
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return -1;
			}
			mem_pd.wifi_state = THREAD_CREATE;
			MEM_PD_MGR_INFO("cp wifi creat thread ok");
		} else {
			if (mem_pd.wifi_state != THREAD_CREATE) {
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return 0;
			}
			sprdwcn_bus_aon_writeb(0x1b0, 0x10);
			/* instead of cp wifi delet thread, inform sdio. */
			if (wait_for_completion_timeout(
				&(mem_pd.wifi_close_completion),
			msecs_to_jiffies(CP_CREAT_THREAD_TIMEROUT))
				<= 0) {
				MEM_PD_MGR_INFO("wifi delete fail");
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return -1;
			}
			mem_pd_power_switch(subsys, val);
			mem_pd.wifi_state = THREAD_DELETE;
			MEM_PD_MGR_INFO("cp wifi delete thread ok");
		}
		mutex_unlock(&(mem_pd.mem_pd_lock));
		break;
	case MARLIN_BLUETOOTH:
		MEM_PD_MGR_INFO("qi.sun sentrt mem_pd_mgr cnt");
		mutex_lock(&(mem_pd.mem_pd_lock));
		MEM_PD_MGR_INFO("marlin bt state:%d, subsys %d power %d\n",
				mem_pd.bt_state, subsys, val);
		if (val) {
			if (mem_pd.bt_state != THREAD_DELETE) {
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return 0;
			}
			mem_pd_power_switch(subsys, val);
			mem_pd_download_mem_bin(subsys);
			ap_int_cp_wifi_bin_done(subsys);
		if (wait_for_completion_timeout(
			&(mem_pd.bt_open_completion),
			msecs_to_jiffies(CP_CREAT_THREAD_TIMEROUT)) <= 0) {
			MEM_PD_MGR_INFO("cp bt creat thread fail");
			mutex_unlock(&(mem_pd.mem_pd_lock));
			return -1;
			}
			mem_pd.bt_state = THREAD_CREATE;
			MEM_PD_MGR_INFO("cp bt creat thread ok");
		} else {
			if (mem_pd.bt_state != THREAD_CREATE) {
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return 0;
			}
			if (wait_for_completion_timeout(
				&(mem_pd.bt_close_completion),
			msecs_to_jiffies(CP_CREAT_THREAD_TIMEROUT))
				<= 0) {
				MEM_PD_MGR_INFO("bt delete fail");
				mutex_unlock(&(mem_pd.mem_pd_lock));
				return -1;
			}
			mem_pd_power_switch(subsys, val);
			mem_pd.bt_state = THREAD_DELETE;
			MEM_PD_MGR_INFO("cp bt delete thread ok");
		}
		mutex_unlock(&(mem_pd.mem_pd_lock));
		break;
	default:
		return 0;
	}
	return 0;
}

int mem_pd_init(void)
{
#ifdef MEM_PD_PASS
	return 0;
#endif
	MEM_PD_MGR_INFO("%s enter\n", __func__);
	mutex_init(&(mem_pd.mem_pd_lock));
	init_completion(&(mem_pd.wifi_open_completion));
	init_completion(&(mem_pd.wifi_close_completion));
	init_completion(&(mem_pd.bt_open_completion));
	init_completion(&(mem_pd.bt_close_completion));
	init_completion(&(mem_pd.save_bin_completion));
	mem_pd.wifi_mem = kmalloc(MEM_PD_WIFI_SIZE, GFP_KERNEL);
	if (!mem_pd.wifi_mem) {
		MEM_PD_MGR_INFO("mem pd wifi save buff malloc Failed.");
		return -1;
	}
	mem_pd.bt_mem = kmalloc(MEM_PD_BT_SIZE, GFP_KERNEL);
	if (!mem_pd.bt_mem) {
		MEM_PD_MGR_INFO("mem pd bt save buff malloc Failed.");
		return -1;
	}
	mem_pd.wifi_clear = kmalloc(MEM_PD_WIFI_SIZE, GFP_KERNEL);
	if (!mem_pd.wifi_clear) {
		MEM_PD_MGR_INFO("mem pd clear buff malloc Failed.");
		return -1;
	}
	mem_pd.bt_clear = kmalloc(MEM_PD_BT_SIZE, GFP_KERNEL);
	if (!mem_pd.bt_clear) {
		MEM_PD_MGR_INFO("mem pd clear buff malloc Failed.");
		return -1;
	}
	mem_pd_pub_int_RegCb();
	mem_pd.after_init = 0x5E;
	/* mem_pd.wifi_state = 0; */
	/* mem_pd.bt_state = 0; */
	/* mem_pd.cp_version = 0; */
	/* mem_pd.cp_mem_all_off = 0; */
	memset(mem_pd.wifi_clear, 0x0,
		MEM_PD_WIFI_SIZE);
	memset(mem_pd.bt_clear, 0x0,
		MEM_PD_BT_SIZE);
	MEM_PD_MGR_INFO("%s ok!\n", __func__);
	return 0;
}

int mem_pd_exit(void)
{
#ifdef MEM_PD_PASS
	return 0;
#endif
	MEM_PD_MGR_INFO("%s enter\n", __func__);
	/* atomic_set(&(slp_mgr.cp2_state), STAY_SLPING); */
	/* sleep_active_modules = 0; */
	/* wake_cnt = 0; */
	mutex_destroy(&(mem_pd.mem_pd_lock));
	/* mutex_destroy(&(slp_mgr.wakeup_lock)); */
	kfree(mem_pd.wifi_mem);
	mem_pd.wifi_mem = NULL;
	kfree(mem_pd.bt_mem);
	mem_pd.bt_mem = NULL;
	kfree(mem_pd.wifi_clear);
	kfree(mem_pd.bt_clear);
	mem_pd.wifi_clear = NULL;
	mem_pd.bt_clear = NULL;
	MEM_PD_MGR_INFO("%s ok!\n", __func__);
	return 0;
}

