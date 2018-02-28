#ifndef __TIMERX_H
#define __TIMERX_H

#include "stm32f10x.h"


//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  


//通过改变TIM3->CCR2的值来改变占空比，从而控制LED0的亮度
#define LED0_PWM_VAL	TIM3->CCR2    
#define SWTIMER_INTERVAL	1
//void TIM4_Int_Init(u16 arr, u16 psc);
//void TIM4_PWM_Init(u16 arr, u16 psc);

extern u32 uip_timer;
void TIM6_Int_Init(u16 arr, u16 psc);
//void output_init(void);
#endif
