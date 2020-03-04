/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "linux/gnss.h"

#include "wcn_glb.h"
#include "wcn_glb_reg.h"
#include "wcn_procfs.h"

static struct mutex marlin_lock;
static struct wifi_calibration wifi_data;

/* efuse data */
static const u32
s_wifi_efuse_id[WCN_PLATFORM_TYPE][WIFI_EFUSE_BLOCK_COUNT] = {
	{41, 42, 43},	/* SharkLE */
	{38, 39, 40},	/* PIKE2 */
	{41, 42, 43},	/* SharkL3 */
};
static const u32 s_wifi_efuse_default_value[WIFI_EFUSE_BLOCK_COUNT] = {
0x11111111, 0x22222222, 0x33333333};	/* Until now, the value is error */

static char gnss_firmware_parent_path[FIRMWARE_FILEPATHNAME_LENGTH_MAX];
static char firmware_file_name[FIRMWARE_FILEPATHNAME_LENGTH_MAX];
static const u32 s_gnss_efuse_id[GNSS_EFUSE_BLOCK_COUNT] = {40, 42, 43};

void wcn_boot_init(void)
{
	mutex_init(&marlin_lock);
}

static int wcn_get_firmware_path(char *firmwarename, char *firmware_path)
{
	if (firmwarename == NULL || firmware_path == NULL)
		return -EINVAL;

	memset(firmware_path, 0, FIRMWARE_FILEPATHNAME_LENGTH_MAX);
	if (strcmp(firmwarename, WCN_MARLIN_DEV_NAME) == 0) {
		if (parse_firmware_path(firmware_path))
			return -EINVAL;
	} else if (strcmp(firmwarename, WCN_GNSS_DEV_NAME) == 0) {
		int folder_path_length = 0;
		/*
		 * GNSS firmware path is the same as BTWF
		 * But the function parse_firmware_path return path
		 * includes filename of wcnmodem
		 */
		if (parse_firmware_path(firmware_path))
			return -EINVAL;
		folder_path_length = strlen(firmware_path)
			-strlen(WCN_BTWF_FILENAME);
		*(firmware_path + folder_path_length) = 0;
		strcpy(gnss_firmware_parent_path, firmware_path);

	} else
		return -EINVAL;

	pr_info("%s:wcn_dev->firmware_path:%s\n",
		__func__,
		firmware_path);

	return 0;
}

/* only wifi need it */
static void marlin_write_cali_data(void)
{
	phys_addr_t phy_addr;
	u32 cali_flag;

	/* get cali para from RF */
	get_connectivity_config_param(&wifi_data.config_data);
	get_connectivity_cali_param(&wifi_data.cali_data);

	/* copy calibration file data to target ddr address */
	phy_addr = s_wcn_device.btwf_device->base_addr +
		   (phys_addr_t)&s_wssm_phy_offset_p->wifi.calibration_data;
	wcn_write_data_to_phy_addr(phy_addr, &wifi_data, sizeof(wifi_data));

	/* notify CP to cali */
	cali_flag = WIFI_CALIBRATION_FLAG_VALUE;
	phy_addr = s_wcn_device.btwf_device->base_addr +
		   (phys_addr_t)&s_wssm_phy_offset_p->wifi.calibration_flag;
	wcn_write_data_to_phy_addr(phy_addr, &cali_flag, sizeof(cali_flag));

	pr_info("%s finish\n", __func__);
}

/* only wifi need it: AP save the cali data to ini file */
static void marlin_save_cali_data(void)
{
	phys_addr_t phy_addr;

	if (s_wcn_device.btwf_device) {
		memset(&wifi_data.cali_data, 0x0,
					sizeof(struct wifi_cali_t));
		/* copy calibration file data to target ddr address */
		phy_addr = s_wcn_device.btwf_device->base_addr +
			   (phys_addr_t)&s_wssm_phy_offset_p->
			   wifi.calibration_data +
			   sizeof(struct wifi_config_t);
		wcn_read_data_from_phy_addr(phy_addr, &wifi_data.cali_data,
					    sizeof(struct wifi_cali_t));
		dump_cali_file(&wifi_data.cali_data);
		pr_info("%s finish\n", __func__);
	}
}

