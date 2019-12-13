#ifndef __KEY_H
#define __KEY_H
	 
#include "bitmap.h"
#include "output.h"

//#define  FAN_DOWN_CODE     0x04
#define  FAN_MODE_CODE     0x04
#define  COOL_DOWN_CODE    0x02 
//#define  FAN_UP_CODE       0x08
#define  SYS_MODE_CODE       0x08
#define  COOL_UP_CODE      0x01  
#define  HEAT_UP_CODE      0x10
#define  HEAT_DOWN_CODE    0x02
#define  MENU_CODE         0x0c
#define  HOLD_CODE		   	 0x48
#define  OCCUPIED_CODE     COOL_UP_CODE
#define  UNOCCUPIED_CODE   FAN_UP_CODE
		
extern uint8 item_menu;
extern uint8 current_item;
extern uint8  prev_keypad_state;
extern uint8 setpoint_adjust_flag;
extern uint8 down_key;
extern uint16 lastkey_counter;
extern uint8 keypad_value;
extern PTBDSTR KEY_TEMP;
extern uint8 key_pressed;
extern uint8 menu_id;
extern uint8 pre_menu_id;
void keypad_ini(void);
void keypad_handler( void );
#endif
