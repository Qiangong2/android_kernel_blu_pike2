/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/marlin_platform.h>
#include <linux/printk.h>
#include <linux/sdiom_rx_api.h>
#include <linux/sdiom_tx_api.h>
#include <linux/sipc.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/wait.h>
#include <soc/sprd/wcn_bus.h>
#ifdef CONFIG_INTEGRATED_MARLIN2_GE2
#include <linux/wcn_integrate_platform.h>
#endif
#ifndef CONFIG_INTEGRATED_MARLIN2_GE2
#include "gnss_common.h"
#endif
#include "gnss_dump.h"
#include "mdbg_type.h"
#ifndef CONFIG_INTEGRATED_MARLIN2_GE2
#include "../../include/wcn_glb_reg.h"
#endif

#define GNSSDUMP_INFO(format, arg...) pr_info("gnss_dump: " format, ## arg)
#define GNSSDUMP_ERR(format, arg...) pr_info("gnss_dump: " format, ## arg)

static struct file *gnss_dump_file;
static	loff_t pos;
#define GNSS_MEMDUMP_PATH			"/data/gnss/gnssdump.mem"

#ifndef CONFIG_INTEGRATED_MARLIN2_GE2
struct gnss_mem_dump {
	uint address;
	uint length;
};

/* dump cp firmware firstly, wait for next adding */
static struct gnss_mem_dump gnss_marlin3_dump[] = {
	{GNSS_CP_START_ADDR, GNSS_FIRMWARE_MAX_SIZE}, /* gnss firmware code */
};
#endif

static int gnss_creat_gnss_dump_file(void)
{
	gnss_dump_file = filp_open(GNSS_MEMDUMP_PATH,
		O_RDWR | O_CREAT | O_TRUNC, 0666);
	GNSSDUMP_INFO("gnss_creat_gnss_dump_file entry\n");
	if (IS_ERR(gnss_dump_file)) {
		GNSSDUMP_ERR("%s error is %p\n",
			__func__, gnss_dump_file);
		return -1;
	}
	if (sys_chmod(GNSS_MEMDUMP_PATH, 0666) != 0)
		GNSSDUMP_ERR("%s chmod	error\n", __func__);

	return 0;
}

#ifdef CONFIG_INTEGRATED_MARLIN2_GE2
static void gnss_write_data_to_phy_addr(u32 phy_addr,
					      void *src_data, u32 size)
{
	void *virt_addr;

	GNSSDUMP_INFO("gnss_write_data_to_phy_addr entry\n");
	virt_addr = shmem_ram_vmap_nocache(phy_addr, size);
	if (virt_addr) {
		memcpy(virt_addr, src_data, size);
		shmem_ram_unmap(virt_addr);
	} else
		GNSSDUMP_ERR("%s shmem_ram_vmap_nocache fail\n", __func__);
}

static void gnss_read_data_from_phy_addr(u32 phy_addr,
					  void *tar_data, u32 size)
{
	void *virt_addr;

	GNSSDUMP_INFO("gnss_read_data_from_phy_addr\n");
	virt_addr = shmem_ram_vmap_nocache(phy_addr, size);
	if (virt_addr) {
		memcpy(tar_data, virt_addr, size);
		shmem_ram_unmap(virt_addr);
	} else
		GNSSDUMP_ERR("%s shmem_ram_vmap_nocache fail\n", __func__);
}

static void gnss_hold_cpu(void)
{
	struct regmap *regmap;
	u32 value;
	u32 base_addr;
	int i = 0;

	GNSSDUMP_INFO("gnss_hold_cpu entry\n");
	regmap = wcn_get_gnss_regmap(REGMAP_ANLG_WRAP_WCN);
	wcn_regmap_read(regmap, 0X20, &value);
	value |= 1 << 2;
	wcn_regmap_raw_write_bit(regmap, 0X20, value);

	wcn_regmap_read(regmap, 0X24, &value);
	value |= 1 << 3;
	wcn_regmap_raw_write_bit(regmap, 0X24, value);

	value = GNSS_CACHE_FLAG_VALUE;
	base_addr = wcn_get_gnss_base_addr();
	gnss_write_data_to_phy_addr(base_addr + GNSS_CACHE_FLAG_ADDR,
		(void *)&value, 4);
	while (i < 3) {
		gnss_read_data_from_phy_addr(base_addr + GNSS_CACHE_FLAG_ADDR,
			(void *)&value, 4);
		if (value == GNSS_CACHE_END_VALUE)
			break;
		i++;
		msleep(50);
	}
	if (value != GNSS_CACHE_END_VALUE)
		GNSSDUMP_ERR("%s gnss cache failed value %d\n", __func__,
			value);

	value = 0;
	wcn_regmap_raw_write_bit(regmap, 0X20, value);
	wcn_regmap_raw_write_bit(regmap, 0X24, value);
	msleep(200);
}

