#include "pinctrl-common.h"

/* pin_number  ctrl_pin bit_offset bit_width */

/* pin_ctrl_reg0 */
#define SC9850_VIO28_0_IRTE		SPRD_PIN_INFO(0, 1, 11, 1, 0)
#define SC9850_VIO_SD2_IRTE		SPRD_PIN_INFO(1, 1, 10, 1, 0)
#define SC9850_VIO_SD0_IRTE		SPRD_PIN_INFO(2, 1, 9, 1, 0)
#define SC9850_VIO_SIM2_IRTE		SPRD_PIN_INFO(3, 1, 8, 1, 0)
#define SC9850_VIO_SIM1_IRTE		SPRD_PIN_INFO(4, 1, 7, 1, 0)
#define SC9850_VIO_SIM0_IRTE		SPRD_PIN_INFO(5, 1, 6, 1, 0)
#define SC9850_VIO28_0_MS		SPRD_PIN_INFO(6, 1, 5, 1, 0)
#define SC9850_VIO_SD2_MS		SPRD_PIN_INFO(7, 1, 4, 1, 0)
#define SC9850_VIO_SD0_MS		SPRD_PIN_INFO(8, 1, 3, 1, 0)
#define SC9850_VIO_SIM2_MS		SPRD_PIN_INFO(9, 1, 2, 1, 0)
#define SC9850_VIO_SIM1_MS		SPRD_PIN_INFO(10, 1, 1, 1, 0)
#define SC9850_VIO_SIM0_MS		SPRD_PIN_INFO(11, 1, 0, 1, 0)

/* pin_ctrl_reg2 */
#define SC9850_SPSPI_PIN_IN_SEL		SPRD_PIN_INFO(12, 1, 31, 1, 2)
#define SC9850_UART1_USB30_PHY_SEL	SPRD_PIN_INFO(13, 1, 30, 1, 2)
#define SC9850_USB30_PHY_DM_OE		SPRD_PIN_INFO(14, 1, 29, 1, 2)
#define SC9850_USB30_PHY_DP_OE		SPRD_PIN_INFO(15, 1, 28, 1, 2)
#define SC9850_UART5_SYS_SEL		SPRD_PIN_INFO(16, 1, 25, 3, 2)
#define SC9850_ORP_URXD_PIN_IN_SEL	SPRD_PIN_INFO(17, 1, 24, 1, 2)
#define SC9850_SIM2_SYS_SEL		SPRD_PIN_INFO(18, 1, 23, 1, 2)
#define SC9850_SIM1_SYS_SEL		SPRD_PIN_INFO(19, 1, 22, 1, 2)
#define SC9850_SIM0_SYS_SEL		SPRD_PIN_INFO(20, 1, 21, 1, 2)
#define SC9850_CLK26MHZ_BUF_OUT_SEL	SPRD_PIN_INFO(21, 1, 20, 1, 2)
#define SC9850_UART4_SYS_SEL		SPRD_PIN_INFO(22, 1, 16, 3, 2)
#define SC9850_UART3_SYS_SEL		SPRD_PIN_INFO(23, 1, 13, 3, 2)
#define SC9850_UART2_SYS_SEL		SPRD_PIN_INFO(24, 1, 10, 3, 2)
#define SC9850_UART1_SYS_SEL		SPRD_PIN_INFO(25, 1, 7, 3, 2)
#define SC9850_UART0_SYS_SEL		SPRD_PIN_INFO(26, 1, 4, 3, 2)
#define SC9850_UART24_LOOP_SEL		SPRD_PIN_INFO(27, 1, 3, 1, 2)
#define SC9850_UART23_LOOP_SEL		SPRD_PIN_INFO(28, 1, 2, 1, 2)
#define SC9850_UART14_LOOP_SEL		SPRD_PIN_INFO(29, 1, 1, 1, 2)
#define SC9850_UART13_LOOP_SEL		SPRD_PIN_INFO(30, 1, 0, 1, 2)

/* pin_ctrl_reg3 */
#define SC9850_IIS3_SYS_SEL		SPRD_PIN_INFO(31, 1, 18, 4, 3)
#define SC9850_IIS2_SYS_SEL		SPRD_PIN_INFO(32, 1, 14, 4, 3)
#define SC9850_IIS1_SYS_SEL		SPRD_PIN_INFO(33, 1, 10, 4, 3)
#define SC9850_IIS0_SYS_SEL		SPRD_PIN_INFO(34, 1, 6, 4, 3)
#define SC9850_IIS23_LOOP_SEL		SPRD_PIN_INFO(35, 1, 5, 1, 3)
#define SC9850_IIS13_LOOP_SEL		SPRD_PIN_INFO(36, 1, 4, 1, 3)
#define SC9850_IIS12_LOOP_SEL		SPRD_PIN_INFO(37, 1, 3, 1, 3)
#define SC9850_IIS03_LOOP_SEL		SPRD_PIN_INFO(38, 1, 2, 1, 3)
#define SC9850_IIS02_LOOP_SEL		SPRD_PIN_INFO(39, 1, 1, 1, 3)
#define SC9850_IIS01_LOOP_SEL		SPRD_PIN_INFO(40, 1, 0, 1, 3)

/* pin_ctrl_reg4 */
#define SC9850_IIS6_SYS_SEL		SPRD_PIN_INFO(41, 1, 27, 4, 4)
#define SC9850_IIS5_SYS_SEL		SPRD_PIN_INFO(42, 1, 23, 4, 4)
#define SC9850_IIS4_SYS_SEL		SPRD_PIN_INFO(43, 1, 19, 4, 4)
#define SC9850_I2C_INF6_SYS_SEL		SPRD_PIN_INFO(44, 1, 8, 2, 4)
#define SC9850_I2C_INF4_SYS_SEL		SPRD_PIN_INFO(45, 1, 6, 2, 4)
#define SC9850_I2C_INF2_SYS_SEL		SPRD_PIN_INFO(46, 1, 4, 2, 4)
#define SC9850_I2C_INF1_SYS_SEL		SPRD_PIN_INFO(47, 1, 2, 2, 4)
#define SC9850_I2C_INF0_SYS_SEL		SPRD_PIN_INFO(48, 1, 0, 2, 4)