/* only wifi need it */
static void marlin_write_efuse_data(void)
{
	phys_addr_t phy_addr;
	u32 iloop = 0;
	u32 tmp_value[WIFI_EFUSE_BLOCK_COUNT];
	u32 chip_type;

	chip_type = wcn_platform_chip_type();
	/* get data from Efuse */
	memset(&tmp_value, 0, sizeof(tmp_value[0])*WIFI_EFUSE_BLOCK_COUNT);
	for (iloop = 0; iloop < WIFI_EFUSE_BLOCK_COUNT; iloop++) {
		tmp_value[iloop] =
		sprd_efuse_double_read(
				s_wifi_efuse_id[chip_type][iloop], true);
		if (tmp_value[iloop] == 0)
			tmp_value[iloop] = s_wifi_efuse_default_value[iloop];
	}

	for (iloop = 0; iloop < WIFI_EFUSE_BLOCK_COUNT; iloop++)
		pr_info("%s s_wifi_efuse_id[%d][%d]=%d\n", __func__,
				chip_type,
				iloop,
				s_wifi_efuse_id[chip_type][iloop]);
	/* copy efuse data to target ddr address */
	phy_addr = s_wcn_device.btwf_device->base_addr +
		   (phys_addr_t)&s_wssm_phy_offset_p->wifi.efuse[0];
	wcn_write_data_to_phy_addr(phy_addr, &tmp_value,
				sizeof(tmp_value[0])*WIFI_EFUSE_BLOCK_COUNT);

	pr_info("%s finish\n", __func__);
}

/* used for provide efuse data to gnss */
static void gnss_write_efuse_data(void)
{
	phys_addr_t phy_addr;
	u32 iloop = 0;
	u32 tmp_value[GNSS_EFUSE_BLOCK_COUNT];

	/* get data from Efuse */
	memset(&tmp_value, 0, sizeof(tmp_value[0]) * GNSS_EFUSE_BLOCK_COUNT);
	for (iloop = 0; iloop < GNSS_EFUSE_BLOCK_COUNT; iloop++) {
		tmp_value[iloop] =
		sprd_efuse_double_read(s_gnss_efuse_id[iloop], true);
	}

	/* copy efuse data to target ddr address */
	phy_addr = s_wcn_device.gnss_device->base_addr +
				   GNSS_EFUSE_DATA_OFFSET;
	wcn_write_data_to_phy_addr(phy_addr, &tmp_value,
				sizeof(tmp_value[0]) * GNSS_EFUSE_BLOCK_COUNT);

	pr_info("%s finish\n", __func__);
}

/* used for distinguish Pike2 or sharkle */
static void gnss_write_version_data(void)
{
	phys_addr_t phy_addr;
	u32 tmp_aon_id[2];

	tmp_aon_id[0] = g_platform_chip_id.aon_chip_id0;
	tmp_aon_id[1] = g_platform_chip_id.aon_chip_id1;
	phy_addr = wcn_get_gnss_base_addr() +
				   GNSS_REC_AON_CHIPID_OFFSET;
	wcn_write_data_to_phy_addr(phy_addr, &tmp_aon_id,
				GNSS_REC_AON_CHIPID_SIZE);

	pr_info("%s finish\n", __func__);
}

static char *wcn_load_firmware_data(struct wcn_device *wcn_dev)
{
	int read_len, size, i;
	char *data = NULL;
	char *wcn_image_buffer;
	struct file *file;
	unsigned int imag_size;
	bool is_marlin;

	pr_info("%s entry\n", __func__);

	if (wcn_dev == NULL)
		return NULL;
	if (strlen(wcn_dev->firmware_path) == 0) {
		/* get firmware path */
		if (wcn_get_firmware_path(wcn_dev->name,
				&wcn_dev->firmware_path[0]) < 0) {
			pr_err("wcn_get_firmware path Failed!\n");
			return NULL;
		}
		pr_info("%s firmware path=%s\n", __func__,
				&wcn_dev->firmware_path[0]);
	}
	is_marlin = strcmp(wcn_dev->name, WCN_MARLIN_DEV_NAME) == 0 ?
				true : false;
	if (!is_marlin) {
		strcpy(&wcn_dev->firmware_path[0],
					gnss_firmware_parent_path);
		strcat(&wcn_dev->firmware_path[0],
					&wcn_dev->firmware_path_ext[0]);
		pr_info("gnss path=%s\n", &wcn_dev->firmware_path[0]);
		gnss_file_path_set(&wcn_dev->firmware_path[0]);
	}
	/* try to open file */
	for (i = 1; i <= WCN_OPEN_MAX_CNT; i++) {
		file = filp_open(wcn_dev->firmware_path, O_RDONLY, 0);
		if (IS_ERR(file)) {
			pr_err("try open file %s,count_num:%d,%s, file=%p\n",
				wcn_dev->firmware_path, i, __func__, file);
			ssleep(1);
		} else
			break;
	}
	if (IS_ERR(file)) {
		pr_err("%s open file %s error\n",
			wcn_dev->firmware_path, __func__);
		return NULL;
	}
	pr_info("marlin %s open image file %s  successfully\n",
		__func__, wcn_dev->firmware_path);

	/* read file to buffer */
	size = imag_size = wcn_dev->file_length;
	wcn_image_buffer = vmalloc(size);
	if (!wcn_image_buffer) {
		fput(file);
		pr_err("no memory\n");
		return NULL;

	}
	pr_info("wcn_image_buffer=%p\n", wcn_image_buffer);

	data = wcn_image_buffer;
	do {
		read_len = kernel_read(file, 0, wcn_image_buffer, size);
		if (read_len > 0) {
			size -= read_len;
			wcn_image_buffer += read_len;
		}
	} while ((read_len > 0) && (size > 0));
	fput(file);
	pr_info("After read, wcn_image_buffer=%p\n", wcn_image_buffer);

	wcn_image_buffer = data;

#if WCN_INTEGRATE_PLATFORM_DEBUG
	if (s_wcn_debug_case == WCN_START_MARLIN_DDR_FIRMWARE_DEBUG)
		memcpy(wcn_image_buffer, &marlin_firmware_bin[0], imag_size);
	else if (s_wcn_debug_case == WCN_START_GNSS_DDR_FIRMWARE_DEBUG)
		memcpy(wcn_image_buffer, &gnss_firmware_bin[0], imag_size);
#endif

	/* copy file data to target ddr address */
	wcn_write_data_to_phy_addr(wcn_dev->base_addr, data, imag_size);

	vfree(wcn_image_buffer);

	pr_info("%s finish\n", __func__);

	return data;
}