static int gnss_dump_ap_register_data(u32 addr, u32 len)
{
	u32 value = 0;
	u8 *ptr = NULL;
	long int ret;
	void  *apreg_buffer = NULL;
	mm_segment_t fs;

	GNSSDUMP_INFO("%s ap reg data len %d\n", __func__, len);
	if (IS_ERR(gnss_dump_file)) {
		gnss_dump_file = filp_open(GNSS_MEMDUMP_PATH,
		O_RDWR | O_CREAT | O_APPEND, 0666);
		if (IS_ERR(gnss_dump_file)) {
			GNSSDUMP_ERR("%s  open file mem error\n", __func__);
			return PTR_ERR(gnss_dump_file);
		}
	}

	apreg_buffer = vmalloc(len);
	if (!apreg_buffer) {
		GNSSDUMP_ERR("%s vmalloc apreg_buffer error\n", __func__);
		return -ENOMEM;
	}

	ptr = (u8 *)&value;
	gnss_read_data_from_phy_addr(addr, &value, len);
	memset(apreg_buffer, 0, len);
	memcpy(apreg_buffer, ptr, len);
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = gnss_dump_file->f_pos;
	ret = vfs_write(gnss_dump_file, apreg_buffer, len, &pos);
	gnss_dump_file->f_pos = pos;
	vfree(apreg_buffer);
	set_fs(fs);
	if (ret != len)
		return -1;

	return ret;
}

static int gnss_dump_cp_register_data(u32 addr, u32 len)
{
	struct regmap *regmap;
	u32 i;
	u8 *buf = NULL;
	u8 *ptr = NULL;
	long int ret;
	void  *iram_buffer = NULL;
	mm_segment_t fs;

	GNSSDUMP_INFO(" start dump cp register!addr:%x,len:%d\n", addr, len);
	buf = kzalloc(len, GFP_KERNEL);
	if (!buf) {
		GNSSDUMP_ERR("%s kzalloc buf error\n", __func__);
		return -ENOMEM;
	}

	if (IS_ERR(gnss_dump_file)) {
		gnss_dump_file = filp_open(GNSS_MEMDUMP_PATH,
			O_RDWR | O_CREAT | O_APPEND, 0666);
		if (IS_ERR(gnss_dump_file)) {
			GNSSDUMP_ERR("%s  open file mem error\n", __func__);
			return PTR_ERR(gnss_dump_file);
		}
	}

	iram_buffer = vmalloc(len);
	if (!iram_buffer) {
		GNSSDUMP_ERR("%s vmalloc iram_buffer error\n", __func__);
		return -ENOMEM;
	}

	regmap = wcn_get_gnss_regmap(REGMAP_ANLG_WRAP_WCN);
	wcn_regmap_raw_write_bit(regmap, 0XFF4, addr);
	for (i = 0; i < len / 4; i++) {
		ptr = buf + i * 4;
		wcn_regmap_read(regmap, 0XFFC, (u32 *)ptr);
	}

	memset(iram_buffer, 0, len);
	memcpy(iram_buffer, buf, len);
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = gnss_dump_file->f_pos;
	ret = vfs_write(gnss_dump_file, iram_buffer, len, &pos);
	gnss_dump_file->f_pos = pos;
	kfree(buf);
	vfree(iram_buffer);
	set_fs(fs);
	if (ret != len) {
		GNSSDUMP_ERR("gnss_dump_cp_register_data failed  size is %ld\n",
			ret);
		return -1;
	}
	GNSSDUMP_INFO("gnss_dump_cp_register_data finish  size is  %ld\n",
		ret);

	return ret;
}

