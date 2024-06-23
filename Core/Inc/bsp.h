
#ifndef __BSP_H
#define __BSP_H

#include "EventRecorder.h"
#include "EventRecorderConf.h"
#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "bsp_dwt.h"
#include "stm32f4xx_hal.h"

#include "main.h"
#include "PUDI_APP.h"  
#include "stdlib.h"
#include "stdio.h"

#include "lwip/udp.h"
#include "bsp_can.h"
#include "bsp_gpio.h"
#include "bsp_uart_fifo.h"
#include "bsp_dma.h"
#include "bsp_uart_dma.h"

#include "ring_buffer.h"
#include "ring_buffer_chapter.h"

#include "udp_echoserver.h"

#include "Task_can.h"
#include "Task_led.h"
#include "Task_lwip.h"
#include "Task_steer_gear.h"

#include "SEGGER_RTT.h"

//#include "Task_start.h"

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */


//#define	DEBUGPR 1


extern int16_t Data_out[200];

extern unsigned short Cal_Sum(uint8_t * Data_In , uint8_t len);
extern void out_sine(unsigned char gethz, unsigned char getdu, unsigned short* getnumber, short dataout[200]);
//extern void out_sine(unsigned char gethz , unsigned char getdu);
/*���ζ��д���*/
extern unsigned char adcbuff[4096]; 
extern ring_buffer rb_adc_rx;

/*���ζ���*/
//extern unsigned char spibuff[4096]; 
//extern ring_buffer rb_spi_rx;	

/*����������1ģʽ*/
extern unsigned char steering_gear_mode1[4096]; 
extern ring_buffer stgear_rx;	


/*����������2ģʽ*/
extern unsigned char steering_gear_mode2[4096]; 
extern ring_buffer stgear_rxmode2;							/*���ζ���*/

/* ���涨ʱ��ID */
extern void timer_Periodic_App(void *argument);
extern osTimerId_t  timerID_Periodic;        				
extern const osTimerAttr_t timer_Periodic_Attr;


extern void SystemClock_Config(void);
extern uint8_t PUDI_ReadAngle(uint8_t *state,uint8_t *CRC8, int32_t* ReadRegData);
extern void bsp_Spi_Cs_InitGpio(void);
extern void bsp_LED_InitGpio(void);

extern ADC_HandleTypeDef hadc1;


/*udpͨ�ý��պ���*/
struct Get_UdPData
{
	unsigned char ucudpdata[50];
};

//#pragma pack(2)

/*��λ���·���������Э��*/
struct GETUDPDATA
{
	unsigned char ucGetUdpHand[2];	//֡ͷ
	unsigned char ucGetUdploop;		//֡���
	unsigned char ucGetUdpID;			//ID
	unsigned char ucSetUdpSampling_Rate[2];	  //������
	unsigned char ucSetUdpSampling_Number[2]; //��������	
	
	/*��������*/
	unsigned char dataucGetUdpHand[2]; 	//��������֡ͷ
	unsigned char dataucGetUdplen;			//֡��
	unsigned char dataucGetUdploop[2];	//��ˮ��
	
	unsigned char dataucsteering_gear1[2];	//���ָ��1L
	unsigned char dataucsteering_gear2[2];	//���ָ��2L
	
	unsigned char dataucChick[2];
	unsigned char dataucend;
};





/*����������1.��λ�����͸���������������*/
struct STEER_UART_DATA
{
	unsigned short 	steer_Hand;			//֡ͷ
	unsigned char 	steer_len;			//֡��
	unsigned short  steer_loop;			//֡���
	unsigned short 	steering_gear1;		//���ָ��1
	unsigned short  steering_gear2;		//���ָ��2
	unsigned short  chickdata;				//У����
	unsigned char  enddata;					//֡β
};

/*����������1.��λ�����ϸ���λ������ֵ*/
struct COM_Steering_gear
{
	unsigned char ucSetUdpHand[2];			//ͷ
	unsigned char ucSetlen;							//����
	unsigned char ucSetUdploop[2];						//���
	unsigned char ucSetUdpID;									//ID
	unsigned char ucSetUdpSampling_Rate[2];	  //������
	unsigned char ucSetUdpSampling_Number[2]; //��������
	unsigned char getucSetlen[2];				//���ճ���
	unsigned char steering_gear1[2];	//���1
	unsigned char steering_gear2[2];	//���2
	unsigned char spi_data[2];				//SPI
	unsigned char chick[2];						//У��
};

