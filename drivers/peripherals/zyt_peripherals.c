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

/*主设备和从设备号变量*/
static int zyt_peripherals_major = 0;
static int zyt_peripherals_minor = 0;

/*设备类别和设备变量*/
static struct class* zyt_peripherals_class = NULL;
static struct zyt_peripherals_dev* peripherals_dev = NULL;
static struct proc_dir_entry *entry = NULL;
#define ww_NULL ((int)0)


/*传统的设备文件操作方法*/
static int zyt_peripherals_open(struct inode* inode, struct file* filp);
static int zyt_peripherals_release(struct inode* inode, struct file* filp);
static ssize_t zyt_peripherals_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos);
static ssize_t zyt_peripherals_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos);

/*设备文件操作方法表*/
static struct file_operations zyt_peripherals_fops = {
	.owner = THIS_MODULE,
	.open = zyt_peripherals_open,
	.release = zyt_peripherals_release,
	.read = zyt_peripherals_read,
	.write = zyt_peripherals_write, 
};

/*访问设置属性方法*/
static ssize_t zyt_peripherals_val_show(struct device* dev, struct device_attribute* attr,  char* buf);
static ssize_t zyt_peripherals_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count);

/*定义设备属性*/
static DEVICE_ATTR(val, 0664, zyt_peripherals_val_show, zyt_peripherals_val_store);

/********************************************************************for check or get camera id and name******************************************************************/
char main_camera_id[32]={0};
char sub_camera_id[32]={0};
char main_ext_camera_id[32]={0};
char sub_ext_camera_id[32]={0};

/*访问设置属性方法*/
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


/*定义设备属性*/
static DEVICE_ATTR(sensor_main, 0664, ww6_main_camera_id_show, ww6_main_camera_id_store);
static DEVICE_ATTR(sensor_sub, 0664, ww6_sub_camera_id_show, ww6_sub_camera_id_store);
static DEVICE_ATTR(sensor_main_ext, 0664, ww6_main_ext_camera_id_show, ww6_main_ext_camera_id_store);
static DEVICE_ATTR(sensor_sub_ext, 0664, ww6_sub_ext_camera_id_show, ww6_sub_ext_camera_id_store);
/**************************************************************************************************************************************/

//----------------------------------------------------------------------------------

/*打开设备方法*/
static int zyt_peripherals_open(struct inode* inode, struct file* filp) {
	struct zyt_peripherals_dev* dev;        
	
	/*将自定义设备结构体保存在文件指针的私有数据域中，以便访问设备时拿来用*/
	dev = container_of(inode->i_cdev, struct zyt_peripherals_dev, dev);
	filp->private_data = dev;
	
	return 0;
}

/*设备文件释放时调用，空实现*/
static int zyt_peripherals_release(struct inode* inode, struct file* filp) {
	return 0;
}

/*读取设备的寄存器val的值*/
static ssize_t zyt_peripherals_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos) {
	ssize_t err = 0;
	struct zyt_peripherals_dev* dev = filp->private_data;        

	/*同步访问*/
	if(down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}

	if(count < sizeof(dev->val)) {
		goto out;
	}        

	/*将寄存器val的值拷贝到用户提供的缓冲区*/
	if(copy_to_user(buf, &(dev->val), sizeof(dev->val))) {
		err = -EFAULT;
		goto out;
	}

	err = sizeof(dev->val);

out:
	up(&(dev->sem));
	return err;
}

