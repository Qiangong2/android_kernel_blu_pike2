/***********************************Flash Type************************************/
/*Flash���� Ĭ��ΪDDR2*/
//#define WW6_FLASH_TYPE_DDR3
/***********************************Flash Type************************************/


/***********************************LCD************************************/
#define CONFIG_FB_9340_XINGNUO_P70S
#define CONFIG_FB_GC9306_HG_CTC
/***********************************LCD************************************/


/************************************TP************************************/
/*TP�����,��kernel\drivers\input\touchscreenĿ¼�����(TP��������)*/


/************************************TP************************************/


/*****************************CAMERA DRIVER********************************/
//�Զ������ƣ���vendor\sprd\open-source\libs\libcamera\sensor\Ŀ¼�����


//ǰ������ͷ����ƣ�����vendor\sprd\open-source\libs\libcamera\sc8830\src\sensor_cfg.c�ļ�
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

//�ж�A OR B���ܺ�
//#define WW6_JUDGE_BOARD_AB
//�����ж�A OR B���GPIO��
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
