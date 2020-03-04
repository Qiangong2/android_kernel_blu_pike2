/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 * Filename : slp_mgr.h
 * Abstract : This file is a implementation for itm sipc command/event function
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __SLP_MGR_H__
#define __SLP_MGR_H__

#include <linux/marlin_platform.h>

#define SLP_MGR_HEADER "[slp_mgr]"
#define SLP_MGR_ERR(fmt, args...)	\
	pr_err(SLP_MGR_HEADER fmt "\n", ## args)
#define SLP_MGR_INFO(fmt, args...)	\
	pr_info(SLP_MGR_HEADER fmt "\n", ## args)
#define SLP_MGR_DBG(fmt, args...)	\
	pr_debug(SLP_MGR_HEADER fmt "\n", ## args)

/* wakeup status */
#define	WAKE_START		0
#define	WAKE_TIMEOUT	1
#define	WAKE_SUCCESS	2

/* cp2 sleep status */
#define	STAY_SLPING		0
#define	STAY_AWAKING	1

struct slp_mgr_t {
	struct mutex    drv_slp_lock;
	struct mutex    wakeup_lock;
	struct completion wakeup_ack_completion;
	unsigned int active_module;
	atomic_t  cp2_state;
};

enum slp_subsys {
	SLP_TX = 0,
	SLP_RX,
	SLP_MAX,
};

#ifdef SLP_MGR_TEST
int slp_test_init(void);
#endif
struct slp_mgr_t *slp_get_info(void);
int slp_mgr_init(void);
int slp_mgr_deinit(void);
void slp_mgr_drv_sleep(enum marlin_sub_sys subsys, bool enable);
int slp_mgr_wakeup(enum marlin_sub_sys subsys);

#endif
