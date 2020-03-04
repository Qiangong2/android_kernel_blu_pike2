#include "pinctrl-common.h"

/* pin_number  ctrl_pin bit_offset bit_width */

/* pin_ctrl_reg0 */
#define	WIFI_COEXIST_PIN_IN_SEL		SPRD_PIN_INFO(0, 1, 28, 1, 0)
#define	WPD_EMMCPD			SPRD_PIN_INFO(1, 1, 15, 1, 0)
#define	WPD_ADPD			SPRD_PIN_INFO(2, 1, 13, 1, 0)
#define	WPD_IO_2_1PD			SPRD_PIN_INFO(3, 1, 12, 1, 0)
#define	WPD_SIM2PD			SPRD_PIN_INFO(4, 1, 10, 1, 0)
#define	WPD_SIM1PD			SPRD_PIN_INFO(5, 1, 9, 1, 0)
#define	WPD_SIM0PD			SPRD_PIN_INFO(6, 1, 8, 1, 0)
#define	WPD_SD0PD			SPRD_PIN_INFO(7, 1, 7, 1, 0)
#define	WPD_CAMPD			SPRD_PIN_INFO(8, 1, 6, 1, 0)
#define	WPD_SD2PD			SPRD_PIN_INFO(9, 1, 5, 1, 0)
#define	SIM2_SYS_DLP_MODE		SPRD_PIN_INFO(10, 1, 2, 1, 0)
#define	SIM1_SYS_DLP_MODE		SPRD_PIN_INFO(11, 1, 1, 1, 0)
#define	SIM0_SYS_DLP_MODE		SPRD_PIN_INFO(12, 1, 0, 1, 0)

/* pin_ctrl_reg1 */
#define	SIMRST2_ISO_VALUE		SPRD_PIN_INFO(13, 1, 12, 1, 1)
#define	SIMDA2_ISO_VALUE		SPRD_PIN_INFO(14, 1, 11, 1, 1)
#define	SIMCLK2_ISO_VALUE		SPRD_PIN_INFO(15, 1, 10, 1, 1)
#define	SIMRST1_ISO_VALUE		SPRD_PIN_INFO(16, 1, 9, 1, 1)
#define	SIMDA1_ISO_VALUE		SPRD_PIN_INFO(17, 1, 8, 1, 1)
#define	SIMCLK1_ISO_VALUE		SPRD_PIN_INFO(18, 1, 7, 1, 1)
#define	SIMRST0_ISO_VALUE		SPRD_PIN_INFO(19, 1, 6, 1, 1)
#define	SIMDA0_ISO_VALUE		SPRD_PIN_INFO(20, 1, 5, 1, 1)
#define	SIMCLK0_ISO_VALUE		SPRD_PIN_INFO(21, 1, 4, 1, 1)
#define	U4RXD_PIN_IN_SEL		SPRD_PIN_INFO(22, 1, 3, 1, 1)

/* pin_ctrl_reg2 */
#define	UART5_SYS_SEL			SPRD_PIN_INFO(23, 1, 29, 2, 2)
#define	SIM2_SYS_SEL			SPRD_PIN_INFO(24, 1, 23, 1, 2)
#define	SIM1_SYS_SEL			SPRD_PIN_INFO(25, 1, 22, 1, 2)
#define	SIM0_SYS_SEL			SPRD_PIN_INFO(26, 1, 21, 1, 2)
#define	UART4_SYS_SEL			SPRD_PIN_INFO(27, 1, 18, 3, 2)
#define	UART3_SYS_SEL			SPRD_PIN_INFO(28, 1, 15, 3, 2)
#define	UART2_SYS_SEL			SPRD_PIN_INFO(29, 1, 11, 4, 2)
#define	UART1_SYS_SEL			SPRD_PIN_INFO(30, 1, 7, 4, 2)
#define	UART0_SYS_SEL			SPRD_PIN_INFO(31, 1, 4, 3, 2)
#define	UART24_LOOP_SEL			SPRD_PIN_INFO(32, 1, 3, 1, 2)
#define	UART23_LOOP_SEL			SPRD_PIN_INFO(33, 1, 2, 1, 2)
#define	UART14_LOOP_SEL			SPRD_PIN_INFO(34, 1, 1, 1, 2)
#define	UART13_LOOP_SEL			SPRD_PIN_INFO(35, 1, 0, 1, 2)

/* pin_ctrl_reg3 */
#define	WDRST_OUT_SEL			SPRD_PIN_INFO(36, 1, 29, 3, 3)
#define	IIS3_SYS_SEL			SPRD_PIN_INFO(37, 1, 18, 4, 3)
#define	IIS2_SYS_SEL			SPRD_PIN_INFO(38, 1, 14, 4, 3)
#define	IIS1_SYS_SEL			SPRD_PIN_INFO(39, 1, 10, 3, 3)
#define	IIS0_SYS_SEL			SPRD_PIN_INFO(40, 1, 6, 4, 3)
#define	IIS23_LOOP_SEL			SPRD_PIN_INFO(41, 1, 5, 1, 3)
#define	IIS13_LOOP_SEL			SPRD_PIN_INFO(42, 1, 4, 1, 3)
#define	IIS12_LOOP_SEL			SPRD_PIN_INFO(43, 1, 3, 1, 3)
#define	IIS03_LOOP_SEL			SPRD_PIN_INFO(44, 1, 2, 1, 3)
#define	IIS02_LOOP_SEL			SPRD_PIN_INFO(45, 1, 1, 1, 3)
#define	IIS01_LOOP_SEL			SPRD_PIN_INFO(46, 1, 0, 1, 3)

