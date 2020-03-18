#include "relay.h"

void RELAY_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void relay_on(void)

{

  GPIO_SetBits(GPIOA,GPIO_Pin_5);

}


void relay_off(void)

{ 

  GPIO_ResetBits(GPIOA,GPIO_Pin_5);

}







