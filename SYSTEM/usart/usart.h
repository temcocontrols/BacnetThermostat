#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f10x.h"
#include "define.h"

//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	


	  	


void uart1_init(u32 bound);
//#ifdef TSTAT_ZIGBEE	
void uart4_init(u32 bound);
//#endif //#TSTAT_ZIGBEE
#ifdef TSTAT_CO2
void uart3_init(u32 bound);
#endif//TSTAT_CO2

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

#endif


