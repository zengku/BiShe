#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usart3.h"
#include "beep.h"
#include "adc.h"
#include "dht11.h" 
#include "relay.h"
#include "rtc.h"
#include "timer.h"
#include "wkup.h"
#include "exti.h"
#include "gizwits_product.h"

#define Temp_alarm  24  
#define alarm_num 3  
#define ADC_val 50
#define ADC_min 600

void Init(void);
void Gizwits_Init(void);
void show(void);
void cnt(void);
void CD_OK(void);
void userHandle(void);

extern dataPoint_t currentDataPoint;
u8 wifi_sta=0;

u16 j = 0;
u32 g = 0;
u8 temperature, humidity;  	 
static u8 temp_num=0;
u16 adcx, adcy;
float tempx, tempy;


uint8_t read_buff[5]={0};
u8 read_sta=0;
uint8_t lcd_buff[70]={0};


int main(void)
{
	int key;
	u8 wifi_con;
//	u8 r=0;
//	u8 v=0;
//	u32 d;
	
	Init();
	DHT11_Read_Data(&temperature,&humidity);//��ȡDHT11������

	
	while(1) 
	{	
	    userHandle();		
		
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
		}
			
		if(wifi_con!=wifi_sta)
	    {
		   wifi_con=wifi_sta;
		   POINT_COLOR=RED;		
		   if(wifi_con){
				LCD_ShowString(278,170,200,16,16,"Wifi:Connect");
		   }else{
				LCD_ShowString(278,170,200,16,16,"Wifi:Close  ");
		   }
		   POINT_COLOR=BLUE;
	    }
			
//		if(r == 1){
//			j++;
//		}else{
//			j = 0;
//		}
		
//		if(v == 1){
//			CD_OK();
//		}else{
//			LED1=!LED1;
//		}
		
		
		if(EXTI_Line4){
			show();			
//			if(Get_Adc_Average(ADC_Channel_1,10) > ADC_val){
//				for(d = 0; d < (60*60*1.5); d++)
//				delay_ms(1000);
//				d=0;
//				Sys_Enter_Standby();
//			}
//			r = 1;
//			v = 1;			
		}else 
		if(EXTI_Line3){
//			r = 0;
//			v = 0;
//			d = 0;
			LCD_ShowString(60,230,200,16,16,"Temp:   C");	 
			LCD_ShowString(60,250,200,16,16,"LDC_U:0.000V");
			LCD_ShowString(60,270,200,16,16,"LDC_I:0.000A");
//			LCD_ShowString(60,290,200,16,16,"Time:000s");
		}
				
	}
}



