/******************** (C) COPYRIGHT 2013 Freecom ********************
 *
 * File Name          : cdc_com.c
 * Description        : Compatible Driver Configure
 *
 *******************************************************************************
 *
 * ˼·:
 * 		
 *		��Ҫ��װһЩ�ӿ������ʵ�� Gsensor Msensor FM TP �ȵļ���
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THE PRESENT SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, FOR THE SOLE
 * PURPOSE TO SUPPORT YOUR APPLICATION DEVELOPMENT.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
 *

 ******************************************************************************/
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/mutex.h>

static struct mutex	 ctp_mutex;
static int mutex_mark=-1;


void cdc_tp_lock_mutex(void)
{
    if(-1==mutex_mark)
    {
        mutex_mark=1;
        mutex_init(&ctp_mutex);
    }
    mutex_lock(&ctp_mutex);
}

void cdc_tp_unlock_mutex(void)
{
    if(1==mutex_mark)
    {
		mutex_unlock(&ctp_mutex);
    }
}

int cdc_tp_device_id(int id)
{
    static int device_ctp_id=0;
    unsigned long flags;
    local_irq_save(flags);
    
    if(id!=0)
    {
        device_ctp_id=id;
    }
    if(id==0xFFFF)
    {
        device_ctp_id=0;
    }
    local_irq_restore(flags);
    //CTP_DEBUG("ctp chip id(0x%x)",device_ctp_id);
    return device_ctp_id;
}

EXPORT_SYMBOL_GPL(cdc_tp_lock_mutex);
EXPORT_SYMBOL_GPL(cdc_tp_unlock_mutex);
EXPORT_SYMBOL_GPL(cdc_tp_device_id);

static int	gsensor_id=0;
int cdc_gsensor_device_id(int id)
{
	unsigned long	flags;
	local_irq_save(flags);

	if(id!=0)
	{
		gsensor_id=id;
	}
	if(id==0xFFFF)
	{
		gsensor_id=0;
	}
	local_irq_restore(flags);
	printk("Gsensor Chip Id(0x%x)", gsensor_id);
	return gsensor_id;
}
EXPORT_SYMBOL_GPL(cdc_gsensor_device_id);

static int	plsensor_id=0;
int cdc_plsensor_device_id(int id)
{
	unsigned long	flags;
	local_irq_save(flags);

	if(id!=0)
	{
		plsensor_id=id;
	}
	if(id==0xFFFF)
	{
		plsensor_id=0;
	}
	local_irq_restore(flags);
	printk("Plsensor Chip Id(0x%x)", plsensor_id);
	return plsensor_id;
}
EXPORT_SYMBOL_GPL(cdc_plsensor_device_id);

static int	fpsensor_id=0;
int cdc_fp_device_id(int id)
{
	unsigned long	flags;
	local_irq_save(flags);

	if(id!=0)
	{
		fpsensor_id=id;
	}
	if(id==0xFFFF)
	{
		fpsensor_id=0;
	}
	local_irq_restore(flags);
	printk("Fpsensor Chip Id(0x%x)", fpsensor_id);
	return fpsensor_id;
}
EXPORT_SYMBOL_GPL(cdc_fp_device_id);


