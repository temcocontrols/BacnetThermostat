#ifndef 	__CRC_H
#define 	__CRC_H


#include "stm32f10x.h"
#include "types.h"

void init_crc16(void) ;
void crc16_byte(u8 ch) ;
u16 crc16(u8 *p, u8 length);


extern u8 CRClo ;
extern u8 CRChi ;
#endif 
