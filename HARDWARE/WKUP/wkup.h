#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������STM32������
//�������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
					    
#define WKUP_KD PAin(0)			//PA0 ����Ƿ��ⲿWK_UP��������
	 
u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void Sys_Exit_Standby(void);	//����
void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif

