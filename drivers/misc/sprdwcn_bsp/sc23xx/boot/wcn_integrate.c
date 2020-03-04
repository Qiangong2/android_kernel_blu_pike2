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
#include "wcn_glb.h"
#include "wcn_glb_reg.h"
#include "wcn_procfs.h"

static int wcn_open_module;
static int wcn_module_state_change;
char functionmask[8];
marlin_reset_callback marlin_reset_func;
void *marlin_callback_para;
static u32 reset_flag;
struct platform_chip_id g_platform_chip_id;
static u32 g_platform_chip_type;
static const struct wcn_chip_type wcn_chip_type[] = {
	{0x96360000, WCN_SHARKLE_CHIP_AA_OR_AB},
	{0x96360002, WCN_SHARKLE_CHIP_AC},
	{0x96360003, WCN_SHARKLE_CHIP_AD},
};

struct wcn_special_share_mem *s_wssm_phy_offset_p =
	(struct wcn_special_share_mem *)WCN_SPECIAL_SHARME_MEM_ADDR;

enum wcn_aon_chip_id wcn_get_aon_chip_id(void)
{
	u32 aon_chip_id;
	int i;

	if (s_wcn_device.btwf_device) {
		wcn_regmap_read(s_wcn_device.btwf_device->rmap[REGMAP_AON_APB],
			WCN_AON_CHIP_ID, &aon_chip_id);
		pr_info("%s aon_chip_id=0x%08x\n", __func__, aon_chip_id);
		for (i = 0; i < ARRAY_SIZE(wcn_chip_type); i++) {
			if (wcn_chip_type[i].chipid == aon_chip_id)
				return wcn_chip_type[i].chiptype;
		}
	}
	return WCN_AON_CHIP_ID_INVALID;
}
EXPORT_SYMBOL_GPL(wcn_get_aon_chip_id);

#define WCN_VMAP_RETRY_CNT (20)
static void *wcn_mem_ram_vmap(phys_addr_t start, size_t size,
			      int noncached, unsigned int *count)
{
	struct page **pages;
	phys_addr_t page_start;
	unsigned int page_count;
	pgprot_t prot;
	unsigned int i;
	void *vaddr;
	phys_addr_t addr;
	int retry = 0;

	page_start = start - offset_in_page(start);
	page_count = DIV_ROUND_UP(size + offset_in_page(start), PAGE_SIZE);
	*count = page_count;
	if (noncached)
		prot = pgprot_noncached(PAGE_KERNEL);
	else
		prot = PAGE_KERNEL;

retry1:
	pages = kmalloc_array(page_count, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		if (retry++ < WCN_VMAP_RETRY_CNT) {
			usleep_range(8000, 10000);
			goto retry1;
		} else {
			pr_err("%s malloc err\n", __func__);
			return NULL;
		}
	}

	for (i = 0; i < page_count; i++) {
		addr = page_start + i * PAGE_SIZE;
		pages[i] = pfn_to_page(addr >> PAGE_SHIFT);
	}
retry2:
	vaddr = vm_map_ram(pages, page_count, -1, prot);
	if (!vaddr) {
		if (retry++ < WCN_VMAP_RETRY_CNT) {
			usleep_range(8000, 10000);
			goto retry2;
		} else {
			pr_err("%s vmap err\n", __func__);
			goto out;
		}
	} else {
		vaddr += offset_in_page(start);
	}
out:
	kfree(pages);

	return vaddr;
}

void wcn_mem_ram_unmap(const void *mem, unsigned int count)
{
	vm_unmap_ram(mem - offset_in_page(mem), count);
}

void *wcn_mem_ram_vmap_nocache(phys_addr_t start, size_t size,
			       unsigned int *count)
{
	return wcn_mem_ram_vmap(start, size, 1, count);
}

int wcn_write_data_to_phy_addr(phys_addr_t phy_addr,
			       void *src_data, u32 size)
{
	int i;
	char *virt_addr, *src;
	unsigned int cnt;

	src = (char *)src_data;
	virt_addr = (char *)wcn_mem_ram_vmap_nocache(phy_addr, size, &cnt);
	if (virt_addr) {
		/* crash so remove the memcpy */
		for (i = 0; i < size; i++)
			virt_addr[i] = src[i];
		wcn_mem_ram_unmap(virt_addr, cnt);
		return 0;
	}

	pr_err("%s wcn_mem_ram_vmap_nocache fail\n", __func__);
	return -1;
}