/* pin_ctrl_reg4 */
#define	VSD2_MS				SPRD_PIN_INFO(47, 1, 7, 1, 4)
#define	VSD0_MS				SPRD_PIN_INFO(48, 1, 5, 1, 4)
#define	VSIM2_MS			SPRD_PIN_INFO(49, 1, 4, 1, 4)
#define	VSIM1_MS			SPRD_PIN_INFO(50, 1, 3, 1, 4)
#define	VSIM0_MS			SPRD_PIN_INFO(51, 1, 2, 1, 4)
#define	VIO_2_1_MS			SPRD_PIN_INFO(52, 1, 1, 1, 4)

/* pin_ctrl_reg5 */
#define	CARD_DET_SEL			SPRD_PIN_INFO(53, 1, 17, 3, 5)
#define	SIM0_DET_SEL			SPRD_PIN_INFO(54, 1, 16, 1, 5)
#define	AP_SIM0_BD_EB			SPRD_PIN_INFO(55, 1, 15, 1, 5)
#define	AP_EMMC_BD_EB			SPRD_PIN_INFO(56, 1, 14, 1, 5)
#define	AP_SDIO2_BD_EB			SPRD_PIN_INFO(57, 1, 13, 1, 5)
#define	AP_SDIO1_BD_EB			SPRD_PIN_INFO(58, 1, 12, 1, 5)
#define	AP_SDIO0_BD_EB			SPRD_PIN_INFO(59, 1, 11, 1, 5)
#define	PUBCP_SDIO_BD_EB		SPRD_PIN_INFO(60, 1, 10, 1, 5)
#define	PUBCP_SIM1_BD_EB		SPRD_PIN_INFO(61, 1, 9, 1, 5)
#define	PUBCP_SIM0_BD_EB		SPRD_PIN_INFO(62, 1, 8, 1, 5)

