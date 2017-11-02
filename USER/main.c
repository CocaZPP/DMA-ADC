#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "dma.h"
#include <adc.h>
u8 buf[20];
 u32 data2[4];  
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LCD_Init();					//LCD��ʼ�� 
	Init_adc();
  INit_dma();

	while(1)
	{
		
		LCD_ShowNum(30,100,555,4,24);
 sprintf((char *)buf,"PA4:%.3lf",((float)data2[0]*(float)3.3/4095)); 
LCD_ShowString(70,0,400,400,24,buf); 

sprintf((char *)buf,"PA5:%.3lf",((float)data2[1]*(float)3.3/4095)); 
LCD_ShowString(70,32,400,400,24,buf); 

sprintf((char *)buf,"PA6:%.3lf",((float)data2[2]*(float)3.3/4095)); 
LCD_ShowString(70,72,400,400,24,buf);
   
	}		    
}