int wcn_read_data_from_phy_addr(phys_addr_t phy_addr,
				void *tar_data, u32 size)
{
	int i;
	char *virt_addr, *tar;
	unsigned int cnt;

	tar = (char *)tar_data;
	virt_addr = wcn_mem_ram_vmap_nocache(phy_addr, size, &cnt);
	if (virt_addr) {
		/* crash so remove the memcpy */
		for (i = 0; i < size; i++)
			tar[i] = virt_addr[i];
		wcn_mem_ram_unmap(virt_addr, cnt);
		return 0;
	}

	pr_err("%s wcn_mem_ram_vmap_nocache fail\n", __func__);
	return -1;
}

u32 wcn_platform_chip_id(void)
{
	return g_platform_chip_id.aon_chip_id;
}

u32 wcn_platform_chip_type(void)
{
	return g_platform_chip_type;
}

u32 wcn_get_cp2_comm_rx_count(void)
{
	u32 rx_count;
	phys_addr_t phy_addr;

	phy_addr = s_wcn_device.btwf_device->base_addr +
		   (phys_addr_t)&s_wssm_phy_offset_p->marlin.loopcheck_cnt;
	wcn_read_data_from_phy_addr(phy_addr,
				    &rx_count, sizeof(u32));
	WCN_INFO("%s cp2 comm rx count :%d\n", __func__, rx_count);

	return rx_count;
}

phys_addr_t wcn_get_btwf_base_addr(void)
{
	return s_wcn_device.btwf_device->base_addr;
}

phys_addr_t wcn_get_btwf_init_status_addr(void)
{
	return s_wcn_device.btwf_device->base_addr +
	       (phys_addr_t)&s_wssm_phy_offset_p->marlin.init_status;
}

phys_addr_t wcn_get_btwf_sleep_addr(void)
{
	return s_wcn_device.btwf_device->base_addr +
	       (phys_addr_t)&s_wssm_phy_offset_p->cp2_sleep_status;
}

struct regmap *wcn_get_btwf_regmap(u32 regmap_type)
{
	return s_wcn_device.btwf_device->rmap[regmap_type];
}

struct regmap *wcn_get_gnss_regmap(u32 regmap_type)
{
	return s_wcn_device.gnss_device->rmap[regmap_type];
}

phys_addr_t wcn_get_gnss_base_addr(void)
{
	return s_wcn_device.gnss_device->base_addr;
}

bool wcn_get_download_status(void)
{
	return s_wcn_device.btwf_device->download_status;
}

void wcn_set_download_status(bool status)
{
	s_wcn_device.btwf_device->download_status = status;
}

int wcn_get_module_status_changed(void)
{
	return wcn_module_state_change;
}
EXPORT_SYMBOL_GPL(wcn_get_module_status_changed);

void wcn_set_module_status_changed(bool status)
{
	wcn_module_state_change = status;
}

int marlin_get_module_status(void)
{
	return wcn_open_module;
}
EXPORT_SYMBOL_GPL(marlin_get_module_status);

int marlin_reset_register_notify(void *callback_func, void *para)
{
	marlin_reset_func = (marlin_reset_callback)callback_func;
	marlin_callback_para = para;

	return 0;
}
EXPORT_SYMBOL_GPL(marlin_reset_register_notify);

int marlin_reset_unregister_notify(void)
{
	marlin_reset_func = NULL;
	marlin_callback_para = NULL;

	return 0;
}
EXPORT_SYMBOL_GPL(marlin_reset_unregister_notify);

int wcn_get_reset_flag_status(void)
{
	return reset_flag;
}

void  wcn_set_reset_flag_status(u32 flag)
{
	reset_flag = flag;
	pr_info("%s %d reset_flag %d\n", __func__, __LINE__, reset_flag);
}

void wcn_sys_reset(void)
{
	u32 i;

	for (i = 0; i < WCN_GNSS_ALL; i++)
		stop_integrate_wcn(i);
	pr_info("%s %d reset finish!\n", __func__, __LINE__);
}


void wcn_set_module_state(bool status)
{
	if (s_wcn_device.btwf_device->wcn_open_status & WCN_MARLIN_MASK)
		wcn_open_module = 1;
	else
		wcn_open_module = 0;
	wcn_module_state_change = 1;
	if (status)
		loopcheck_first_boot_clear();
	wcn_set_download_status(status);
	WCN_INFO("[%s] cp2 power status:%d\n", __func__, status);
	wakeup_loopcheck_int();
}


