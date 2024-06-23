#include "bsp.h"

/* ���ƶ˿ڶ�Ӧ��RCCʱ�� */
#define GPIO_GPIO_CLK_ALLENABLE() {	  \
		__HAL_RCC_GPIOD_CLK_ENABLE(); \
		__HAL_RCC_GPIOC_CLK_ENABLE(); \
		__HAL_RCC_GPIOA_CLK_ENABLE(); \
	};
/*����GPIO�ص����ƺ���*/
//1 . �������������ֱ�ʵ��GPIO�������͹���

/*SIG P3 LED3*/
#define GPIO_PORT_SIG3  GPIOA
#define GPIO_PIN_SIG3		GPIO_PIN_8
	
/*SIG P4 ���������*/
#define GPIO_PORT_SIG4  GPIOD
#define GPIO_PIN_SIG4		GPIO_PIN_10
	
/*SIG P7 �Ƹ˵��2*/
#define GPIO_PORT_SIG7  GPIOD
#define GPIO_PIN_SIG7		GPIO_PIN_2	

/*SIG P1 LED1*/
#define GPIO_PORT_SIG1  GPIOD
#define GPIO_PIN_SIG1		GPIO_PIN_11	
	
/*SIG P2 LED2*/
#define GPIO_PORT_SIG2  GPIOC
#define GPIO_PIN_SIG2		GPIO_PIN_8	

/*SIG P5*/
#define GPIO_PORT_SIG5  GPIOC
#define GPIO_PIN_SIG5		GPIO_PIN_9

/*SIG P6 �Ƹ˵��1*/
#define GPIO_PORT_SIG6  GPIOC
#define GPIO_PIN_SIG6	  GPIO_PIN_12

/**
  * @brief
  * @param
  * @retval
  */
void Get_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* ��GPIOʱ�� */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* ����GPIO */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   		/* ����������� */
	GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* ���������費ʹ�� */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		/* GPIO�ٶȵȼ� */
	HAL_GPIO_Init(GPIOD , &GPIO_InitStruct);


	GPIO_InitStruct.Pin =  GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   		/* ����������� */
	GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* ���������費ʹ�� */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		/* GPIO�ٶȵȼ� */
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
}	


/**
  * @brief
  * @param
  * @retval
  */
void bsp_InitGpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* ��GPIOʱ�� */
	GPIO_GPIO_CLK_ALLENABLE();

	/*
	�������е�LEDָʾ��GPIOΪ�������ģʽ
	���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
	�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
//	bsp_GpioOff(1);
//	bsp_GpioOff(2);
//	bsp_GpioOff(3);
//	bsp_GpioOff(4);
//	bsp_GpioOff(5);
//	bsp_GpioOff(6);
//	bsp_GpioOff(7);
	
	GPIO_InitStruct.Pin = GPIO_PIN_SIG3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIO_PORT_SIG3 , &GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_SIG4 | GPIO_PIN_SIG7 | GPIO_PIN_SIG1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   		
	GPIO_InitStruct.Pull = GPIO_PULLUP;               	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		
	HAL_GPIO_Init(GPIO_PORT_SIG4 , &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_SIG2 | GPIO_PIN_SIG5 | GPIO_PIN_SIG6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   		
	GPIO_InitStruct.Pull = GPIO_PULLUP;               	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		
	HAL_GPIO_Init(GPIO_PORT_SIG2 , &GPIO_InitStruct);
}

/**
  * @brief
  * @param
  * @retval
  */
