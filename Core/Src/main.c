/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "includes.h"
#include "bsp.h"
#include <math.h>

static void MX_TIM6_Init(void);

/************************************** ȫ�ֱ��� *******************************************/

/*��λ���볡����*/
unsigned char adcbuff[4096]; 
ring_buffer rb_adc_rx;							/*���ζ���*/

/*����������1ģʽ*/
unsigned char steering_gear_mode1[4096]; 
ring_buffer stgear_rx;							/*���ζ���*/

/*����������2ģʽ*/
unsigned char steering_gear_mode2[4096]; 
ring_buffer stgear_rxmode2;					/*���ζ���*/

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;

SPI_HandleTypeDef hspi2;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);

/************************************** ���� *******************************************/

/*	
AppTaskEthCheck
1.	��������		
		���߲�μ��
2.	���������ݣ�		
		��������Ƿ���룬����������������,Ȼ����˳�����
3.	���ȼ���		 
		High1
4.	��������
		250ms��ѯִ��
*/
void AppTaskEthCheck(void *argument);
const osThreadAttr_t ThreadEthCheck_Attr =
{
	.name = "osRtxEthCheckThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh1,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskEthCheck = NULL;	

/*	
AppTaskLwIPPro
1.	��������		
		LWIP�������ݽ�����ѯ
2.	��������	
		��ѯ������������
3.	���ȼ���		
		High7
4.	��������
		20ms��ѯִ��
*/
void AppTaskLwIPPro(void *argument);
const osThreadAttr_t ThreadLwIPPro_Attr = 		
{
	.name = "osRtxLwIPProThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh7,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskLwIPPro = NULL;	

/*	
AppTaskSteering_gear
1.	��������
		����������ģʽһ
2.	��������
		ȡ������忨���͵Ĵ��ڶ������ݣ������������ش��Ƕ� �� SPI���� ���� ���ζ���stgear_rx�У�
		������忨�ش��Ĳ�����������ת��UDP���ݴ������λ��
3.	���ȼ���
		High4
4.	��������
		����������

*/
void AppTaskSteering_gear(void *argument);
const osThreadAttr_t ThreadSteering_gear_Attr = 
{
	.name = "osRtxSteering_gearThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh4,
	.stack_size = 4096,
};
osThreadId_t ThreadIdTaskSteering_gear = NULL;


/*
AppTaskUdpNetWork
1.	��������	
2.	��������	
		1.	������λ������Ƕȿ���ָ���ͨ��DMA����
		2.	������λ������ѡ�˶�ģʽ����������������������˶�ָ��
		3.	������λ����ֹͣ����ģʽ
		4.	������λ���ĵ�λ���볡����ģʽ��
		5.	������λ���Ķ����������ģʽ��ͨ��DMA����
3.	���ȼ���
		High6
4.	��������
		����������

*/
void AppTaskUdpNetWork(void *argument);					
const osThreadAttr_t ThreadUdpNetWork_Attr =
{
	.name = "osRtxUdpNetWorkThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh6,
	.stack_size = 4096,
};
osThreadId_t ThreadIdTaskUdpNetWork = NULL;


/***
AppTaskGetDataAndUDP
1.	��������		
2.	��������		
		1.	����event_Sampling_ID �л�ģʽ�� ��λ���볡���գ����ζ�������������800����
		2.	����event_Sampling_ID �л�ģʽ������Ƕȼ��1�����ζ�������������800���ͣ�
		3.	����event_Sampling_ID �л�ģʽ������Ƕȼ��2�����ζ�������������800����
3.	���ȼ���		
		High3
5.	��������
		200ms��ѯִ��
***/
void AppTaskGetDataAndUDP(void *argument);		
const osThreadAttr_t ThreadGetDataAndUDP_Attr = 
{
	.name = "osRtxGetDataAndUDPThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh3,
	.stack_size = 12288,
};
osThreadId_t ThreadIdTaskGetDataAndUDP = NULL;


/**
AppTaskStart
1.	��������
			��������
2.	��������
3.	���ȼ���
			High7
6.	��������
			ִ�й���ر�
**/
void AppTaskStart(void *argument);				
const osThreadAttr_t ThreadStart_Attr =
{
	.name = "osRtxStartThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh7,
	.stack_size = 2048,
};
osThreadId_t ThreadIdStart = NULL;


/**
AppTaskARMSteer_gear
1.	��������
2.	��������
			�·������˶�ģʽ���ݣ�������ǰ���͵����ҽǶȺͽ��յ��Ķ�������Ƕȴ��������
3.	���ȼ���
			High5
4.	��������
			����������

**/
void AppTaskARMSteer_gear(void *argument);		
const osThreadAttr_t ThreadARMSteer_gear_Attr = 
{
	.name = "osRtxARMSteer_gearThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 4096,
};
osThreadId_t ThreadIdTaskARMSteer_gear = NULL;


/************************************** ��ʱ�� *******************************************/
void timer_Periodic_App(void *argument);
osTimerId_t  timerID_Periodic = NULL;
const osTimerAttr_t timer_Periodic_Attr = { .name = "periodic timer", };

/************************************** ��Ϣ���� *******************************************/
osMessageQueueId_t msgQueue_UdpResData;				/*����UDP����*/
const osMessageQueueAttr_t UdpResData_Attr ={.name = "UdpResData",};

osMessageQueueId_t msgQueue_rxuart1Data; 			/*����1��������*/		
const osMessageQueueAttr_t rxuart1Data_Attr ={.name = "rxuart1Data", };
/************************************** �¼���־λ **********************************************/

/* ���߲���¼���־������ */
const osEventFlagsAttr_t event_cable_Attr = { .name = "event_cable", };
osEventFlagsId_t event_cable_ID = NULL;

/* ���ڻ�ȡ���ݽ� �¼���־������ */
const osEventFlagsAttr_t event_UdpGetData_Attr = { .name = "event_UdpGetData", };
osEventFlagsId_t event_UdpGetData_ID = NULL;

/* �������� */
const osEventFlagsAttr_t event_Sampling_Attr = { .name = "event_Sampling", };
osEventFlagsId_t event_Sampling_ID = NULL;

/* HZ model ģʽ */
const osEventFlagsAttr_t event_HzMode_Attr = { .name = "event_HzMode", };
osEventFlagsId_t event_HzMode_ID = NULL;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	HAL_SuspendTick();
	osKernelInitialize();                                  
	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr);  
	osKernelStart();
}

