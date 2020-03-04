#ifndef _ZYT_PERIPHERALS_H_
#define _ZYT_PERIPHERALS_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define USE_ZYT_PERIPHERALS

#define ZYT_PERIPHERALS_DEVICE_NODE_NAME  "zyt_peripherals"
#define ZYT_PERIPHERALS_DEVICE_FILE_NAME  "zyt_peripherals"
#define ZYT_PERIPHERALS_DEVICE_PROC_NAME  "zyt_peripherals"
#define ZYT_PERIPHERALS_DEVICE_CLASS_NAME "zyt_peripherals"

#define ZYT_PERIPHERALS_UNKNOWN	"Unknown"
#define ZYT_PERIPHERALS_NOT_SUPP	"Not support"

//摄像头枚举
typedef enum zyt_peripherals_sensor_type
{
	SENSOR_MAIN = 0,
	SENSOR_SUB,
	SENSOR_DEVICE2,
	SENSOR_DEVICE3,
	SENSOR_MAX
}E_ZYT_PERIPHERALS_SENSOR_TYPE;

//字符设备
struct zyt_peripherals_dev {
	int val;
	char peripherals_info[256];
	struct semaphore sem;
	struct cdev dev;
};

//外设类型枚举
typedef enum zyt_peripherals_type
{
	TYPE_TP = 0,
		
	TYPE_LCD,
	
	TYPE_CAMERA_MAIN,
	TYPE_CAMERA_SUB,	
	TYPE_CAMERA_MAIN_EXT,
	TYPE_CAMERA_SUB_EXT,
	
	TYPE_FLASH,

	TYPE_GSENSOR,

	TYPE_PLSENSOR,

	TYPE_FP,
	
	TYPE_MAX
}E_ZYT_PERIPHERALS_TYPE;

//外设信息结构体类型
typedef struct peripherals_ID_2_name
{
	int id;
	char name[32];
}PERIPHERALS_ID_2_NAME;

//FLASH信息结构体类型
typedef struct flash_ID_2_name
{
	char id[32];
	char name[128];
}FLASH_ID_2_NAME;

//TP 外设信息
const PERIPHERALS_ID_2_NAME zyt_peripherals_TP_info[] = 
{	
	//GT-汇顶
	{0x0818, "GT868"},
	{0x0960, "GT960"},
	{0x096f, "GT960F"},
	{0x5663, "GT5XXX"},

	//MSG-辰明
	{0x2133, "MSG2XXX"},
	
	//IT-联阳
	{0x7260, "IT7260"},
	
	//FT-敦泰
	{0x6206, "FT6X0X"},

	//GSL-思立微
	{0x1680, "GSL1680"},

	//TMA-赛普拉斯
	{0x0140, "TMA140"},
	
	//ZET-君耀
	{0x6221, "ZET622X"},

	//INC-集创
	{0x8711, "ICN87XX"},

	//NT-精瑞
	{0x1104, "NT11004"},

	//BF-比亚迪
	{0x6862, "BFXXXX"},

	//add your type here

	//default
	{0x0000, ZYT_PERIPHERALS_UNKNOWN},
};

//FLASH 信息
const FLASH_ID_2_NAME zyt_peripherals_FLASH_info[] = 
{
	//{ 注意<32byte, 注意<128byte },
	
	//Micro
	{"0x2c,0xbc,0x90,0x55,0x56", "0x2c,0xbc,0x90,0x55,0x56-Micro MT29C4G48MAZBAAKS-5WT"},
	{"0x2c,0xbc,0x90,0x66,0x54", "0x2c,0xbc,0x90,0x66,0x54-Micro MT29C4G48MAAHBAAKS-5WT"},

	//Samsung
	{"0xec,0xbc,0x00,0x66,0x56", "0xec,0xbc,0x00,0x66,0x56-Samsung "},
	{"0xec,0xbc,0x01,0x55,0x48", "0xec,0xbc,0x01,0x55,0x48-Samsung KA300U013A-BJJT"},

	//Toshiba
	{"0x98,0xbc,0x90,0x66,0x76", "0x98,0xbc,0x90,0x66,0x76-Toshiba TYCC0A211531KC/TYBC0A111557KC"},	
	
	//
	{"0xad,0xbc,0x90,0x55,0x54", "0xad,0xbc,0x90,0x55,0x54-?"},
	{"0x2c,0xb3,0x90,0x66,0x64", "0x2c,0xb3,0x90,0x66,0x64-?"},
	
	//default
	{"NULL", ZYT_PERIPHERALS_UNKNOWN},
};

//GSENSOR 外设信息
const PERIPHERALS_ID_2_NAME zyt_peripherals_GSENSOR_info[] = 
{
	//MMA-3D
	{0x8451, "MMA8451"},
	{0x8452, "MMA8452"},
	{0x8453, "MMA8453"},

	//MXC-2D
	{0x6225, "MXC622X"},

	//LIS-3D
	{0x33, "LIS3DH"},
	{0x32, "N2DMTR"},

	//KIONIX-3D
	{0x1013, "KIONIX"},

	//MC3XXX -3D
	{0x306D,"MC3XXX"},

	//MXC400X -3D
	{0x4005,"MXC4005XC"},
	
	//default
	{0x0000, ZYT_PERIPHERALS_UNKNOWN},
};

//PLSENSOR 外设信息
const PERIPHERALS_ID_2_NAME zyt_peripherals_PLSENSOR_info[] = 
{
	//AP
	{0x3212, "AP32XX"},

	//APDS
	{0x9901, "APDS990X"},

	//STK
	{0x3101, "STK31XX"},
	{0x3310, "STK3X1X"},

	//EPL
	{0x2182, "EPL2182"},
	{0x2590, "EPL259X"},

	//default
	{0x0000, "TP_PS"},
};

//FINGERPRINT 外设信息
const PERIPHERALS_ID_2_NAME zyt_peripherals_FP_info[] = 
{	//microarray
	{0x0121, "AFS121"},

	//jmetrics
	{0x3018, "JMT3018"},

	//icnt
	{0x7152, "ICNT7152"},
	
	//sunwave
	{0x0513, "SUNW513"},
		
	//default
	{0x0000, ZYT_PERIPHERALS_UNKNOWN},
};


#endif