#if REGMAP_UPDATE_BITS_ENABLE
static void wcn_regmap_update_bit(struct wcn_device *ctrl,
					 u32 index,
					 u32 mask,
					 u32 val)
{
	u32 type;
	u32 reg;
	int ret;

	type = ctrl->ctrl_type[index];
	reg = ctrl->ctrl_reg[index];

	ret = regmap_update_bits(ctrl->rmap[type],
			   reg,
			   mask,
			   val);
	if (ret)
		pr_err("[%s]regmap_update_bits ret=%d\n", __func__, ret);
}

static void wcn_regmap_write_bit(struct wcn_device *ctrl,
					 u32 index,
					 u32 mask,
					 u32 val)
{
	u32 type;
	u32 reg;
	int ret;

	type = ctrl->ctrl_type[index];
	reg = ctrl->ctrl_reg[index];

	ret = regmap_write_bits(ctrl->rmap[type],
			   reg,
			   mask,
			   val);
	if (ret)
		pr_err("[%s]regmap_write_bits ret=%d\n", __func__, ret);
}
#endif

void wcn_regmap_raw_write_bit(struct regmap *cur_regmap,
				     u32 reg,
				     unsigned int val)
{
	int ret;
	u32 val_tmp = val;

	ret = regmap_raw_write(cur_regmap, reg, (const void *)&val_tmp, 4);
	if (ret)
		pr_err("[%s]regmap_raw_write ret=%d\n", __func__, ret);
}

/* addr_offset:some REGs has twice group, one read and another write */
void wcn_regmap_read(struct regmap *cur_regmap,
			    u32 reg,
			    unsigned int *val)
{
	(void)regmap_read(cur_regmap, reg, val);
}

/* WCN SYS include BTWF and GNSS sys, ret: 0 is sleep, else is not */
u32 wcn_get_sleep_status(void)
{
	struct wcn_device *wcn_dev;
	u32 sleep_status = 0;
	u32 wcn_sleep_status_mask = 0xf000;

	wcn_dev = s_wcn_device.btwf_device ?
		s_wcn_device.btwf_device : s_wcn_device.gnss_device;
	if (wcn_dev) {
		wcn_regmap_read(wcn_dev->rmap[REGMAP_PMU_APB],
						0x00d4,
						&sleep_status);

		return (sleep_status & wcn_sleep_status_mask);
	}

	return 0;
}

void wcn_power_domain_set(struct wcn_device *wcn_dev, u32 set_type)
{
	u32 offset0 = 0, offset1 = 0;
	u32 bitmap0 = 0, bitmap1 = 0;

	if (wcn_platform_chip_type() == WCN_PLATFORM_TYPE_PIKE2) {
		if (set_type == 1) {
			offset0 = 0X2050;
			offset1 = 0X1050;
			bitmap0 = 1 << 24;
			bitmap1 = 1 << 25;
		} else {
			offset0 = 0X2050;
			offset1 = 0X2050;
			bitmap0 = 1 << 24;
			bitmap1 = 1 << 25;
		}
	} else {
		if (set_type == 1) {
			offset0 = 0X2100;
			offset1 = 0X1100;
			bitmap0 = 1 << 24;
			bitmap1 = 1 << 25;
		} else {
			offset0 = 0X2100;
			offset1 = 0X2100;
			bitmap0 = 1 << 24;
			bitmap1 = 1 << 25;
		}
	}
	wcn_regmap_raw_write_bit(wcn_dev->rmap[REGMAP_PMU_APB],
					 offset0, bitmap0);
	wcn_regmap_raw_write_bit(wcn_dev->rmap[REGMAP_PMU_APB],
					 offset1, bitmap1);
}

void wcn_xtl_auto_sel(bool enable)
{
	struct regmap *regmap;
	u32 value;

	regmap = wcn_get_btwf_regmap(REGMAP_PMU_APB);
	wcn_regmap_read(regmap, 0x338, &value);

	if (enable) {
		value |= 1 << 4;
		wcn_regmap_raw_write_bit(regmap, 0x338, value);
	} else {
		value &= ~(1 << 4);
		wcn_regmap_raw_write_bit(regmap, 0X338, value);
	}
}

