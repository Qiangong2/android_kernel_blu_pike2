#ifndef __WCN_INTEGRATE_BOOT_H__
#define __WCN_INTEGRATE_BOOT_H__

int start_integrate_wcn(u32 subsys);
int stop_integrate_wcn(u32 subsys);
int start_integrate_wcn_truely(u32 subsys);
int stop_integrate_wcn_truely(u32 subsys);
int wcn_proc_native_start(void *arg);
int wcn_proc_native_stop(void *arg);
void wcn_boot_init(void);
void wcn_power_wq(struct work_struct *pwork);

#endif
