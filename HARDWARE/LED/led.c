#include "led.h"
#include "modbus.h"
//u16 led_bank1 = 0 ;
//u16 led_bank2 = 0 ;
//u8  heart_beat_led =  0; 
//u8	tx_count = 0 ;
//u8  rx_count = 0 ;
//u8 dim_timer_setting[28];
//void LED_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOE, GPIO_InitStructure.GPIO_Pin);	
//}
/*****************************LED TABLE------T3-22AI**************************************/
/*
PE14 = 0
PE15 = 1
PE0-->HEATBEAT 
PE1-->INPUT0 PE2-->INPUT1 PE3-->INPUT2 PE4-->INPUT3 PE5-->INPUT4 PE6-->INPUT5 
PE7-->INPUT6 PE8-->INPUT7 PE9-->INPUT8 PE10-->INPUT9 PE11-->INPUT10 PE12-->TX PE13-->RX  

PE14 = 1
PE15 = 0
PE0-->HEATBEAT 
PE1-->INPUT11 PE2-->INPUT12 PE3-->INPUT13 PE4-->INPUT14 PE5-->INPUT15 PE6-->INPUT16 
PE7-->INPUT17 PE8-->INPUT18 PE9-->INPUT19 PE10-->INPUT20 PE11-->INPUT21 PE12-->TX PE12-->RX  
*/
/****************************************************************************/
//void tabulate_LED_STATE(void)
//{
//  static u16 beat_count = 0;	
//  static u8 i = 0 ;
//  beat_count ++ ;
//  if(beat_count >=10 ) 
//  {
//		beat_count = 0 ;
//	  if(heart_beat_led == LED_ON)
//		heart_beat_led = LED_OFF ;
//		else
//		heart_beat_led = LED_ON ;
//		if(heart_beat_led == LED_ON)
//		{
//	
//				led_bank2 &= ~(1<<13) ;
//		}
//		else
//		{
//				led_bank2 |= (1<<13) ;
//		}

//  }
//  for(i=0; i<11; i++)
//  {
//		if(modbus.input[i]>512)  
//		{
//					led_bank2 &= ~(1<<i) ;
//		}
//		else
//		{
//					led_bank2 |= (1<<i) ;
//		}
//		if(modbus.input[i+11]>512)  
//		{
//					led_bank1 &= ~(1<<i) ;
//		}
//		else
//		{
//					led_bank1 |= (1<<i) ;
//		}
//  }
//  if(tx_count>0) tx_count-- ;
//  if(rx_count>0) rx_count-- ;
//  
//  if(tx_count>0) 
//  {
//	  led_bank2 &= ~(1<<11) ;
//  }
//  else  		 
//		led_bank2 |= (1<<11) ;
//  if(rx_count>0) 
//	  led_bank2 &= ~(1<<12) ;
//  else  		
//	  led_bank2 |= (1<<12) ;
//}

//void refresh_led(void)
//{
//	static u8 led_switch = 0 ;
//	led_switch = !led_switch ;
//	if(led_switch)
//	{
//		led_bank1 &= ~(1<<14);
//		led_bank1 |= (1<<15);
//		GPIO_Write(GPIOE, led_bank1) ;
//	}
//	else
//	{
//		
//		led_bank2 |= (1<<14);
//		led_bank2 &= ~(1<<15);
//		GPIO_Write(GPIOE, led_bank2) ;
//	}
//}