/* registers definitions for controller CTL_PIN */
#define	SC9833_EXTINT0			SPRD_PIN_INFO(63, 0, 0, 0, 0)
#define	SC9833_EXTINT1			SPRD_PIN_INFO(65, 0, 0, 0, 0)
#define	SC9833_SCL3			SPRD_PIN_INFO(67, 0, 0, 0, 0)
#define	SC9833_SDA3			SPRD_PIN_INFO(69, 0, 0, 0, 0)
#define	SC9833_RFSDA0			SPRD_PIN_INFO(71, 0, 0, 0, 0)
#define	SC9833_RFSCK0			SPRD_PIN_INFO(73, 0, 0, 0, 0)
#define	SC9833_RFSEN0			SPRD_PIN_INFO(75, 0, 0, 0, 0)
#define	SC9833_RFSDA1			SPRD_PIN_INFO(77, 0, 0, 0, 0)
#define	SC9833_RFSCK1			SPRD_PIN_INFO(79, 0, 0, 0, 0)
#define	SC9833_RFSEN1			SPRD_PIN_INFO(81, 0, 0, 0, 0)
#define	SC9833_RF_LVDS0_ADC_ON		SPRD_PIN_INFO(83, 0, 0, 0, 0)
#define	SC9833_RF_LVDS0_DAC_ON		SPRD_PIN_INFO(85, 0, 0, 0, 0)
#define	SC9833_RFCTL15			SPRD_PIN_INFO(87, 0, 0, 0, 0)
#define	SC9833_RFCTL16			SPRD_PIN_INFO(89, 0, 0, 0, 0)
#define	SC9833_RFCTL17			SPRD_PIN_INFO(91, 0, 0, 0, 0)
#define	SC9833_RFCTL0			SPRD_PIN_INFO(93, 0, 0, 0, 0)
#define	SC9833_RFCTL1			SPRD_PIN_INFO(95, 0, 0, 0, 0)
#define	SC9833_RFCTL2			SPRD_PIN_INFO(97, 0, 0, 0, 0)
#define	SC9833_RFCTL3			SPRD_PIN_INFO(99, 0, 0, 0, 0)
#define	SC9833_RFCTL4			SPRD_PIN_INFO(101, 0, 0, 0, 0)
#define	SC9833_RFCTL5			SPRD_PIN_INFO(103, 0, 0, 0, 0)
#define	SC9833_RFCTL6			SPRD_PIN_INFO(105, 0, 0, 0, 0)
#define	SC9833_RFCTL7			SPRD_PIN_INFO(107, 0, 0, 0, 0)
#define	SC9833_RFCTL8			SPRD_PIN_INFO(109, 0, 0, 0, 0)
#define	SC9833_RFCTL9			SPRD_PIN_INFO(111, 0, 0, 0, 0)
#define	SC9833_RFCTL10			SPRD_PIN_INFO(113, 0, 0, 0, 0)
#define	SC9833_RFCTL11			SPRD_PIN_INFO(115, 0, 0, 0, 0)
#define	SC9833_RFCTL12			SPRD_PIN_INFO(117, 0, 0, 0, 0)
#define	SC9833_RFCTL13			SPRD_PIN_INFO(119, 0, 0, 0, 0)
#define	SC9833_RFCTL14			SPRD_PIN_INFO(121, 0, 0, 0, 0)
#define	SC9833_XTL_EN			SPRD_PIN_INFO(123, 0, 0, 0, 0)
#define	SC9833_RFFE0_SCK		SPRD_PIN_INFO(125, 0, 0, 0, 0)
#define	SC9833_RFFE0_SDA		SPRD_PIN_INFO(127, 0, 0, 0, 0)
#define	SC9833_RFFE1_SCK		SPRD_PIN_INFO(129, 0, 0, 0, 0)
#define	SC9833_RFFE1_SDA		SPRD_PIN_INFO(131, 0, 0, 0, 0)
#define	SC9833_DTDO_LTE			SPRD_PIN_INFO(133, 0, 0, 0, 0)
#define	SC9833_DTDI_LTE			SPRD_PIN_INFO(135, 0, 0, 0, 0)
#define	SC9833_DTCK_LTE			SPRD_PIN_INFO(137, 0, 0, 0, 0)
#define	SC9833_DTMS_LTE			SPRD_PIN_INFO(139, 0, 0, 0, 0)
#define	SC9833_DRTCK_LTE		SPRD_PIN_INFO(141, 0, 0, 0, 0)
#define	SC9833_SIMCLK0			SPRD_PIN_INFO(143, 0, 0, 0, 0)
#define	SC9833_SIMDA0			SPRD_PIN_INFO(145, 0, 0, 0, 0)
#define	SC9833_SIMRST0			SPRD_PIN_INFO(147, 0, 0, 0, 0)
#define	SC9833_SIMCLK1			SPRD_PIN_INFO(149, 0, 0, 0, 0)
#define	SC9833_SIMDA1			SPRD_PIN_INFO(151, 0, 0, 0, 0)
#define	SC9833_SIMRST1			SPRD_PIN_INFO(153, 0, 0, 0, 0)
#define	SC9833_SIMCLK2			SPRD_PIN_INFO(155, 0, 0, 0, 0)
#define	SC9833_SIMDA2			SPRD_PIN_INFO(157, 0, 0, 0, 0)
#define	SC9833_SIMRST2			SPRD_PIN_INFO(159, 0, 0, 0, 0)
#define	SC9833_IIS3DI			SPRD_PIN_INFO(161, 0, 0, 0, 0)
#define	SC9833_IIS3DO			SPRD_PIN_INFO(163, 0, 0, 0, 0)
#define	SC9833_IIS3CLK			SPRD_PIN_INFO(165, 0, 0, 0, 0)
#define	SC9833_IIS3LRCK			SPRD_PIN_INFO(167, 0, 0, 0, 0)
#define	SC9833_LCM_RSTN			SPRD_PIN_INFO(169, 0, 0, 0, 0)
#define	SC9833_DSI_TE			SPRD_PIN_INFO(171, 0, 0, 0, 0)
#define	SC9833_SCL2			SPRD_PIN_INFO(173, 0, 0, 0, 0)
#define	SC9833_SDA2			SPRD_PIN_INFO(175, 0, 0, 0, 0)
#define	SC9833_SPI0_CSN			SPRD_PIN_INFO(177, 0, 0, 0, 0)
#define	SC9833_SPI0_DO			SPRD_PIN_INFO(179, 0, 0, 0, 0)
#define	SC9833_SPI0_DI			SPRD_PIN_INFO(181, 0, 0, 0, 0)
#define	SC9833_SPI0_CLK			SPRD_PIN_INFO(183, 0, 0, 0, 0)
#define	SC9833_SD0_D3			SPRD_PIN_INFO(185, 0, 0, 0, 0)
#define	SC9833_SD0_D2			SPRD_PIN_INFO(187, 0, 0, 0, 0)
#define	SC9833_SD0_CMD			SPRD_PIN_INFO(189, 0, 0, 0, 0)
#define	SC9833_SD0_D0			SPRD_PIN_INFO(191, 0, 0, 0, 0)
#define	SC9833_SD0_D1			SPRD_PIN_INFO(193, 0, 0, 0, 0)
#define	SC9833_SD0_CLK0			SPRD_PIN_INFO(195, 0, 0, 0, 0)

#define	SC9833_SD2_CLK			SPRD_PIN_INFO(199, 0, 0, 0, 0)
#define	SC9833_SD2_CMD			SPRD_PIN_INFO(201, 0, 0, 0, 0)
#define	SC9833_SD2_D0			SPRD_PIN_INFO(203, 0, 0, 0, 0)
#define	SC9833_SD2_D1			SPRD_PIN_INFO(205, 0, 0, 0, 0)
#define	SC9833_SD2_D2			SPRD_PIN_INFO(207, 0, 0, 0, 0)
#define	SC9833_SD2_D3			SPRD_PIN_INFO(209, 0, 0, 0, 0)