int wcn_power_enable_sys_domain(bool enable)
{
	int ret = 0;
	u32 btwf_open = false;
	u32 gnss_open = false;
	static u32 sys_domain;

	if (s_wcn_device.btwf_device &&
		s_wcn_device.btwf_device->wcn_open_status & WCN_MARLIN_MASK)
		btwf_open = true;
	if (s_wcn_device.gnss_device &&
		s_wcn_device.gnss_device->wcn_open_status & WCN_GNSS_ALL_MASK)
		gnss_open = true;

	if (enable && (sys_domain == false)) {
		if (wcn_platform_chip_type() == WCN_PLATFORM_TYPE_PIKE2)
			wcn_xtl_auto_sel(false);
		wcn_power_domain_set(s_wcn_device.btwf_device, 0);
		if (wcn_platform_chip_type() == WCN_PLATFORM_TYPE_PIKE2)
			wcn_xtl_auto_sel(true);
		sys_domain = true;
		pr_info("%s %d clear WCN SYS TOP PD\n", __func__, __LINE__);
	} else if ((!btwf_open) && (!gnss_open) && sys_domain) {
		if (wcn_platform_chip_type() ==
				WCN_PLATFORM_TYPE_PIKE2)
			wcn_xtl_auto_sel(false);
		wcn_power_domain_set(s_wcn_device.btwf_device, 1);
		sys_domain = false;
		pr_info("%s %d set WCN SYS TOP PD\n", __func__, __LINE__);
	}
	pr_info("%s enable = %d, ret = %d, btwf_open=%d, gnss_open=%d\n",
		     __func__, enable, ret, btwf_open, gnss_open);

	return ret;
}

/*
 * wcn_sys_soft_reset was used by BTWF and GNSS together
 * both BTWF and GNSS not work, we should set it.
 */
void wcn_sys_soft_reset(void)
{
	u32 btwf_open = false;
	u32 gnss_open = false;
	u32 offset = 0;
	u32 bitmap = 0;
	struct regmap *rmap = NULL;
	struct wcn_device *wcn_dev;

	wcn_dev = s_wcn_device.btwf_device ?
		s_wcn_device.btwf_device : s_wcn_device.gnss_device;
	if (!wcn_dev)
		return;

	if (s_wcn_device.btwf_device &&
		s_wcn_device.btwf_device->wcn_open_status & WCN_MARLIN_MASK)
		btwf_open = true;
	if (s_wcn_device.gnss_device &&
		s_wcn_device.gnss_device->wcn_open_status & WCN_GNSS_ALL_MASK)
		gnss_open = true;

	if (!btwf_open && !gnss_open) {
		if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_PIKE2) {
			offset  = 0X10b0;
			bitmap = 1 << 7;
		} else if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_SHARKLE) {
			offset  = 0X10b0;
			bitmap = 1 << 9;
		} else if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_SHARKL3) {
			offset  = 0X10b0;
			bitmap = 1 << 16;
		}
		rmap = wcn_dev->rmap[REGMAP_PMU_APB];
		if (rmap)
			wcn_regmap_raw_write_bit(rmap, offset, bitmap);
		pr_info("%s\n", __func__);
	}
}

void wcn_sys_ctrl_26m(bool enable)
{
	struct regmap *regmap;
	u32 value;

	regmap = wcn_get_btwf_regmap(REGMAP_ANLG_PHY_G6);
	wcn_regmap_read(regmap, 0x28, &value);

	if (enable) {
		value &= ~(1 << 2);
		wcn_regmap_raw_write_bit(regmap, 0X28, value);
	} else {
		value |= 1 << 2;
		wcn_regmap_raw_write_bit(regmap, 0X28, value);
	}
}

/*
 * wcn_sys_soft_release was used by BTWF and GNSS together
 * both BTWF and GNSS not work, we should set it.
 */
