#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/kernel.h>
#include <linux/slab.h>

//----------------------------------
#include "zyt_peripherals.h"

#include <linux/cdc_com.h>

/*���豸�ʹ��豸�ű���*/
static int zyt_peripherals_major = 0;
static int zyt_peripherals_minor = 0;

/*�豸�����豸����*/
static struct class* zyt_peripherals_class = NULL;
static struct zyt_peripherals_dev* peripherals_dev = NULL;
static struct proc_dir_entry *entry = NULL;
#define ww_NULL ((int)0)


/*��ͳ���豸�ļ���������*/
static int zyt_peripherals_open(struct inode* inode, struct file* filp);
static int zyt_peripherals_release(struct inode* inode, struct file* filp);
static ssize_t zyt_peripherals_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos);
static ssize_t zyt_peripherals_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos);

/*�豸�ļ�����������*/
static struct file_operations zyt_peripherals_fops = {
	.owner = THIS_MODULE,
	.open = zyt_peripherals_open,
	.release = zyt_peripherals_release,
	.read = zyt_peripherals_read,
	.write = zyt_peripherals_write, 
};

/*�����������Է���*/
static ssize_t zyt_peripherals_val_show(struct device* dev, struct device_attribute* attr,  char* buf);
static ssize_t zyt_peripherals_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count);

/*�����豸����*/
static DEVICE_ATTR(val, 0664, zyt_peripherals_val_show, zyt_peripherals_val_store);

/********************************************************************for check or get camera id and name******************************************************************/
char main_camera_id[32]={0};
char sub_camera_id[32]={0};
char main_ext_camera_id[32]={0};
char sub_ext_camera_id[32]={0};

/*�����������Է���*/
static ssize_t ww6_main_camera_id_show(struct device* dev, struct device_attribute* attr, char* buf) 
{

	return sprintf(buf, "%s\n", main_camera_id);

}

static ssize_t ww6_main_camera_id_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) 
{

	sprintf(main_camera_id,"%s\n",buf);
	
	return count;

}

static ssize_t ww6_sub_camera_id_show(struct device* dev, struct device_attribute* attr, char* buf) 
{
	return sprintf(buf, "%s\n", sub_camera_id);
}

static ssize_t ww6_sub_camera_id_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) 
{
	sprintf(sub_camera_id,"%s\n",buf);

	return count;

}
static ssize_t ww6_main_ext_camera_id_show(struct device* dev, struct device_attribute* attr, char* buf) 
{
	return sprintf(buf, "%s\n", main_ext_camera_id);
}

static ssize_t ww6_main_ext_camera_id_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) 
{
	sprintf(main_ext_camera_id,"%s\n",buf);

	return count;

}
static ssize_t ww6_sub_ext_camera_id_show(struct device* dev, struct device_attribute* attr, char* buf) 
{
	return sprintf(buf, "%s\n", sub_ext_camera_id);
}

static ssize_t ww6_sub_ext_camera_id_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) 
{
	sprintf(sub_ext_camera_id,"%s\n",buf);

	return count;

}


/*�����豸����*/
static DEVICE_ATTR(sensor_main, 0664, ww6_main_camera_id_show, ww6_main_camera_id_store);
static DEVICE_ATTR(sensor_sub, 0664, ww6_sub_camera_id_show, ww6_sub_camera_id_store);
static DEVICE_ATTR(sensor_main_ext, 0664, ww6_main_ext_camera_id_show, ww6_main_ext_camera_id_store);
static DEVICE_ATTR(sensor_sub_ext, 0664, ww6_sub_ext_camera_id_show, ww6_sub_ext_camera_id_store);
/**************************************************************************************************************************************/

//----------------------------------------------------------------------------------

/*���豸����*/
static int zyt_peripherals_open(struct inode* inode, struct file* filp) {
	struct zyt_peripherals_dev* dev;        
	
	/*���Զ����豸�ṹ�屣�����ļ�ָ���˽���������У��Ա�����豸ʱ������*/
	dev = container_of(inode->i_cdev, struct zyt_peripherals_dev, dev);
	filp->private_data = dev;
	
	return 0;
}

/*�豸�ļ��ͷ�ʱ���ã���ʵ��*/
static int zyt_peripherals_release(struct inode* inode, struct file* filp) {
	return 0;
}