#define	SC9833_PTEST			SPRD_PIN_INFO(213, 0, 0, 0, 0)
#define	SC9833_ANA_INT			SPRD_PIN_INFO(215, 0, 0, 0, 0)
#define	SC9833_EXT_RST_B		SPRD_PIN_INFO(217, 0, 0, 0, 0)
#define	SC9833_CHIP_SLEEP		SPRD_PIN_INFO(219, 0, 0, 0, 0)
#define	SC9833_XTL_BUF_EN0		SPRD_PIN_INFO(221, 0, 0, 0, 0)
#define	SC9833_XTL_BUF_EN1		SPRD_PIN_INFO(223, 0, 0, 0, 0)
#define	SC9833_CLK_32K			SPRD_PIN_INFO(225, 0, 0, 0, 0)
#define	SC9833_AUD_SCLK			SPRD_PIN_INFO(227, 0, 0, 0, 0)
#define	SC9833_AUD_ADD0			SPRD_PIN_INFO(229, 0, 0, 0, 0)
#define	SC9833_AUD_ADSYNC		SPRD_PIN_INFO(231, 0, 0, 0, 0)
#define	SC9833_AUD_DAD1			SPRD_PIN_INFO(233, 0, 0, 0, 0)
#define	SC9833_AUD_DAD0			SPRD_PIN_INFO(235, 0, 0, 0, 0)
#define	SC9833_AUD_DASYNC		SPRD_PIN_INFO(237, 0, 0, 0, 0)
#define	SC9833_ADI_D			SPRD_PIN_INFO(239, 0, 0, 0, 0)

#define	SC9833_ADI_SCLK			SPRD_PIN_INFO(243, 0, 0, 0, 0)
#define	SC9833_U1TXD			SPRD_PIN_INFO(245, 0, 0, 0, 0)
#define	SC9833_U1RXD			SPRD_PIN_INFO(247, 0, 0, 0, 0)
#define	SC9833_U2TXD			SPRD_PIN_INFO(249, 0, 0, 0, 0)
#define	SC9833_U2RXD			SPRD_PIN_INFO(251, 0, 0, 0, 0)
#define	SC9833_U4TXD			SPRD_PIN_INFO(253, 0, 0, 0, 0)
#define	SC9833_U4RXD			SPRD_PIN_INFO(255, 0, 0, 0, 0)
#define	SC9833_IIS1DI			SPRD_PIN_INFO(257, 0, 0, 0, 0)
#define	SC9833_IIS1DO			SPRD_PIN_INFO(259, 0, 0, 0, 0)
#define	SC9833_IIS1CLK			SPRD_PIN_INFO(261, 0, 0, 0, 0)
#define	SC9833_IIS1LRCK			SPRD_PIN_INFO(263, 0, 0, 0, 0)
#define	SC9833_SPI2_CSN			SPRD_PIN_INFO(265, 0, 0, 0, 0)
#define	SC9833_SPI2_DO			SPRD_PIN_INFO(267, 0, 0, 0, 0)
#define	SC9833_SPI2_DI			SPRD_PIN_INFO(269, 0, 0, 0, 0)
#define	SC9833_SPI2_CLK			SPRD_PIN_INFO(271, 0, 0, 0, 0)


#define	SC9833_MTCK_ARM			SPRD_PIN_INFO(277, 0, 0, 0, 0)
#define	SC9833_MTMS_ARM			SPRD_PIN_INFO(279, 0, 0, 0, 0)

#define	SC9833_EMMC_D2			SPRD_PIN_INFO(283, 0, 0, 0, 0)
#define	SC9833_EMMC_D5			SPRD_PIN_INFO(285, 0, 0, 0, 0)
#define	SC9833_EMMC_D1			SPRD_PIN_INFO(287, 0, 0, 0, 0)
#define	SC9833_EMMC_D4			SPRD_PIN_INFO(289, 0, 0, 0, 0)

#define	SC9833_EMMC_DS			SPRD_PIN_INFO(293, 0, 0, 0, 0)
#define	SC9833_EMMC_CLK			SPRD_PIN_INFO(295, 0, 0, 0, 0)
#define	SC9833_EMMC_D6			SPRD_PIN_INFO(297, 0, 0, 0, 0)
#define	SC9833_EMMC_D3			SPRD_PIN_INFO(299, 0, 0, 0, 0)
#define	SC9833_EMMC_D0			SPRD_PIN_INFO(301, 0, 0, 0, 0)
#define	SC9833_EMMC_CMD			SPRD_PIN_INFO(303, 0, 0, 0, 0)
#define	SC9833_EMMC_D7			SPRD_PIN_INFO(305, 0, 0, 0, 0)
#define	SC9833_EMMC_RST			SPRD_PIN_INFO(307, 0, 0, 0, 0)
#define	SC9833_MEMS_MIC_CLK0		SPRD_PIN_INFO(309, 0, 0, 0, 0)
#define	SC9833_MEMS_MIC_DATA0		SPRD_PIN_INFO(311, 0, 0, 0, 0)
#define	SC9833_MEMS_MIC_CLK1		SPRD_PIN_INFO(313, 0, 0, 0, 0)
#define	SC9833_MEMS_MIC_DATA1		SPRD_PIN_INFO(315, 0, 0, 0, 0)
#define	SC9833_SD1_CLK			SPRD_PIN_INFO(317, 0, 0, 0, 0)
#define	SC9833_SD1_CMD			SPRD_PIN_INFO(319, 0, 0, 0, 0)
#define	SC9833_SD1_D0			SPRD_PIN_INFO(321, 0, 0, 0, 0)