/* pin_ctrl_reg5 */
#define SC9850_GPIO_INF7_SYS_SEL	SPRD_PIN_INFO(49, 1, 27, 1, 5)
#define SC9850_GPIO_INF6_SYS_SEL	SPRD_PIN_INFO(50, 1, 26, 1, 5)
#define SC9850_GPIO_INF5_SYS_SEL	SPRD_PIN_INFO(51, 1, 25, 1, 5)
#define SC9850_GPIO_INF4_SYS_SEL	SPRD_PIN_INFO(52, 1, 24, 1, 5)
#define SC9850_GPIO_INF3_SYS_SEL	SPRD_PIN_INFO(53, 1, 23, 1, 5)
#define SC9850_GPIO_INF2_SYS_SEL	SPRD_PIN_INFO(54, 1, 22, 1, 5)
#define SC9850_GPIO_INF1_SYS_SEL	SPRD_PIN_INFO(55, 1, 21, 1, 5)
#define SC9850_GPIO_INF0_SYS_SEL	SPRD_PIN_INFO(56, 1, 20, 1, 5)
#define SC9850_WDRST_OUT_SEL		SPRD_PIN_INFO(57, 1, 16, 3, 5)
#define SC9850_ADI_SYNC_PIN_OUT_SEL	SPRD_PIN_INFO(58, 1, 14, 1, 5)
#define SC9850_CMRST_SEL		SPRD_PIN_INFO(59, 1, 13, 1, 5)
#define SC9850_CMPD_SEL			SPRD_PIN_INFO(60, 1, 12, 1, 5)
#define SC9850_TEST_DBG_MODE11		SPRD_PIN_INFO(61, 1, 11, 1, 5)
#define SC9850_TEST_DBG_MODE10		SPRD_PIN_INFO(62, 1, 10, 1, 5)
#define SC9850_TEST_DBG_MODE9		SPRD_PIN_INFO(63, 1, 9, 1, 5)
#define SC9850_TEST_DBG_MODE8		SPRD_PIN_INFO(64, 1, 8, 1, 5)
#define SC9850_TEST_DBG_MODE7		SPRD_PIN_INFO(65, 1, 7, 1, 5)
#define SC9850_TEST_DBG_MODE6		SPRD_PIN_INFO(66, 1, 6, 1, 5)
#define SC9850_TEST_DBG_MODE5		SPRD_PIN_INFO(67, 1, 5, 1, 5)
#define SC9850_TEST_DBG_MODE4		SPRD_PIN_INFO(68, 1, 4, 1, 5)
#define SC9850_TEST_DBG_MODE3		SPRD_PIN_INFO(69, 1, 3, 1, 5)
#define SC9850_TEST_DBG_MODE2		SPRD_PIN_INFO(70, 1, 2, 1, 5)
#define SC9850_TEST_DBG_MODE1		SPRD_PIN_INFO(71, 1, 1, 1, 5)
#define SC9850_TEST_DBG_MODE0		SPRD_PIN_INFO(72, 1, 0, 1, 5)

/* pin_ctrl_reg6 */
#define SC9850_SP_EIC_DPAD3_SEL		SPRD_PIN_INFO(73, 1, 24, 8, 6)
#define SC9850_SP_EIC_DPAD2_SEL		SPRD_PIN_INFO(74, 1, 16, 8, 6)
#define SC9850_SP_EIC_DPAD1_SEL		SPRD_PIN_INFO(75, 1, 8, 8, 6)
#define SC9850_SP_EIC_DPAD0_SEL		SPRD_PIN_INFO(76, 1, 0, 8, 6)

/* pin_ctrl_reg7 */
#define SC9850_SP_EIC_DPAD7_SEL		SPRD_PIN_INFO(77, 1, 24, 8, 7)
#define SC9850_SP_EIC_DPAD6_SEL		SPRD_PIN_INFO(78, 1, 16, 8, 7)
#define SC9850_SP_EIC_DPAD5_SEL		SPRD_PIN_INFO(79, 1, 8, 8, 7)
#define SC9850_SP_EIC_DPAD4_SEL		SPRD_PIN_INFO(80, 1, 0, 8, 7)