void Init(void)
{
	delay_init(); //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2
	uart_init(115200); //���ڳ�ʼ��������Ϊ 115200
	LED_Init(); //LED �˿ڳ�ʼ��
	BEEP_Init();//beep��ʼ��
	EXTIX_Init();//exti��ʼ��
	RELAY_Init();	//JDQ��ʼ��
	WKUP_Init();	//hkup��ʼ��
	KEY_Init();	//key��ʼ��
	Adc_Init(); //ADC ��ʼ��
	Gizwits_Init();//Э���ʼ��
	RTC_Init();	//RTC��ʼ��
	TIM3_Int_Init(9999,7199);
	//TIM4_Int_Init(1000,36000-1);//Tout=(4-1+1)*(36000-1+1)/36,000,000=4/1,000=4ms
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		LCD_ShowString(60,210,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(60,210,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(60,210,200,16,16,"DHT11 OK");
	LCD_Init(); //LCD ��ʼ��
	POINT_COLOR=RED; 
	LCD_ShowString(60,50,200,16,16,"BiYeSheJi");
	LCD_ShowString(60,70,200,16,16,"LDCCDJC");
	LCD_ShowString(60,90,200,16,16,"ZJH");
	LCD_ShowString(60,130,200,16,16,"    -  -  ");	   
	LCD_ShowString(60,170,200,16,16,"  :  :  ");
	POINT_COLOR=BLUE; 
 	LCD_ShowString(60,230,200,16,16,"Temp:   C");	 
	LCD_ShowString(60,250,200,16,16,"LDC_U:0.000V");
	LCD_ShowString(60,270,200,16,16,"LDC_I:0.000A");
//	LCD_ShowString(60,290,200,16,16,"Time:000s");	

}

void Gizwits_Init(void)
{
	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}


void show(void)
{	
	static u8 e=0;
	
	if(e==10)//ÿ2S��ȡһ��
	{
	//	DHT11_Read_Data(&temperature,&humidity);//��ȡDHT11������
		LCD_ShowxNum(60+32+8,230,temperature,3,16,0); 
	
		adcx=Get_Adc_Average(ADC_Channel_1,10);
				//LCD_ShowxNum(156,330,adcx,4,16,0);//��ʾADC��ֵ		
		
		tempx=(float)adcx*(3.3/4096)*9.59-0.28;
		adcx=tempx;
		LCD_ShowxNum(60+48,250,adcx,1,16,0);//��ʾ��ѹֵ
		tempx-=adcx;                                                 
		tempx*=1000;
		LCD_ShowxNum(60+48+16,250,tempx,3,16,0X80);

													
		if(Get_Adc(ADC_Channel_1) > ADC_val){
			tempy=(float)tempx/600;
			adcy=tempy;
			LCD_ShowxNum(60+48,270,adcy,1,16,0);//��ʾ����ֵ
			tempy-=adcy;
			tempy*=1000;
			LCD_ShowxNum(60+48+16,250,tempy,3,16,0X80);	
		}else{
			LCD_ShowString(60,270,200,16,16,"LDC_I:0.000A");		
		}
	}
	delay_ms(10);
	e++;
	if(e==20)
	{
		e=0;
		LED1=!LED1;
	}
	
}

//void CD_OK(void)
//{
//	u32 d;
//	if(Get_Adc_Average(ADC_Channel_1,10) > ADC_val){
//		for(d = 0; d < (60*60*1.5); d++)
//		delay_ms(1000);
//		d=0;
//		Sys_Enter_Standby();
//	}
//}


//void  cnt(void)
//{
//	
//	delay_ms(1000);	
//	
//}

void userHandle(void)
{
	static u8 k=0;
	u8  c, y;
	uint16_t  data, data1, data2, data3, data4;
	
	adcx=Get_Adc_Average(ADC_Channel_1,10);
	if(wifi_sta)
	{
		if(Get_Adc_Average(ADC_Channel_1,10) < ADC_min)
		{
			if(read_sta==1) //���յ�һ������
			{
				printf("receive data:");
				for(c=0;c<sizeof(currentDataPoint.valueTIMER);c++)
				{
					 printf("%-4x",read_buff[c]);//���ڴ�ӡ	 				 
				}
				printf("\r\n");
				sprintf((char*)lcd_buff,"%x %x %x %x %x",
							 read_buff[0],read_buff[1],read_buff[2],read_buff[3],read_buff[4]);
				LCD_ShowString(30+32,210+70+30,200,16,16,lcd_buff);//Һ����ʾ
				
				if(read_buff[0] > 0){
					relay_off();		
					data1 = read_buff[1];	//��
					data2 = read_buff[2];	//ʱ
					data3 = read_buff[3];	//��
					data4 = read_buff[4];	//��				
					data = (data1*24*60*60 + data2*60*60 + data3*60 + data4);					
					for(y=0; y<data; y++){
						delay_ms(1000);
					}
					relay_on();
				}else{
					LED0 = !LED0;
				}	
				read_sta=0;//�����־λ
			}
		}
	}
	
	 if(k==10)//ÿ2S��ȡһ��
	 {
//		 adcx=Get_Adc_Average(ADC_Channel_1,10);
		 currentDataPoint.valueu = (float)adcx*(3.3/4096)*9.59-0.28;
		 currentDataPoint.valueTemperature = temperature ;			 
		 if(temperature>=Temp_alarm)
		 {  
			 temp_num++;
			 if(temp_num>=alarm_num)
			 {
				 temp_num=0;
				 relay_off();
				 currentDataPoint.valueTemp_alarm=1;//�¶ȴ����ޱ���
				 LCD_Fill(140+16,150+60+10,140+16*16,165+60+10+16,RED);					 
				 BEEP=!BEEP;
				 delay_ms(50);
				 BEEP=!BEEP;
				 delay_ms(50);
				 BEEP=!BEEP;
				 delay_ms(50);
				 BEEP=!BEEP;
				 delay_ms(50);
				 BEEP=!BEEP;
				 
			 }else
			 {
				 if(currentDataPoint.valueTemp_alarm==0)
				  LCD_Fill(140+16,150+60+10,140+16*16,165+60+10+16,GREEN);
			 }  
			 
		 }else  
		 {
			 currentDataPoint.valueTemp_alarm=0;
			 LCD_Fill(140+16,150+60+10,140+16*16,165+60+10+16,GREEN);
			 
		 }
	 }
	 delay_ms(10);
	 k++;
	 if(k==20){ 
		 k = 0;
	 }
	else
	{
		if(temp_num!=0) 
		{
			temp_num=0;
		}
	}
}




				