#define	SC9833_SD1_D1			SPRD_PIN_INFO(325, 0, 0, 0, 0)
#define	SC9833_SD1_D2			SPRD_PIN_INFO(327, 0, 0, 0, 0)
#define	SC9833_SD1_D3			SPRD_PIN_INFO(329, 0, 0, 0, 0)
#define	SC9833_IIS0DI			SPRD_PIN_INFO(331, 0, 0, 0, 0)
#define	SC9833_IIS0DO			SPRD_PIN_INFO(333, 0, 0, 0, 0)
#define	SC9833_IIS0CLK			SPRD_PIN_INFO(335, 0, 0, 0, 0)
#define	SC9833_IIS0LRCK			SPRD_PIN_INFO(337, 0, 0, 0, 0)
#define	SC9833_CLK_AUX0			SPRD_PIN_INFO(339, 0, 0, 0, 0)
#define	SC9833_U0TXD			SPRD_PIN_INFO(341, 0, 0, 0, 0)
#define	SC9833_U0RXD			SPRD_PIN_INFO(343, 0, 0, 0, 0)
#define	SC9833_U0CTS			SPRD_PIN_INFO(345, 0, 0, 0, 0)
#define	SC9833_U0RTS			SPRD_PIN_INFO(347, 0, 0, 0, 0)
#define	SC9833_U3TXD			SPRD_PIN_INFO(349, 0, 0, 0, 0)
#define	SC9833_U3RXD			SPRD_PIN_INFO(351, 0, 0, 0, 0)
#define	SC9833_U3CTS			SPRD_PIN_INFO(353, 0, 0, 0, 0)
#define	SC9833_U3RTS			SPRD_PIN_INFO(355, 0, 0, 0, 0)
#define	SC9833_RFCTL18			SPRD_PIN_INFO(357, 0, 0, 0, 0)
#define	SC9833_RFCTL19			SPRD_PIN_INFO(359, 0, 0, 0, 0)
#define	SC9833_KEYOUT0			SPRD_PIN_INFO(361, 0, 0, 0, 0)
#define	SC9833_KEYOUT1			SPRD_PIN_INFO(363, 0, 0, 0, 0)
#define	SC9833_KEYOUT2			SPRD_PIN_INFO(365, 0, 0, 0, 0)
#define	SC9833_KEYIN0			SPRD_PIN_INFO(367, 0, 0, 0, 0)
#define	SC9833_KEYIN1			SPRD_PIN_INFO(369, 0, 0, 0, 0)
#define	SC9833_KEYIN2			SPRD_PIN_INFO(371, 0, 0, 0, 0)
#define	SC9833_CMPD2			SPRD_PIN_INFO(373, 0, 0, 0, 0)
#define	SC9833_CMRST2			SPRD_PIN_INFO(375, 0, 0, 0, 0)
#define	SC9833_CMMCLK0			SPRD_PIN_INFO(377, 0, 0, 0, 0)
#define	SC9833_CMMCLK1			SPRD_PIN_INFO(379, 0, 0, 0, 0)
#define	SC9833_CMRST0			SPRD_PIN_INFO(381, 0, 0, 0, 0)
#define	SC9833_CMRST1			SPRD_PIN_INFO(383, 0, 0, 0, 0)
#define	SC9833_CMPD0			SPRD_PIN_INFO(385, 0, 0, 0, 0)
#define	SC9833_CMPD1			SPRD_PIN_INFO(387, 0, 0, 0, 0)
#define	SC9833_SCL0			SPRD_PIN_INFO(389, 0, 0, 0, 0)
#define	SC9833_SDA0			SPRD_PIN_INFO(391, 0, 0, 0, 0)
/* registers definitions for controller CTL_PIN_MISC */
#define	SC9833_MISC_EXTINT0		SPRD_PIN_INFO(64, 2, 0, 0, 0)
#define	SC9833_MISC_EXTINT1		SPRD_PIN_INFO(66, 2, 0, 0, 0)
#define	SC9833_MISC_SCL3		SPRD_PIN_INFO(68, 2, 0, 0, 0)
#define	SC9833_MISC_SDA3		SPRD_PIN_INFO(70, 2, 0, 0, 0)
#define	SC9833_MISC_RFSDA0		SPRD_PIN_INFO(72, 2, 0, 0, 0)
#define	SC9833_MISC_RFSCK0		SPRD_PIN_INFO(74, 2, 0, 0, 0)
#define	SC9833_MISC_RFSEN0		SPRD_PIN_INFO(76, 2, 0, 0, 0)
#define	SC9833_MISC_RFSDA1		SPRD_PIN_INFO(78, 2, 0, 0, 0)
#define	SC9833_MISC_RFSCK1		SPRD_PIN_INFO(80, 2, 0, 0, 0)
#define	SC9833_MISC_RFSEN1		SPRD_PIN_INFO(82, 2, 0, 0, 0)
#define	SC9833_MISC_RF_LVDS0_ADC_ON	SPRD_PIN_INFO(84, 2, 0, 0, 0)
#define	SC9833_MISC_RF_LVDS0_DAC_ON	SPRD_PIN_INFO(86, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL15		SPRD_PIN_INFO(88, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL16		SPRD_PIN_INFO(90, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL17		SPRD_PIN_INFO(92, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL0		SPRD_PIN_INFO(94, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL1		SPRD_PIN_INFO(96, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL2		SPRD_PIN_INFO(98, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL3		SPRD_PIN_INFO(100, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL4		SPRD_PIN_INFO(102, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL5		SPRD_PIN_INFO(104, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL6		SPRD_PIN_INFO(106, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL7		SPRD_PIN_INFO(108, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL8		SPRD_PIN_INFO(110, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL9		SPRD_PIN_INFO(112, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL10		SPRD_PIN_INFO(114, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL11		SPRD_PIN_INFO(116, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL12		SPRD_PIN_INFO(118, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL13		SPRD_PIN_INFO(120, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL14		SPRD_PIN_INFO(122, 2, 0, 0, 0)
#define	SC9833_MISC_XTL_EN		SPRD_PIN_INFO(124, 2, 0, 0, 0)
#define	SC9833_MISC_RFFE0_SCK		SPRD_PIN_INFO(126, 2, 0, 0, 0)
#define	SC9833_MISC_RFFE0_SDA		SPRD_PIN_INFO(128, 2, 0, 0, 0)
#define	SC9833_MISC_RFFE1_SCK		SPRD_PIN_INFO(130, 2, 0, 0, 0)
#define	SC9833_MISC_RFFE1_SDA		SPRD_PIN_INFO(132, 2, 0, 0, 0)
#define	SC9833_MISC_DTDO_LTE		SPRD_PIN_INFO(134, 2, 0, 0, 0)
#define	SC9833_MISC_DTDI_LTE		SPRD_PIN_INFO(136, 2, 0, 0, 0)
#define	SC9833_MISC_DTCK_LTE		SPRD_PIN_INFO(138, 2, 0, 0, 0)
#define	SC9833_MISC_DTMS_LTE		SPRD_PIN_INFO(140, 2, 0, 0, 0)
#define	SC9833_MISC_DRTCK_LTE		SPRD_PIN_INFO(142, 2, 0, 0, 0)
#define	SC9833_MISC_SIMCLK0		SPRD_PIN_INFO(144, 2, 0, 0, 0)
#define	SC9833_MISC_SIMDA0		SPRD_PIN_INFO(146, 2, 0, 0, 0)
#define	SC9833_MISC_SIMRST0		SPRD_PIN_INFO(148, 2, 0, 0, 0)
#define	SC9833_MISC_SIMCLK1		SPRD_PIN_INFO(150, 2, 0, 0, 0)
#define	SC9833_MISC_SIMDA1		SPRD_PIN_INFO(152, 2, 0, 0, 0)
#define	SC9833_MISC_SIMRST1		SPRD_PIN_INFO(154, 2, 0, 0, 0)
#define	SC9833_MISC_SIMCLK2		SPRD_PIN_INFO(156, 2, 0, 0, 0)
#define	SC9833_MISC_SIMDA2		SPRD_PIN_INFO(158, 2, 0, 0, 0)
#define	SC9833_MISC_SIMRST2		SPRD_PIN_INFO(160, 2, 0, 0, 0)
#define	SC9833_MISC_IIS3DI		SPRD_PIN_INFO(162, 2, 0, 0, 0)
#define	SC9833_MISC_IIS3DO		SPRD_PIN_INFO(164, 2, 0, 0, 0)
#define	SC9833_MISC_IIS3CLK		SPRD_PIN_INFO(166, 2, 0, 0, 0)
#define	SC9833_MISC_IIS3LRCK		SPRD_PIN_INFO(168, 2, 0, 0, 0)
#define	SC9833_MISC_LCM_RSTN		SPRD_PIN_INFO(170, 2, 0, 0, 0)
#define	SC9833_MISC_DSI_TE		SPRD_PIN_INFO(172, 2, 0, 0, 0)
#define	SC9833_MISC_SCL2		SPRD_PIN_INFO(174, 2, 0, 0, 0)
#define	SC9833_MISC_SDA2		SPRD_PIN_INFO(176, 2, 0, 0, 0)
#define	SC9833_MISC_SPI0_CSN		SPRD_PIN_INFO(178, 2, 0, 0, 0)
#define	SC9833_MISC_SPI0_DO		SPRD_PIN_INFO(180, 2, 0, 0, 0)
#define	SC9833_MISC_SPI0_DI		SPRD_PIN_INFO(182, 2, 0, 0, 0)
#define	SC9833_MISC_SPI0_CLK		SPRD_PIN_INFO(184, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_D3		SPRD_PIN_INFO(186, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_D2		SPRD_PIN_INFO(188, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_CMD		SPRD_PIN_INFO(190, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_D0		SPRD_PIN_INFO(192, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_D1		SPRD_PIN_INFO(194, 2, 0, 0, 0)
#define	SC9833_MISC_SD0_CLK0		SPRD_PIN_INFO(196, 2, 0, 0, 0)

