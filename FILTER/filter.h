#ifndef _FILTER_H
#define _FILTER_H

#include "stm32f10x.h"
#define TYPE2_10K_THERM	  0
#define TYPE3_10K_THERM	  1 
#define TYPE4_10K_THERM	  2 
/*range define*/
#define RAW_DATA	0
#define C_TYPE2		1
#define F_TYPE2		2
#define PERCENT	  	3
#define ON_OFF		4
#define OFF_ON		5
#define PULSE		6
#define LIGHTING	7
#define C_TYPE3		8
#define F_TYPE3		9
#define NO_USE		10
#define V0_5		11
#define V0_10		12
#define I0_20ma		13




u16 filter_inputs(u8 channel,u16 input) ;
#endif 
