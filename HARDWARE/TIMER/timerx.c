#include "timerx.h"
#include "led.h"
#include "modbus.h"
#include "portmacro.h"
extern volatile uint16_t SilenceTime;


//定时器4中断服务程序	 
//void TIM4_IRQHandler(void)
//{ 		    		  			    
////	if(TIM4->SR & 0X0001)		//溢出中断
//	if(TIM_GetFlagStatus(TIM4, TIM_IT_Update) == SET)
//	{
//		LED1 = !LED1;			    				   				     	    	
//	}
////	TIM4->SR &= ~(1 << 0);		//清除中断标志位 
//	TIM_ClearFlag(TIM4, TIM_IT_Update);	
//}

//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
//void TIM4_Int_Init(u16 arr, u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	
//	TIM_TimeBaseStructure.TIM_Period = arr;
//	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	
//	//Timer3 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);								//根据指定的参数初始化NVIC寄存器
//	
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
//}

////TIM4 PWM部分初始化 
////PWM输出初始化
////arr：自动重装值
////psc：时钟预分频数
//void TIM4_PWM_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;// | GPIO_Pin_8 | GPIO_Pin_9;  //TIM3_CH1-4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	
//	TIM_TimeBaseStructure.TIM_Period = arr;
//	TIM_TimeBaseStructure.TIM_Prescaler = psc;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
////	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);//initial channel2 of TIM4
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);//initial channel3 of TIM4
////	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//	
////	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
////	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM4, ENABLE);									  
//}  	 

//void set_pwm_duty(TIM_TypeDef* TIMx, u8 channel, u16 duty)
//{
//	u16 compare_value = 0;
//	compare_value = duty; 
//	// calculate the compare value and then set it to change the pwm duty
//	
//	switch(channel)
//	{
//		case 1:
//			TIM_SetCompare1(TIMx, compare_value);
//		break;
//		case 2:
//			TIM_SetCompare2(TIMx, compare_value);
//		break;
//		case 3:
//			TIM_SetCompare3(TIMx, compare_value);
//		break;
//		case 4:
//			TIM_SetCompare4(TIMx, compare_value);
//		break;
//	}
//}




////定时器3中断服务程序	 
//void TIM3_IRQHandler(void)
//{ 		    		  			    
////	if(TIM3->SR & 0X0001)		//溢出中断
//	if(TIM_GetFlagStatus(TIM3, TIM_IT_Update) == SET)
//	{
//		LED1 = !LED1;			    				   				     	    	
//	}
////	TIM3->SR &= ~(1 << 0);		//清除中断标志位 
//	TIM_ClearFlag(TIM3, TIM_IT_Update);	
//}

////通用定时器3中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器3!
//void TIM3_Int_Init(u16 arr, u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	
//	TIM_TimeBaseStructure.TIM_Period = arr;
//	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	//Timer3 NVIC 配置
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);								//根据指定的参数初始化NVIC寄存器
//	
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//}

////TIM3 PWM部分初始化 
////PWM输出初始化
////arr：自动重装值
////psc：时钟预分频数
//void TIM3_PWM_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //TIM3_CH1->PC6, TIM3_CH2->PC7, TIM3_CH3->PC8, TIM3_CH4->PC9
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //TIM3_CH1-4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	TIM_TimeBaseStructure.TIM_Period = arr;
//	TIM_TimeBaseStructure.TIM_Prescaler = psc;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//	
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM3, ENABLE);									  
//}  	 

//void set_pwm_duty(TIM_TypeDef* TIMx, u8 channel, u16 duty)
//{
//	u16 compare_value = 0;
//	// calculate the compare value and then set it to change the pwm duty
//	
//	switch(channel)
//	{
//		case 1:
//			TIM_SetCompare1(TIMx, compare_value);
//		break;
//		case 2:
//			TIM_SetCompare2(TIMx, compare_value);
//		break;
//		case 3:
//			TIM_SetCompare3(TIMx, compare_value);
//		break;
//		case 4:
//			TIM_SetCompare4(TIMx, compare_value);
//		break;
//	}
//}

/////////////////////////////////////////////////////////////////////////////////////////
u32 uip_timer = 0;	//uip 计时器，每10ms增加1.
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)//1ms
{
//	if(TIM6->SR & 0X0001)	//溢出中断
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) == SET)
	{
		uip_timer++;		//uip计时器增加1
	}
	if(SilenceTime < 5000)
	{
		//SilenceTime ++ ;
		//miliseclast_cur = miliseclast_cur + SWTIMER_INTERVAL;
		SilenceTime = SilenceTime + SWTIMER_INTERVAL;
	}
	else
	{
			SilenceTime = 0 ;
	}
//	TIM6->SR &= ~(1 << 0);	//清除中断标志位 
	TIM_ClearFlag(TIM6, TIM_IT_Update);		
}

//基本定时器6中断初始化					  
//arr：自动重装值。		
//psc：时钟预分频数		 
//Tout= ((arr+1)*(psc+1))/Tclk；
void TIM6_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	//Timer3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								//根据指定的参数初始化NVIC寄存器
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
//	RCC->APB1ENR |= 1 << 4;					//TIM6时钟使能    
// 	TIM6->ARR = arr;  						//设定计数器自动重装值 
//	TIM6->PSC = psc;  			 			//设置预分频器.
// 	TIM6->DIER |= 1 << 0;   				//允许更新中断				
// 	TIM6->CR1 |= 0x01;    					//使能定时器6
//	MY_NVIC_Init(0, 0, TIM6_IRQn, 2);		//抢占1，子优先级2，组2		
}