#define	SC9833_MISC_SD2_CLK		SPRD_PIN_INFO(200, 2, 0, 0, 0)
#define	SC9833_MISC_SD2_CMD		SPRD_PIN_INFO(202, 2, 0, 0, 0)
#define	SC9833_MISC_SD2_D0		SPRD_PIN_INFO(204, 2, 0, 0, 0)
#define	SC9833_MISC_SD2_D1		SPRD_PIN_INFO(206, 2, 0, 0, 0)
#define	SC9833_MISC_SD2_D2		SPRD_PIN_INFO(208, 2, 0, 0, 0)
#define	SC9833_MISC_SD2_D3		SPRD_PIN_INFO(210, 2, 0, 0, 0)

#define	SC9833_MISC_PTEST		SPRD_PIN_INFO(214, 2, 0, 0, 0)
#define	SC9833_MISC_ANA_INT		SPRD_PIN_INFO(216, 2, 0, 0, 0)
#define	SC9833_MISC_EXT_RST_B		SPRD_PIN_INFO(218, 2, 0, 0, 0)
#define	SC9833_MISC_CHIP_SLEEP		SPRD_PIN_INFO(220, 2, 0, 0, 0)
#define	SC9833_MISC_XTL_BUF_EN0		SPRD_PIN_INFO(222, 2, 0, 0, 0)
#define	SC9833_MISC_XTL_BUF_EN1		SPRD_PIN_INFO(224, 2, 0, 0, 0)
#define	SC9833_MISC_CLK_32K		SPRD_PIN_INFO(226, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_SCLK		SPRD_PIN_INFO(228, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_ADD0		SPRD_PIN_INFO(230, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_ADSYNC		SPRD_PIN_INFO(232, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_DAD1		SPRD_PIN_INFO(234, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_DAD0		SPRD_PIN_INFO(236, 2, 0, 0, 0)
#define	SC9833_MISC_AUD_DASYNC		SPRD_PIN_INFO(238, 2, 0, 0, 0)
#define	SC9833_MISC_ADI_D		SPRD_PIN_INFO(240, 2, 0, 0, 0)

