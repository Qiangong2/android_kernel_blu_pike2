/***********************************Flash Type************************************/
/*Flash类型 默认为DDR2*/
//#define WW6_FLASH_TYPE_DDR3
/***********************************Flash Type************************************/


/***********************************LCD************************************/
#define CONFIG_FB_9340_XINGNUO_P70S
#define CONFIG_FB_GC9306_HG_CTC
/***********************************LCD************************************/


/************************************TP************************************/
/*TP宏控制,在kernel\drivers\input\touchscreen目录下添加(TP反向问题)*/


/************************************TP************************************/


/*****************************CAMERA DRIVER********************************/
//自定义宏控制，在vendor\sprd\open-source\libs\libcamera\sensor\目录下添加


//前后摄像头宏控制，用于vendor\sprd\open-source\libs\libcamera\sc8830\src\sensor_cfg.c文件
/*****BACK_CAMERA*****/
/*****BACK_CAMERA*****/

/*****FRONT_CAMERA*****/
/*****FRONT_CAMERA*****/

/*****************************CAMERA DRIVER********************************/
/*****************************BACKLIGHT CURRENT***************************/

/*****************************BACKLIGHT CURRENT***************************/

/*****************************KEYPAD-BACKLIGHT CURRENT***************************/

/*****************************BACKLIGHT CURRENT***************************/


/*****************************DOUBLE CAMERA FLASH LIGHT  CURRENT***************************/

/*****************************CAMERA FLASH LIGHT  CURRENT***************************/
/*****************************IR LED REMOTE CONTROL (HongWaiXian)***************************/

/*****************************IR LED REMOTE CONTROL (HongWaiXian)***************************/
/*****************************PESENSOR DISTANCE ENHANCE***************************/

/*****************************PESENSOR DISTANCE ENHANCE***************************/

//#define WW6_DRV_VIBRATOR_V	35

//判断A OR B板总宏
//#define WW6_JUDGE_BOARD_AB
//用来判断A OR B板的GPIO口
//#define GPIO_BOARD_DIFF 131

#define WW6_DRV_CAMERA_MAIN_SUB_EXCHANGE

// 0								/*15 ma*/
// (BIT(0))							/*30 ma*/
// (BIT(1))							/*45 ma*/
// (BIT(0)|BIT(1))					/*60 ma*/
// (BIT(2))							/*75 ma*/
// (BIT(2)|BIT(0))					/*90 ma*/
// (BIT(2)|BIT(1))					/*105 ma*/
// (BIT(2)|BIT(1)|BIT(0))			/*120 ma*/
// (BIT(3))							/*135 ma*/
// (BIT(3)|BIT(0))					/*150 ma*/
// (BIT(3)|BIT(1))					/*165 ma*/
// (BIT(3)|BIT(1)|BIT(0))			/*180 ma*/
// (BIT(3)|BIT(2))					/*195 ma*/
// (BIT(3)|BIT(2)|BIT(0))			/*210 ma*/
// (BIT(3)|BIT(2)|BIT(1))			/*225 ma*/
// (BIT(3)|BIT(2)|BIT(1)|BIT(0))	/*240 ma*/

#define WW6_DEV_TORCH_LIGHT_CURRENT_VALUE  (BIT(2)|BIT(1))
#define WW6_DEV_FLASH_LIGHT_CURRENT_VALUE  (BIT(2)|BIT(1))

//add useragent productmodel
#define WW6_USER_AGENT_NAME "IPRO_I324MS4"

//add default bluetooth name
#define BTM_DEF_LOCAL_NAME "SMART ONE"
