#include "timer.h"
#include "led.h"
#include "gizwits_product.h"
#include "rtc.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

volatile extern u16 j;

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�	
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			gizTimerMs();
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			rtc();		
//			LCD_ShowxNum(60+16*3,290,j,3,16,0);
		}
}

void rtc(void)
{
	u8 t = 0;
	if(t!=calendar.sec)
	{
		t=calendar.sec;
		LCD_ShowNum(60,130,calendar.w_year,4,16);									  
		LCD_ShowNum(100,130,calendar.w_month,2,16);									  
		LCD_ShowNum(124,130,calendar.w_date,2,16);	 
		switch(calendar.week)
		{
			case 0:
				LCD_ShowString(60,150,200,16,16,"Sunday   ");
				break;
			case 1:
					LCD_ShowString(60,150,200,16,16,"Monday   ");
				break;
			case 2:
				LCD_ShowString(60,150,200,16,16,"Tuesday  ");
				break;
			case 3:
				LCD_ShowString(60,150,200,16,16,"Wednesday");
				break;
			case 4:
				LCD_ShowString(60,150,200,16,16,"Thursday ");
				break;
			case 5:
					LCD_ShowString(60,150,200,16,16,"Friday   ");
				break;
			case 6:
				LCD_ShowString(60,150,200,16,16,"Saturday ");
				break;  
		}
		LCD_ShowNum(60,170,calendar.hour,2,16);									  
		LCD_ShowNum(84,170,calendar.min,2,16);									  
		LCD_ShowNum(108,170,calendar.sec,2,16);
	}	
}