#define	SC9833_MISC_ADI_SCLK		SPRD_PIN_INFO(244, 2, 0, 0, 0)
#define	SC9833_MISC_U1TXD		SPRD_PIN_INFO(246, 2, 0, 0, 0)
#define	SC9833_MISC_U1RXD		SPRD_PIN_INFO(248, 2, 0, 0, 0)
#define	SC9833_MISC_U2TXD		SPRD_PIN_INFO(250, 2, 0, 0, 0)
#define	SC9833_MISC_U2RXD		SPRD_PIN_INFO(252, 2, 0, 0, 0)
#define	SC9833_MISC_U4TXD		SPRD_PIN_INFO(254, 2, 0, 0, 0)
#define	SC9833_MISC_U4RXD		SPRD_PIN_INFO(256, 2, 0, 0, 0)
#define	SC9833_MISC_IIS1DI		SPRD_PIN_INFO(258, 2, 0, 0, 0)
#define	SC9833_MISC_IIS1DO		SPRD_PIN_INFO(260, 2, 0, 0, 0)
#define	SC9833_MISC_IIS1CLK		SPRD_PIN_INFO(262, 2, 0, 0, 0)
#define	SC9833_MISC_IIS1LRCK		SPRD_PIN_INFO(264, 2, 0, 0, 0)
#define	SC9833_MISC_SPI2_CSN		SPRD_PIN_INFO(266, 2, 0, 0, 0)
#define	SC9833_MISC_SPI2_DO		SPRD_PIN_INFO(268, 2, 0, 0, 0)
#define	SC9833_MISC_SPI2_DI		SPRD_PIN_INFO(270, 2, 0, 0, 0)
#define	SC9833_MISC_SPI2_CLK		SPRD_PIN_INFO(272, 2, 0, 0, 0)


#define	SC9833_MISC_MTCK_ARM		SPRD_PIN_INFO(278, 2, 0, 0, 0)
#define	SC9833_MISC_MTMS_ARM		SPRD_PIN_INFO(280, 2, 0, 0, 0)

#define	SC9833_MISC_EMMC_D2		SPRD_PIN_INFO(284, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D5		SPRD_PIN_INFO(286, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D1		SPRD_PIN_INFO(288, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D4		SPRD_PIN_INFO(290, 2, 0, 0, 0)