/* registers definitions for controller CTL_PIN */
#define SC9850_RFCTL20			SPRD_PIN_INFO(81, 0, 0, 0, 0)
#define SC9850_RFCTL21			SPRD_PIN_INFO(83, 0, 0, 0, 0)
#define SC9850_RFCTL30			SPRD_PIN_INFO(85, 0, 0, 0, 0)
#define SC9850_RFCTL31			SPRD_PIN_INFO(87, 0, 0, 0, 0)
#define SC9850_RFCTL32			SPRD_PIN_INFO(89, 0, 0, 0, 0)
#define SC9850_RFCTL33			SPRD_PIN_INFO(91, 0, 0, 0, 0)
#define SC9850_RFCTL34			SPRD_PIN_INFO(93, 0, 0, 0, 0)
#define SC9850_RFCTL35			SPRD_PIN_INFO(95, 0, 0, 0, 0)
#define SC9850_RFCTL36			SPRD_PIN_INFO(97, 0, 0, 0, 0)
#define SC9850_RFCTL37			SPRD_PIN_INFO(99, 0, 0, 0, 0)
#define SC9850_RFCTL22			SPRD_PIN_INFO(101, 0, 0, 0, 0)
#define SC9850_RFCTL23			SPRD_PIN_INFO(103, 0, 0, 0, 0)
#define SC9850_RFCTL24			SPRD_PIN_INFO(105, 0, 0, 0, 0)
#define SC9850_RFCTL25			SPRD_PIN_INFO(107, 0, 0, 0, 0)
#define SC9850_RFCTL26			SPRD_PIN_INFO(109, 0, 0, 0, 0)
#define SC9850_RFCTL27			SPRD_PIN_INFO(111, 0, 0, 0, 0)
#define SC9850_RFCTL28			SPRD_PIN_INFO(113, 0, 0, 0, 0)
#define SC9850_RFCTL29			SPRD_PIN_INFO(115, 0, 0, 0, 0)
#define SC9850_SCL2			SPRD_PIN_INFO(117, 0, 0, 0, 0)
#define SC9850_SDA2			SPRD_PIN_INFO(119, 0, 0, 0, 0)
#define SC9850_MTCK_ARM			SPRD_PIN_INFO(121, 0, 0, 0, 0)
#define SC9850_MTMS_ARM			SPRD_PIN_INFO(123, 0, 0, 0, 0)
#define SC9850_XTL_EN0			SPRD_PIN_INFO(125, 0, 0, 0, 0)
#define SC9850_PTEST			SPRD_PIN_INFO(127, 0, 0, 0, 0)
#define SC9850_AUD_DAD1			SPRD_PIN_INFO(129, 0, 0, 0, 0)
#define SC9850_AUD_ADD0			SPRD_PIN_INFO(131, 0, 0, 0, 0)
#define SC9850_AUD_ADSYNC		SPRD_PIN_INFO(133, 0, 0, 0, 0)
#define SC9850_AUD_SCLK			SPRD_PIN_INFO(135, 0, 0, 0, 0)
#define SC9850_CHIP_SLEEP		SPRD_PIN_INFO(137, 0, 0, 0, 0)
#define SC9850_CLK_32K			SPRD_PIN_INFO(139, 0, 0, 0, 0)
#define SC9850_DCDC_ARM_EN		SPRD_PIN_INFO(141, 0, 0, 0, 0)
#define SC9850_EXT_RST_B		SPRD_PIN_INFO(143, 0, 0, 0, 0)
#define SC9850_ADI_D			SPRD_PIN_INFO(145, 0, 0, 0, 0)
#define SC9850_ADI_SCLK			SPRD_PIN_INFO(147, 0, 0, 0, 0)
#define SC9850_XTL_EN1			SPRD_PIN_INFO(149, 0, 0, 0, 0)
#define SC9850_ANA_INT			SPRD_PIN_INFO(151, 0, 0, 0, 0)
#define SC9850_AUD_DAD0			SPRD_PIN_INFO(153, 0, 0, 0, 0)
#define SC9850_AUD_DASYNC		SPRD_PIN_INFO(155, 0, 0, 0, 0)
#define SC9850_LCM_RSTN			SPRD_PIN_INFO(157, 0, 0, 0, 0)
#define SC9850_DSI_TE			SPRD_PIN_INFO(159, 0, 0, 0, 0)
#define SC9850_PWMA			SPRD_PIN_INFO(161, 0, 0, 0, 0)
#define SC9850_EXTINT0			SPRD_PIN_INFO(163, 0, 0, 0, 0)
#define SC9850_EXTINT1			SPRD_PIN_INFO(165, 0, 0, 0, 0)
#define SC9850_SDA1			SPRD_PIN_INFO(167, 0, 0, 0, 0)
#define SC9850_SCL1			SPRD_PIN_INFO(169, 0, 0, 0, 0)
#define SC9850_SIMCLK2			SPRD_PIN_INFO(171, 0, 0, 0, 0)
#define SC9850_SIMDA2			SPRD_PIN_INFO(173, 0, 0, 0, 0)
#define SC9850_SIMRST2			SPRD_PIN_INFO(175, 0, 0, 0, 0)
#define SC9850_SIMCLK1			SPRD_PIN_INFO(177, 0, 0, 0, 0)
#define SC9850_SIMDA1			SPRD_PIN_INFO(179, 0, 0, 0, 0)
#define SC9850_SIMRST1			SPRD_PIN_INFO(181, 0, 0, 0, 0)
#define SC9850_SIMCLK0			SPRD_PIN_INFO(183, 0, 0, 0, 0)
#define SC9850_SIMDA0			SPRD_PIN_INFO(185, 0, 0, 0, 0)
#define SC9850_SIMRST0			SPRD_PIN_INFO(187, 0, 0, 0, 0)
#define SC9850_SD2_CMD			SPRD_PIN_INFO(189, 0, 0, 0, 0)
#define SC9850_SD2_D0			SPRD_PIN_INFO(191, 0, 0, 0, 0)
#define SC9850_SD2_D1			SPRD_PIN_INFO(193, 0, 0, 0, 0)
#define SC9850_SD2_CLK			SPRD_PIN_INFO(195, 0, 0, 0, 0)
#define SC9850_SD2_D2			SPRD_PIN_INFO(197, 0, 0, 0, 0)
#define SC9850_SD2_D3			SPRD_PIN_INFO(199, 0, 0, 0, 0)
#define SC9850_SD0_D3			SPRD_PIN_INFO(201, 0, 0, 0, 0)
#define SC9850_SD0_D2			SPRD_PIN_INFO(203, 0, 0, 0, 0)
#define SC9850_SD0_CMD			SPRD_PIN_INFO(205, 0, 0, 0, 0)
#define SC9850_SD0_D0			SPRD_PIN_INFO(207, 0, 0, 0, 0)
#define SC9850_SD0_D1			SPRD_PIN_INFO(209, 0, 0, 0, 0)
#define SC9850_SD0_CLK			SPRD_PIN_INFO(211, 0, 0, 0, 0)
#define SC9850_EMMC_CMD_reserved	SPRD_PIN_INFO(213, 0, 0, 0, 0)
#define SC9850_EMMC_CMD			SPRD_PIN_INFO(215, 0, 0, 0, 0)
#define SC9850_EMMC_D6			SPRD_PIN_INFO(217, 0, 0, 0, 0)
#define SC9850_EMMC_D7			SPRD_PIN_INFO(219, 0, 0, 0, 0)
#define SC9850_EMMC_CLK			SPRD_PIN_INFO(221, 0, 0, 0, 0)
#define SC9850_EMMC_D5			SPRD_PIN_INFO(223, 0, 0, 0, 0)
#define SC9850_EMMC_D4			SPRD_PIN_INFO(225, 0, 0, 0, 0)
#define SC9850_EMMC_DS			SPRD_PIN_INFO(227, 0, 0, 0, 0)
#define SC9850_EMMC_D3_reserved		SPRD_PIN_INFO(229, 0, 0, 0, 0)
#define SC9850_EMMC_D3			SPRD_PIN_INFO(231, 0, 0, 0, 0)
#define SC9850_EMMC_RST			SPRD_PIN_INFO(233, 0, 0, 0, 0)
#define SC9850_EMMC_D1			SPRD_PIN_INFO(235, 0, 0, 0, 0)
#define SC9850_EMMC_D2			SPRD_PIN_INFO(237, 0, 0, 0, 0)
#define SC9850_EMMC_D0			SPRD_PIN_INFO(239, 0, 0, 0, 0)
#define SC9850_IIS0DI			SPRD_PIN_INFO(241, 0, 0, 0, 0)
#define SC9850_IIS0DO			SPRD_PIN_INFO(243, 0, 0, 0, 0)
#define SC9850_IIS0CLK			SPRD_PIN_INFO(245, 0, 0, 0, 0)
#define SC9850_IIS0LRCK			SPRD_PIN_INFO(247, 0, 0, 0, 0)
#define SC9850_SD1_CLK			SPRD_PIN_INFO(249, 0, 0, 0, 0)
#define SC9850_SD1_CMD			SPRD_PIN_INFO(251, 0, 0, 0, 0)
#define SC9850_SD1_D0			SPRD_PIN_INFO(253, 0, 0, 0, 0)
#define SC9850_SD1_D1			SPRD_PIN_INFO(255, 0, 0, 0, 0)
#define SC9850_SD1_D2			SPRD_PIN_INFO(257, 0, 0, 0, 0)
#define SC9850_SD1_D3			SPRD_PIN_INFO(259, 0, 0, 0, 0)
#define SC9850_CLK_AUX0			SPRD_PIN_INFO(261, 0, 0, 0, 0)
#define SC9850_WIFI_COEXIST		SPRD_PIN_INFO(263, 0, 0, 0, 0)
#define SC9850_BEIDOU_COEXIST		SPRD_PIN_INFO(265, 0, 0, 0, 0)
#define SC9850_U3TXD			SPRD_PIN_INFO(267, 0, 0, 0, 0)
#define SC9850_U3RXD			SPRD_PIN_INFO(269, 0, 0, 0, 0)
#define SC9850_U3CTS			SPRD_PIN_INFO(271, 0, 0, 0, 0)
#define SC9850_U3RTS			SPRD_PIN_INFO(273, 0, 0, 0, 0)
#define SC9850_U0TXD			SPRD_PIN_INFO(275, 0, 0, 0, 0)
#define SC9850_U0RXD			SPRD_PIN_INFO(277, 0, 0, 0, 0)
#define SC9850_U0CTS			SPRD_PIN_INFO(279, 0, 0, 0, 0)
#define SC9850_U0RTS			SPRD_PIN_INFO(281, 0, 0, 0, 0)
#define SC9850_IIS1DI			SPRD_PIN_INFO(283, 0, 0, 0, 0)
#define SC9850_IIS1DO			SPRD_PIN_INFO(285, 0, 0, 0, 0)
#define SC9850_IIS1CLK			SPRD_PIN_INFO(287, 0, 0, 0, 0)
#define SC9850_IIS1LRCK			SPRD_PIN_INFO(289, 0, 0, 0, 0)
#define SC9850_SPI0_CSN			SPRD_PIN_INFO(291, 0, 0, 0, 0)
#define SC9850_SPI0_DO			SPRD_PIN_INFO(293, 0, 0, 0, 0)
#define SC9850_SPI0_DI			SPRD_PIN_INFO(295, 0, 0, 0, 0)
#define SC9850_SPI0_CLK			SPRD_PIN_INFO(297, 0, 0, 0, 0)
#define SC9850_U2TXD			SPRD_PIN_INFO(299, 0, 0, 0, 0)
#define SC9850_U2RXD			SPRD_PIN_INFO(301, 0, 0, 0, 0)
#define SC9850_U4TXD			SPRD_PIN_INFO(303, 0, 0, 0, 0)
#define SC9850_U4RXD			SPRD_PIN_INFO(305, 0, 0, 0, 0)
#define SC9850_CMMCLK1			SPRD_PIN_INFO(307, 0, 0, 0, 0)
#define SC9850_CMRST1			SPRD_PIN_INFO(309, 0, 0, 0, 0)
#define SC9850_CMMCLK0			SPRD_PIN_INFO(311, 0, 0, 0, 0)
#define SC9850_CMRST0			SPRD_PIN_INFO(313, 0, 0, 0, 0)
#define SC9850_CMPD0			SPRD_PIN_INFO(315, 0, 0, 0, 0)
#define SC9850_CMPD1			SPRD_PIN_INFO(317, 0, 0, 0, 0)
#define SC9850_SCL0			SPRD_PIN_INFO(319, 0, 0, 0, 0)
#define SC9850_SDA0			SPRD_PIN_INFO(321, 0, 0, 0, 0)
#define SC9850_SDA6			SPRD_PIN_INFO(323, 0, 0, 0, 0)
#define SC9850_SCL6			SPRD_PIN_INFO(325, 0, 0, 0, 0)
#define SC9850_U1TXD			SPRD_PIN_INFO(327, 0, 0, 0, 0)
#define SC9850_U1RXD			SPRD_PIN_INFO(329, 0, 0, 0, 0)
#define SC9850_KEYOUT0			SPRD_PIN_INFO(331, 0, 0, 0, 0)
#define SC9850_KEYOUT1			SPRD_PIN_INFO(333, 0, 0, 0, 0)
#define SC9850_KEYOUT2			SPRD_PIN_INFO(335, 0, 0, 0, 0)
#define SC9850_KEYIN0			SPRD_PIN_INFO(337, 0, 0, 0, 0)
#define SC9850_KEYIN1			SPRD_PIN_INFO(339, 0, 0, 0, 0)
#define SC9850_KEYIN2			SPRD_PIN_INFO(341, 0, 0, 0, 0)
#define SC9850_IIS3DI			SPRD_PIN_INFO(343, 0, 0, 0, 0)
#define SC9850_IIS3DO			SPRD_PIN_INFO(345, 0, 0, 0, 0)
#define SC9850_IIS3CLK			SPRD_PIN_INFO(347, 0, 0, 0, 0)
#define SC9850_IIS3LRCK			SPRD_PIN_INFO(349, 0, 0, 0, 0)
#define SC9850_RFCTL0			SPRD_PIN_INFO(351, 0, 0, 0, 0)
#define SC9850_RFCTL1			SPRD_PIN_INFO(353, 0, 0, 0, 0)
#define SC9850_RFCTL10			SPRD_PIN_INFO(355, 0, 0, 0, 0)
#define SC9850_RFCTL11			SPRD_PIN_INFO(357, 0, 0, 0, 0)
#define SC9850_RFCTL12			SPRD_PIN_INFO(359, 0, 0, 0, 0)
#define SC9850_RFCTL13			SPRD_PIN_INFO(361, 0, 0, 0, 0)
#define SC9850_RFCTL14			SPRD_PIN_INFO(363, 0, 0, 0, 0)
#define SC9850_RFCTL15			SPRD_PIN_INFO(365, 0, 0, 0, 0)
#define SC9850_RFCTL16			SPRD_PIN_INFO(367, 0, 0, 0, 0)
#define SC9850_RFCTL17			SPRD_PIN_INFO(369, 0, 0, 0, 0)
#define SC9850_RFCTL18			SPRD_PIN_INFO(371, 0, 0, 0, 0)
#define SC9850_RFCTL19			SPRD_PIN_INFO(373, 0, 0, 0, 0)
#define SC9850_RFCTL2			SPRD_PIN_INFO(375, 0, 0, 0, 0)
#define SC9850_EXTINT5			SPRD_PIN_INFO(377, 0, 0, 0, 0)
#define SC9850_EXTINT6			SPRD_PIN_INFO(379, 0, 0, 0, 0)
#define SC9850_EXTINT7			SPRD_PIN_INFO(381, 0, 0, 0, 0)
#define SC9850_GPIO30			SPRD_PIN_INFO(383, 0, 0, 0, 0)
#define SC9850_GPIO31			SPRD_PIN_INFO(385, 0, 0, 0, 0)
#define SC9850_GPIO32			SPRD_PIN_INFO(387, 0, 0, 0, 0)
#define SC9850_GPIO33			SPRD_PIN_INFO(389, 0, 0, 0, 0)
#define SC9850_GPIO34			SPRD_PIN_INFO(391, 0, 0, 0, 0)
#define SC9850_RFCTL3			SPRD_PIN_INFO(393, 0, 0, 0, 0)
#define SC9850_RFCTL4			SPRD_PIN_INFO(395, 0, 0, 0, 0)
#define SC9850_RFCTL5			SPRD_PIN_INFO(397, 0, 0, 0, 0)
#define SC9850_RFCTL6			SPRD_PIN_INFO(399, 0, 0, 0, 0)
#define SC9850_RFCTL7			SPRD_PIN_INFO(401, 0, 0, 0, 0)
#define SC9850_RFCTL8			SPRD_PIN_INFO(403, 0, 0, 0, 0)
#define SC9850_RFCTL9			SPRD_PIN_INFO(405, 0, 0, 0, 0)
#define SC9850_RFFE0_SCK0		SPRD_PIN_INFO(407, 0, 0, 0, 0)
#define SC9850_GPIO38			SPRD_PIN_INFO(409, 0, 0, 0, 0)
#define SC9850_RFFE0_SDA0		SPRD_PIN_INFO(411, 0, 0, 0, 0)
#define SC9850_GPIO39			SPRD_PIN_INFO(413, 0, 0, 0, 0)
#define SC9850_RFFE1_SCK0		SPRD_PIN_INFO(415, 0, 0, 0, 0)
#define SC9850_GPIO181			SPRD_PIN_INFO(417, 0, 0, 0, 0)
#define SC9850_RFFE1_SDA0		SPRD_PIN_INFO(419, 0, 0, 0, 0)
#define SC9850_GPIO182			SPRD_PIN_INFO(421, 0, 0, 0, 0)
#define SC9850_RF_LVDS0_ADC_ON		SPRD_PIN_INFO(423, 0, 0, 0, 0)
#define SC9850_RF_LVDS0_DAC_ON		SPRD_PIN_INFO(425, 0, 0, 0, 0)
#define SC9850_RFSCK0			SPRD_PIN_INFO(427, 0, 0, 0, 0)
#define SC9850_RFSDA0			SPRD_PIN_INFO(429, 0, 0, 0, 0)
#define SC9850_RFSEN0			SPRD_PIN_INFO(431, 0, 0, 0, 0)
#define SC9850_RF_LVDS1_ADC_ON		SPRD_PIN_INFO(433, 0, 0, 0, 0)
#define SC9850_RF_LVDS1_DAC_ON		SPRD_PIN_INFO(435, 0, 0, 0, 0)
#define SC9850_RFSCK1			SPRD_PIN_INFO(437, 0, 0, 0, 0)
#define SC9850_RFSDA1			SPRD_PIN_INFO(439, 0, 0, 0, 0)
#define SC9850_RFSEN1			SPRD_PIN_INFO(441, 0, 0, 0, 0)
#define SC9850_RFCTL38			SPRD_PIN_INFO(443, 0, 0, 0, 0)
#define SC9850_RFCTL39			SPRD_PIN_INFO(445, 0, 0, 0, 0)