/*��λ���յ� -> ��������޸�*/
struct Parameter_Steering_gear
{
	unsigned char ucpsdata[25];
};

/*��λ���·� -> ��������޸�*/



/*��λ���볡����. UDP���ݴ������*/
struct SETUDPDATA
{
	unsigned char ucSetUdpHand[2];						//ͷ
	unsigned char ucSetUdploop;								//���
	unsigned char ucSetUdpID;									//ID
	unsigned char ucSetUdpSampling_Rate[2];	  //������
	unsigned char ucSetUdpSampling_Number[2]; //��������
	unsigned char getdata[800];								//��ADC���� 200��
};


/*����������1*/
struct Steering_gear_Mode1
{
	unsigned char SgUdpHand[2];		//ͷ
	unsigned char SgUdpNum;				//֡���
	unsigned char SgUdpID;				//֡ID
	unsigned char SgUdpSampling_Rate[2];		//������
	unsigned char SgUdpSampling_Number[2];	//������
	unsigned char SgUdpSteering_gear_spi[800];
};


typedef struct
{
	unsigned char *pRxBuf;
	unsigned char rxFlag;
	unsigned char rxLen;
	const unsigned char rxSize;	/*const �����ڽṹ���������ʱ��ֵ */
}BspUart_t;

extern BspUart_t g_tBspUsart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUart5;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart3;
extern BspUart_t g_tBspUsart6;

extern struct GETUDPDATA SetUdpData;




void Error_Handler(void);
/**********	RTX5����	***********/

/************************* ��Ϣ���� *******************************/

/*���ڽ�������UDP����*/
extern osMessageQueueId_t msgQueue_UdpResData; 			
extern const osMessageQueueAttr_t UdpResData_Attr;


/*����1��������*/		
extern osMessageQueueId_t msgQueue_rxuart1Data; 	
extern const osMessageQueueAttr_t rxuart1Data_Attr;


/************************* �¼���־�� *******************************/

/*���߲�α�־ �¼���־��*/
extern osEventFlagsId_t event_cable_ID;
extern const osEventFlagsAttr_t event_cable_Attr;

/* ���ڻ�ȡ���ݽ� �¼���־������ */
extern const osEventFlagsAttr_t event_UdpGetData_Attr;
extern osEventFlagsId_t event_UdpGetData_ID;

/* �������� */
extern const osEventFlagsAttr_t event_Sampling_Attr;
extern osEventFlagsId_t event_Sampling_ID;


extern const osEventFlagsAttr_t event_HzMode_Attr;
extern osEventFlagsId_t event_HzMode_ID;
/************************* ���� *******************************/

extern void AppTaskGetDataAndUDP(void *argument);	/*UDP����������*/
extern const osThreadAttr_t ThreadGetDataAndUDP_Attr;
extern osThreadId_t ThreadIdTaskGetDataAndUDP;


extern osThreadId_t ThreadIdStart;								/*��������*/
extern void AppTaskUDPSend(void *argument);
extern const osThreadAttr_t TaskUDPSend_Attr;
extern osThreadId_t ThreadIdTaskUDPSend;


extern osThreadId_t ThreadIdTaskLwIPPro;					/*LWIP������ѯ*/
extern const osThreadAttr_t ThreadLwIPPro_Attr;
extern void AppTaskLwIPPro(void *argument);

extern osThreadId_t ThreadIdTaskEthCheck;					/*���߲��*/
extern void AppTaskEthCheck(void *argument);
extern const osThreadAttr_t ThreadEthCheck_Attr;


extern void AppTaskUdpNetWork(void *argument);	/* UDP����������� */
extern const osThreadAttr_t ThreadUdpNetWork_Attr;
extern osThreadId_t ThreadIdTaskUdpNetWork;


extern void AppTaskSteering_gear(void *argument);	/*���ָ���������*/
extern const osThreadAttr_t ThreadSteering_gear_Attr;
extern osThreadId_t ThreadIdTaskSteering_gear;

extern void AppTaskARMSteer_gear(void *argument);
extern const osThreadAttr_t ThreadARMSteer_gear_Attr;
extern osThreadId_t ThreadIdTaskARMSteer_gear;


#endif