static void gnss_dump_ap_register(void)
{
	gnss_dump_ap_register_data(DUMP_REG_PMU_SLEEP_CTRL, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_PMU_SLEEP_STATUS, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_SYS_EN_STATUS, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_WCN_SYS_CFG, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_GNSS_CLK_STATUS, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_WCN_PD_STATUS, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_BT_WIFI_PD, sizeof(u32));
	gnss_dump_ap_register_data(DUMP_REG_GNSS_STATUS, sizeof(u32));
}

static void gnss_dump_cp_register(void)
{
	u32 count;

	count = gnss_dump_cp_register_data(DUMP_REG_GNSS_APB_CTRL_ADDR,
			DUMP_REG_GNSS_APB_CTRL_LEN);
	GNSSDUMP_INFO("gnss dump gnss_apb_ctrl_reg %u ok!\n", count);

	count = gnss_dump_cp_register_data(DUMP_REG_GNSS_AHB_CTRL_ADDR,
			DUMP_REG_GNSS_AHB_CTRL_LEN);
	GNSSDUMP_INFO("gnss dump manu_clk_ctrl_reg %u ok!\n", count);

	count = gnss_dump_cp_register_data(DUMP_COM_SYS_CTRL_ADDR,
			DUMP_COM_SYS_CTRL_LEN);
	GNSSDUMP_INFO("gnss dump com_sys_ctrl_reg %u ok!\n", count);

	count = gnss_dump_cp_register_data(DUMP_WCN_CP_CLK_CORE_ADDR,
			DUMP_WCN_CP_CLK_LEN);
	GNSSDUMP_INFO("gnss dump manu_clk_ctrl_reg %u ok!\n", count);
}

static void gnss_dump_register(void)
{
	gnss_dump_ap_register();
	gnss_dump_cp_register();
	GNSSDUMP_INFO("gnss dump register ok!\n");
}

static void gnss_dump_iram(void)
{
	u32 count;

	count = gnss_dump_cp_register_data(DUMP_IRAM_START_ADDR,
			GNSS_CP_IRAM_DATA_NUM * 4);
	GNSSDUMP_INFO("gnss dump iram %u ok!\n", count);
}

static int gnss_dump_share_memory(u32 len)
{
	void *virt_addr;
	u32 base_addr;
	long int ret;
	mm_segment_t fs;
	void  *ddr_buffer = NULL;

	if (len == 0)
		return -1;
	GNSSDUMP_INFO("gnss_dump_share_memory\n");
	fs = get_fs();
	set_fs(KERNEL_DS);
	base_addr = wcn_get_gnss_base_addr();
	virt_addr = shmem_ram_vmap_nocache(base_addr, len);
	if (!virt_addr) {
		GNSSDUMP_ERR(" %s shmem_ram_vmap_nocache fail\n", __func__);
		return -1;
	}

	if (IS_ERR(gnss_dump_file)) {
		gnss_dump_file = filp_open(GNSS_MEMDUMP_PATH,
			O_RDWR | O_CREAT | O_APPEND, 0666);
		if (IS_ERR(gnss_dump_file)) {
			GNSSDUMP_ERR("%s open file mem error\n", __func__);
			return PTR_ERR(gnss_dump_file);
		}
	}

	ddr_buffer = vmalloc(len);
	if (!ddr_buffer) {
		GNSSDUMP_ERR(" %s vmalloc ddr_buffer fail\n", __func__);
		return -1;
	}
	memset(ddr_buffer, 0, len);
	memcpy(ddr_buffer, virt_addr, len);
	pos = gnss_dump_file->f_pos;
	ret = vfs_write(gnss_dump_file, ddr_buffer, len, &pos);
	gnss_dump_file->f_pos = pos;
	shmem_ram_unmap(virt_addr);
	set_fs(fs);
	vfree(ddr_buffer);
	if (ret != len) {
		GNSSDUMP_ERR("%s dump ddr error,data len is %ld\n", __func__,
			ret);
		return -1;
	}

	GNSSDUMP_INFO("gnss dump share memory  size = %ld\n", ret);

	return 0;
}

