#ifndef _CDC_COM_H_
#define _CDC_COM_H_

#define __WW6_CDC_COM__

int cdc_tp_device_id(int id);
void cdc_tp_lock_mutex(void);
void cdc_tp_unlock_mutex(void);

int cdc_gsensor_device_id(int id);

int cdc_plsensor_device_id(int id);

int cdc_fp_device_id(int id);

#endif
