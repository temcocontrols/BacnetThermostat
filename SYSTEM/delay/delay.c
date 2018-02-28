#include "delay.h"

#ifdef SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif

static u8  fac_us = 0;	//us延时倍乘数			   
static u16 fac_ms = 0;	//ms延时倍乘数,在os下,代表每个节拍的ms数
   
//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
#ifdef SYSTEM_SUPPORT_OS
	u32 reload;
#endif
 	SysTick->CTRL &= ~(1 << 2);				//SYSTICK使用外部时钟源	 
	fac_us = SYSCLK / 8;					//不论是否使用ucos,fac_us都需要使用
	    
#ifdef SYSTEM_SUPPORT_OS
	reload = SYSCLK / 8;					//每秒钟的计数次数 单位为K	   
	reload *= 1000000 / configTICK_RATE_HZ;	//根据configTICK_RATE_HZ设定溢出时间
											//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	fac_ms = 1000 / configTICK_RATE_HZ;		//代表ucos可以延时的最少单位	   
// 	SysTick->CTRL |= 1 << 1;				//开启SYSTICK中断
	SysTick->LOAD = reload; 				//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL |= 1 << 0;				//开启SYSTICK    
#else
	fac_ms = (u16)fac_us * 1000;			//非os下,代表每个ms需要的systick时钟数   
#endif
}								    

#ifdef SYSTEM_SUPPORT_OS
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD;				//LOAD的值	    	 
	ticks = nus * fac_us; 					//需要的节拍数	  		 
	tcnt = 0;

// 	vTaskSuspendAll();						//阻止os调度，防止打断us延时
	taskENTER_CRITICAL();
	told = SysTick->VAL;        			//刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)
				tcnt += told - tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;	    

			told = tnow;
			if(tcnt >= ticks)				//时间超过/等于要延迟的时间,则退出.
				break;
		}  
	};
// 	xTaskResumeAll();						//开启os调度
	taskEXIT_CRITICAL(); 									    
}

//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{	
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)	//如果os已经在跑了
	{		  
		if(nms >= fac_ms)					//延时的时间大于ucos的最少时间周期 
		{
   			vTaskDelay(nms / fac_ms);		//ucos延时
		}
		nms %= fac_ms;						//ucos已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));				//普通方式延时 
}

#else	//不用os时
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus * fac_us;	//时间加载	  		 
	SysTick->VAL = 0x00;			//清空计数器
	SysTick->CTRL = 0x01;			//开始倒数 	 
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));	//等待时间到达 
	  
	SysTick->CTRL = 0x00;			//关闭计数器
	SysTick->VAL = 0X00;			//清空计数器	 
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * fac_ms;	//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;				//清空计数器
	SysTick->CTRL = 0x01;				//开始倒数  
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));	//等待时间到达   

	SysTick->CTRL = 0x00;				//关闭计数器
	SysTick->VAL = 0X00;				//清空计数器	  	    
} 
#endif