/*
 * This function is used to use the firmware subsystem
 * to load the wcn image.And at the same time support
 * for reading from the partition image.The first way
 * to use the first.
 */
static int wcn_download_image(struct wcn_device *wcn_dev)
{
	const struct firmware *firmware;
	char *load_fimrware_ret = NULL;
	bool is_marlin;
	int err;

	is_marlin = strcmp(wcn_dev->name, WCN_MARLIN_DEV_NAME) == 0 ?
				true : false;
	memset(firmware_file_name, 0, FIRMWARE_FILEPATHNAME_LENGTH_MAX);

	if (!is_marlin) {
		if (s_wcn_device.gnss_type == WCN_GNSS_TYPE_GL)
			strcpy(firmware_file_name, WCN_GNSS_FILENAME);
		else if (s_wcn_device.gnss_type == WCN_GNSS_TYPE_BD)
			strcpy(firmware_file_name, WCN_GNSS_BD_FILENAME);
		else
			return -EINVAL;
	}

	if (is_marlin)
		strcpy(firmware_file_name, WCN_BTWF_FILENAME);

	strcat(firmware_file_name, ".bin");
	pr_info("%s: loading image [%s] from firmware subsystem ...\n",
			__func__, firmware_file_name);
	err = request_firmware_direct(&firmware, firmware_file_name, NULL);
	if (err < 0) {
		pr_err("%s no find image [%s] errno:(%d)(ignore!!)\n",
				__func__, firmware_file_name, err);
		load_fimrware_ret = wcn_load_firmware_data(wcn_dev);
		if (load_fimrware_ret == NULL) {
			pr_err("%s:wcn_load_firmware_data ERR!\n", __func__);
			return -EINVAL;
		}
	} else {
		pr_info("%s: image size = %d\n", __func__, (int)firmware->size);
		if (wcn_write_data_to_phy_addr(wcn_dev->base_addr,
					       (void *)firmware->data,
					       firmware->size)) {
			pr_err("%s wcn_mem_ram_vmap_nocache fail\n", __func__);
			release_firmware(firmware);
			return -ENOMEM;
		}

		release_firmware(firmware);
		pr_info("%s: loading image [%s] successfully!\n",
				__func__, firmware_file_name);
	}

	return 0;
}

