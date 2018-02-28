#ifndef _TSTAT7_MENU_H_
#define _TSATA7_MENU_H_H


#include "define.h" 
#include "bitmap.h"
#ifdef TSTAT7_ARM

void Keypad_Receive(uint8 prev_keypad_state);
void vTstat7_Menu( void *pvParameters );
#endif



#endif

