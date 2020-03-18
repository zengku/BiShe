#include "time4.h"
#include "rtc.h"
#include "lcd.h"

extern u16 j;
void TIM4_Int_Init(u16 arr,u16 psc)
{
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
     //定时器TIM4初始化
     TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
     TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
     TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
 
    //中断优先级NVIC设置
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM3中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //从优先级3级
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
     NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
 
 
     TIM_Cmd(TIM4, ENABLE); //使能TIMx
}
 
 
//定时器4中断服务程序
void TIM4_IRQHandler(void) //TIM3中断
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查TIM4更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); //清除TIMx更新中断标志
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

 