/* load firmware and boot up sys. */
int wcn_proc_native_start(void *arg)
{
	struct wcn_device *wcn_dev = (struct wcn_device *)arg;
	bool is_marlin;
	phys_addr_t phy_addr;
	int err;

	pr_info("[%s]enter\n", __func__);

	if (!wcn_dev)
		return -ENODEV;

	is_marlin = strcmp(wcn_dev->name, WCN_MARLIN_DEV_NAME) == 0 ?
				true : false;

	/* when hot restart handset, the DDR value is error */
	if (is_marlin) {
		u32 tmp_value = MARLIN_CP_INIT_START_MAGIC;

		phy_addr = wcn_dev->base_addr +
			   (phys_addr_t)&(s_wssm_phy_offset_p->
			   marlin.init_status);
		wcn_write_data_to_phy_addr(phy_addr, &tmp_value,
					   sizeof(tmp_value));
	}

	err = wcn_download_image(wcn_dev);
	if (err < 0) {
		pr_err("%s wcn download image err!\n", __func__);
		return -1;
	}

	/* wifi need calibrate */
	if (is_marlin && s_wcn_device.btwf_calibrated == false) {
		marlin_write_cali_data();
		marlin_write_efuse_data();
	}

	/* gnss need prepare some data before bootup */
	if (!is_marlin) {
		gnss_write_version_data();
		gnss_write_efuse_data();
	}

	/* boot up system */
	wcn_cpu_bootup(wcn_dev);

	/* wifi need polling CP ready */
	if (is_marlin) {
		u32 wait_count = 0;
		u32 magic_value = 0;

		phy_addr = wcn_dev->base_addr +
			   (phys_addr_t)&(s_wssm_phy_offset_p->
			   marlin.init_status);
		for (wait_count = 0; wait_count < MARLIN_WAIT_CP_INIT_COUNT;
			wait_count++) {
			wcn_read_data_from_phy_addr(phy_addr,
						    &magic_value, sizeof(u32));
			if (magic_value == MARLIN_CP_INIT_READY_MAGIC)
				break;

			msleep(MARLIN_WAIT_CP_INIT_POLL_TIME_MS);
			pr_info("[%s] magic_value=%d, wait_count=%d\n",
				__func__, magic_value, wait_count);
		}

		/* get CP ready flag failed */
		if (wait_count >= MARLIN_WAIT_CP_INIT_COUNT) {
			pr_err("MARLIN boot cp timeout!\n");
			magic_value = MARLIN_CP_INIT_FALIED_MAGIC;
			wcn_write_data_to_phy_addr(phy_addr,
					&magic_value, sizeof(u32));
			return -1;
		}

		/* save cali data to INI file */
		if (s_wcn_device.btwf_calibrated == false) {
			u32 cali_flag;
			phys_addr_t cali_phy_addr;

			marlin_save_cali_data();

			/* clear notify CP calibration flag */
			cali_flag = WIFI_CALIBRATION_FLAG_CLEAR_VALUE;
			cali_phy_addr = s_wcn_device.btwf_device->base_addr +
					(phys_addr_t)&(s_wssm_phy_offset_p->
					wifi.calibration_flag);
			wcn_write_data_to_phy_addr(cali_phy_addr, &cali_flag,
							sizeof(cali_flag));

			s_wcn_device.btwf_calibrated = true;
		}

		/* set success flag */
		magic_value = MARLIN_CP_INIT_SUCCESS_MAGIC;
		wcn_write_data_to_phy_addr(phy_addr,
					&magic_value, sizeof(u32));
	}

	complete(&wcn_dev->download_done);

	return 0;
}

int wcn_proc_native_stop(void *arg)
{
	struct wcn_device *wcn_dev = arg;
	u32 iloop_index;
	u32 reg_nr = 0;
	unsigned int val;
	u32 reg_read;
	u32 type;

	pr_info("[%s]enter\n", __func__);

	if (wcn_dev == NULL)
		return -EINVAL;

	reg_nr = wcn_dev->reg_shutdown_nr < REG_SHUTDOWN_CNT_MAX ?
		wcn_dev->reg_shutdown_nr : REG_SHUTDOWN_CNT_MAX;
	for (iloop_index = 0; iloop_index < reg_nr; iloop_index++) {
		val = 0;
		type = wcn_dev->ctrl_shutdown_type[iloop_index];
		reg_read = wcn_dev->ctrl_shutdown_reg[iloop_index] -
			wcn_dev->ctrl_shutdown_rw_offset[iloop_index];
		wcn_regmap_read(wcn_dev->rmap[type],
				   reg_read,
				   &val
				   );
		pr_info("%s: ctrl_shutdown_reg[%d] = 0x%x, val=0x%x\n",
				__func__, iloop_index, reg_read, val);

		wcn_regmap_raw_write_bit(wcn_dev->rmap[type],
				wcn_dev->ctrl_shutdown_reg[iloop_index],
				wcn_dev->ctrl_shutdown_value[iloop_index]);
		udelay(wcn_dev->ctrl_shutdown_us_delay[iloop_index]);
		wcn_regmap_read(wcn_dev->rmap[type],
				   reg_read,
				   &val
				   );
		pr_info("%s: ctrl_reg[%d] = 0x%x, val=0x%x\n",
				__func__, iloop_index, reg_read, val);
	}

	return 0;
}

/*  GNSS assert workaround */
#define GNSS_TEST_OFFSET 0x150050
#define GNSS_TEST_MAGIC 0x12345678
static void gnss_clear_boot_flag(void)
{
	phys_addr_t phy_addr;
	u32 magic_value;

	phy_addr = wcn_get_gnss_base_addr() +
		   GNSS_TEST_OFFSET;
	wcn_read_data_from_phy_addr(phy_addr,
				    &magic_value, sizeof(u32));
	pr_info("[%s]  value is 0x%x\n", __func__, magic_value);
	magic_value = 0;
	wcn_write_data_to_phy_addr(phy_addr, &magic_value,
				   sizeof(u32));

	pr_info("%s finish\n", __func__);
}