static int gnss_integrated_dump_mem(void)
{
	int ret = 0;

	GNSSDUMP_INFO("gnss_dump_mem entry\n");
	gnss_hold_cpu();
	ret = gnss_creat_gnss_dump_file();
	if (ret == -1) {
		GNSSDUMP_ERR("%s create mem dump file  error\n", __func__);
		return -1;
	}
	ret = gnss_dump_share_memory(GNSS_SHARE_MEMORY_SIZE);
	gnss_dump_iram();
	gnss_dump_register();
	if (gnss_dump_file != NULL)
		filp_close(gnss_dump_file, NULL);

	return ret;
}

#else
static int gnss_ext_hold_cpu(void)
{
	uint temp = 0;
	int ret = 0;

	GNSSDUMP_INFO("%s entry\n", __func__);
	temp = BIT_GNSS_APB_MCU_AP_RST_SOFT;
	ret = sprdwcn_bus_reg_write(REG_GNSS_APB_MCU_AP_RST + GNSS_SET_OFFSET,
		&temp, 4);
	if (ret < 0) {
		WCN_ERR("%s marlin3_gnss write reset reg error:%d\n",
			__func__, ret);
		return ret;
	}
	return ret;
}

static int gnss_ext_dump_data(unsigned int start_addr, int len)
{
	u8 *buf = NULL;
	int count = 0, ret = 0, trans_size;
	mm_segment_t fs;

	GNSSDUMP_INFO("%s, addr:%x,len:%d\n", __func__, start_addr, len);
	buf = kzalloc(DUMP_PACKET_SIZE, GFP_KERNEL);
	if (!buf) {
		GNSSDUMP_ERR("%s kzalloc buf error\n", __func__);
		return -ENOMEM;
	}

	if (IS_ERR(gnss_dump_file)) {
		gnss_dump_file = filp_open(GNSS_MEMDUMP_PATH,
			O_RDWR | O_CREAT | O_APPEND, 0666);
		if (IS_ERR(gnss_dump_file)) {
			GNSSDUMP_ERR("%s  open file mem error\n", __func__);
			kfree(buf);
			return PTR_ERR(gnss_dump_file);
		}
	}
	fs = get_fs();
	set_fs(KERNEL_DS);
	count = 0;
	while (count < len) {
		trans_size = (len - count) > DUMP_PACKET_SIZE ?
			DUMP_PACKET_SIZE : (len - count);
		ret = sprdwcn_bus_direct_read(start_addr + count, buf,
					      trans_size);
		if (ret < 0) {
			WCN_ERR("%s dump memory error:%d\n", __func__, ret);
			break;
		}
		pos = gnss_dump_file->f_pos;
		ret = vfs_write(gnss_dump_file, buf, trans_size, &pos);
		gnss_dump_file->f_pos = pos;

		count += trans_size;
	}

	kfree(buf);
	set_fs(fs);
	if (ret != len) {
		GNSSDUMP_ERR("%s failed size is %d\n", __func__, count);
		return -1;
	}
	GNSSDUMP_INFO("%s finish size is %d\n", __func__, count);
	return ret;
}

static int gnss_ext_dump_mem(void)
{
	int ret = 0;
	int i = 0;

	GNSSDUMP_INFO("%s entry\n", __func__);
	gnss_ext_hold_cpu();
	ret = gnss_creat_gnss_dump_file();
	if (ret == -1) {
		GNSSDUMP_ERR("%s create file error\n", __func__);
		return -1;
	}
	for (i = 0;
		i < ARRAY_SIZE(gnss_marlin3_dump);
		i++)
		if (gnss_ext_dump_data(gnss_marlin3_dump[i].address,
			gnss_marlin3_dump[i].length)) {
			GNSSDUMP_ERR("%s dumpdata i %d error\n", __func__, i);
			break;
		}

	if (gnss_dump_file != NULL)
		filp_close(gnss_dump_file, NULL);

	return ret;
}
#endif

int gnss_dump_mem(void)
{
#ifdef CONFIG_INTEGRATED_MARLIN2_GE2
	return gnss_integrated_dump_mem();
#else
	return gnss_ext_dump_mem();
#endif
}