/*写设备的寄存器值val*/
static ssize_t zyt_peripherals_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos) {
	struct zyt_peripherals_dev* dev = filp->private_data;
	ssize_t err = 0;        

	/*同步访问*/
	if(down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;        
	}        

	if(count != sizeof(dev->val)) {
		goto out;        
	}        

	/*将用户提供的缓冲区的值写到设备寄存器去*/
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

// 以下信息需要与 Sensor_drv_u.c中的定义保持一致
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

//解析摄像头文件信息

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

	//获得 SENSOR_MAIN 信息
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

	//获得 SENSOR_SUB 信息
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

	//获得 SENSOR_MAIN_EXT 信息
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

	//获得 SENSOR_SUB_EXT 信息
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

/*通过id获取设备名字，内部使用*/
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

/*读取寄存器peripherals_info的值到缓冲区buf中，内部使用*/
static ssize_t __zyt_peripherals_get_peripherals_info(struct zyt_peripherals_dev* dev, char* buf) {
	int len = 0;
	int ascending_len = 0;

	/*同步访问*/
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}    

	if(0 == b_has_got_peripherals_info)
	{
		printk(KERN_ALERT"First time to get peripherals info.\n");	
		
		//清空数据
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

		//保存旧数据
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
/*读取寄存器val的值到缓冲区buf中，内部使用*/
static ssize_t __zyt_peripherals_get_val(struct zyt_peripherals_dev* dev, char* buf) {
	int val = 0;    
	printk(KERN_ALERT"__zyt_peripherals_get_val .\n");

	/*同步访问*/
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}        
	
	val = dev->val;   
	up(&(dev->sem));	   

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}
#endif

/*把缓冲区buf的值写到设备寄存器val中去，内部使用*/
static ssize_t __zyt_peripherals_set_val(struct zyt_peripherals_dev* dev, const char* buf, size_t count) {
	int val = 0;        
	printk(KERN_ALERT"__zyt_peripherals_set_val .\n");	

	/*将字符串转换成数字*/        
	val = simple_strtol(buf, NULL, 10);        

	/*同步访问*/        
	if(down_interruptible(&(dev->sem))) {                
		return -ERESTARTSYS;        
	}        

	dev->val = val;        
	up(&(dev->sem));

	return count;
}

/*读取设备属性val*/
static ssize_t zyt_peripherals_val_show(struct device* dev, struct device_attribute* attr, char* buf) {
	struct zyt_peripherals_dev* hdev = (struct zyt_peripherals_dev*)dev_get_drvdata(dev);        
	printk(KERN_ALERT"zyt_peripherals_val_show b_has_read_peripherals_info = %d.\n",b_has_read_peripherals_info);	

	#ifdef USE_ZYT_PERIPHERALS
	return __zyt_peripherals_get_peripherals_info(hdev, buf);
	#else
	return __zyt_peripherals_get_val(hdev, buf);
	#endif
}

/*写设备属性val*/
static ssize_t zyt_peripherals_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) { 
	struct zyt_peripherals_dev* hdev = (struct zyt_peripherals_dev*)dev_get_drvdata(dev);  
	printk(KERN_ALERT"zyt_peripherals_val_store .\n");	
	return __zyt_peripherals_set_val(hdev, buf, count);
}
//----------------------------------------------------------------------------------

/*读取设备寄存器val的值，保存在page缓冲区中*/
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

/*把缓冲区的值buff保存到设备寄存器val中去*/
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

	/*先把用户提供的缓冲区值拷贝到内核缓冲区中去*/
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

/*创建/proc/zyt_peripherals文件*/
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

/*删除/proc/zyt_peripherals文件*/
static void zyt_peripherals_remove_proc(void) {
	printk(KERN_ALERT"zyt_peripherals_remove_proc .\n");

	//remove_proc_entry(ZYT_PERIPHERALS_DEVICE_PROC_NAME, NULL);
	proc_remove(entry);
}