/* used for distinguish Pike2 or sharkle */
static void gnss_read_boot_flag(void)
{
	phys_addr_t phy_addr;
	u32 magic_value;
	u32 wait_count;

	phy_addr = wcn_get_gnss_base_addr() +
		   GNSS_TEST_OFFSET;
	for (wait_count = 0; wait_count < MARLIN_WAIT_CP_INIT_COUNT;
		wait_count++) {
		wcn_read_data_from_phy_addr(phy_addr,
					    &magic_value, sizeof(u32));
		if (magic_value == GNSS_TEST_MAGIC)
			break;

		msleep(MARLIN_WAIT_CP_INIT_POLL_TIME_MS);
		pr_info("[%s] magic_value=%d, wait_count=%d\n",
			__func__, magic_value, wait_count);
	}

	pr_info("%s finish\n", __func__);
}

/* flag:1 disable cp2 deep
 * flag:0 enable cp2 deep
 */
static void wcn_wakeup_cp2(struct wcn_device *wcn_dev, int flag)
{
	int i;
	phys_addr_t phy_addr;
	u32 val;

	phy_addr = s_wcn_device.btwf_device->base_addr +
		   (phys_addr_t)&s_wssm_phy_offset_p->sleep_flag_addr;
	wcn_read_data_from_phy_addr(phy_addr, &val, sizeof(u32));
	pr_info("%s read 0x%x\n", __func__, val);

	if (flag && wcn_dev == s_wcn_device.gnss_device &&
	    s_wcn_device.btwf_device->wcn_open_status) {
		/* need more safe, FIXME */
		if (unlikely(sprdwcn_bus_get_carddump_status() != 0)) {
			pr_err("%s not set sleep switch 0 as dump now!\n",
			       __func__);
			return;
		}
		val = WCN_CP2_WAKEUP_ACK_CLEANVAL;
		wcn_write_data_to_phy_addr(phy_addr, &val, sizeof(u32));
		mdbg_send("AT+SLEEP_SWITCH=0\r",
			  strlen("AT+SLEEP_SWITCH=0\r"),
			  MDBG_SUBTYPE_AT);
		for (i = 0; i < 100; i++) {
			usleep_range(8000, 10000);
			wcn_read_data_from_phy_addr(phy_addr, &val,
						    sizeof(u32));
			if (val == WCN_CP2_WAKEUP_ACK_VAL)
				break;
		}
		if (i == 100)
			pr_err("%s wait CP2 wakeup ACK status erro:0x%x\n",
			       __func__, val);
		else
			pr_info("%s wait CP2 wakeup ACK status OK!\n",
				__func__);
	}

	if (!flag && wcn_dev == s_wcn_device.gnss_device &&
	    s_wcn_device.btwf_device->wcn_open_status) {
		/* need more safe, FIXME */
		if (unlikely(sprdwcn_bus_get_carddump_status() != 0)) {
			pr_err("%s not set sleep switch 1 as dump now!\n",
			       __func__);
			return;
		}
		pr_info("%s set sleep switch 1\n", __func__);
		mdbg_send("AT+SLEEP_SWITCH=1\r", strlen("AT+SLEEP_SWITCH=1\r"),
			  MDBG_SUBTYPE_AT);
	}
}

void wcn_power_wq(struct work_struct *pwork)
{
	struct wcn_device *wcn_dev;
	struct delayed_work *ppower_wq;
	u32 is_marlin = 0;
	static u32 califlag;

	ppower_wq = container_of(pwork, struct delayed_work, work);
	wcn_dev = container_of(ppower_wq, struct wcn_device, power_wq);
	pr_info("%s start\n", __func__);

	pr_info("%s %d wcn_dev name :%s\n", __func__, __LINE__,
		wcn_dev->name);
	wcn_power_enable_vddcon(true);
	if ((strcmp(wcn_dev->name, WCN_MARLIN_DEV_NAME) == 0)) {
		/* ASIC: disable vddcon, wifipa interval time > 1ms */
		usleep_range(VDDWIFIPA_VDDCON_MIN_INTERVAL_TIME,
			VDDWIFIPA_VDDCON_MAX_INTERVAL_TIME);
		wcn_marlin_power_enable_vddwifipa(true);
	}

	if (strcmp(wcn_dev->name, WCN_MARLIN_DEV_NAME) == 0)
		is_marlin = 1;

	if ((!is_marlin) && califlag) {
		gnss_clear_boot_flag();
		wcn_wakeup_cp2(wcn_dev, 1);
	}

	wcn_power_enable_sys_domain(true);
	wcn_proc_native_start(wcn_dev);

	if ((!is_marlin) && califlag) {
		gnss_read_boot_flag();
		wcn_wakeup_cp2(wcn_dev, 0);
	}

	if (!is_marlin)
		califlag = 1;

	pr_info("%s finish\n", __func__);
}