/**
  * @brief  ��������
  * @retval None
  */
void AppTaskStart(void *argument)
{
	const uint16_t usFrequency = 100;
	uint32_t tick;	
	HAL_ResumeTick();
	GPIO_LEDInitialization();
	bsp_InitGpio();	
	bsp_LED_InitGpio();
	MX_SPI2_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	
	MX_USART1_UART_Init();
	
	/**UART DMA config**/	
	__HAL_UART_ENABLE_IT(&husart1, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA	/* �������ڽ��� ��С��������󳤶�rxSize */	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#else
	HAL_UART_Receive_IT(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#endif

	bsp_Spi_Cs_InitGpio();
	RB_Init(&rb_adc_rx , 			adcbuff , 						4096);	/* ��λ���볡����ģʽ ����*/
	RB_Init(&stgear_rx , 			steering_gear_mode1 , 4096);	/* ���ģʽ1ģʽ	����*/
	RB_Init(&stgear_rxmode2 , steering_gear_mode2 , 4096);	/* ���ģʽ2ģʽ	����*/
	
	EventRecorderInitialize(EventRecordAll, 1U);						/* ��ʼ��EventRecorder������ */
	EventRecorderStart();		
	
	msgQueue_UdpResData  = 	osMessageQueueNew(12 , sizeof(struct Get_UdPData) , &UdpResData_Attr); /* ����UDP������Ϣ����*/		
	msgQueue_rxuart1Data  = osMessageQueueNew(12 , sizeof(BspUart_t) , 					&rxuart1Data_Attr);	
	event_cable_ID 		= 	osEventFlagsNew(&event_cable_Attr);					
	event_Sampling_ID = 	osEventFlagsNew(&event_Sampling_Attr);
	event_HzMode_ID 	= 	osEventFlagsNew(&event_HzMode_Attr);	
	if(net_init() != 0){
		//printf("lwip is config fild \r\n");
	}
	
	/* LWIP���� udp��ѯ���� */
	ThreadIdTaskLwIPPro = osThreadNew(AppTaskLwIPPro, NULL, &ThreadLwIPPro_Attr);	
	tick += usFrequency + 500;                          
	osDelayUntil(tick);	
	
	/* ���߲��״̬���*/	
	ThreadIdTaskEthCheck = osThreadNew(AppTaskEthCheck, NULL, &ThreadEthCheck_Attr); 	
	tick += usFrequency + 500;                          
	osDelayUntil(tick);		

	/* UDP �������ݽ�������*/
	ThreadIdTaskUdpNetWork = osThreadNew(AppTaskUdpNetWork, NULL, &ThreadUdpNetWork_Attr);	
	tick += usFrequency + 500;                          
	osDelayUntil(tick);
	
	/**	����λ�������������	**/
	ThreadIdTaskGetDataAndUDP = osThreadNew(AppTaskGetDataAndUDP, NULL, &ThreadGetDataAndUDP_Attr); 	
	tick += usFrequency + 500;                          
	osDelayUntil(tick);
	
	/*������ݽ������*/
	ThreadIdTaskSteering_gear = osThreadNew(AppTaskSteering_gear, NULL, &ThreadSteering_gear_Attr);		
	tick += usFrequency + 500;                          
	osDelayUntil(tick);	

	/* �����ʱ���˷��˺ܶ���Դ����Ҫ�رյ�ʱ��ر��� */
	timerID_Periodic = osTimerNew(timer_Periodic_App , osTimerPeriodic , NULL , &timer_Periodic_Attr);	
	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);			/* ����ͨ��0����������*/
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);	/* ����ͨ��0����������*/	
	SEGGER_RTT_SetTerminal(0);
	SEGGER_RTT_printf(0, "the project is runing \r\n");
	
	tick = osKernelGetTickCount(); 
	while(1){
		tick += usFrequency;                          
		osDelayUntil(tick);
		osThreadExit();
	}
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void){
  /* USER CODE BEGIN ADC1_Init 0 */
  /* USER CODE END ADC1_Init 0 */
  ADC_ChannelConfTypeDef sConfig = {0};
  /* USER CODE BEGIN ADC1_Init 1 */
  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK){
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  /* USER CODE END ADC1_Init 2 */
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void){
  /* USER CODE BEGIN TIM6_Init 0 */
  /* USER CODE END TIM6_Init 0 */
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  /* USER CODE BEGIN TIM6_Init 1 */
  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4199;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim6.Init.Period = 1;
	htim6.Init.Period = 20;	// 1ms
//	htim6.Init.Period = 10;	// 0.5ms
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK){
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK){
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */
  /* USER CODE END TIM6_Init 2 */
}


/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI2_Init(void){
  /* USER CODE BEGIN SPI2_Init 0 */
  /* USER CODE END SPI2_Init 0 */
  /* USER CODE BEGIN SPI2_Init 1 */
  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  //hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK){
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */
  /* USER CODE END SPI2_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void){
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