//----------------------------------------------------------------------------------
/*初始化设备*/
static int  __zyt_peripherals_setup_dev(struct zyt_peripherals_dev* dev) {
	int err;
	dev_t devno = MKDEV(zyt_peripherals_major, zyt_peripherals_minor);

	memset(dev, 0, sizeof(struct zyt_peripherals_dev));

	cdev_init(&(dev->dev), &zyt_peripherals_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &zyt_peripherals_fops;        

	/*注册字符设备*/
	err = cdev_add(&(dev->dev),devno, 1);
	if(err) {
		return err;
	}        

	/*初始化信号量和寄存器val的值*/
	//init_MUTEX(&(dev->sem));
	sema_init(&(dev->sem), 1);
	dev->val = 0;

	return 0;
}

/*模块加载方法*/
static int __init zyt_peripherals_init(void){ 
	int err = -1;
	dev_t dev = 0;
	struct device* temp = NULL;

	printk(KERN_ALERT"Initializing zyt_peripherals device.\n");        

	/*动态分配主设备和从设备号*/
	err = alloc_chrdev_region(&dev, 0, 1, ZYT_PERIPHERALS_DEVICE_NODE_NAME);
	if(err < 0) {
		printk(KERN_ALERT"Failed to alloc char dev region.\n");
		goto fail;
	}

	zyt_peripherals_major = MAJOR(dev);
	zyt_peripherals_minor = MINOR(dev);        

	/*分配zyt_peripherals_dev 设备结构体变量*/
	peripherals_dev = kzalloc(sizeof(struct zyt_peripherals_dev), GFP_KERNEL);
	if(!peripherals_dev) {
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc peripherals_dev.\n");
		goto unregister;
	}        

	/*初始化设备*/
	err = __zyt_peripherals_setup_dev(peripherals_dev);
	if(err) {
		printk(KERN_ALERT"Failed to setup dev: %d.\n", err);
		goto cleanup;
	}        

	/*在/sys/class/目录下创建设备类别目录zyt_peripherals*/
	zyt_peripherals_class = class_create(THIS_MODULE, ZYT_PERIPHERALS_DEVICE_CLASS_NAME);
	if(IS_ERR(zyt_peripherals_class)) {
		err = PTR_ERR(zyt_peripherals_class);
		printk(KERN_ALERT"Failed to create zyt_peripherals class.\n");
		goto destroy_cdev;
	}        

	/*在/dev/目录和/sys/class/zyt_peripherals目录下分别创建设备文件zyt_peripherals*/
	temp = device_create(zyt_peripherals_class, NULL, dev, "%s", ZYT_PERIPHERALS_DEVICE_FILE_NAME);
	if(IS_ERR(temp)) {
		err = PTR_ERR(temp);
		printk(KERN_ALERT"Failed to create zyt_peripherals device.");
		goto destroy_class;
	}        

	/*在/sys/class/zyt_peripherals/zyt_peripherals目录下创建属性文件val*/
	err = device_create_file(temp, &dev_attr_val);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute val.");                
		goto destroy_device;
	}

	/*在/sys/class/zyt_peripherals/zyt_peripherals目录下创建属性文件sensor_main*/
	err = device_create_file(temp, &dev_attr_sensor_main);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_main.");                
		goto destroy_device;
	}

	/*在/sys/class/zyt_peripherals/zyt_peripherals目录下创建属性文件sensor_sub*/
	err = device_create_file(temp, &dev_attr_sensor_sub);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_sub.");                
		goto destroy_device;
	}

	/*在/sys/class/zyt_peripherals/zyt_peripherals目录下创建属性文件sensor_main_ext*/
	err = device_create_file(temp, &dev_attr_sensor_main_ext);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_main_ext.");                
		goto destroy_device;
	}

	/*在/sys/class/zyt_peripherals/zyt_peripherals目录下创建属性文件sensor_sub_ext*/
	err = device_create_file(temp, &dev_attr_sensor_sub_ext);
	if(err < 0) {
		printk(KERN_ALERT"Failed to create attribute sensor_sub_ext.");                
		goto destroy_device;
	}

	dev_set_drvdata(temp, peripherals_dev);        

	/*创建/proc/zyt_peripherals文件*/
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

/*模块卸载方法*/
static void __exit zyt_peripherals_exit(void) {
	dev_t devno = MKDEV(zyt_peripherals_major, zyt_peripherals_minor);

	printk(KERN_ALERT"Destroy zyt_peripherals device.\n");        

	/*删除/proc/zyt_peripherals文件*/
	zyt_peripherals_remove_proc();        

	/*销毁设备类别和设备*/
	if(zyt_peripherals_class) {
		device_destroy(zyt_peripherals_class, MKDEV(zyt_peripherals_major, zyt_peripherals_minor));
		class_destroy(zyt_peripherals_class);
	}        

	/*删除字符设备和释放设备内存*/
	if(peripherals_dev) {
		cdev_del(&(peripherals_dev->dev));
		kfree(peripherals_dev);
	}        

	/*释放设备号*/
	unregister_chrdev_region(devno, 1);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ZYT Peripherals Driver");

module_init(zyt_peripherals_init);
module_exit(zyt_peripherals_exit);


