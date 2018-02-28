///*--------------------pid.h-------------------*/
/*--------------------pid.h-------------------*/

#define PIDTEST

#ifdef TSTAT_ARM
	#define idata
	#define xdata
	#define u16 		unsigned short int
	#define u8 			unsigned char
		
	#define uint16		unsigned short int
	#define uint8		unsigned char
	#define uint32		unsigned long
		
	#define int16		signed short int
	#define int8		signed char
	#define int32		signed long
#else
	#define u16 unsigned int
	#define u8 unsigned char
		
	#define uint16		unsigned int
	#define uint8		unsigned char
	#define uint32		unsigned long
		
	#define int16		signed int
	#define int8		signed char
	#define int32		signed long
#endif



#ifdef PIDTEST
extern int16 pidtest1;
extern int16 pidtest2;
extern int16 pidtest3;
#endif

#ifndef NO_PID
	#define NO_PID	  		9
#endif

#ifndef	PID_LOOP1
	#define PID_LOOP1	  	0
#endif

#ifndef	PID_LOOP2
	#define PID_LOOP2	  	1
#endif

#ifndef PID_LOOP3
	#define PID_LOOP3	  	2
#endif

#ifndef PID_COMPATIBLE_AI1  
#define PID_COMPATIBLE_AI1 1
#endif

#ifndef	PID_INTERNAL_TEMPERATURE
	#define PID_INTERNAL_TEMPERATURE	  	2
#endif

#ifndef	PID_AVERAGE_TEMPERATURE_INTERNAL_AI1
	#define PID_AVERAGE_TEMPERATURE_INTERNAL_AI1	  	3
#endif

#ifndef PID_HUM_TEMPERATURE
	#define PID_HUM_TEMPERATURE	  	4
#endif

#ifndef PID_AI1
	#define PID_AI1	  				5
#endif

#ifndef PID_AI2
	#define PID_AI2	  				6
#endif

#ifndef PID_AI3
	#define PID_AI3	  				7
#endif

#ifndef	PID_AI4
	#define PID_AI4	  				8
#endif

#ifndef PID_AI5
	#define PID_AI5	  	    		9
#endif

#ifndef	PID_AI6
	#define PID_AI6	  				10
#endif

#ifndef	PID_AI7
	#define PID_AI7	  				11
#endif

#ifndef	PID_AI8
	#define PID_AI8	  				12
#endif

#ifndef	PID_CO2
	#define PID_CO2	  				13
#endif

#ifndef	PID_HUM
	#define PID_HUM	  				14
#endif

#ifndef	PID_AIRFLOW_SENSOR
	#define PID_AIRFLOW_SENSOR	  	15
#endif

#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI2
	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI2	  	16
#endif

#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI3
	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI3	  	17
#endif

#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI4
	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI4	  	18
#endif

#ifndef DAY_MODE
	#define DAY_MODE  			  	1
#endif

#ifndef OFF  
	#define OFF 					0
#endif

#ifndef FAN_OFF  
	#define FAN_OFF 				0
#endif




#ifdef TSTAT_ARM
extern u8 read_eeprom(u16 ReadAddr);
//extern void write_eeprom(u16 WriteAddr, u8 DataToWrite);
extern int16 idata loop_setpoint[3]; 
extern int32 xdata iterm_sum_1sec[3];
extern uint8 xdata pid3_input_select;
extern uint8 xdata pid_setpoint[3] ;
extern uint8 xdata pid[3];
extern uint8  xdata universal_pid[3][6]; 
//extern uint8 xdata heat_pid3_table;
//extern uint8 xdata cool_pid3_table;
extern uint16 xdata cooling_db;
extern uint16 xdata heating_db;
extern uint8 xdata fan_speed_user;
extern uint8 init_PID_flag ; 	// Initialize the PID flag, stops reset windup problems
extern uint8 override_timer;
extern uint8 occupied;
extern int16 xdata analog_input[4];
extern int16 xdata mul_analog_input[10] ;
extern int16 xdata mul_analog_in_buffer[10];
extern int16 xdata pre_mul_analog_input[10]; //used to filter  readings
extern uint16 xdata co2_data;
extern int16 xdata humidity;									
extern int16 idata temperature ; //global temperature x 10, filtered,
void update_pid(uint8 sample);

