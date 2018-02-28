#ifndef _FLASHSTORE_H
#define _FLASHSTORE_H

#include "stm32f10x.h"



// must comply with memory setup in define.h file
// block names
 enum
{
	FLASH_MEMORY,
 	BLOCK_COUNT
};


u16 const  BLOCK_SIZE_CONST[BLOCK_COUNT]=
{
	2048	// FLASH_MEMORY
};


unsigned int const  START_BLOCK_CONST[BLOCK_COUNT][2]=
{
    {125,126},	// MEMORY_RS485
};



unsigned int const	 ELEMENT_SIZE_CONST[BLOCK_COUNT]=
{
	2,		// FLASH_MEMORY, for 2 bytes long
};


























#endif