static void wcn_clear_ddr_gnss_cali_bit(void)
{
	phys_addr_t phy_addr;
	u32 value;
	struct wcn_device *wcn_dev;

	wcn_dev = s_wcn_device.btwf_device;
	if (wcn_dev) {
		value = GNSS_CALIBRATION_FLAG_CLEAR_ADDR_CP;
		phy_addr = wcn_dev->base_addr +
			   (phys_addr_t)&s_wssm_phy_offset_p->gnss_flag_addr;
		wcn_write_data_to_phy_addr(phy_addr, &value, sizeof(u32));
		pr_info("%s set gnss flag off:0x%x\n", __func__, value);
	}
	wcn_dev = s_wcn_device.gnss_device;
	value = GNSS_CALIBRATION_FLAG_CLEAR_VALUE;
	phy_addr = wcn_dev->base_addr + GNSS_CALIBRATION_FLAG_CLEAR_ADDR;
	wcn_write_data_to_phy_addr(phy_addr, &value, sizeof(u32));
	pr_info("%s %d  clear gnss ddr bit\n", __func__, __LINE__);
}

static void wcn_set_nognss(u32 val)
{
	phys_addr_t phy_addr;
	struct wcn_device *wcn_dev;

	wcn_dev = s_wcn_device.btwf_device;
	if (wcn_dev) {
		phy_addr = wcn_dev->base_addr +
			   (phys_addr_t)&s_wssm_phy_offset_p->include_gnss;
		wcn_write_data_to_phy_addr(phy_addr, &val, sizeof(u32));
		pr_info("%s gnss:%u\n", __func__, val);
	}
}

int start_integrate_wcn_truely(u32 subsys)
{
	struct wcn_device *wcn_dev;
	u32 subsys_bit = 1 << subsys;

	pr_info("%s %d start subsys:%d\n",
			__func__, __LINE__, subsys);
	if (subsys_bit & WCN_MARLIN_MASK)
		wcn_dev = s_wcn_device.btwf_device;
	else if ((subsys_bit & WCN_GNSS_MASK) ||
		(subsys_bit & WCN_GNSS_BD_MASK)) {
		wcn_dev = s_wcn_device.gnss_device;
	} else {
		pr_err("%s err: subsys=%d!\n", __func__, subsys);
		return -EINVAL;
	}

	if (!wcn_dev) {
		pr_err("%s fail: subsys=%d!\n", __func__, subsys);
		return -EINVAL;
	}

	if (wcn_dev == s_wcn_device.gnss_device) {
		if (subsys_bit & WCN_GNSS_MASK) {
			strcpy(&wcn_dev->firmware_path_ext[0],
			       WCN_GNSS_FILENAME);
			s_wcn_device.gnss_type = WCN_GNSS_TYPE_GL;
			pr_info("wcn gnss path=%s\n",
				&wcn_dev->firmware_path_ext[0]);
		} else {
			strcpy(&wcn_dev->firmware_path_ext[0],
			       WCN_GNSS_BD_FILENAME);
			s_wcn_device.gnss_type = WCN_GNSS_TYPE_BD;
			pr_info("wcn bd path=%s\n",
				&wcn_dev->firmware_path_ext[0]);
		}
	}

	mutex_lock(&(wcn_dev->power_lock));

	/* Check whether opened already */
	if ((wcn_dev->wcn_open_status & WCN_MARLIN_MASK)
		&& (subsys_bit & WCN_MARLIN_MASK)) {
		wcn_dev->wcn_open_status |= subsys_bit;
		mutex_unlock(&(wcn_dev->power_lock));
		pr_info("%s: marlin opened already = %d, subsys=%d!\n",
			__func__, wcn_dev->wcn_open_status, subsys);
		return 0;
	} else if (((wcn_dev->wcn_open_status & WCN_GNSS_MASK) &&
		(subsys_bit & WCN_GNSS_MASK)) ||
		((wcn_dev->wcn_open_status & WCN_GNSS_BD_MASK) &&
		(subsys_bit & WCN_GNSS_BD_MASK))) {
		wcn_dev->wcn_open_status |= subsys_bit;
		mutex_unlock(&(wcn_dev->power_lock));
		pr_info("%s: gnss opened already = %d, subsys=%d!\n",
			__func__, wcn_dev->wcn_open_status, subsys);
		return 0;
	}

	pr_info("%s subsys %d\n", __func__, subsys);

	/* Not opened, so first open */
	init_completion(&wcn_dev->download_done);

	schedule_delayed_work(&wcn_dev->power_wq,
				msecs_to_jiffies(0));

	if (wait_for_completion_timeout(&wcn_dev->download_done,
		msecs_to_jiffies(MARLIN_WAIT_CP_INIT_MAX_TIME)) <= 0) {
		/* marlin download fail dump memory */
		if (subsys_bit & WCN_MARLIN_MASK)
			mdbg_assert_interface("MARLIN boot cp timeout");

		mutex_unlock(&(wcn_dev->power_lock));

		return -1;
	}

	wcn_dev->wcn_open_status |= (1 << subsys);

	if (wcn_dev->wcn_open_status & WCN_MARLIN_MASK)
		wcn_set_module_state(true);

	mutex_unlock(&(wcn_dev->power_lock));

	return 0;
}