void wcn_sys_soft_release(void)
{
	u32 btwf_open = false;
	u32 gnss_open = false;
	u32 offset = 0;
	u32 bitmap = 0;
	struct regmap *rmap = NULL;

	if (s_wcn_device.btwf_device == NULL &&
		s_wcn_device.gnss_device == NULL)
		return;

	if (s_wcn_device.btwf_device &&
		s_wcn_device.btwf_device->wcn_open_status & WCN_MARLIN_MASK)
		btwf_open = true;
	if (s_wcn_device.gnss_device &&
		s_wcn_device.gnss_device->wcn_open_status & WCN_GNSS_ALL_MASK)
		gnss_open = true;

	if (!btwf_open && !gnss_open) {
		if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_PIKE2) {
			offset  = 0X20b0;
			bitmap = 1 << 7;
		} else if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_SHARKLE) {
			offset  = 0X20b0;
			bitmap = 1 << 9;
		} else if (wcn_platform_chip_type() ==
						WCN_PLATFORM_TYPE_SHARKL3) {
			offset  = 0X20b0;
			bitmap = 1 << 16;
		}
		if (s_wcn_device.btwf_device) {
			rmap = s_wcn_device.btwf_device->rmap[REGMAP_PMU_APB];
		} else if (s_wcn_device.gnss_device) {
			rmap = s_wcn_device.gnss_device->rmap[REGMAP_PMU_APB];
		} else {
			pr_err("%s no devices\n", __func__);
			return;
		}
		wcn_regmap_raw_write_bit(rmap, offset, bitmap);
		pr_info("%s\n", __func__);
	}
}

/*
 * wcn_sys_deep_sleep_en was used by BTWF and GNSS together
 * both BTWF and GNSS not work, we should set it.
 */
void wcn_sys_deep_sleep_en(void)
{
	struct regmap *rmap = NULL;

	if (wcn_platform_chip_type() != WCN_PLATFORM_TYPE_PIKE2) {
		if (s_wcn_device.btwf_device) {
			rmap = s_wcn_device.btwf_device->rmap[REGMAP_PMU_APB];
		} else if (s_wcn_device.gnss_device) {
			rmap = s_wcn_device.gnss_device->rmap[REGMAP_PMU_APB];
		} else {
			pr_err("%s no devices\n", __func__);
			return;
		}
		wcn_regmap_raw_write_bit(rmap, 0x1244, 1 << 0);
		pr_info("%s\n", __func__);
	}
}

/* The VDDCON default value is 1.6V, we should set it to 1.2v */
void wcn_power_set_vddcon(u32 value)
{
	if (s_wcn_device.vddwcn != NULL)
		regulator_set_voltage(s_wcn_device.vddwcn,
				      value, value);
}

/*
 * NOTES:regulator function has compute-counter
 * We needn't judge GNSS and BTWF coxist case now.
 * But we should reserve the open status to debug.
 */
int wcn_power_enable_vddcon(bool enable)
{
	int ret = 0;
	u32 btwf_open = false;
	u32 gnss_open = false;

	if (s_wcn_device.btwf_device &&
		s_wcn_device.btwf_device->wcn_open_status & WCN_MARLIN_MASK)
		btwf_open = true;
	if (s_wcn_device.gnss_device &&
		s_wcn_device.gnss_device->wcn_open_status & WCN_GNSS_ALL_MASK)
		gnss_open = true;

	mutex_lock(&(s_wcn_device.vddwcn_lock));
	if (s_wcn_device.vddwcn != NULL) {
		if (enable) {
			ret = regulator_enable(s_wcn_device.vddwcn);
			s_wcn_device.vddwcn_en_count++;
			if (wcn_platform_chip_type() ==
				WCN_PLATFORM_TYPE_SHARKLE)
				wcn_sys_ctrl_26m(true);
		} else if (regulator_is_enabled(s_wcn_device.vddwcn)) {
			ret = regulator_disable(s_wcn_device.vddwcn);
			s_wcn_device.vddwcn_en_count--;
			if ((wcn_platform_chip_type() ==
				WCN_PLATFORM_TYPE_SHARKLE)
				&& (s_wcn_device.vddwcn_en_count == 0)) {
				wcn_sys_ctrl_26m(false);
			}
		}

		pr_info("%s:enable=%d,en_count=%d,ret=%d,btwf=%d,gnss=%d\n",
			     __func__, enable, s_wcn_device.vddwcn_en_count,
			     ret, btwf_open, gnss_open);
		if (s_wcn_device.vddwcn_en_count > 2 ||
		    s_wcn_device.vddwcn_en_count < 0)
			pr_err("%s:vddwcn_en_count=%d",
				__func__, s_wcn_device.vddwcn_en_count);
	}
	mutex_unlock(&(s_wcn_device.vddwcn_lock));

	return ret;
}