/*��ȡ�豸�ļĴ���val��ֵ*/
static ssize_t zyt_peripherals_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos) {
	ssize_t err = 0;
	struct zyt_peripherals_dev* dev = filp->private_data;        

	/*ͬ������*/
	if(down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}

	if(count < sizeof(dev->val)) {
		goto out;
	}        

	/*���Ĵ���val��ֵ�������û��ṩ�Ļ�����*/
	if(copy_to_user(buf, &(dev->val), sizeof(dev->val))) {
		err = -EFAULT;
		goto out;
	}

	err = sizeof(dev->val);

out:
	up(&(dev->sem));
	return err;
}

/*д�豸�ļĴ���ֵval*/
static ssize_t zyt_peripherals_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos) {
	struct zyt_peripherals_dev* dev = filp->private_data;
	ssize_t err = 0;        

	/*ͬ������*/
	if(down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;        
	}        

	if(count != sizeof(dev->val)) {
		goto out;        
	}        

	/*���û��ṩ�Ļ�������ֵд���豸�Ĵ���ȥ*/
	if(copy_from_user(&(dev->val), buf, count)) {
		err = -EFAULT;
		goto out;
	}

	err = sizeof(dev->val);

out:
	up(&(dev->sem));
	return err;
}
#ifdef USE_ZYT_PERIPHERALS
//----------------------------------------------------------------------------------
//extern char zyt_peripherals_LCD_name[];
char zyt_peripherals_LCD_name[]="Don't use";

// ������Ϣ��Ҫ�� Sensor_drv_u.c�еĶ��屣��һ��
//#define ZYT_PERIPHERALS_SENSOR_PARA_MAIN	"/data/misc/media/zyt_peripherals.sensorMain"
//#define ZYT_PERIPHERALS_SENSOR_PARA_SUB	"/data/misc/media/zyt_peripherals.sensorSub"

#define ZYT_PERIPHERALS_SENSOR_PARA_MAIN	"/sys/class/zyt_peripherals/zyt_peripherals/sensor_main"
#define ZYT_PERIPHERALS_SENSOR_PARA_SUB		"/sys/class/zyt_peripherals/zyt_peripherals/sensor_sub"
#define ZYT_PERIPHERALS_SENSOR_PARA_MAIN_EXT	"/sys/class/zyt_peripherals/zyt_peripherals/sensor_main_ext"
#define ZYT_PERIPHERALS_SENSOR_PARA_SUB_EXT		"/sys/class/zyt_peripherals/zyt_peripherals/sensor_sub_ext"

#define ZYT_SENSOR_MAIN_FLAG "SENSOR_MAIN:"
#define ZYT_SENSOR_SUB_FLAG "SENSOR_SUB:"
#define ZYT_SENSOR_MAIN_EXT_FLAG "SENSOR_MAIN_EXT:"
#define ZYT_SENSOR_SUB_EXT_FLAG "SENSOR_SUB_EXT:"
#define ZYT_SENSOR_END_FLAG ';'
//--

char sensor_name_buf[32] = {0};

//static char Flash_id[32] = {0};

static char old_peripherals_info[256] = {0};
static int b_has_got_peripherals_info = 0;
static int b_has_read_peripherals_info = 0;

//��������ͷ�ļ���Ϣ

