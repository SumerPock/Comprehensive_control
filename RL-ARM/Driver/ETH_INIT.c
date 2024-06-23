/*
*********************************************************************************************************
*
*	模块名称 : 以太网初始化
*	文件名称 : ETH_INIT.c
*	版    本 : V1.0
*	说    明 : 以太网相关变量，状态消息和以太网服务程序。
*
*	修改记录 :
*		版本号   日期         作者        说明
*		V1.0    2019-10-12   Eric2013     首版
*
*	Copyright (C), 2019-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"


/*
   移植注意事项：
   除了本文件中的配置，还有三个地方要根据自己制作的硬件做适配
   1、bsp.h文件中，函数MPU_Config做的以太网收发描述符地址空间的MPU配置。
   2、stm32h7xx_hal_conf.h文件做的发送ETH_TX_DESC_CNT和接收ETH_RX_DESC_CNT描述符个数配置。
   3、根据使用的以太网PHY芯片，选择合适的PHY驱动。
*/

/*
*********************************************************************************************************
*	                                  用于本文件的调试
*********************************************************************************************************
*/
#if 1
	#define printf_eth printf
#else
	#define printf_debug(...)
#endif

/*
**********************************************************************************************************
											变量
**********************************************************************************************************
*/

/* 以太网连接状态，0和表示初始临时状态，1表示连接上，2表示断开 */
__IO uint8_t  g_ucEthLinkStatus = 0;  

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
