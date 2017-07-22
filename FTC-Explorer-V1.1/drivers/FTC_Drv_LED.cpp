/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_LED.cpp
 * 描述    ：LED
**********************************************************************************/
#include "FTC_Drv_LED.h"

FTC_LED led;

void FTC_LED::Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/*
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=5000;
	//prescaler is 1200,that is 72000000/72/500=2Hz;
	TIM_TimeBaseStructure.TIM_Prescaler=7200-1;	//set clock division 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	//clear the TIM2 overflow interrupt flag
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//TIM2 overflow interrupt enable
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//enable TIM2
	TIM_Cmd(TIM2,ENABLE);
  */
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC  , ENABLE  );
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14;			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	/*
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  , ENABLE  );
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
*/
}

void FTC_LED::ON1(void)
{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void FTC_LED::ON2(void)
{		
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
}

void FTC_LED::OFF1(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void FTC_LED::OFF2(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
}
uint8_t sum=0;
/*void TIM2_IRQHandler(void)
{
    sum=(sum)%2;
	  if(sum)
		{
			  led.ON1();
			  led.OFF2();
		}
		else
		{
		    led.ON2();
			  led.OFF1();
		}
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
*/
/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/