#else
extern signed int idata loop_setpoint[3]; 
extern signed long xdata iterm_sum_1sec[3];
extern unsigned char xdata pid3_input_select;
extern unsigned char xdata pid_setpoint[3] ;
extern unsigned char xdata pid[3];
extern unsigned char  xdata universal_pid[3][6]; 
extern unsigned char xdata heat_pid3_table;
extern unsigned char xdata cool_pid3_table;
extern unsigned int xdata cooling_db;
extern unsigned int xdata heating_db;
extern unsigned char xdata fan_speed_user;
extern unsigned char init_PID_flag ; 	// Initialize the PID flag, stops reset windup problems
extern unsigned char override_timer;
extern unsigned char occupied;
extern signed int xdata analog_input[4];
extern signed int xdata mul_analog_input[10] ;
extern signed int xdata mul_analog_in_buffer[10];
extern signed int xdata pre_mul_analog_input[10]; //used to filter  readings
extern unsigned int xdata co2_data;
extern signed int xdata humidity;									
extern signed int idata temperature ; //global temperature x 10, filtered,
void update_pid(uint8 sample);
extern unsigned char xdata pid3_heat_db;
extern unsigned char xdata pid3_cool_db;
extern unsigned int xdata pid3_pterm;
extern unsigned int xdata pid3_iterm;
extern unsigned char read_eeprom( unsigned int addr );
//extern void write_eeprom( unsigned int addr, unsigned char ch);
#endif




//below is used for Tstat6 128K version 

//#include "define.h"

//#ifndef NO_PID
//	#define NO_PID	  		9
//#endif

//#ifndef	PID_LOOP1
//	#define PID_LOOP1	  	0
//#endif

//#ifndef	PID_LOOP2
//	#define PID_LOOP2	  	1
//#endif

//#ifndef PID_LOOP3
//	#define PID_LOOP3	  	2
//#endif

//#ifndef	PID_INTERNAL_TEMPERATURE
//	#define PID_INTERNAL_TEMPERATURE	  	2
//#endif

//#ifndef	PID_AVERAGE_TEMPERATURE_INTERNAL_AI1
//	#define PID_AVERAGE_TEMPERATURE_INTERNAL_AI1	  	3
//#endif

//#ifndef PID_HUM_TEMPERATURE
//	#define PID_HUM_TEMPERATURE	  	4
//#endif

//#ifndef PID_AI1
//	#define PID_AI1	  				5
//#endif

//#ifndef PID_AI2
//	#define PID_AI2	  				6
//#endif

//#ifndef PID_AI3
//	#define PID_AI3	  				7
//#endif

//#ifndef	PID_AI4
//	#define PID_AI4	  				8
//#endif

//#ifndef PID_AI5
//	#define PID_AI5	  	    		9
//#endif

//#ifndef	PID_AI6
//	#define PID_AI6	  				10
//#endif

//#ifndef	PID_AI7
//	#define PID_AI7	  				11
//#endif

//#ifndef	PID_AI8
//	#define PID_AI8	  				12
//#endif

//#ifndef	PID_CO2
//	#define PID_CO2	  				13
//#endif

//#ifndef	PID_HUM
//	#define PID_HUM	  				14
//#endif

//#ifndef	PID_AIRFLOW_SENSOR
//	#define PID_AIRFLOW_SENSOR	  	15
//#endif

//#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI2
//	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI2	  	16
//#endif

//#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI3
//	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI3	  	17
//#endif

//#ifndef	PID_AVERAGE_TEMPERATURE_AI1_TO_AI4
//	#define PID_AVERAGE_TEMPERATURE_AI1_TO_AI4	  	18
//#endif

//#ifndef DAY_MODE
//	#define DAY_MODE  			  	1
//#endif

//#ifndef OFF  
//	#define OFF 					0
//#endif

//#ifndef FAN_OFF  
//	#define FAN_OFF 				0
//#endif

//extern struct link xdata b; 
//extern signed int idata loop_setpoint[3]; 
//extern signed long xdata iterm_sum_1sec[3];
//extern unsigned char xdata pid3_input_select;
//extern uint8 xdata pid_setpoint[3] ;
//extern unsigned char xdata pid[3];
//extern unsigned char  xdata universal_pid[3][6]; 

//extern unsigned char xdata heat_pid3_table;
//extern unsigned char xdata cool_pid3_table;
//extern unsigned int xdata cooling_db;
//extern unsigned int xdata heating_db;
////extern unsigned char xdata pid3_heat_db;
////extern unsigned char xdata pid3_cool_db;
//extern unsigned int xdata pid3_pterm;
//extern unsigned int xdata pid3_iterm;
//extern unsigned char xdata fan_speed_user;
//extern uint8 xdata init_PID_flag ; 	// Initialize the PID flag, stops reset windup problems

//extern bool override_timer;
//extern bool occupied;


//extern signed int xdata analog_input[4];
//extern signed int xdata mul_analog_input[10] ;
//extern signed int xdata mul_analog_in_buffer[10];
//extern signed int xdata pre_mul_analog_input[10]; //used to filter  readings
//extern unsigned int xdata co2_data;
//extern signed int xdata humidity;									

//extern signed int idata temperature ; // global temperature x 10, filtered,
////end define
//void update_pid(void);

//extern unsigned char read_eeprom( unsigned int addr );

//extern void write_eeprom( unsigned int addr, unsigned char ch );

