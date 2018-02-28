#ifndef __LED_H
#define __LED_H
 
#include "bitmap.h"


#define LED_ON	0
#define LED_OFF	1	

#define LED0	PBout(10)// DS0
#define LED1	PBout(11)// DS1
#define LED2	PCout(4)// DS2
#define LED3	PCout(5)// DS3

// DS0
#define LED0_OFF()	GPIO_SetBits(GPIOB, GPIO_Pin_10);
#define LED0_ON()	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
// DS1
#define LED1_OFF()	GPIO_SetBits(GPIOB, GPIO_Pin_11);
#define LED1_ON()	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
// DS2
#define LED2_OFF()	GPIO_SetBits(GPIOC, GPIO_Pin_4);
#define LED2_ON()	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
// DS3
#define LED3_OFF()	GPIO_SetBits(GPIOC, GPIO_Pin_5);
#define LED3_ON()	GPIO_ResetBits(GPIOC, GPIO_Pin_5);


//void LED_Init(void);
//void tabulate_LED_STATE(void);
//void refresh_led(void);
//extern u8	tx_count  ;
//extern u8   rx_count  ;
//extern u8 dim_timer_setting[28] ;
		 				    
#endif

















