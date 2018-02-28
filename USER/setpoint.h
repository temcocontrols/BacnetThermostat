#ifndef _SETPOINT_H_
#define _SETPOINT_H_
#define TSTAT6_ARM
#include "24cxx.h"

//#define TSTAT6_CODE
//#ifdef TSTAT6_CODE

//#endif
#ifdef TSTAT_ARM
#define uint16		unsigned short int
#define uint8		unsigned char
#define uint32		unsigned long
	
#define int16		signed short int
#define int8		signed char
#define int32		signed long
#else
#define uint16		unsigned  int
#define uint8		unsigned char
#define uint32		unsigned long
	
#define int16		signed  int
#define int8		signed char
#define int32		signed long
#endif // TSTAT_ARM

//define global varible here
#define DHOME_SP  			1
#define DAY_COOLING_SP  	2
#define DAY_HEATING_SP  	3
#define NIGHT_SP  			4
#define NIGHT_COOLING_SP  	5
#define NIGHT_HEATING_SP  	6
#define MAX_SP				7
#define MIN_SP				8
#define NHOME_SP			9
#define SLEEP_SP_LIB      10
#define HOLIDAY_SP_LIB		11
#define AWAY_SP_LIB						12



#define DAY_COOLING_DB      1
#define DAY_HEATING_DB      2
#define NIGHT_COOLING_DB    3
#define NIGHT_HEATING_DB    4

extern uint8 setpoint_select;
int16 Get_current_setpoint(uint8 sp_sel);
int16 Read_setpoint(uint8 sp);
void Write_setpoint(uint8 sp_name, int16 sp_value);
void Set_day_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_day_cooling_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_day_heating_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_night_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_night_cooling_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_night_heating_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void change_setpoint_deadband(int16 eep_id, uint8 sp_lowbyte);
void accept_keypad_setpoint(int16 menu_buf_setpoint);
void Set_holiday_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);
void Set_sleep_setpoint(uint8 sp_highbyte, uint8 sp_lowbyte);

//extern unsigned char read_eeprom( unsigned short int  addr );
//extern void write_eeprom(unsigned short int addr, unsigned char ch);
#endif