/* The VDDCON default value is 1.6V, we should set it to 1.2v */
void wcn_power_set_vddwifipa(u32 value)
{
	struct wcn_device *btwf_device = s_wcn_device.btwf_device;

	if (btwf_device->vddwifipa != NULL)
		regulator_set_voltage(btwf_device->vddwifipa,
				      value, value);
	pr_info("%s %d  value %d\n", __func__, __LINE__, value);
}

/* NOTES: wifipa: only used by WIFI module */
int wcn_marlin_power_enable_vddwifipa(bool enable)
{
	int ret = 0;
	struct wcn_device *btwf_device = s_wcn_device.btwf_device;

	mutex_lock(&(btwf_device->vddwifipa_lock));
	if (btwf_device->vddwifipa != NULL) {
		if (enable)
			ret = regulator_enable(btwf_device->vddwifipa);
		else if (regulator_is_enabled(btwf_device->vddwifipa))
			ret = regulator_disable(btwf_device->vddwifipa);

		pr_info("%s enable = %d, ret = %d\n", __func__, enable, ret);
	}
	mutex_unlock(&(btwf_device->vddwifipa_lock));

	return ret;
}

u32 wcn_parse_platform_chip_id(struct wcn_device *wcn_dev)
{
	wcn_regmap_read(wcn_dev->rmap[REGMAP_AON_APB],
					WCN_AON_CHIP_ID0,
					&g_platform_chip_id.aon_chip_id0);
	wcn_regmap_read(wcn_dev->rmap[REGMAP_AON_APB],
					WCN_AON_CHIP_ID1,
					&g_platform_chip_id.aon_chip_id1);
	wcn_regmap_read(wcn_dev->rmap[REGMAP_AON_APB],
					WCN_AON_PLATFORM_ID0,
					&g_platform_chip_id.aon_platform_id0);
	wcn_regmap_read(wcn_dev->rmap[REGMAP_AON_APB],
					WCN_AON_PLATFORM_ID1,
					&g_platform_chip_id.aon_platform_id1);
	wcn_regmap_read(wcn_dev->rmap[REGMAP_AON_APB],
					WCN_AON_CHIP_ID,
					&g_platform_chip_id.aon_chip_id);

	if (g_platform_chip_id.aon_chip_id0 == PIKE2_CHIP_ID0 &&
		g_platform_chip_id.aon_chip_id1 == PIKE2_CHIP_ID1)
		g_platform_chip_type = WCN_PLATFORM_TYPE_PIKE2;
	else if (g_platform_chip_id.aon_chip_id0 == SHARKLE_CHIP_ID0 &&
		g_platform_chip_id.aon_chip_id1 == SHARKLE_CHIP_ID1)
		g_platform_chip_type = WCN_PLATFORM_TYPE_SHARKLE;
	else if (g_platform_chip_id.aon_chip_id0 == SHARKL3_CHIP_ID0 &&
		g_platform_chip_id.aon_chip_id1 == SHARKL3_CHIP_ID1)
		g_platform_chip_type = WCN_PLATFORM_TYPE_SHARKL3;
	else
		pr_err("[%s] aon_chip_id0:[%d],id1[%d]\n", __func__,
					g_platform_chip_id.aon_chip_id0,
					g_platform_chip_id.aon_chip_id1);

	pr_info("[%s] platform chip type: [%d]\n", __func__,
							g_platform_chip_type);

	return 0;
}

void mdbg_hold_cpu(void)
{
	struct regmap *regmap;
	u32 value;
	phys_addr_t init_addr;

	if (wcn_platform_chip_type() == WCN_PLATFORM_TYPE_SHARKL3)
		regmap = wcn_get_btwf_regmap(REGMAP_WCN_REG);
	else
		regmap = wcn_get_btwf_regmap(REGMAP_ANLG_WRAP_WCN);
	wcn_regmap_read(regmap, 0X20, &value);
	value |= 1 << 3;
	wcn_regmap_raw_write_bit(regmap, 0X20, value);

	wcn_regmap_read(regmap, 0X24, &value);
	value |= 1 << 2;
	wcn_regmap_raw_write_bit(regmap, 0X24, value);

	value = MDBG_CACHE_FLAG_VALUE;
	init_addr = wcn_get_btwf_init_status_addr();
	wcn_write_data_to_phy_addr(init_addr, (void *)&value, 4);
	value = 0;
	wcn_regmap_raw_write_bit(regmap, 0X20, value);
	wcn_regmap_raw_write_bit(regmap, 0X24, value);
	msleep(200);
}

