#include "time4.h"
#include "rtc.h"
#include "lcd.h"

extern u16 j;
void TIM4_Int_Init(u16 arr,u16 psc)
{
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
     //��ʱ��TIM4��ʼ��
     TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
     TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
     TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
 
    //�ж����ȼ�NVIC����
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM3�ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
     NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
 
 
     TIM_Cmd(TIM4, ENABLE); //ʹ��TIMx
}
 
 
//��ʱ��4�жϷ������
void TIM4_IRQHandler(void) //TIM3�ж�
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���TIM4�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); //���TIMx�����жϱ�־
        rtc();
		j++;
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

 