#define	SC9833_MISC_EMMC_DS		SPRD_PIN_INFO(294, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_CLK		SPRD_PIN_INFO(296, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D6		SPRD_PIN_INFO(298, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D3		SPRD_PIN_INFO(300, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D0		SPRD_PIN_INFO(302, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_CMD		SPRD_PIN_INFO(304, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_D7		SPRD_PIN_INFO(306, 2, 0, 0, 0)
#define	SC9833_MISC_EMMC_RST		SPRD_PIN_INFO(308, 2, 0, 0, 0)
#define	SC9833_MISC_MEMS_MIC_CLK0	SPRD_PIN_INFO(310, 2, 0, 0, 0)
#define	SC9833_MISC_MEMS_MIC_DATA0		SPRD_PIN_INFO(312, 2, 0, 0, 0)
#define	SC9833_MISC_MEMS_MIC_CLK1	SPRD_PIN_INFO(314, 2, 0, 0, 0)
#define	SC9833_MISC_MEMS_MIC_DATA1		SPRD_PIN_INFO(316, 2, 0, 0, 0)
#define	SC9833_MISC_SD1_CLK		SPRD_PIN_INFO(318, 2, 0, 0, 0)
#define	SC9833_MISC_SD1_CMD		SPRD_PIN_INFO(320, 2, 0, 0, 0)
#define	SC9833_MISC_SD1_D0		SPRD_PIN_INFO(322, 2, 0, 0, 0)

#define	SC9833_MISC_SD1_D1		SPRD_PIN_INFO(326, 2, 0, 0, 0)
#define	SC9833_MISC_SD1_D2		SPRD_PIN_INFO(328, 2, 0, 0, 0)
#define	SC9833_MISC_SD1_D3		SPRD_PIN_INFO(330, 2, 0, 0, 0)
#define	SC9833_MISC_IIS0DI		SPRD_PIN_INFO(332, 2, 0, 0, 0)
#define	SC9833_MISC_IIS0DO		SPRD_PIN_INFO(334, 2, 0, 0, 0)
#define	SC9833_MISC_IIS0CLK		SPRD_PIN_INFO(336, 2, 0, 0, 0)
#define	SC9833_MISC_IIS0LRCK		SPRD_PIN_INFO(338, 2, 0, 0, 0)
#define	SC9833_MISC_CLK_AUX0		SPRD_PIN_INFO(340, 2, 0, 0, 0)
#define	SC9833_MISC_U0TXD		SPRD_PIN_INFO(342, 2, 0, 0, 0)
#define	SC9833_MISC_U0RXD		SPRD_PIN_INFO(344, 2, 0, 0, 0)
#define	SC9833_MISC_U0CTS		SPRD_PIN_INFO(346, 2, 0, 0, 0)
#define	SC9833_MISC_U0RTS		SPRD_PIN_INFO(348, 2, 0, 0, 0)
#define	SC9833_MISC_U3TXD		SPRD_PIN_INFO(350, 2, 0, 0, 0)
#define	SC9833_MISC_U3RXD		SPRD_PIN_INFO(352, 2, 0, 0, 0)
#define	SC9833_MISC_U3CTS		SPRD_PIN_INFO(354, 2, 0, 0, 0)
#define	SC9833_MISC_U3RTS		SPRD_PIN_INFO(356, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL18		SPRD_PIN_INFO(358, 2, 0, 0, 0)
#define	SC9833_MISC_RFCTL19		SPRD_PIN_INFO(360, 2, 0, 0, 0)
#define	SC9833_MISC_KEYOUT0		SPRD_PIN_INFO(362, 2, 0, 0, 0)
#define	SC9833_MISC_KEYOUT1		SPRD_PIN_INFO(364, 2, 0, 0, 0)
#define	SC9833_MISC_KEYOUT2		SPRD_PIN_INFO(366, 2, 0, 0, 0)
#define	SC9833_MISC_KEYIN0		SPRD_PIN_INFO(368, 2, 0, 0, 0)
#define	SC9833_MISC_KEYIN1		SPRD_PIN_INFO(370, 2, 0, 0, 0)
#define	SC9833_MISC_KEYIN2		SPRD_PIN_INFO(372, 2, 0, 0, 0)
#define	SC9833_MISC_CMPD2		SPRD_PIN_INFO(374, 2, 0, 0, 0)
#define	SC9833_MISC_CMRST2		SPRD_PIN_INFO(376, 2, 0, 0, 0)
#define	SC9833_MISC_CMMCLK0		SPRD_PIN_INFO(378, 2, 0, 0, 0)
#define	SC9833_MISC_CMMCLK1		SPRD_PIN_INFO(380, 2, 0, 0, 0)
#define	SC9833_MISC_CMRST0		SPRD_PIN_INFO(382, 2, 0, 0, 0)
#define	SC9833_MISC_CMRST1		SPRD_PIN_INFO(384, 2, 0, 0, 0)
#define	SC9833_MISC_CMPD0		SPRD_PIN_INFO(386, 2, 0, 0, 0)
#define	SC9833_MISC_CMPD1		SPRD_PIN_INFO(388, 2, 0, 0, 0)
#define	SC9833_MISC_SCL0		SPRD_PIN_INFO(390, 2, 0, 0, 0)
#define	SC9833_MISC_SDA0		SPRD_PIN_INFO(392, 2, 0, 0, 0)