int start_integrate_wcn(u32 subsys)
{
	static u32 first_time;
	u32 btwf_subsys;
	u32 ret;

	pr_info("%s:%d\n", __func__, subsys);
	mutex_lock(&marlin_lock);
	if (unlikely(!first_time)) {
		if (s_wcn_device.gnss_device) {
			/* clear ddr gps cali bit */
			wcn_set_nognss(WCN_INTERNAL_INCLUD_GNSS_VAL);
			wcn_clear_ddr_gnss_cali_bit();
			ret = start_integrate_wcn_truely(WCN_GNSS);
			if (ret) {
				mutex_unlock(&marlin_lock);
				return ret;
			}
		} else {
			wcn_set_nognss(WCN_INTERNAL_NOINCLUD_GNSS_VAL);
			pr_info("%s not include gnss\n", __func__);
		}

		if (s_wcn_device.btwf_device) {
			if (subsys == WCN_GNSS || subsys == WCN_GNSS_BD)
				btwf_subsys = WCN_MARLIN_MDBG;
			else
				btwf_subsys = subsys;
			ret = start_integrate_wcn_truely(btwf_subsys);
			if (ret) {
				mutex_unlock(&marlin_lock);
				return ret;
			}
		}
		pr_info("%s %d first time, start gnss and btwf subsys:%d\n",
				__func__, __LINE__, subsys);

		/* after cali,gnss powerdown itself,AP sync state by stop op */
		if (s_wcn_device.gnss_device)
			stop_integrate_wcn_truely(WCN_GNSS);

		if (s_wcn_device.btwf_device &&
		    (subsys == WCN_GNSS || subsys == WCN_GNSS_BD))
			stop_integrate_wcn_truely(btwf_subsys);
		first_time = 1;
	}
	ret = start_integrate_wcn_truely(subsys);
	mutex_unlock(&marlin_lock);

	return ret;
}

int start_marlin(u32 subsys)
{
	if (unlikely(sprdwcn_bus_get_carddump_status() != 0)) {
		pr_err("err:in dump status subsys=%d!\n", subsys);
		return -1;
	}
	if (unlikely(get_loopcheck_status() >= 2)) {
		pr_err("err:loopcheck status error subsys=%d!\n", subsys);
		return -1;
	}
	if (unlikely(wcn_get_reset_flag_status())) {
		pr_err("err:in reset status subsys=%d!\n", subsys);
		return -1;
	}

	return start_integrate_wcn(subsys);
}
EXPORT_SYMBOL_GPL(start_marlin);