static char* _zyt_peripherals_parse_sensor_info(E_ZYT_PERIPHERALS_TYPE sensor_type)
{	
	struct file* fp;
	mm_segment_t fs;
	loff_t pos = 0;

	char *temp = NULL;
	char *search_flag = NULL;
	
	char buf_file[32] = {0};
	int i = 0;

	printk("%s,sensor_type=%d", __func__, sensor_type);

	memset(sensor_name_buf, 0, sizeof(sensor_name_buf));

	//��� SENSOR_MAIN ��Ϣ
	if(TYPE_CAMERA_MAIN == sensor_type)
	{
		fp = filp_open(ZYT_PERIPHERALS_SENSOR_PARA_MAIN, O_RDONLY, 0444);
		if(IS_ERR(fp))
		{
			printk(KERN_ALERT"Set to Unknown-file MAIN not exist.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
		else
		{
			printk(KERN_ALERT"Sensor_main file exist.\n");

			fs = get_fs();
			set_fs(KERNEL_DS);

			//vfs_read(fp, buf_file, sizeof(buf_file), &pos);
			fp->f_op->read(fp, buf_file, sizeof(buf_file), &fp->f_pos);
			
			set_fs(fs);

			filp_close(fp, NULL);
		}
		printk(KERN_ALERT"buf_file=%s.\n", buf_file);
		search_flag = strstr(buf_file, ZYT_SENSOR_MAIN_FLAG);
		if(NULL != search_flag)
		{
			temp = search_flag + strlen(ZYT_SENSOR_MAIN_FLAG);
			
			while(ZYT_SENSOR_END_FLAG != *temp)
			{
				sensor_name_buf[i++] = *temp;
				temp++;
			}
			printk(KERN_ALERT"Get SENSOR_MAIN str:%s.\n", sensor_name_buf);
			return sensor_name_buf;
		}
		else
		{
			printk(KERN_ALERT"Set to Unknown-search_flag null.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
	}

	//��� SENSOR_SUB ��Ϣ
	if(TYPE_CAMERA_SUB == sensor_type)
	{
		fp = filp_open(ZYT_PERIPHERALS_SENSOR_PARA_SUB, O_RDONLY, 0444);
		if(IS_ERR(fp))
		{
			printk(KERN_ALERT"Set to Unknown-file SUB not exist.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
		else
		{
			printk(KERN_ALERT"Sensor_sub file exist.\n");

			fs = get_fs();
			set_fs(KERNEL_DS);

			vfs_read(fp, buf_file, sizeof(buf_file), &pos);
			
			set_fs(fs);

			filp_close(fp, NULL);
		}
		printk(KERN_ALERT"buf_file=%s.\n", buf_file);
		search_flag = strstr(buf_file, ZYT_SENSOR_SUB_FLAG);
		if(NULL != search_flag)
		{
			temp = search_flag + strlen(ZYT_SENSOR_SUB_FLAG);
			
			while(ZYT_SENSOR_END_FLAG != *temp)
			{
				sensor_name_buf[i++] = *temp;
				temp++;
			}
			printk(KERN_ALERT"Get SENSOR_SUB str:%s.\n", sensor_name_buf);
			return sensor_name_buf;
		}
		else
		{
			printk(KERN_ALERT"Set to Unknown-search_flag null.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
	}

	//��� SENSOR_MAIN_EXT ��Ϣ
	if(TYPE_CAMERA_MAIN_EXT== sensor_type)
	{
		fp = filp_open(ZYT_PERIPHERALS_SENSOR_PARA_MAIN_EXT, O_RDONLY, 0444);
		if(IS_ERR(fp))
		{
			printk(KERN_ALERT"Set to Unknown-file MAIN EXT not exist.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
		else
		{
			printk(KERN_ALERT"Sensor_main EXT file exist.\n");

			fs = get_fs();
			set_fs(KERNEL_DS);

			//vfs_read(fp, buf_file, sizeof(buf_file), &pos);
			fp->f_op->read(fp, buf_file, sizeof(buf_file), &fp->f_pos);
			
			set_fs(fs);

			filp_close(fp, NULL);
		}
		printk(KERN_ALERT"buf_file=%s.\n", buf_file);
		search_flag = strstr(buf_file, ZYT_SENSOR_MAIN_EXT_FLAG);
		if(NULL != search_flag)
		{
			temp = search_flag + strlen(ZYT_SENSOR_MAIN_EXT_FLAG);
			
			while(ZYT_SENSOR_END_FLAG != *temp)
			{
				sensor_name_buf[i++] = *temp;
				temp++;
			}
			printk(KERN_ALERT"Get SENSOR_MAIN EXT str:%s.\n", sensor_name_buf);
			return sensor_name_buf;
		}
		else
		{
			printk(KERN_ALERT"Set to Unknown-search_flag null.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
	}

	//��� SENSOR_SUB_EXT ��Ϣ
	if(TYPE_CAMERA_SUB_EXT== sensor_type)
	{
		fp = filp_open(ZYT_PERIPHERALS_SENSOR_PARA_SUB_EXT, O_RDONLY, 0444);
		if(IS_ERR(fp))
		{
			printk(KERN_ALERT"Set to Unknown-file SUB EXT not exist.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
		else
		{
			printk(KERN_ALERT"Sensor_sub EXT file exist.\n");

			fs = get_fs();
			set_fs(KERNEL_DS);

			vfs_read(fp, buf_file, sizeof(buf_file), &pos);
			
			set_fs(fs);

			filp_close(fp, NULL);
		}
		printk(KERN_ALERT"buf_file=%s.\n", buf_file);
		search_flag = strstr(buf_file, ZYT_SENSOR_SUB_EXT_FLAG);
		if(NULL != search_flag)
		{
			temp = search_flag + strlen(ZYT_SENSOR_SUB_EXT_FLAG);
			
			while(ZYT_SENSOR_END_FLAG != *temp)
			{
				sensor_name_buf[i++] = *temp;
				temp++;
			}
			printk(KERN_ALERT"Get SENSOR_SUB EXT str:%s.\n", sensor_name_buf);
			return sensor_name_buf;
		}
		else
		{
			printk(KERN_ALERT"Set to Unknown-search_flag null.\n");
			return ZYT_PERIPHERALS_UNKNOWN;
		}
	}

	return ZYT_PERIPHERALS_UNKNOWN;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char TP_FW_VER=1;
char TP_NAME[20];

u8 *TP_FW_VER_MA = NULL;

/*ͨ��id��ȡ�豸���֣��ڲ�ʹ��*/
static int __zyt_peripherals_get_name_by_id(int id, char *name, E_ZYT_PERIPHERALS_TYPE type)
{
	int i = 0;
	
	if(NULL == name)
		return 0;

	switch(type)
	{
		case TYPE_TP:
			{
				int total_currType_num = sizeof(zyt_peripherals_TP_info)/sizeof(PERIPHERALS_ID_2_NAME);

				for(i = 0; i < total_currType_num; i++)
				{
					if(id == zyt_peripherals_TP_info[i].id)
					{
					if (0x6206== zyt_peripherals_TP_info[i].id)
						sprintf(name, "[TP]%s%s%x",TP_NAME," FwVer:0x",TP_FW_VER);//
					else if (0x2133== zyt_peripherals_TP_info[i].id)
						sprintf(name, "[TP]%s%s%s",TP_NAME," FwVer:",TP_FW_VER_MA);
					else
						sprintf(name, "[TP]%s", zyt_peripherals_TP_info[i].name);
						break;
					}
				}

				if(i == total_currType_num)
				{
					sprintf(name, "[TP]%s", zyt_peripherals_TP_info[i-1].name);
				}
			}
			break;

		case TYPE_LCD:
			{
				//printk(KERN_ALERT"strlen(zyt_peripherals_LCD_name): %d.\n", strlen(zyt_peripherals_LCD_name));	
				if(0 < strlen(zyt_peripherals_LCD_name))
				{
					sprintf(name, "[LCD]%s", zyt_peripherals_LCD_name);
				}
				else
				{
					sprintf(name, "[LCD]%s", ZYT_PERIPHERALS_UNKNOWN);
				}
			}
			break;

		case TYPE_CAMERA_MAIN:
			{
				printk(KERN_ALERT"get_name_by_id [CAMERA_MAIN]: %s.\n", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_MAIN));	
				sprintf(name , "[CAMERA_MAIN]%s", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_MAIN));
			}
			break;

		case TYPE_CAMERA_SUB:
			{
				printk(KERN_ALERT"get_name_by_id [CAMERA_SUB]: %s.\n", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_SUB));	
				sprintf(name , "[CAMERA_SUB]%s", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_SUB));
			}
			break;
		case TYPE_CAMERA_MAIN_EXT:
			{
				printk(KERN_ALERT"get_name_by_id [CAMERA_MAIN_EXT]: %s.\n", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_MAIN_EXT));	
				sprintf(name , "[CAMERA_MAIN_EXT]%s", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_MAIN_EXT));
			}
			break;

		case TYPE_CAMERA_SUB_EXT:
			{
				printk(KERN_ALERT"get_name_by_id [CAMERA_SUB_EXT]: %s.\n", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_SUB_EXT));	
				sprintf(name , "[CAMERA_SUB_EXT]%s", _zyt_peripherals_parse_sensor_info(TYPE_CAMERA_SUB_EXT));
			}
			break;
		/*case TYPE_FLASH:
			{
				int total_currType_num = sizeof(zyt_peripherals_FLASH_info)/sizeof(FLASH_ID_2_NAME);

				for(i = 0; i < total_currType_num; i++)
				{
					if(0 == strcmp((char*)id, zyt_peripherals_FLASH_info[i].id))
					{
						sprintf(name, "[FLASH]%s", zyt_peripherals_FLASH_info[i].name);
						break;
					}
				}

				if(i == total_currType_num)
				{
					sprintf(name, "[FLASH]%s", zyt_peripherals_FLASH_info[i-1].name);
				}
			}
			break;
      */
		case TYPE_GSENSOR:
			{
				#if 1//def CONFIG_ZYT_GSENSOR_COMPATIBLE
				int total_currType_num = sizeof(zyt_peripherals_GSENSOR_info)/sizeof(PERIPHERALS_ID_2_NAME);
				printk(KERN_ALERT"TYPE_GSENSOR id=0x%x.\n", (int)id);

				for(i = 0; i < total_currType_num; i++)
				{
					if((int)id == zyt_peripherals_GSENSOR_info[i].id)
					{
						sprintf(name, "[GSENSOR]%s", zyt_peripherals_GSENSOR_info[i].name);
						break;
					}
				}

				if(i == total_currType_num)
				{
					sprintf(name, "[GSENSOR]%s", zyt_peripherals_GSENSOR_info[i-1].name);
				}
				#else
					sprintf(name, "[GSENSOR]%s", ZYT_PERIPHERALS_NOT_SUPP);
				#endif
			}
			break;

		case TYPE_PLSENSOR:
			{
				#if 1//def CONFIG_ZYT_PLSENSOR_COMPATIBLE
				int total_currType_num = sizeof(zyt_peripherals_PLSENSOR_info)/sizeof(PERIPHERALS_ID_2_NAME);
				printk(KERN_ALERT"TYPE_PLSENSOR id=0x%x.\n", (int)id);

				for(i = 0; i < total_currType_num; i++)
				{
					if((int)id == zyt_peripherals_PLSENSOR_info[i].id)
					{
						sprintf(name, "[PLSENSOR]%s", zyt_peripherals_PLSENSOR_info[i].name);
						break;
					}
				}

				if(i == total_currType_num)
				{
					sprintf(name, "[PLSENSOR]%s", zyt_peripherals_PLSENSOR_info[i-1].name);
				}
				#else
					sprintf(name, "[PLSENSOR]%s", ZYT_PERIPHERALS_NOT_SUPP);
				#endif
			}
			break;
		case TYPE_FP:
			{
				int total_currType_num = sizeof(zyt_peripherals_FP_info)/sizeof(PERIPHERALS_ID_2_NAME);
				printk(KERN_ALERT"TYPE_FPSENSOR id=0x%x.\n", (int)id);
				
				for(i = 0; i < total_currType_num; i++)
				{
					if((int)id == zyt_peripherals_FP_info[i].id)
					{
						sprintf(name, "[FPSENSOR]%s", zyt_peripherals_FP_info[i].name);
						break;
					}
				}
		
				if(i == total_currType_num)
				{
					sprintf(name, "[FPSENSOR]%s", zyt_peripherals_TP_info[i-1].name);
				}
			}
			break;

		default:
			break;
	}
//	printk(KERN_ALERT"strlen(name): %d.\n", strlen(name));		
	return strlen(name);
}

/*��ȡ�Ĵ���peripherals_info��ֵ��������buf�У��ڲ�ʹ��*/
static ssize_t __zyt_peripherals_get_peripherals_info(struct zyt_peripherals_dev* dev, char* buf) {
	int len = 0;
	int ascending_len = 0;

	/*ͬ������*/
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}    

	if(0 == b_has_got_peripherals_info)
	{
		printk(KERN_ALERT"First time to get peripherals info.\n");	
		
		//�������
		memset(dev->peripherals_info, 0, sizeof(dev->peripherals_info));
		memset(old_peripherals_info, 0, sizeof(old_peripherals_info));

		//for TP name
		len = __zyt_peripherals_get_name_by_id(cdc_tp_device_id(0), dev->peripherals_info, TYPE_TP);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		//for LCD name
		len = __zyt_peripherals_get_name_by_id(ww_NULL, dev->peripherals_info+ascending_len, TYPE_LCD);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		//for main CAMERA name
		len = __zyt_peripherals_get_name_by_id(ww_NULL, dev->peripherals_info+ascending_len, TYPE_CAMERA_MAIN);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';
		//for sub CAMERA name
		len = __zyt_peripherals_get_name_by_id(ww_NULL, dev->peripherals_info+ascending_len, TYPE_CAMERA_SUB);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';
		//for main ext CAMERA name
		len = __zyt_peripherals_get_name_by_id(ww_NULL, dev->peripherals_info+ascending_len, TYPE_CAMERA_MAIN_EXT);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';
		//for sub ext CAMERA name
		len = __zyt_peripherals_get_name_by_id(ww_NULL, dev->peripherals_info+ascending_len, TYPE_CAMERA_SUB_EXT);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		//for GSENSOR name
		len = __zyt_peripherals_get_name_by_id(cdc_gsensor_device_id(0), dev->peripherals_info+ascending_len, TYPE_GSENSOR);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		//for PLSENSOR name
		len = __zyt_peripherals_get_name_by_id(cdc_plsensor_device_id(0), dev->peripherals_info+ascending_len, TYPE_PLSENSOR);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		#if 0
		//for FLASH name
		len = __zyt_peripherals_get_name_by_id((void*)Flash_id, dev->peripherals_info+ascending_len, TYPE_FLASH);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';
		#endif

		//for FP name
		len = __zyt_peripherals_get_name_by_id(cdc_fp_device_id(0), dev->peripherals_info+ascending_len, TYPE_FP);
		ascending_len += len;
		dev->peripherals_info[ascending_len++] = ';';

		//���������
		//b_has_got_peripherals_info = 1;
		strncpy(old_peripherals_info, dev->peripherals_info, strlen(dev->peripherals_info));
	}
	else
	{
		printk(KERN_ALERT"Return old peripherals info.\n");	
		strncpy(dev->peripherals_info, old_peripherals_info, strlen(old_peripherals_info));
	}

	up(&(dev->sem));        

	return snprintf(buf, PAGE_SIZE, "%s\n", dev->peripherals_info);
}
#endif

#ifndef USE_ZYT_PERIPHERALS
/*��ȡ�Ĵ���val��ֵ��������buf�У��ڲ�ʹ��*/
static ssize_t __zyt_peripherals_get_val(struct zyt_peripherals_dev* dev, char* buf) {
	int val = 0;    
	printk(KERN_ALERT"__zyt_peripherals_get_val .\n");

	/*ͬ������*/
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}        
	
	val = dev->val;   
	up(&(dev->sem));	   

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}
#endif

/*�ѻ�����buf��ֵд���豸�Ĵ���val��ȥ���ڲ�ʹ��*/
static ssize_t __zyt_peripherals_set_val(struct zyt_peripherals_dev* dev, const char* buf, size_t count) {
	int val = 0;        
	printk(KERN_ALERT"__zyt_peripherals_set_val .\n");	

	/*���ַ���ת��������*/        
	val = simple_strtol(buf, NULL, 10);        

	/*ͬ������*/        
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}        

	dev->val = val;        
	up(&(dev->sem));

	return count;
}

/*��ȡ�豸����val*/
static ssize_t zyt_peripherals_val_show(struct device* dev, struct device_attribute* attr, char* buf) {
	struct zyt_peripherals_dev* hdev = (struct zyt_peripherals_dev*)dev_get_drvdata(dev);        
	printk(KERN_ALERT"zyt_peripherals_val_show b_has_read_peripherals_info = %d.\n",b_has_read_peripherals_info);	

	#ifdef USE_ZYT_PERIPHERALS
	return __zyt_peripherals_get_peripherals_info(hdev, buf);
	#else
	return __zyt_peripherals_get_val(hdev, buf);
	#endif
}

/*д�豸����val*/
static ssize_t zyt_peripherals_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) { 
	struct zyt_peripherals_dev* hdev = (struct zyt_peripherals_dev*)dev_get_drvdata(dev);  
	printk(KERN_ALERT"zyt_peripherals_val_store .\n");	
	return __zyt_peripherals_set_val(hdev, buf, count);
}
//----------------------------------------------------------------------------------

/*��ȡ�豸�Ĵ���val��ֵ��������page��������*/
static ssize_t zyt_peripherals_proc_read(struct file* filp, char __user *page, size_t count, loff_t* f_pos) {
	int ret = 0;  
	printk(KERN_ALERT"zyt_peripherals_proc_read b_has_read_peripherals_info = %d.\n",b_has_read_peripherals_info);  
	if(b_has_read_peripherals_info > 0) {
		b_has_read_peripherals_info = 0;
		return 0;
	}
	
	#ifdef USE_ZYT_PERIPHERALS
	ret = __zyt_peripherals_get_peripherals_info(peripherals_dev, page);
	#else
	ret = __zyt_peripherals_get_val(peripherals_dev, page);
	#endif
	b_has_read_peripherals_info = 1;
	return ret;
}

/*�ѻ�������ֵbuff���浽�豸�Ĵ���val��ȥ*/
static ssize_t zyt_peripherals_proc_write(struct file* filp, const char __user *buff, size_t len, loff_t *data) {
	int err = 0;
	char* page = NULL;

//	printk(KERN_ALERT"zyt_peripherals_proc_write len = %d.\n", len);
	
	if(len > PAGE_SIZE) {
		//printk(KERN_ALERT"The buff is too large: %lu.\n", len);
		return -EFAULT;
	}

	page = (char*)__get_free_page(GFP_KERNEL);
	if(!page) {                
		printk(KERN_ALERT"Failed to alloc page.\n");
		return -ENOMEM;
	}        

	/*�Ȱ��û��ṩ�Ļ�����ֵ�������ں˻�������ȥ*/
	if(copy_from_user(page, buff, len)) {
		printk(KERN_ALERT"Failed to copy buff from user.\n");                
		err = -EFAULT;
		goto out;
	}

	err = __zyt_peripherals_set_val(peripherals_dev, page, len);

out:
	free_page((unsigned long)page);
	return err;
}

static const struct file_operations zyt_peripherals_proc_ops = {
	.read = zyt_peripherals_proc_read,
	.write = zyt_peripherals_proc_write,
};

/*����/proc/zyt_peripherals�ļ�*/
static void zyt_peripherals_create_proc(void) {
	struct proc_dir_entry *entry;
	printk(KERN_ALERT"zyt_peripherals_create_proc .\n");	
	entry = proc_create(ZYT_PERIPHERALS_DEVICE_PROC_NAME, 0, NULL, &zyt_peripherals_proc_ops);
	if(entry) {
		//entry->owner = THIS_MODULE;
		//entry->read_proc = zyt_peripherals_proc_read;
		//entry->write_proc = zyt_peripherals_proc_write;
	}
}

/*ɾ��/proc/zyt_peripherals�ļ�*/
static void zyt_peripherals_remove_proc(void) {
	printk(KERN_ALERT"zyt_peripherals_remove_proc .\n");

	//remove_proc_entry(ZYT_PERIPHERALS_DEVICE_PROC_NAME, NULL);
	proc_remove(entry);
}

//----------------------------------------------------------------------------------
/*��ʼ���豸*/
static int  __zyt_peripherals_setup_dev(struct zyt_peripherals_dev* dev) {
	int err;
	dev_t devno = MKDEV(zyt_peripherals_major, zyt_peripherals_minor);

	memset(dev, 0, sizeof(struct zyt_peripherals_dev));

	cdev_init(&(dev->dev), &zyt_peripherals_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &zyt_peripherals_fops;        

	/*ע���ַ��豸*/
	err = cdev_add(&(dev->dev),devno, 1);
	if(err) {
		return err;
	}        

	/*��ʼ���ź����ͼĴ���val��ֵ*/
	//init_MUTEX(&(dev->sem));
	sema_init(&(dev->sem), 1);
	dev->val = 0;

	return 0;
}

/*ģ����ط���*/
static int __init zyt_peripherals_init(void){ 
	int err = -1;
	dev_t dev = 0;
	struct device* temp = NULL;

	printk(KERN_ALERT"Initializing zyt_peripherals device.\n");        

	/*��̬�������豸�ʹ��豸��*/
	err = alloc_chrdev_region(&dev, 0, 1, ZYT_PERIPHERALS_DEVICE_NODE_NAME);
	if(err < 0) {
		printk(KERN_ALERT"Failed to alloc char dev region.\n");
		goto fail;
	}

	zyt_peripherals_major = MAJOR(dev);
	zyt_peripherals_minor = MINOR(dev);        

	/*����zyt_peripherals_dev �豸�ṹ�����*/
	peripherals_dev = kzalloc(sizeof(struct zyt_peripherals_dev), GFP_KERNEL);
	if(!peripherals_dev) {
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc peripherals_dev.\n");
		goto unregister;
	}        

	/*��ʼ���豸*/
	err = __zyt_peripherals_setup_dev(peripherals_dev);
	if(err) {
		printk(KERN_ALERT"Failed to setup dev: %d.\n", err);
		goto cleanup;
	}        

	/*��/sys/class/Ŀ¼�´����豸���Ŀ¼zyt_peripherals*/
	zyt_peripherals_class = class_create(THIS_MODULE, ZYT_PERIPHERALS_DEVICE_CLASS_NAME);
	if(IS_ERR(zyt_peripherals_class)) {
		err = PTR_ERR(zyt_peripherals_class);
		printk(KERN_ALERT"Failed to create zyt_peripherals class.\n");
		goto destroy_cdev;
	}        

	/*��/dev/Ŀ¼��/sys/class/zyt_peripheralsĿ¼�·ֱ𴴽��豸�ļ�zyt_peripherals*/
	temp = device_create(zyt_peripherals_class, NULL, dev, "%s", ZYT_PERIPHERALS_DEVICE_FILE_NAME);
	if(IS_ERR(temp)) {
		err = PTR_ERR(temp);
		printk(KERN_ALERT"Failed to create zyt_peripherals device.");
		goto destroy_class;
	}        

	/*��/sys/class/zyt_peripherals/zyt_peripheralsĿ¼�´��������ļ�val*/
	err = device_create_file(temp, &dev_attr_val);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute val.");                
		goto destroy_device;
	}

	/*��/sys/class/zyt_peripherals/zyt_peripheralsĿ¼�´��������ļ�sensor_main*/
	err = device_create_file(temp, &dev_attr_sensor_main);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_main.");                
		goto destroy_device;
	}

	/*��/sys/class/zyt_peripherals/zyt_peripheralsĿ¼�´��������ļ�sensor_sub*/
	err = device_create_file(temp, &dev_attr_sensor_sub);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_sub.");                
		goto destroy_device;
	}

	/*��/sys/class/zyt_peripherals/zyt_peripheralsĿ¼�´��������ļ�sensor_main_ext*/
	err = device_create_file(temp, &dev_attr_sensor_main_ext);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_main_ext.");                
		goto destroy_device;
	}

	/*��/sys/class/zyt_peripherals/zyt_peripheralsĿ¼�´��������ļ�sensor_sub_ext*/
	err = device_create_file(temp, &dev_attr_sensor_sub_ext);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_sub_ext.");                
		goto destroy_device;
	}

	dev_set_drvdata(temp, peripherals_dev);        

	/*����/proc/zyt_peripherals�ļ�*/
	zyt_peripherals_create_proc();

	printk(KERN_ALERT"Succedded to initialize zyt_peripherals device.\n");
	return 0;

destroy_device:
	device_destroy(zyt_peripherals_class, dev);

destroy_class:
	class_destroy(zyt_peripherals_class);

destroy_cdev:
	cdev_del(&(peripherals_dev->dev));

cleanup:
	kfree(peripherals_dev);

unregister:
	unregister_chrdev_region(MKDEV(zyt_peripherals_major, zyt_peripherals_minor), 1);

fail:
	return err;
}

/*ģ��ж�ط���*/
static void __exit zyt_peripherals_exit(void) {
	dev_t devno = MKDEV(zyt_peripherals_major, zyt_peripherals_minor);

	printk(KERN_ALERT"Destroy zyt_peripherals device.\n");        

	/*ɾ��/proc/zyt_peripherals�ļ�*/
	zyt_peripherals_remove_proc();        

	/*�����豸�����豸*/
	if(zyt_peripherals_class) {
		device_destroy(zyt_peripherals_class, MKDEV(zyt_peripherals_major, zyt_peripherals_minor));
		class_destroy(zyt_peripherals_class);
	}        

	/*ɾ���ַ��豸���ͷ��豸�ڴ�*/
	if(peripherals_dev) {
		cdev_del(&(peripherals_dev->dev));
		kfree(peripherals_dev);
	}        

	/*�ͷ��豸��*/
	unregister_chrdev_region(devno, 1);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ZYT Peripherals Driver");

module_init(zyt_peripherals_init);
module_exit(zyt_peripherals_exit);