void GPIO_LEDInitialization(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStructure.Pin 		= GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief
  * @param
  * @retval
  */
void GPIO_OUTPUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief
  * @param
  * @retval
  */
void bsp_GpioOn(unsigned char _no)
{
	_no--;
	if (_no == 0)
	{
		GPIO_PORT_SIG1->BSRR = GPIO_PIN_SIG1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_SIG2->BSRR = GPIO_PIN_SIG2;
	}	
	else if (_no == 2)
	{
		GPIO_PORT_SIG3->BSRR = GPIO_PIN_SIG3;
	}	
	else if (_no == 3)
	{
		GPIO_PORT_SIG4->BSRR = GPIO_PIN_SIG4;
	}
	else if (_no == 4)
	{
		GPIO_PORT_SIG5->BSRR = GPIO_PIN_SIG5;
	}
	else if (_no == 5)
	{
		GPIO_PORT_SIG6->BSRR = GPIO_PIN_SIG6;
	}
	else if(_no == 6)
	{
		GPIO_PORT_SIG7->BSRR = GPIO_PIN_SIG7;
	}
}

/**
  * @brief
  * @param
  * @retval
  */
void bsp_GpioOff(unsigned char _no)
{
	_no--;
	if (_no == 0)
	{
		GPIO_PORT_SIG1->BSRR = (uint32_t)GPIO_PIN_SIG1 << 16U;
	}
	else if (_no == 1)
	{
		GPIO_PORT_SIG2->BSRR = (uint32_t)GPIO_PIN_SIG2 << 16U;
	}	
	else if (_no == 2)
	{
		GPIO_PORT_SIG3->BSRR = (uint32_t)GPIO_PIN_SIG3 << 16U;
	}
	else if (_no == 3)
	{
		GPIO_PORT_SIG4->BSRR = (uint32_t)GPIO_PIN_SIG4 << 16U;
	}
	else if (_no == 4)
	{
		GPIO_PORT_SIG5->BSRR = (uint32_t)GPIO_PIN_SIG5 << 16U;
	}
	else if (_no == 5)
	{
		GPIO_PORT_SIG6->BSRR = (uint32_t)GPIO_PIN_SIG6 << 16U;
	}	
	else if(_no == 6)
	{
		GPIO_PORT_SIG7->BSRR = (uint32_t)GPIO_PIN_SIG7 << 16U;
	}
}

/**
  * @brief 	��תָ����LEDָʾ��
  * @param 	 _no : ָʾ����ţ���Χ 1 - 4
  * @retval ��������
  */
void bsp_GpioToggle(unsigned char _no)
{
	if (_no == 1)
	{
		GPIO_PORT_SIG1->ODR ^= GPIO_PIN_SIG1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_SIG2->ODR ^= GPIO_PIN_SIG2;
	}	
	else if (_no == 3)
	{
		GPIO_PORT_SIG3->ODR ^= GPIO_PIN_SIG3;
	}
	else if (_no == 4)
	{
		GPIO_PORT_SIG4->ODR ^= GPIO_PIN_SIG4;
	}
	else if (_no == 5)
	{
		GPIO_PORT_SIG5->ODR ^= GPIO_PIN_SIG5;
	}
	else if (_no == 6)
	{
		GPIO_PORT_SIG6->ODR ^= GPIO_PIN_SIG6;
	}
	else if (_no == 7)
	{
		GPIO_PORT_SIG7->ODR ^= GPIO_PIN_SIG7;
	}
}

/**
  * @brief 	�ж�LEDָʾ���Ƿ��Ѿ�����
  * @param 	_no : ָʾ����ţ���Χ 1 - 4
  * @retval 1��ʾ�Ѿ�������0��ʾδ����
  */
unsigned char bsp_IsGpioOn(unsigned char _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_SIG1->ODR & GPIO_PIN_SIG1) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_SIG2->ODR & GPIO_PIN_SIG2) == 1)
		{
			return 1;
		}
		return 0;
	}	
	else if (_no == 3)
	{
		if ((GPIO_PORT_SIG3->ODR & GPIO_PIN_SIG3) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 4)
	{
		if ((GPIO_PORT_SIG4->ODR & GPIO_PIN_SIG4) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 5)
	{
		if ((GPIO_PORT_SIG5->ODR & GPIO_PIN_SIG5) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 6)
	{
		if ((GPIO_PORT_SIG6->ODR & GPIO_PIN_SIG6) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 7)
	{
		if ((GPIO_PORT_SIG7->ODR & GPIO_PIN_SIG7) == 1)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