int stop_integrate_wcn_truely(u32 subsys)
{
	struct wcn_device *wcn_dev;
	u32 subsys_bit = 1 << subsys;
	u32 wait_sleep_count = 0;

	/* Check Parameter whether valid */
	if (subsys_bit & WCN_MARLIN_MASK)
		wcn_dev = s_wcn_device.btwf_device;
	else if ((subsys_bit&WCN_GNSS_MASK) || (subsys_bit&WCN_GNSS_BD_MASK))
		wcn_dev = s_wcn_device.gnss_device;
	else {
		pr_err("%s err: subsys=%d!\n", __func__, subsys);
		return -EINVAL;
	}

	if (wcn_dev == NULL) {
		pr_err("%s fail: subsys=%d!\n", __func__, subsys);
		return -EINVAL;
	}

	mutex_lock(&(wcn_dev->power_lock));

	/* Check whether already opened */
	if ((wcn_dev->wcn_open_status & WCN_MARLIN_MASK)
		&& (subsys_bit & WCN_MARLIN_MASK)) {	/* Marlin opened */
		wcn_dev->wcn_open_status &= ~subsys_bit;

		/* the last module will stop,AP should wait CP2 sleep */
		if ((wcn_dev->wcn_open_status & WCN_MARLIN_MASK) == 0) {
			for (wait_sleep_count = 0;
				wait_sleep_count < WCN_WAIT_SLEEP_MAX_COUNT;
				wait_sleep_count++) {
				if (wcn_platform_chip_type() !=
					WCN_PLATFORM_TYPE_PIKE2) {
					if (wcn_get_sleep_status() == 0)
						break;
					msleep(20);
					pr_info("%s: wait_sleep_count=%d!\n",
						__func__,
						wait_sleep_count);
				}
			}
		}

		/* only one module works: stop CPU */
		if ((wcn_dev->wcn_open_status & WCN_MARLIN_MASK) == 0) {
			wcn_proc_native_stop(wcn_dev);
			wcn_power_enable_sys_domain(false);
		}

		/* stop common resources if can disable it */
		if ((wcn_dev->wcn_open_status & WCN_MARLIN_MASK) == 0) {
			wcn_marlin_power_enable_vddwifipa(false);
			/* ASIC: disable vddcon, wifipa interval time > 1ms */
			usleep_range(VDDWIFIPA_VDDCON_MIN_INTERVAL_TIME,
				VDDWIFIPA_VDDCON_MAX_INTERVAL_TIME);
			wcn_power_enable_vddcon(false);
			wcn_sys_soft_reset();
			msleep(20);
			wcn_sys_soft_release();
			wcn_sys_deep_sleep_en();
		}

		pr_info("%s: wcn_open_status = %d, stop subsys=%d!\n",
			__func__,
			wcn_dev->wcn_open_status,
			subsys);

		wcn_set_module_state(false);
		mutex_unlock(&(wcn_dev->power_lock));

		return 0;
	} else if (((wcn_dev->wcn_open_status & WCN_GNSS_MASK) &&
		(subsys_bit & WCN_GNSS_MASK)) ||
		((wcn_dev->wcn_open_status & WCN_GNSS_BD_MASK) &&
		(subsys_bit & WCN_GNSS_BD_MASK))) {
		wcn_dev->wcn_open_status &= ~subsys_bit;

		/* the last module will stop, AP should wait CP2 sleep */
		if ((wcn_dev->wcn_open_status & WCN_GNSS_ALL_MASK) == 0) {
			for (wait_sleep_count = 0;
				wait_sleep_count < WCN_WAIT_SLEEP_MAX_COUNT;
				wait_sleep_count++) {
				if (wcn_platform_chip_type() !=
					WCN_PLATFORM_TYPE_PIKE2) {
					if (wcn_get_sleep_status() == 0)
						break;
					msleep(20);
					pr_info("%s: wait_sleep_count=%d!\n",
						__func__, wait_sleep_count);
				}
			}
		}

		/*
		 * Though until now, GNSS has only one module
		 * maybe someday later, it has one more subsys.
		 */
		if (((wcn_dev->wcn_open_status & WCN_GNSS_MASK) == 0) ||
			((wcn_dev->wcn_open_status & WCN_GNSS_BD_MASK) == 0)) {
			wcn_proc_native_stop(wcn_dev);
			wcn_power_enable_sys_domain(false);
			wcn_power_enable_vddcon(false);
			wcn_sys_soft_reset();
			msleep(20);
			wcn_sys_soft_release();
			wcn_sys_deep_sleep_en();
		}

		pr_info("%s: wcn_open_status = %d, stop subsys=%d!\n",
			__func__, wcn_dev->wcn_open_status, subsys);
		mutex_unlock(&(wcn_dev->power_lock));

		return 0;
	}

	/* It wants to stop not opened device */
	pr_err("%s err: subsys = %d, and %s is not open!\n",
	       __func__, subsys, wcn_dev->name);
	mutex_unlock(&(wcn_dev->power_lock));

	return -EINVAL;
}

int stop_integrate_wcn(u32 subsys)
{
	u32 ret;

	mutex_lock(&marlin_lock);
	ret = stop_integrate_wcn_truely(subsys);
	mutex_unlock(&marlin_lock);

	return ret;
}

int stop_marlin(u32 subsys)
{
	if (unlikely(sprdwcn_bus_get_carddump_status() != 0)) {
		pr_err("err:in dump status subsys=%d!\n", subsys);
		return -1;
	}
	if (unlikely(get_loopcheck_status() >= 2)) {
		pr_err("err:loopcheck status error subsys=%d!\n", subsys);
		return -1;
	}
	if (unlikely(wcn_get_reset_flag_status())) {
		pr_err("err:in reset status subsys=%d!\n", subsys);
		return -1;
	}

	return stop_integrate_wcn(subsys);
}
EXPORT_SYMBOL_GPL(stop_marlin);