/* registers definitions for controller CTL_PIN_MISC */

#define SC9850_RFCTL20_MISC			SPRD_PIN_INFO(82, 0, 0, 0, 0)
#define SC9850_RFCTL21_MISC			SPRD_PIN_INFO(84, 0, 0, 0, 0)
#define SC9850_RFCTL30_MISC			SPRD_PIN_INFO(86, 0, 0, 0, 0)
#define SC9850_RFCTL31_MISC			SPRD_PIN_INFO(88, 0, 0, 0, 0)
#define SC9850_RFCTL32_MISC			SPRD_PIN_INFO(90, 0, 0, 0, 0)
#define SC9850_RFCTL33_MISC			SPRD_PIN_INFO(92, 0, 0, 0, 0)
#define SC9850_RFCTL34_MISC			SPRD_PIN_INFO(94, 0, 0, 0, 0)
#define SC9850_RFCTL35_MISC			SPRD_PIN_INFO(96, 0, 0, 0, 0)
#define SC9850_RFCTL36_MISC			SPRD_PIN_INFO(98, 0, 0, 0, 0)
#define SC9850_RFCTL37_MISC			SPRD_PIN_INFO(100, 0, 0, 0, 0)
#define SC9850_RFCTL22_MISC			SPRD_PIN_INFO(102, 0, 0, 0, 0)
#define SC9850_RFCTL23_MISC			SPRD_PIN_INFO(104, 0, 0, 0, 0)
#define SC9850_RFCTL24_MISC			SPRD_PIN_INFO(106, 0, 0, 0, 0)
#define SC9850_RFCTL25_MISC			SPRD_PIN_INFO(108, 0, 0, 0, 0)
#define SC9850_RFCTL26_MISC			SPRD_PIN_INFO(110, 0, 0, 0, 0)
#define SC9850_RFCTL27_MISC			SPRD_PIN_INFO(112, 0, 0, 0, 0)
#define SC9850_RFCTL28_MISC			SPRD_PIN_INFO(114, 0, 0, 0, 0)
#define SC9850_RFCTL29_MISC			SPRD_PIN_INFO(116, 0, 0, 0, 0)
#define SC9850_SCL2_MISC			SPRD_PIN_INFO(118, 0, 0, 0, 0)
#define SC9850_SDA2_MISC			SPRD_PIN_INFO(120, 0, 0, 0, 0)
#define SC9850_MTCK_ARM_MISC			SPRD_PIN_INFO(122, 0, 0, 0, 0)
#define SC9850_MTMS_ARM_MISC			SPRD_PIN_INFO(124, 0, 0, 0, 0)
#define SC9850_XTL_EN0_MISC			SPRD_PIN_INFO(126, 0, 0, 0, 0)
#define SC9850_PTEST_MISC			SPRD_PIN_INFO(128, 0, 0, 0, 0)
#define SC9850_AUD_DAD1_MISC			SPRD_PIN_INFO(130, 0, 0, 0, 0)
#define SC9850_AUD_ADD0_MISC			SPRD_PIN_INFO(132, 0, 0, 0, 0)
#define SC9850_AUD_ADSYNC_MISC			SPRD_PIN_INFO(134, 0, 0, 0, 0)
#define SC9850_AUD_SCLK_MISC			SPRD_PIN_INFO(136, 0, 0, 0, 0)
#define SC9850_CHIP_SLEEP_MISC			SPRD_PIN_INFO(138, 0, 0, 0, 0)
#define SC9850_CLK_32K_MISC			SPRD_PIN_INFO(140, 0, 0, 0, 0)
#define SC9850_DCDC_ARM_EN_MISC			SPRD_PIN_INFO(142, 0, 0, 0, 0)
#define SC9850_EXT_RST_B_MISC			SPRD_PIN_INFO(144, 0, 0, 0, 0)
#define SC9850_ADI_D_MISC			SPRD_PIN_INFO(146, 0, 0, 0, 0)
#define SC9850_ADI_SCLK_MISC			SPRD_PIN_INFO(148, 0, 0, 0, 0)
#define SC9850_XTL_EN1_MISC			SPRD_PIN_INFO(150, 0, 0, 0, 0)
#define SC9850_ANA_INT_MISC			SPRD_PIN_INFO(152, 0, 0, 0, 0)
#define SC9850_AUD_DAD0_MISC			SPRD_PIN_INFO(154, 0, 0, 0, 0)
#define SC9850_AUD_DASYNC_MISC			SPRD_PIN_INFO(156, 0, 0, 0, 0)
#define SC9850_LCM_RSTN_MISC			SPRD_PIN_INFO(158, 0, 0, 0, 0)
#define SC9850_DSI_TE_MISC			SPRD_PIN_INFO(160, 0, 0, 0, 0)
#define SC9850_PWMA_MISC			SPRD_PIN_INFO(162, 0, 0, 0, 0)
#define SC9850_EXTINT0_MISC			SPRD_PIN_INFO(164, 0, 0, 0, 0)
#define SC9850_EXTINT1_MISC			SPRD_PIN_INFO(166, 0, 0, 0, 0)
#define SC9850_SDA1_MISC			SPRD_PIN_INFO(168, 0, 0, 0, 0)
#define SC9850_SCL1_MISC			SPRD_PIN_INFO(170, 0, 0, 0, 0)
#define SC9850_SIMCLK2_MISC			SPRD_PIN_INFO(172, 0, 0, 0, 0)
#define SC9850_SIMDA2_MISC			SPRD_PIN_INFO(174, 0, 0, 0, 0)
#define SC9850_SIMRST2_MISC			SPRD_PIN_INFO(176, 0, 0, 0, 0)
#define SC9850_SIMCLK1_MISC			SPRD_PIN_INFO(178, 0, 0, 0, 0)
#define SC9850_SIMDA1_MISC			SPRD_PIN_INFO(180, 0, 0, 0, 0)
#define SC9850_SIMRST1_MISC			SPRD_PIN_INFO(182, 0, 0, 0, 0)
#define SC9850_SIMCLK0_MISC			SPRD_PIN_INFO(184, 0, 0, 0, 0)
#define SC9850_SIMDA0_MISC			SPRD_PIN_INFO(186, 0, 0, 0, 0)
#define SC9850_SIMRST0_MISC			SPRD_PIN_INFO(188, 0, 0, 0, 0)
#define SC9850_SD2_CMD_MISC			SPRD_PIN_INFO(190, 0, 0, 0, 0)
#define SC9850_SD2_D0_MISC			SPRD_PIN_INFO(192, 0, 0, 0, 0)
#define SC9850_SD2_D1_MISC			SPRD_PIN_INFO(194, 0, 0, 0, 0)
#define SC9850_SD2_CLK_MISC			SPRD_PIN_INFO(196, 0, 0, 0, 0)
#define SC9850_SD2_D2_MISC			SPRD_PIN_INFO(198, 0, 0, 0, 0)
#define SC9850_SD2_D3_MISC			SPRD_PIN_INFO(200, 0, 0, 0, 0)
#define SC9850_SD0_D3_MISC			SPRD_PIN_INFO(202, 0, 0, 0, 0)
#define SC9850_SD0_D2_MISC			SPRD_PIN_INFO(204, 0, 0, 0, 0)
#define SC9850_SD0_CMD_MISC			SPRD_PIN_INFO(206, 0, 0, 0, 0)
#define SC9850_SD0_D0_MISC			SPRD_PIN_INFO(208, 0, 0, 0, 0)
#define SC9850_SD0_D1_MISC			SPRD_PIN_INFO(210, 0, 0, 0, 0)
#define SC9850_SD0_CLK_MISC			SPRD_PIN_INFO(212, 0, 0, 0, 0)
#define SC9850_EMMC_CMD_reserved_MISC		SPRD_PIN_INFO(214, 0, 0, 0, 0)
#define SC9850_EMMC_CMD_MISC			SPRD_PIN_INFO(216, 0, 0, 0, 0)
#define SC9850_EMMC_D6_MISC			SPRD_PIN_INFO(218, 0, 0, 0, 0)
#define SC9850_EMMC_D7_MISC			SPRD_PIN_INFO(220, 0, 0, 0, 0)
#define SC9850_EMMC_CLK_MISC			SPRD_PIN_INFO(222, 0, 0, 0, 0)
#define SC9850_EMMC_D5_MISC			SPRD_PIN_INFO(224, 0, 0, 0, 0)
#define SC9850_EMMC_D4_MISC			SPRD_PIN_INFO(226, 0, 0, 0, 0)
#define SC9850_EMMC_DS_MISC			SPRD_PIN_INFO(228, 0, 0, 0, 0)
#define SC9850_EMMC_D3_reserved_MISC		SPRD_PIN_INFO(230, 0, 0, 0, 0)
#define SC9850_EMMC_D3_MISC			SPRD_PIN_INFO(232, 0, 0, 0, 0)
#define SC9850_EMMC_RST_MISC			SPRD_PIN_INFO(234, 0, 0, 0, 0)
#define SC9850_EMMC_D1_MISC			SPRD_PIN_INFO(236, 0, 0, 0, 0)
#define SC9850_EMMC_D2_MISC			SPRD_PIN_INFO(238, 0, 0, 0, 0)
#define SC9850_EMMC_D0_MISC			SPRD_PIN_INFO(240, 0, 0, 0, 0)
#define SC9850_IIS0DI_MISC			SPRD_PIN_INFO(242, 0, 0, 0, 0)
#define SC9850_IIS0DO_MISC			SPRD_PIN_INFO(244, 0, 0, 0, 0)
#define SC9850_IIS0CLK_MISC			SPRD_PIN_INFO(246, 0, 0, 0, 0)
#define SC9850_IIS0LRCK_MISC			SPRD_PIN_INFO(248, 0, 0, 0, 0)
#define SC9850_SD1_CLK_MISC			SPRD_PIN_INFO(250, 0, 0, 0, 0)
#define SC9850_SD1_CMD_MISC			SPRD_PIN_INFO(252, 0, 0, 0, 0)
#define SC9850_SD1_D0_MISC			SPRD_PIN_INFO(254, 0, 0, 0, 0)
#define SC9850_SD1_D1_MISC			SPRD_PIN_INFO(256, 0, 0, 0, 0)
#define SC9850_SD1_D2_MISC			SPRD_PIN_INFO(258, 0, 0, 0, 0)
#define SC9850_SD1_D3_MISC			SPRD_PIN_INFO(260, 0, 0, 0, 0)
#define SC9850_CLK_AUX0_MISC			SPRD_PIN_INFO(262, 0, 0, 0, 0)
#define SC9850_WIFI_COEXIST_MISC		SPRD_PIN_INFO(264, 0, 0, 0, 0)
#define SC9850_BEIDOU_COEXIST_MISC		SPRD_PIN_INFO(266, 0, 0, 0, 0)
#define SC9850_U3TXD_MISC			SPRD_PIN_INFO(268, 0, 0, 0, 0)
#define SC9850_U3RXD_MISC			SPRD_PIN_INFO(270, 0, 0, 0, 0)
#define SC9850_U3CTS_MISC			SPRD_PIN_INFO(272, 0, 0, 0, 0)
#define SC9850_U3RTS_MISC			SPRD_PIN_INFO(274, 0, 0, 0, 0)
#define SC9850_U0TXD_MISC			SPRD_PIN_INFO(276, 0, 0, 0, 0)
#define SC9850_U0RXD_MISC			SPRD_PIN_INFO(278, 0, 0, 0, 0)
#define SC9850_U0CTS_MISC			SPRD_PIN_INFO(280, 0, 0, 0, 0)
#define SC9850_U0RTS_MISC			SPRD_PIN_INFO(282, 0, 0, 0, 0)
#define SC9850_IIS1DI_MISC			SPRD_PIN_INFO(284, 0, 0, 0, 0)
#define SC9850_IIS1DO_MISC			SPRD_PIN_INFO(286, 0, 0, 0, 0)
#define SC9850_IIS1CLK_MISC			SPRD_PIN_INFO(288, 0, 0, 0, 0)
#define SC9850_IIS1LRCK_MISC			SPRD_PIN_INFO(290, 0, 0, 0, 0)
#define SC9850_SPI0_CSN_MISC			SPRD_PIN_INFO(292, 0, 0, 0, 0)
#define SC9850_SPI0_DO_MISC			SPRD_PIN_INFO(294, 0, 0, 0, 0)
#define SC9850_SPI0_DI_MISC			SPRD_PIN_INFO(296, 0, 0, 0, 0)
#define SC9850_SPI0_CLK_MISC			SPRD_PIN_INFO(298, 0, 0, 0, 0)
#define SC9850_U2TXD_MISC			SPRD_PIN_INFO(300, 0, 0, 0, 0)
#define SC9850_U2RXD_MISC			SPRD_PIN_INFO(302, 0, 0, 0, 0)
#define SC9850_U4TXD_MISC			SPRD_PIN_INFO(304, 0, 0, 0, 0)
#define SC9850_U4RXD_MISC			SPRD_PIN_INFO(306, 0, 0, 0, 0)
#define SC9850_CMMCLK1_MISC			SPRD_PIN_INFO(308, 0, 0, 0, 0)
#define SC9850_CMRST1_MISC			SPRD_PIN_INFO(310, 0, 0, 0, 0)
#define SC9850_CMMCLK0_MISC			SPRD_PIN_INFO(312, 0, 0, 0, 0)
#define SC9850_CMRST0_MISC			SPRD_PIN_INFO(314, 0, 0, 0, 0)
#define SC9850_CMPD0_MISC			SPRD_PIN_INFO(316, 0, 0, 0, 0)
#define SC9850_CMPD1_MISC			SPRD_PIN_INFO(318, 0, 0, 0, 0)
#define SC9850_SCL0_MISC			SPRD_PIN_INFO(320, 0, 0, 0, 0)
#define SC9850_SDA0_MISC			SPRD_PIN_INFO(322, 0, 0, 0, 0)
#define SC9850_SDA6_MISC			SPRD_PIN_INFO(324, 0, 0, 0, 0)
#define SC9850_SCL6_MISC			SPRD_PIN_INFO(326, 0, 0, 0, 0)
#define SC9850_U1TXD_MISC			SPRD_PIN_INFO(328, 0, 0, 0, 0)
#define SC9850_U1RXD_MISC			SPRD_PIN_INFO(330, 0, 0, 0, 0)
#define SC9850_KEYOUT0_MISC			SPRD_PIN_INFO(332, 0, 0, 0, 0)
#define SC9850_KEYOUT1_MISC			SPRD_PIN_INFO(334, 0, 0, 0, 0)
#define SC9850_KEYOUT2_MISC			SPRD_PIN_INFO(336, 0, 0, 0, 0)
#define SC9850_KEYIN0_MISC			SPRD_PIN_INFO(338, 0, 0, 0, 0)
#define SC9850_KEYIN1_MISC			SPRD_PIN_INFO(340, 0, 0, 0, 0)
#define SC9850_KEYIN2_MISC			SPRD_PIN_INFO(342, 0, 0, 0, 0)
#define SC9850_IIS3DI_MISC			SPRD_PIN_INFO(344, 0, 0, 0, 0)
#define SC9850_IIS3DO_MISC			SPRD_PIN_INFO(346, 0, 0, 0, 0)
#define SC9850_IIS3CLK_MISC			SPRD_PIN_INFO(348, 0, 0, 0, 0)
#define SC9850_IIS3LRCK_MISC			SPRD_PIN_INFO(350, 0, 0, 0, 0)
#define SC9850_RFCTL0_MISC			SPRD_PIN_INFO(352, 0, 0, 0, 0)
#define SC9850_RFCTL1_MISC			SPRD_PIN_INFO(354, 0, 0, 0, 0)
#define SC9850_RFCTL10_MISC			SPRD_PIN_INFO(356, 0, 0, 0, 0)
#define SC9850_RFCTL11_MISC			SPRD_PIN_INFO(358, 0, 0, 0, 0)
#define SC9850_RFCTL12_MISC			SPRD_PIN_INFO(360, 0, 0, 0, 0)
#define SC9850_RFCTL13_MISC			SPRD_PIN_INFO(362, 0, 0, 0, 0)
#define SC9850_RFCTL14_MISC			SPRD_PIN_INFO(364, 0, 0, 0, 0)
#define SC9850_RFCTL15_MISC			SPRD_PIN_INFO(366, 0, 0, 0, 0)
#define SC9850_RFCTL16_MISC			SPRD_PIN_INFO(368, 0, 0, 0, 0)
#define SC9850_RFCTL17_MISC			SPRD_PIN_INFO(370, 0, 0, 0, 0)
#define SC9850_RFCTL18_MISC			SPRD_PIN_INFO(372, 0, 0, 0, 0)
#define SC9850_RFCTL19_MISC			SPRD_PIN_INFO(374, 0, 0, 0, 0)
#define SC9850_RFCTL2_MISC			SPRD_PIN_INFO(376, 0, 0, 0, 0)
#define SC9850_EXTINT5_MISC			SPRD_PIN_INFO(378, 0, 0, 0, 0)
#define SC9850_EXTINT6_MISC			SPRD_PIN_INFO(380, 0, 0, 0, 0)
#define SC9850_EXTINT7_MISC			SPRD_PIN_INFO(382, 0, 0, 0, 0)
#define SC9850_GPIO30_MISC			SPRD_PIN_INFO(384, 0, 0, 0, 0)
#define SC9850_GPIO31_MISC			SPRD_PIN_INFO(386, 0, 0, 0, 0)
#define SC9850_GPIO32_MISC			SPRD_PIN_INFO(388, 0, 0, 0, 0)
#define SC9850_GPIO33_MISC			SPRD_PIN_INFO(390, 0, 0, 0, 0)
#define SC9850_GPIO34_MISC			SPRD_PIN_INFO(392, 0, 0, 0, 0)
#define SC9850_RFCTL3_MISC			SPRD_PIN_INFO(394, 0, 0, 0, 0)
#define SC9850_RFCTL4_MISC			SPRD_PIN_INFO(396, 0, 0, 0, 0)
#define SC9850_RFCTL5_MISC			SPRD_PIN_INFO(398, 0, 0, 0, 0)
#define SC9850_RFCTL6_MISC			SPRD_PIN_INFO(400, 0, 0, 0, 0)
#define SC9850_RFCTL7_MISC			SPRD_PIN_INFO(402, 0, 0, 0, 0)
#define SC9850_RFCTL8_MISC			SPRD_PIN_INFO(404, 0, 0, 0, 0)
#define SC9850_RFCTL9_MISC			SPRD_PIN_INFO(406, 0, 0, 0, 0)
#define SC9850_RFFE0_SCK0_MISC			SPRD_PIN_INFO(408, 0, 0, 0, 0)
#define SC9850_GPIO38_MISC			SPRD_PIN_INFO(410, 0, 0, 0, 0)
#define SC9850_RFFE0_SDA0_MISC			SPRD_PIN_INFO(412, 0, 0, 0, 0)
#define SC9850_GPIO39_MISC			SPRD_PIN_INFO(414, 0, 0, 0, 0)
#define SC9850_RFFE1_SCK0_MISC			SPRD_PIN_INFO(416, 0, 0, 0, 0)
#define SC9850_GPIO181_MISC			SPRD_PIN_INFO(418, 0, 0, 0, 0)
#define SC9850_RFFE1_SDA0_MISC			SPRD_PIN_INFO(420, 0, 0, 0, 0)
#define SC9850_GPIO182_MISC			SPRD_PIN_INFO(422, 0, 0, 0, 0)
#define SC9850_RF_LVDS0_ADC_ON_MISC		SPRD_PIN_INFO(424, 0, 0, 0, 0)
#define SC9850_RF_LVDS0_DAC_ON_MISC		SPRD_PIN_INFO(426, 0, 0, 0, 0)
#define SC9850_RFSCK0_MISC			SPRD_PIN_INFO(428, 0, 0, 0, 0)
#define SC9850_RFSDA0_MISC			SPRD_PIN_INFO(430, 0, 0, 0, 0)
#define SC9850_RFSEN0_MISC			SPRD_PIN_INFO(432, 0, 0, 0, 0)
#define SC9850_RF_LVDS1_ADC_ON_MISC		SPRD_PIN_INFO(434, 0, 0, 0, 0)
#define SC9850_RF_LVDS1_DAC_ON_MISC		SPRD_PIN_INFO(436, 0, 0, 0, 0)
#define SC9850_RFSCK1_MISC			SPRD_PIN_INFO(438, 0, 0, 0, 0)
#define SC9850_RFSDA1_MISC			SPRD_PIN_INFO(440, 0, 0, 0, 0)
#define SC9850_RFSEN1_MISC			SPRD_PIN_INFO(442, 0, 0, 0, 0)
#define SC9850_RFCTL38_MISC			SPRD_PIN_INFO(444, 0, 0, 0, 0)
#define SC9850_RFCTL39_MISC			SPRD_PIN_INFO(446, 0, 0, 0, 0)
