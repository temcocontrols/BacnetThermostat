#ifndef __H
#define	__H
#include "define.h"
#ifdef TSTAT_ARM
#include <string.h>
#include "stm32f10x.h"	 
#include "bitmap.h"
#include "crc.h"
#include "define.h"
#include "types.h"
#include "output.h"

#define TXEN		PAout(8)
#define SEND			1			//1
#define	RECEIVE		0
#define	READ_VARIABLES				0x03
#define	WRITE_VARIABLES				0x06
#define	MULTIPLE_WRITE_VARIABLES				0x10
#define	CHECKONLINE					0x19
#define DATABUFLEN					200
#define DATABUFLEN_SCAN				12
#define SENDPOOLLEN         		8
#define SERIAL_COM_IDLE				0
#define INVALID_PACKET				1
#define VALID_PACKET				2
#define USART_REC_LEN  			512  	//定义最大接收字节数 200
#define USART_SEND_LEN			512



// *******************modbus.h***********************************
// Header file containing all of the register information for modbus 
// serial communications.
// V.24 first release of modbus.h file.
// V.25
//caution:the tstat response should have a pause between two characrers,but the maximum allowed pause is 1.5 character times or .83 ms * 1.5 or 1.245 ms at 9600 baud.
//  REGISTER ADDRESSES TO BE USED IN MODBUS SERIAL COMMUNICATION




enum {
	SERIALNUMBER_LOWORD   ,          // -	-	Lower 2 bytes of the serial number
	SERIALNUMBER_HIWORD  = 2  ,		// -	-	Upper 2 bytes of teh serial number
	VERSION_NUMBER_LO   = 4  ,		// -	-	Software version
	VERSION_NUMBER_HI,				// -	-	Software version
	TSTAT_ADDRESS,							// 1	254	Device modbus address
	PRODUCT_MODEL,					// -	-	Temco Product Model	1=Tstat5B, 2=Tstat5A, 4=Tstat5C, 12=Tstat5D, 
	HARDWARE_REV,					// -	-	Hardware Revision	 
	PIC882VERSION,						// -	-	PIC firmware version        
	PLUG_N_PLAY,				// -	-	Temporary address for plug-n-play addressing scheme
	ISP_MODE_INDICATION,
	BASE_ADDRESS 	= 14,			// base address select	
	BAUDRATE,//15						// 0	1	Baudrate 0 = 9.6kb/s, 1 = 19.2kb/s
	UPDATE_STATUS	= 16,			// reg 16 status for update_flash											// writing 0x7F means jump to ISP routine											// writing 0x8F means completely erase eeprom					 
	UPDATE_PTR_HI,					// reg 17 pointer for last attempt upload HI
	UPDATE_PTR_LO,					// reg 18 pointer for last attempt upload LO
	SCRATCHPAD_ADDRESS , 			// -	-	Internal scratch pad address	
  HARDWARE_INFORMATION,     // 20 indicate if clock is present	
	MODBUS_BACNET_SWITCH,
	
	MODBUS_ZIGBEE_INDEX	= 51,
	MODBUS_ZIGBEE_INFO1,
	MODBUS_ZIGBEE_INFO48 = 99,

	

































	
	COOLHEATMODE=101,//				// -	-	Heating or cooling mode in effect	0 = coasting, 1 = cooling, 2 = heating
	PID1_MODE_OPERATION,//				// -	-	Current state of Tstat.  High heat -> coasting -> high cool.
	SEQUENCE,//					// 0	2	Sequence of operation , tstat behaves differently according to sequence
	DEGC_OR_F,// 					// 0	1	Temperature units  0 = DegC, 1 = DegF 
	FAN_MODE,//					// 0	3	Number of fan speeds to show on the display 0 = 0 speeds, 3 = 3 speeds
	POWERUP_MODE,//					// 0	3	Powerup mode.  0=Off, 2=On, 2=Last Value, 3=Auto
	AUTO_ONLY,//					// 0	1	Enable or disable manual modes.  0=manual allowed, 1=auto only,2 = DDC mode
	FACTORY_DEFAULTS,//   			// 0	1	Factory defaults  0=no default	
	INFO_BYTE,//						// -	-	Byte that holds info about the tstat
  PREVIOUS_BAUDRATE,//110						// 0	1	Baudrate 0 = 9.6kb/s, 1 = 19.2kb/s
	TSTAT_OVERRIDE_TIMER,//					// 0	255	Determines what controls the state of the LED
	OVERRIDE_TIMER_LEFT,//
	HEAT_COOL_CONFIG,//
	TIMER_ON,//
	TIMER_OFF,//
	TIMER_UNITS,	//
	DEAD_MASTER,	//
	SYSTEM_TIME_FORMAT,//			//		  system format
	FREE_COOL_CONFIG,//				//		 Free cool configure register.bit0,free cool enable/disable,0 = disable,1= enable.
	RS485_MODE,//120	
	TEMPRATURE_CHIP,//TEMPRATURE_SENSOR, //101/121 Calibrated temperature chip reading	(0.1 degrees)
	ANALOG1_RANGE,//							
	ANALOG2_RANGE,//
	ANALOG3_RANGE,//
	ANALOG4_RANGE, 
	ANALOG5_RANGE,//
	ANALOG6_RANGE,//
	ANALOG7_RANGE,//
	ANALOG8_RANGE,//	
	INTERNAL_THERMISTOR,
	ANALOG_INPUT1_VALUE, 					
	ANALOG_INPUT2_VALUE,
	ANALOG_INPUT3_VALUE,  
	ANALOG_INPUT4_VALUE,
	ANALOG_INPUT5_VALUE,
	ANALOG_INPUT6_VALUE,
	ANALOG_INPUT7_VALUE,    
	ANALOG_INPUT8_VALUE,
	EXTERNAL_SENSOR1,  //CO2	//
	EXTERNAL_SENSOR2,  //HUM/		
	INPUT_MANU_ENABLE,//
	FILTER,//											//		0 = OFF, 1 = trigger unoccupied on falling edge, 2 = occupied high, unoccupied low
	INPUT1_FILTER,	
 	INPUT2_FILTER,
	INPUT3_FILTER,
	INPUT4_FILTER,
	INPUT5_FILTER,
	INPUT6_FILTER,
	INPUT7_FILTER,
	INPUT8_FILTER,			//
	CO2_FILTER,
	HUM_FILTER,
	CALIBRATION,//			// 0	255	Thermistor calibration term 
	CALIBRATION_INTERNAL_THERMISTOR,//
	CALIBRATION_ANALOG1,//							
	CALIBRATION_ANALOG2,//				
	CALIBRATION_ANALOG3,//
	CALIBRATION_ANALOG4,//
	CALIBRATION_ANALOG5,//
	CALIBRATION_ANALOG6,//
	CALIBRATION_ANALOG7,//
	CALIBRATION_ANALOG8,// 
	TSTAT_HUMCOUNT1_H ,       //		    //  sensor frequency	   //these parameters must be continued,it won't work  if have blank,don't know why
	TSTAT_HUMRH1_H	,          //				//  calibration point
	TSTAT_HUMCOUNT2_H ,       //			//
	TSTAT_HUMRH2_H	,          //
	ANALOG1_FUNCTION,//
	ANALOG2_FUNCTION,//
	ANALOG3_FUNCTION,//
	ANALOG4_FUNCTION,//
	ANALOG5_FUNCTION,//
	ANALOG6_FUNCTION,//
	ANALOG7_FUNCTION,//
	ANALOG8_FUNCTION,//
	TABLE1_ZERO,  //                  // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
	TABLE1_HALFONE,  //                //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
	TABLE1_ONE,   //                 //              the value's unit in the table is different according to different sensor
	TABLE1_HALFTWO, //               
	TABLE1_TWO,//
	TABLE1_HALFTHREE,//
	TABLE1_THREE,//
	TABLE1_HALFFOUR,//
	TABLE1_FOUR,//
	TABLE1_HALFFIVE,//
	TABLE1_FIVE,//
	TABLE2_ZERO,//
	TABLE2_HALFONE,//
	TABLE2_ONE,//
	TABLE2_HALFTWO,//
	TABLE2_TWO,//
	TABLE2_HALFTHREE,//
	TABLE2_THREE,//
	TABLE2_HALFFOUR,//
	TABLE2_FOUR,//
	TABLE2_HALFFIVE,//
	TABLE2_FIVE ,//
	TSTAT_HUMIDITY_RH,	       //			// relative humidity in percentage
	TSTAT_HUMIDITY_FREQUENCY, //		// raw frequency reading
	TSTAT_HUM_PIC_UPDATE,     //			// write current calibration table to PIC, which table decided by register 427
	TSTAT_HUM_CAL_NUM,	       //			// calibration data number
	TSTAT_HUM_CURRENT_DEFAULT,//			// decide which table will run, default tabel or customer table   current=1 default=0			
	MODE_OUTPUT1,//202	   Determine the output mode. 0, ON/OFF mode; 1, floating valve for cooling;  2, floating valve for heating 3, PWM
	MODE_OUTPUT2,//
	MODE_OUTPUT3,//
	MODE_OUTPUT4,//
	MODE_OUTPUT5,//
	OUTPUT1_SCALE,          //		// 0	20	Sets the full scale voltage of analog output 1											    //		    0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 
	OUTPUT2_SCALE,          //		// 0	20	Sets the full scale voltage of analog output 2
	TSTAT_DIGITAL_OUTPUT_STATUS,  //		// -	-	Relays 1,2,3 and, if on-off type, 4 and 5
	TSTAT_COOLING_VALVE,          //        // -	-	Cooling valve position 0-1000 = 0-10V
	TSTAT_HEATING_VALVE,          //        // -	-	Heating valve position 0-1000 = 0-10V	
	TSTAT_DAC_OFFSET,	           //					 // 0	255	DAC voltage offset
	OUTPUT1_DELAY_OFF_TO_ON,//		// 0	255	OFF to ON delay time (sec)
	OUTPUT2_DELAY_OFF_TO_ON,//		// 0	255	OFF to ON delay time (sec)
	OUTPUT3_DELAY_OFF_TO_ON,// 		// 0	255	OFF to ON delay time (sec)
	OUTPUT4_DELAY_OFF_TO_ON,//		// 0	255	OFF to ON delay time (sec)
	OUTPUT5_DELAY_OFF_TO_ON,// 		// 0	255	OFF to ON delay time (sec)
	OUTPUT6_DELAY_OFF_TO_ON,//
	OUTPUT7_DELAY_OFF_TO_ON,
	DELAY1_OFF_TO_ON_TIME_CURRENT,//				
	DELAY2_OFF_TO_ON_TIME_CURRENT,//				
	DELAY3_OFF_TO_ON_TIME_CURRENT,//			
	DELAY4_OFF_TO_ON_TIME_CURRENT,//			
	DELAY5_OFF_TO_ON_TIME_CURRENT,//
	DELAY6_OFF_TO_ON_TIME_CURRENT,//future
	DELAY7_OFF_TO_ON_TIME_CURRENT,//future
	OUTPUT1_DELAY_ON_TO_OFF,//  		// 0	255	ON to OFF delay time (sec)
	OUTPUT2_DELAY_ON_TO_OFF,//		// 0	255	ON to OFF delay time (sec)
	OUTPUT3_DELAY_ON_TO_OFF,// 		// 0	255	ON to OFF delay time (sec)
	OUTPUT4_DELAY_ON_TO_OFF,// 		// 0	255	ON to OFF delay time (sec)
	OUTPUT5_DELAY_ON_TO_OFF,// 		// 0	255	ON to OFF delay time (sec)
	OUTPUT6_DELAY_ON_TO_OFF,//future
	OUTPUT7_DELAY_ON_TO_OFF,//future
	DELAY1_ON_TO_OFF_TIME_CURRENT,//	            
	DELAY2_ON_TO_OFF_TIME_CURRENT,//				
	DELAY3_ON_TO_OFF_TIME_CURRENT,//				
	DELAY4_ON_TO_OFF_TIME_CURRENT,//				
	DELAY5_ON_TO_OFF_TIME_CURRENT,//	
	DELAY6_ON_TO_OFF_TIME_CURRENT,//future		
	DELAY7_ON_TO_OFF_TIME_CURRENT,//future
	CYCLING_DELAY ,    //			// 0	20	Delay between going out of heating or cooling and then back in. (1 min)      			
	CHANGOVER_DELAY,	  //				// 0	200	Delay between going from cooling to heating or vice versa (1 min)    										     //		0=ON/OFF, 1=0-10V, 2=0-5V, 3=2-10V, 
	VALVE_TRAVEL_TIME ,//
	VALVE_PERCENT,//
	INTERLOCK_OUTPUT1,//
	INTERLOCK_OUTPUT2,//
	INTERLOCK_OUTPUT3,//
	INTERLOCK_OUTPUT4,//
	INTERLOCK_OUTPUT5,//
	INTERLOCK_OUTPUT6,//
	INTERLOCK_OUTPUT7,//
	FREEZE_DELAY_ON,						//
	FREEZE_DELAY_OFF,					//
	OUTPUT_MANU_ENABLE,					//
	MANU_RELAY1,							//
 	MANU_RELAY2,							//
	MANU_RELAY3,							//
	MANU_RELAY4,							//
	MANU_RELAY5,							//
	MANUAL_AO1,
	MANUAL_AO2,
	DEADMASTER_AUTO_MANUAL,//     // DEADMASTER_AUTO_MANUAL = 1, the output will be trigger to "AUTO" mode  //DEADMASTER_AUTO_MANUAL = 0, the default, outputs will not change, stay in whatever mode they were last commanded                                        //DEADMASTER_AUTO_MANUAL = 2, the outputs will go to manual on or off as defined in register 313. 
	DEADMASTER_OUTPUT_STATE,//    (relay1-5) deadmaster config value during deadmaster be triggered
	DEADMASTER_COOL_OUTPUT, //    (analog output - cooling)deadmaster config value during deadmaster be triggered
	DEADMASTER_HEAT_OUTPUT, //
	OUTPUT1_FUNCTION,//	
	OUTPUT2_FUNCTION,//		
	OUTPUT3_FUNCTION,//				
	OUTPUT4_FUNCTION,//			
	OUTPUT5_FUNCTION,//
	OUTPUT6_FUNCTION,//
	OUTPUT7_FUNCTION,//   
 	FAN_SPEED,// 	    // 0	4	Fan speed 0=OFF, 1=Low, 2=MED, 3=HI, 4=AUTO
	PID_OUTPUT1 , //                  // 1    254 reg 247 select which type PID to be caculate for control output
	PID_OUTPUT2 , //  
	PID_OUTPUT3 , //  
	PID_OUTPUT4 , //  
	PID_OUTPUT5 , //  
	PID_OUTPUT6 , //  
	PID_OUTPUT7 , //		
	UNIVERSAL_OUTPUT_BEGIN ,//        control OUTPUT_1TO5,there are 7 status
	UNIVERSAL_OUTPUT_COOL1 ,//
	UNIVERSAL_OUTPUT_COOL2 ,//
	UNIVERSAL_OUTPUT_COOL3 ,//
	UNIVERSAL_OUTPUT_HEAT1 ,//  
	UNIVERSAL_OUTPUT_HEAT2 ,//
	UNIVERSAL_OUTPUT_HEAT3 ,//
	FAN0_OPER_TABLE_COAST,//      	// 1 byte, first 5 lsb shows the relay states, last 3 msb are spares
	FAN0_OPER_TABLE_COOL1,//		// 1st byte is always coasting
	FAN0_OPER_TABLE_COOL2,//      	// next is cooling stages 
	FAN0_OPER_TABLE_COOL3,//      	// then comes heating stages
	FAN0_OPER_TABLE_HEAT1,//      	// there are up to 7 stages, unused stages are pushed to the end of the strucuture, spare if not used
	FAN0_OPER_TABLE_HEAT2,//      	// any of the stages can be heat or cool depending on the number of heat and cool stages  
	FAN0_OPER_TABLE_HEAT3,//      	// defined in registers 276 and 277
	FAN1_OPER_TABLE_COAST,//      	// Address 145
	FAN1_OPER_TABLE_COOL1,//		// 
	FAN1_OPER_TABLE_COOL2,//      	// 
	FAN1_OPER_TABLE_COOL3,//      	// 
	FAN1_OPER_TABLE_HEAT1,//      	// 
	FAN1_OPER_TABLE_HEAT2,//      	// 
	FAN1_OPER_TABLE_HEAT3,//      	// 		
	FAN2_OPER_TABLE_COAST,//      	// Address 
	FAN2_OPER_TABLE_COOL1,//		// 
	FAN2_OPER_TABLE_COOL2,//      	// 
	FAN2_OPER_TABLE_COOL3,//      	// 
	FAN2_OPER_TABLE_HEAT1,//      	// 
	FAN2_OPER_TABLE_HEAT2,//      	// 
	FAN2_OPER_TABLE_HEAT3,//      	// 			
	FAN3_OPER_TABLE_COAST,//      	// Address
	FAN3_OPER_TABLE_COOL1,//		// 
	FAN3_OPER_TABLE_COOL2,//      	// 
	FAN3_OPER_TABLE_COOL3,//      	// 
	FAN3_OPER_TABLE_HEAT1,//      	// 
	FAN3_OPER_TABLE_HEAT2,//      	// 
	FAN3_OPER_TABLE_HEAT3,//      	// 		
	FANAUT_OPER_TABLE_COAST,//     // Address 
	FANAUT_OPER_TABLE_COOL1,//		// 
	FANAUT_OPER_TABLE_COOL2,//     // 
	FANAUT_OPER_TABLE_COOL3,//     // 
	FANAUT_OPER_TABLE_HEAT1,//     // 
	FANAUT_OPER_TABLE_HEAT2,//     // 
	FANAUT_OPER_TABLE_HEAT3,//     // 
	VALVE_OPER_TABLE_BEGIN,// 
	VALVE_OPER_TABLE_COOL1,//	   
	VALVE_OPER_TABLE_COOL2,//
	VALVE_OPER_TABLE_COOL3,//
	VALVE_OPER_TABLE_HEAT1,//
	VALVE_OPER_TABLE_HEAT2,//
	VALVE_OPER_TABLE_HEAT3,//
	HEAT_UNIVERSAL_TABLE	,//
	COOL_UNIVERSAL_TABLE ,//
	HEAT_ORIGINAL_TABLE , //                   
	COOL_ORIGINAL_TABLE , // 
	VALVE_OFF_TABLE_COAST,//	   //OFF table for analog output,
	VALVE_OFF_TABLE_COOL1,//
	VALVE_OFF_TABLE_COOL2,//
	VALVE_OFF_TABLE_COOL3,//
	VALVE_OFF_TABLE_HEAT1,//
	VALVE_OFF_TABLE_HEAT2,//					
	VALVE_OFF_TABLE_HEAT3,//					  
	DEFAULT_SETPOINT,		//
	SETPOINT_CONTROL,		//
	DAYSETPOINT_OPTION,		//	
	MIDDLE_SETPOINT,			//
	DAY_SETPOINT,
	DAY_COOLING_DEADBAND,        //   	// 1	100	Cooling deadband	(0.1 degree)
	DAY_HEATING_DEADBAND,    	// 		// 1	100	Heating deadband	(0.1 degree)
	DAY_COOLING_SETPOINT,		//		// 15	50	(59 and 99 for degrees F)	Cooling setpoint (1degree)
	DAY_HEATING_SETPOINT,  		//		// 10	35	(50 and 95 for degrees F)	Heating setpoint (1degree)
	NIGHT_SETPOINT,
	APPLICATION,//   	                // 0	1	0=Office, 1=Hotel or Residential
 	NIGHT_HEATING_DEADBAND,//		    // 0	35	(0 and 95 for degrees F)	Night heating setback (1 degree)
	NIGHT_COOLING_DEADBAND,//		    // 0	99	(0 and 95 for degrees F)	Night cooling setback (1 degree)
	NIGHT_HEATING_SETPOINT,//		    // 10	99	Night Heating Setpoint	(1 degree)
	NIGHT_COOLING_SETPOINT,//		    // 10	99	Night Cooling Setpoint	(1 degree)
	WINDOW_INTERLOCK_COOLING_SETPOINT,   //TBD
	WINDOW_INTERLOCK_HEATING_SETPOINT,	//TBD
	UNIVERSAL_NIGHTSET,//                
	UNIVERSAL_SET,//                  // 1    254 reg 246 is the setpoint for universal PID 
	UNIVERSAL_HEAT_DB,//universal heating deadband 
	UNIVERSAL_COOL_DB,//universal cooling deadband
	ECOMONY_COOLING_SETPOINT,
	ECOMONY_HEATING_SETPOINT,
	POWERUP_SETPOINT,//		        // 15	50	(59 and 99 for degrees F)	Power up cooling setpoint (1 degree)
	MAX_SETPOINT,//                   // 10	99	(50 and 150 for degrees F)	Max cooling setpoint (1degree)
	MIN_SETPOINT,// 		            // 10	99	(50 and 150 for degrees F)	Min heating setpoint (1degree)
	MAX_SETPOINT2,//  
	MIN_SETPOINT2,//
	MAX_SETPOINT3,//	
	MIN_SETPOINT3,//
	MAX_SETPOINT4,//
	MIN_SETPOINT4,//
	SETPOINT_INCREASE,		// 
	FREEZE_TEMP_SETPOINT,	//
	WALL_SETPOINT,//    wall setpoint ,normal setpoint
	CEILING_SETPOINT,//    celling setpoint
	AVERAGE_SETPOINT,//
	UNOCCUPIED_HEATING,//
	UNOCCUPIED_COOLING,//
	RH_SETPOINT,//
 	CURRENT1_SETPOINT,//  tbd  get rid of this
  TEMP_SELECT,//			 		// 1= external sensor analog input 1 , 2 = internal thermistor, 3 = average the internal thermistor and analog input1
	INPUT1_SELECT,//              	// 1    254 reg 241 select which way the analog input is
	COOLING_PID,//				 	// -	-	Cooling PI Term	0-100 - PID IN REV25
	COOLING_PTERM,//    			 	// 0	255	Cooling P term (0.1 degree)
  COOLING_ITERM ,  //				// 0	255	Cooling I term (0.1 %deg/min)
  UNIVERSAL_PTERM ,//              // 1    254 reg 244 is the address of P term a universal PID
	UNIVERSAL_ITERM ,//		      	// 1    254 reg 245 is the address of I term a universal PID		
	TSTAT_PID_UNIVERSAL ,//                  
	UNITS1_HIGH ,  //                     
	UNITS1_LOW  ,  //                      
	UNITS2_HIGH ,  //                    
	UNITS2_LOW ,   //                    
	TSTAT_PID2_MODE_OPERATION,
  KEYPAD_SELECT ,	//				// 0	3	Number of buttons on the keypad - 0=2, 1=4, 2=6A, 3=6B 
  SPECIAL_MENU_LOCK,//				// 0	1	Special menu lockout via keypad, serial port only 0=unlock, 1=lock	
	DISPLAY,	//						// 0	1	Display setting.  0 = temperature, 1 = setpoint
	ICON,
	LAST_KEY_TIMER,//
	TSTAT_KEYPAD_VALUE,//
	DISPLAY_HUNDRED,//
	DISPLAY_TEN,//
	DISPLAY_DIGITAL,//
	DISPLAY_STATUS,//
	ROUND_DISPLAY,//
	MIN_ADDRESS,//	                // The ID of tstat should be in the range MIN and MAX address
	MAX_ADDRESS,// 
	EEPROM_SIZE,//		
	TIMER_SELECT,//			 
	RTC_YEAR,// 
 	RTC_MONTH,//
 	RTC_WEEK,//
 	RTC_DAY,//
	RTC_HOUR,//
 	RTC_MINUTE,//
 	RTC_SECOND,//
	DIAGNOSTIC_ALARM,//					
	SCHEDULE_DHOME_HOUR,// at home (sun + man)				//schedule
	SCHEDULE_DHOME_MIN,//
	SCHEDULE_WORK_HOUR,// at work (sun + no man)
	SCHEDULE_WORK_MIN,// 
	SCHEDULE_NHOME_HOUR,// at home (sun + man)				//schedule
	SCHEDULE_NHOME_MIN,//	
	SCHEDULE_SLEEP_HOUR,// sleeping (moon + man)
	SCHEDULE_SLEEP_MIN,//
	SCHEDULE_HOLIDAY_BYEAR,// 
	SCHEDULE_HOLIDAY_BMONTH,// away/holiday (no sun&moon + no man)  
	SCHEDULE_HOLIDAY_BDAY,//
	SCHEDULE_HOLIDAY_EYEAR,// 
	SCHEDULE_HOLIDAY_EMONTH,// 					
	SCHEDULE_HOLIDAY_EDAY,//                   
	DAY2_EVENT4_HI,// 
	DAY2_EVENT4_LO,//
	LCD_TURN_OFF,//							
	LINE1_CHAR1,//								
	LINE1_CHAR2,//
	LINE1_CHAR3,//
	LINE1_CHAR4,//  
	LINE2_CHAR1,//							
	LINE2_CHAR2,//
	LINE2_CHAR3,//
	LINE2_CHAR4,// 	
	INTERNAL_CHAR1,//					
	INTERNAL_CHAR2,//
	INTERNAL_CHAR3,//
	INTERNAL_CHAR4,//		
	AI1_CHAR1,//							//input1 user input description
	AI1_CHAR2,//
	AI1_CHAR3,//
	AI1_CHAR4,// 
	AI2_CHAR1,//							//input2 user input description						
	AI2_CHAR2,//
	AI2_CHAR3,//                       
	AI2_CHAR4,//
	AI3_CHAR1,//				           	//input3 user input description			
	AI3_CHAR2,//
	AI3_CHAR3,//
	AI3_CHAR4,// 
	AI4_CHAR1,//						   	//input4 user input description
	AI4_CHAR2,//
	AI4_CHAR3,//
	AI4_CHAR4,//
	AI5_CHAR1,//						   	//input5 user input description
	AI5_CHAR2,//
	AI5_CHAR3,//
	AI5_CHAR4,//
	AI6_CHAR1,//						   	//input6 user input description
	AI6_CHAR2,//
	AI6_CHAR3,//
	AI6_CHAR4,//
	AI7_CHAR1,//						   	//input7 user input description
	AI7_CHAR2,//
	AI7_CHAR3,//                
	AI7_CHAR4,//
	AI8_CHAR1,//						   	//input8 user input description
	AI8_CHAR2,//
	AI8_CHAR3,//
	AI8_CHAR4,//
	OUTPUT1_CHAR1,//					   	//output1 user output description
	OUTPUT1_CHAR2,//
	OUTPUT1_CHAR3,//
	OUTPUT1_CHAR4,//
	OUTPUT2_CHAR1,//						//output2 user output description
	OUTPUT2_CHAR2,//
	OUTPUT2_CHAR3,//
	OUTPUT2_CHAR4,//
	OUTPUT3_CHAR1,//						//output3 user output description
	OUTPUT3_CHAR2,//
	OUTPUT3_CHAR3,//
	OUTPUT3_CHAR4,//
	OUTPUT4_CHAR1,//						//output4 user output description
	OUTPUT4_CHAR2,//
	OUTPUT4_CHAR3,//					
	OUTPUT4_CHAR4,//
	OUTPUT5_CHAR1,//						//output5 user output description
	OUTPUT5_CHAR2,//
	OUTPUT5_CHAR3,//
	OUTPUT5_CHAR4,//
	OUTPUT6_CHAR1,//					    //output6 user output description
	OUTPUT6_CHAR2,//
	OUTPUT6_CHAR3,//
	OUTPUT6_CHAR4,//
	OUTPUT7_CHAR1,//						//output7 user output description
	OUTPUT7_CHAR2,//
	OUTPUT7_CHAR3,//
	OUTPUT7_CHAR4,//
	SCHEDULEA_CHAR1,	//						// 		
	SCHEDULEA_CHAR2,
	SCHEDULEA_CHAR3,
	SCHEDULEA_CHAR4,
	SCHEDULEB_CHAR1,							//		
	SCHEDULEB_CHAR2,
	SCHEDULEB_CHAR3,
	SCHEDULEB_CHAR4,
	SCHEDULEC_CHAR1,							//		
	SCHEDULEC_CHAR2,
	SCHEDULEC_CHAR3,//
	SCHEDULEC_CHAR4,
	SCHEDULED_CHAR1,							//		
	SCHEDULED_CHAR2,
	SCHEDULED_CHAR3,
	SCHEDULED_CHAR4, 
	WALL_CHAR1,							//		
	WALL_CHAR2,
	WALL_CHAR3,
	WALL_CHAR4,
	CEILING_CHAR1,//						//		
	CEILING_CHAR2,
	CEILING_CHAR3,
	CEILING_CHAR4,
	OUTDOOR_CHAR1,//
	OUTDOOR_CHAR2,
	OUTDOOR_CHAR3,
	OUTDOOR_CHAR4,
	AVERAGE_CHAR1,//
	AVERAGE_CHAR2,
	AVERAGE_CHAR3,//
	AVERAGE_CHAR4,
	LCD_SCREEN1,//						
	LCD_SCREEN2,//											   
	DEMAND_RESPONSE,//    	 
 	LOCK_REGISTER, 
	PIR_STAGE,  
	FIRST_CAL_FLAG, //	        // picdataok
	BAC_INSTANCE_LO,//RTC_HUM_CAL_EREASE,//			// erease current PIC calibration data table  
	BAC_INSTANCE_HI,//RTC_HUMCOUNT3_H ,//		
	RTC_HUMRH3_H	,	//		
	RTC_HUMCOUNT4_H ,//		
	RTC_HUMRH4_H	,	//			
	RTC_HUMCOUNT5_H ,//	
	RTC_HUMRH5_H	,	//			
	RTC_HUMCOUNT6_H ,//			
	RTC_HUMRH6_H	,	//			
	RTC_HUMCOUNT7_H ,//			
	RTC_HUMRH7_H	,	//			
	RTC_HUMCOUNT8_H ,//			
	RTC_HUMRH8_H	,	//			
	RTC_HUMCOUNT9_H ,//			
	RTC_HUMRH9_H	,	//				
	RTC_HUMCOUNT10_H,//			
	RTC_HUMRH10_H,   //  
	RTC_HUM_LOCK_A,
	RTC_HUM_LOCK_B,
	LCD_ROTATE_ENABLE,
	SCHEDULE_ON_OFF,//1:ON  0:OFF
	DISP_ITEM_TEMPERATURE, 
	DISP_ITEM_SETPOINT,	   
	DISP_ITEM_AI1,//		   
	DISP_ITEM_AI2,		   
	DISP_ITEM_AI3,//		   
	DISP_ITEM_AI4,		   
	DISP_ITEM_AI5,		   
	DISP_ITEM_AI6,		   
	DISP_ITEM_AI7,		   
	DISP_ITEM_AI8,		   
	DISP_ITEM_MODE,
	DISP_ITEM_USER_INFO,
	DISP_ITEM_CLOCK_DATE,
	DISP_ITEM_CLOCK_TIME,//
	DISP_ITEM_EXTENAL_SENSOR1,
	DISP_ITEM_EXTENAL_SENSOR2,
	DISP_ITEM_EXTENAL_SENSOR3,
	DISP_ITEM_EXTENAL_SENSOR4,
	DISP_ITEM_OUT1,
	DISP_ITEM_OUT2,
	DISP_ITEM_OUT3,//
	DISP_ITEM_OUT4,
	DISP_ITEM_OUT5,//										  
	DISP_ITEM_OUT6,
	DISP_ITEM_OUT7,
	OUTPUT_PWM_AUTO_COAST,// out4 and out5 pwm table 			    		
	OUTPUT_PWM_AUTO_COOL1,//			    		
	OUTPUT_PWM_AUTO_COOL2,//		    		
	OUTPUT_PWM_AUTO_COOL3,//			 		
	OUTPUT_PWM_AUTO_HEAT1,//			  		
	OUTPUT_PWM_AUTO_HEAT2,//			   
	OUTPUT_PWM_AUTO_HEAT3,//	
	PWM_OUT1,
	PWM_OUT2,
	SUN_ICON_CONTROL, //					
	MOON_ICON_CONTROL,//
	EXT_SENSOR_TEMP_CAL,	//
	BUTTON_LEFT1,
	BUTTON_LEFT2,
	BUTTON_LEFT3,
	BUTTON_LEFT4,
	HUM_HEATING_CONTROL,
	HUM_INPUT_MANUAL_ENABLE,
	HUM_INPUT_MANUAL_VALUE,
	CO2_INPUT_MANUAL_ENABLE,
	CO2_INPUT_MANUAL_VALUE,
	CO2_CALIBRATION, //CO2 calibration data
	UNIVERSAL_OFF_TABLE_BEGIN , //PID2 ON/OFF mode off tabale
	UNIVERSAL_OFF_TABLE_COOL1 ,
	UNIVERSAL_OFF_TABLE_COOL2 ,
	UNIVERSAL_OFF_TABLE_COOL3 ,
	UNIVERSAL_OFF_TABLE_HEAT1 , //     
	UNIVERSAL_OFF_TABLE_HEAT2 ,
	UNIVERSAL_OFF_TABLE_HEAT3 ,	
	UNIVERSAL_OFF_VALVE_BEGIN , //PID2 valve mode off tabale
	UNIVERSAL_OFF_VALVE_COOL1 ,
	UNIVERSAL_OFF_VALVE_COOL2 ,
	UNIVERSAL_OFF_VALVE_COOL3 ,
	UNIVERSAL_OFF_VALVE_HEAT1 ,
	UNIVERSAL_OFF_VALVE_HEAT2 ,
	UNIVERSAL_OFF_VALVE_HEAT3 ,//398	
	KEYPAD_REVERSE,
	LIGHT_SENSOR,  //light sensor value 
	PIR_SENSOR_SELECT,
	PIR_SENSOR_VALUE,
	PIR_SENSOR_ZERO,
	UNIVERSAL_VALVE_BEGIN , //control the status of valve
	UNIVERSAL_VALVE_COOL1 ,
	UNIVERSAL_VALVE_COOL2 ,
	UNIVERSAL_VALVE_COOL3 ,
	UNIVERSAL_VALVE_HEAT1 ,
	UNIVERSAL_VALVE_HEAT2 ,
	UNIVERSAL_VALVE_HEAT3 ,
	TSTAT_ID_WRITE_ENABLE,
	SPARE,
	PIR_TIMER,
	SUPPLY_SETPOINT,	  //	
	MAX_SUPPLY_SETPOINT,//
	MIN_SUPPLY_SETPOINT,//
	MAX_AIRFLOW_COOLING,//
	MAX_AIRFLOW_HEATING,//
	OCC_MIN_AIRFLOW,		    //
	AIRFLOW_SETPOINT,   //
  VAV_CONTROL,//576   //
	PID3_INPUT_SELECT,  //
	PID3_VALVE_OPER_TABLE_BEGIN,	// Address  
	PID3_VALVE_OPER_TABLE_COOL1,
	PID3_VALVE_OPER_TABLE_COOL2,//
	PID3_VALVE_OPER_TABLE_COOL3,
	PID3_VALVE_OPER_TABLE_HEAT1,
	PID3_VALVE_OPER_TABLE_HEAT2,
	PID3_VALVE_OPER_TABLE_HEAT3,//
	PID3_COOLING_DB,//
	PID3_HEATING_DB,
	PID3_PTERM,
	PID3_ITERM,
	PID3_HEAT_STAGE,
	PID3_COOL_STAGE,
	PID3_OUTPUT, //
	PID3_OUTPUT_BEGIN , //control OUTPUT_1TO5,there are 7 status
	PID3_OUTPUT_COOL1 ,
	PID3_OUTPUT_COOL2 ,
	PID3_OUTPUT_COOL3 ,
	PID3_OUTPUT_HEAT1 , // 
	PID3_OUTPUT_HEAT2 ,
	PID3_OUTPUT_HEAT3 ,//
	PID3_VALVE_OFF_TABLE_BEGIN,//
	PID3_VALVE_OFF_TABLE_COOL1,
	PID3_VALVE_OFF_TABLE_COOL2,//
	PID3_VALVE_OFF_TABLE_COOL3,
	PID3_VALVE_OFF_TABLE_HEAT1,
	PID3_VALVE_OFF_TABLE_HEAT2,
	PID3_VALVE_OFF_TABLE_HEAT3,  //
	PID3_OFF_OUTPUT_BEGIN , //
	PID3_OFF_OUTPUT_COOL1 ,
	PID3_OFF_OUTPUT_COOL2 ,
	PID3_OFF_OUTPUT_COOL3 ,
	PID3_OFF_OUTPUT_HEAT1 , // 
	PID3_OFF_OUTPUT_HEAT2 ,
	PID3_OFF_OUTPUT_HEAT3 ,//
	WIRELESS_PIR_RESPONSE1,
	WIRELESS_PIR_RESPONSE2,
	WIRELESS_PIR_RESPONSE3,
	WIRELESS_PIR_RESPONSE4,
	WIRELESS_PIR_RESPONSE5,
	TSTAT_HEAT_COOL,
	SPARE1,
	SPARE2,
	TSTAT_HUM_PIC_VERSION,  //515
	INTERNAL_SENSOR_MANUAL, //0 = AUTO, 1 = MANUAL
	PRESSURE_VALUE,
	PRESSURE_MANUAL_VALUE,
	PRESSURE_MANUAL_ENABLE,
	AQ_VALUE,
	AQ_MANUAL_VALUE,
	AQ_MANUAL_ENABLE,
	TEMPERATURE_OF_PRESSURE_SENSOR,
	TEMPERATURE_OF_HUM_SENSOR,
	SP_DISPLAY_SELECT,
	PID3_DAY_SP,
  PID3_NIGHT_SP, 
	TSTAT_BK_SN,
	BK_SNLOLO,
	BK_SNLOHI,	
	BK_SNHILO,
	UNOCC_AIRFLOW,
	PWM_ENABLE,
	SCHEDULE_MODE_NUM,
	TSTAT_NAME_ENABLE,//default value = 0x55 indicate tstat name function is present
	TSTAT_NAME1,
	TSTAT_NAME2,
	TSTAT_NAME3,
	TSTAT_NAME4,
	TSTAT_NAME5,
	TSTAT_NAME6,
	TSTAT_NAME7,
	TSTAT_NAME8,
	TSTAT_NAME9,
	TSTAT_NAME10,
	SHOW_ID,//this regisger will let user display current ID, the time will depending on the written value  	
	TRANSDUCER_RANGE_MIN,  //When set analog output to transducer mode, use these two reg to set range: range_min = 10 means 0V accodrding to 10C  
	TRANSDUCER_RANGE_MAX,
	ICON_MANUAL_MODE, //manual icons enable/disable   0:auto icon   1: manual icon 2: output control: icon will be on/off depending on outputs state.
	ICON_MANUAL_VALUE,//when set icon to munual mode, set value here 
	SOFTWARE_RESET,
	CO2_SELF_CAL,
  CO2_SELF_CAL_TIME,   
	CO2_SELF_CAL_VALUE,
	CO2_RANGE,
	SET_PIR_SENSETIVITY,
	PIR_SPARE,
	MODE1_NAME1,
	MODE1_NAME2,
	MODE1_NAME3,
	MODE1_NAME4,
	MODE2_NAME1,
	MODE2_NAME2,
	MODE2_NAME3,
	MODE2_NAME4,
	MODE3_NAME1,
	MODE3_NAME2, 
	MODE3_NAME3,
	MODE3_NAME4,
	MODE4_NAME1,
	MODE4_NAME2,
	MODE4_NAME3,
	MODE4_NAME4,
	MODE5_NAME1,
	MODE5_NAME2,
	MODE5_NAME3,
	MODE5_NAME4,
  OUTSIDETEM,
	OUTSIDETEM_SLAVE,		
	TSTAT_TEST1,//	
	TSTAT_TEST2,//
	TSTAT_TEST3,
	TSTAT_TEST4,//						
	TSTAT_TEST5,//
	TSTAT_TEST6,
	TSTAT_TEST7,
	TSTAT_TEST8,
	TSTAT_TEST9,
	TSTAT_TEST10,	
	HUM_CALIBRATION,
	PIC_VERSION,
	SLEEP_COOLING_DB,
	SLEEP_HEATING_DB,
	SLEEP_SP,
	SLEEP_COOLING_SP,
	SLEEP_HEATING_SP,
	MAX_WORK_SP,//max work setpoint  40C  
	MIN_WORK_SP,//min work setpoint  16C
	MAX_SLEEP_SP,//max sleeping setpoint 30C		
	MIN_SLEEP_SP,//min sleeping setpoint 15C	
	MAX_HOLIDAY_SP,//
	MIN_HOLIDAY_SP,//min holiday setpoint   >5C
	INPUT_VOLTAGE_TERM,	
	ICON_DAY_OUTPUT_CONTROL,
	ICON_NIGHT_OUTPUT_CONTROL,
	ICON_OCC_OUTPUT_CONTROL,
	ICON_UNOCC_OUTPUT_CONTROL,
	ICON_HEAT_OUTPUT_CONTROL,
	ICON_COOL_OUTPUT_CONTROL,
	ICON_FAN1_OUTPUT_CONTROL,
	ICON_FAN2_OUTPUT_CONTROL,
	ICON_FAN3_OUTPUT_CONTROL,
	OUTPUT_PWM_OFF_COAST,// out4 and out5 pwm table 			    		
	OUTPUT_PWM_OFF_COOL1,//			    		
	OUTPUT_PWM_OFF_COOL2,//		    		
	OUTPUT_PWM_OFF_COOL3,//			 		
	OUTPUT_PWM_OFF_HEAT1,//			  		
	OUTPUT_PWM_OFF_HEAT2,//			   
	OUTPUT_PWM_OFF_HEAT3,//	
  DTERM,	
	PID_SAMPLE_TIME,
	
	
	
	
	
	
	
	
	
	
	
	//BOARD_SELECT = 812, //0: normal tstat8  1:tstat8 occ 2: tstat8 hunter
	SCHEDULE_MONDAY_EVENT1_H = 813,// at home (sun + man)				//schedule
	SCHEDULE_MONDAY_EVENT1_M,//
	SCHEDULE_MONDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_MONDAY_EVENT2_M,// 
	SCHEDULE_MONDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_MONDAY_EVENT3_M,//
	SCHEDULE_MONDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_MONDAY_EVENT4_M,//
	SCHEDULE_MONDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_MONDAY_EVENT5_M,//
	SCHEDULE_MONDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_MONDAY_EVENT6_M,//
	SCHEDULE_TUESDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_TUESDAY_EVENT1_M,//
	SCHEDULE_TUESDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_TUESDAY_EVENT2_M,// 
	SCHEDULE_TUESDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_TUESDAY_EVENT3_M,//
	SCHEDULE_TUESDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_TUESDAY_EVENT4_M,//
	SCHEDULE_TUESDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_TUESDAY_EVENT5_M,//
	SCHEDULE_TUESDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_TUESDAY_EVENT6_M,//
	SCHEDULE_WENSDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_WENSDAY_EVENT1_M,//
	SCHEDULE_WENSDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_WENSDAY_EVENT2_M,// 
	SCHEDULE_WENSDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_WENSDAY_EVENT3_M,//
	SCHEDULE_WENSDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_WENSDAY_EVENT4_M,//
	SCHEDULE_WENSDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_WENSDAY_EVENT5_M,//
	SCHEDULE_WENSDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_WENSDAY_EVENT6_M,//		
	SCHEDULE_THURSDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_THURSDAY_EVENT1_M,//
	SCHEDULE_THURSDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_THURSDAY_EVENT2_M,// 
	SCHEDULE_THURSDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_THURSDAY_EVENT3_M,//
	SCHEDULE_THURSDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_THURSDAY_EVENT4_M,//
	SCHEDULE_THURSDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_THURSDAY_EVENT5_M,//
	SCHEDULE_THURSDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_THURSDAY_EVENT6_M,//	
	SCHEDULE_FRIDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_FRIDAY_EVENT1_M,//
	SCHEDULE_FRIDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_FRIDAY_EVENT2_M,// 
	SCHEDULE_FRIDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_FRIDAY_EVENT3_M,//
	SCHEDULE_FRIDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_FRIDAY_EVENT4_M,//
	SCHEDULE_FRIDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_FRIDAY_EVENT5_M,//
	SCHEDULE_FRIDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_FRIDAY_EVENT6_M,//		
	SCHEDULE_SATDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_SATDAY_EVENT1_M,//
	SCHEDULE_SATDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_SATDAY_EVENT2_M,// 
	SCHEDULE_SATDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_SATDAY_EVENT3_M,//
	SCHEDULE_SATDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_SATDAY_EVENT4_M,//
	SCHEDULE_SATDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_SATDAY_EVENT5_M,//
	SCHEDULE_SATDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_SATDAY_EVENT6_M,//	
	SCHEDULE_SUNDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_SUNDAY_EVENT1_M,//
	SCHEDULE_SUNDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_SUNDAY_EVENT2_M,// 
	SCHEDULE_SUNDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_SUNDAY_EVENT3_M,//
	SCHEDULE_SUNDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_SUNDAY_EVENT4_M,//
	SCHEDULE_SUNDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_SUNDAY_EVENT5_M,//
	SCHEDULE_SUNDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_SUNDAY_EVENT6_M,//		
	SCHEDULE_HOLIDAY_EVENT1_H,// at home (sun + man)				//schedule
	SCHEDULE_HOLIDAY_EVENT1_M,//
	SCHEDULE_HOLIDAY_EVENT2_H,// at work (sun + no man)
	SCHEDULE_HOLIDAY_EVENT2_M,// 
	SCHEDULE_HOLIDAY_EVENT3_H,// at home (sun + man)				//schedule
	SCHEDULE_HOLIDAY_EVENT3_M,//
	SCHEDULE_HOLIDAY_EVENT4_H,// at home (sun + man)				//schedule
	SCHEDULE_HOLIDAY_EVENT4_M,//
	SCHEDULE_HOLIDAY_EVENT5_H,// at home (sun + man)				//schedule
	SCHEDULE_HOLIDAY_EVENT5_M,//
	SCHEDULE_HOLIDAY_EVENT6_H,// at home (sun + man)				//schedule
	SCHEDULE_HOLIDAY_EVENT6_M,//
	SCHEDULE_MONDAY_FLAG_1,
	SCHEDULE_MONDAY_FLAG_2,
	SCHEDULE_TUESDAY_FLAG_1,
	SCHEDULE_TUESDAY_FLAG_2,
	SCHEDULE_WENSDAY_FLAG_1,
	SCHEDULE_WENSDAY_FLAG_2,
	SCHEDULE_THURSDAY_FLAG_1,
	SCHEDULE_THURSDAY_FLAG_2,
	SCHEDULE_FRIDAY_FLAG_1,
	SCHEDULE_FRIDAY_FLAG_2,
	SCHEDULE_SATDAY_FLAG_1,
	SCHEDULE_SATDAY_FLAG_2,
	SCHEDULE_SUNDAY_FLAG_1,
	SCHEDULE_SUNDAY_FLAG_2,
	SCHEDULE_HOLIDAY_FLAG_1,
	SCHEDULE_HOLIDAY_FLAG_2,
	SCHEDULE_DAY_BEGAIN,
	SCHEDULE_DAY_END = SCHEDULE_DAY_BEGAIN + 46, //971	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	AWAY_SP,
	AWAY_COOLING_SP,
	AWAY_HEATING_SP,
	AWAY_COOL_DB,
	AWAY_HEAT_DB,
	BACNET_STATION_NUM,
	BACNET_INSTANCE,
	MODBUS_4TO20MA_BOTTOM,
	MODBUS_4TO20MA_TOP,
	MODBUS_4TO20MA_UNIT_HI,
	MODBUS_4TO20MA_UNIT_LO,
	BAC_T1,
	BAC_T2,
	BAC_T3,
	BAC_T4,
	BAC_T5,
	BAC_T6,
	BAC_T7,
	BAC_T8,
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	FRC_ENABLE = 1000,//free cooling  function enable/disable 0:DIABLE  1:ENABLE
	FRC_BASE_SELECT, //1001 FREE COOLING BASE SELECT 0: TEMPERATUERE BASE 1: ENTHALPY BASE
	FRC_SPACETEM_SOURCE,//1002 FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK	 0: INTERNAL 1: REMOTE
	FRC_SPACETEM_VALUE,//1003CURRENT SPACE TEMPERATURE
	FRC_SPACETEM_UNIT, //1004SPACE TEMPERATURE UNIT
	FRC_SPACETEM_ID,//1005 if internal sensor COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK  if remote sensor, this is pannel number
	FRC_SPACETEM_LASTUPDATE,//1006 FREE COOLING SPACE TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
	FRC_SPACETEM_STATUE,//1007 CHECK IF SPACE TEMPERATURE SOURCE IS WORK WELL
	FRC_SPACETEM_CONFIG,//1008 FREE COOLING SPACE TEMPERATURE STATUS, CHECK RANGE,UNIT 		
	FRC_SUPPLYTEM_SOURCE,//1009 FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
	FRC_SUPPLYTEM_VALUE,//1010CURRENT SUPPLYTEM TEMPERATURE
	FRC_SUPPLYTEM_UNIT, //SUPPLYTEM TEMPERATURE UNIT
	FRC_SUPPLYTEM_ID,//COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	FRC_SUPPLYTEM_LASTUPDATE,// FREE COOLING SUPPLY TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
	FRC_SUPPLYTEM_STATUE,// CHECK IF SUPPLY TEMPERATURE SOURCE IS WORK WELL
	FRC_SUPPLYTEM_CONFIG,//1015 FREE COOLING SUPPLY TEMPERATURE STATUS, CHECK RANGE,UNIT 	
	FRC_OUTDOORTEM_SOURCE,//1016 FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
	FRC_OUTDOORTEM_VALUE,//CURRENT OUTDOORTEM TEMPERATURE
	FRC_OUTDOORTEM_UNIT, //OUTDOORTEM TEMPERATURE UNIT
	FRC_OUTDOORTEM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	FRC_OUTDOORTEM_LASTUPDATE,//1020 FREE COOLING OUTDOOR TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
	FRC_OUTDOORTEM_STATUE,// CHECK IF OUTDOOR TEMPERATURE SOURCE IS WORK WELL
	FRC_OUTDOORTEM_CONFIG,//1022 FREE COOLING OUTDOOR TEMPERATURE STATUS, CHECK RANGE,UNIT 	
	FRC_INDOORHUM_SOURCE,//1023 FREE COOLING INDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
	FRC_INDOORHUM_VALUE,//CURRENT INDOOR HUM 
	FRC_INDOORHUM_UNIT, //1025 INDOO RHUM UNIT
	FRC_INDOORHUM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	FRC_INDOORHUM_LASTUPDATE,//1027 FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
	FRC_INDOORHUM_STATUE,//1028 CHECK IF INDOOR HUMIDITY SOURCE IS WORK WELL
	FRC_INDOORHUM_CONFIG,//1029 FREE COOLING INDOOR HUMIDITY STATUS, CHECK RANGE,UNIT	
	FRC_OUTDOORHUM_SOURCE,//1030 FREE COOLING OUTDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
	FRC_OUTDOORHUM_VALUE,//CURRENT OUTDOOR HUM 
	FRC_OUTDOORHUM_UNIT, //OUTDOOR RHUM UNIT
	FRC_OUTDOORHUM_ID,//1033 COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	FRC_OUTDOORHUM_LASTUPDATE,// FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
	FRC_OUTDOORHUM_STATUE,//1035 CHECK IF OUTDOOR HUMIDITY SOURCE IS WORK WELL
	FRC_OUTDOORHUM_CONFIG,//1036 FREE COOLING OUTDOOR HUMIDITY STATUS, CHECK RANGE,UNIT	
	FRC_MIN_FRESH_AIR, // MINIMAL FRESH AIR, UNIT PERCENTAGE 0-100%
	FRC_MIN_FRESH_AIRTEM, // MINIMAL FRESH AIR TEMPERATURE, UNIT DEGREE C/F
	FRC_OUTPUT_SELECT,
	FRC_OUTPUT_CONFIG,  //1040 FREE COOLING OUTPUT CHECK, OUTPUT SOURCE, OUTPUT FUNCTION
	FRC_PID2_CONFIG,   // FREE COOLING PID2 CONFIG CHECK, CHECK IF PID2 HAS INPUT FROM SUPPLY SENSOR, ALSO CHECK IF IT IS SET TO NEGATIVE COOLING
	FRC_OUTPUT_MODE,  // SET FAN TO OFF
	FRC_TOTAL_CONFIG,//1045 MINIMAL SUPPLY AIR TEMPERATURE UNIT: DEGREE C/F
	FRC_PRESENT_MODE,//
	FRC_OUTDOOR_ENTHALPY,
	FRC_INDOOR_ENTHALPY, //1048
	FRC_PID2_VALUE,


	FRC_SPACETEM_STR_BEGAIN = 11748,
	FRC_SPACETEM_STR_END = FRC_SPACETEM_STR_BEGAIN + 23,
	FRC_SUPPLYTEM_STR_BEGAIN,
	FRC_SUPPLYTEM_STR_END = FRC_SUPPLYTEM_STR_BEGAIN + 23,
	FRC_OUTDOORTEM_STR_BEGAIN,
	FRC_OUTDOORTEM_STR_END = FRC_OUTDOORTEM_STR_BEGAIN + 23,
	FRC_INDOORHUM_STR_BEGAIN,
	FRC_INDOORHUM_STR_END = FRC_INDOORHUM_STR_BEGAIN + 23,
	FRC_OUTDOORHUM_BEGAIN,
	FRC_OUTDOORHUM_END = FRC_OUTDOORHUM_BEGAIN + 23,
	
/*********FOR CALIBRATE HUM SENSOR******************/
/******************add by ye************************/		
	
/******************END*******************************/

};

extern uint8 serial_no_activity;
extern u8 USART_RX_BUFA[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u8 USART_RX_BUFB[USART_REC_LEN];
#ifdef TSTAT_ZIGBEE 
extern u8 USART_RX_BUFC[50];     //接收缓冲,最大USART_REC_LEN个字节.
extern u8 USART_RX_BUFD[50];
#endif

#ifdef TSTAT_CO2
extern u8 USART_RX_BUFE[10];
extern u8 uart_sendC[9];
#endif//TSTAT_CO2

extern u8 dealwithTag ;
#ifdef TSTAT_ZIGBEE 
extern u8 dealwithTagB ;
#endif
void serial_restart(void);
#ifdef TSTAT_ZIGBEE 
void serial_restartB(void);
#endif //TSTAT_ZIGBEE
void modbus_data_cope(u8 XDATA* pData, u16 length, u8 conn_id) ;
void modbus_init(void) ;
#ifdef TSTAT_ZIGBEE 
void modbus_initB(void) ;
#endif
uint8 checkCrc(void);
#ifdef TSTAT_ZIGBEE 
uint8 checkCrcB(void);
#endif
typedef struct 
{
	u8 serial_Num[4];
	u16 software ;
	u8 address ;
	u32 baudrate ;
	u8 update ;
	u8 product ;
	u8 hardware_Rev;
//	u8 SNWriteflag ;
	u8 com_config[3];
	u8 protocal ;
	#ifdef T322AI 
	u16 input[MAX_AI_CHANNEL] ;
	u8  filter_value[MAX_AI_CHANNEL];
	u8  range [MAX_AI_CHANNEL];
	u16  offset[MAX_AI_CHANNEL];
	#endif
	#ifdef T38AI8AO6DO 
	u16 input[MAX_AI_CHANNEL] ;
	u8  filter_value[MAX_AI_CHANNEL];
	u8  range [MAX_AI_CHANNEL];
	u16  offset[MAX_AI_CHANNEL];
	u16	 switch_gourp[2] ;
//	u8 	digit_output[MAX_DOS] ;
//	u16 analog_output[MAX_AOS] ;
	#endif 	
}STR_MODBUS ;
extern STR_MODBUS modbus ;
extern vu8 serial_receive_timeout_count ;
#ifdef TSTAT_ZIGBEE 
extern vu8 serial_receive_timeout_countB ;
#endif
void dealwithData(void) ;
#ifdef TSTAT_ZIGBEE 
void dealwithDataB(void) ;
#endif
//void send_byte(u8 ch, u8 crc) ;
//#ifdef TSTAT_ZIGBEE 
//void send_byteB(u8 ch, u8 crc) ;
//#endif
void USART_SendDataString(u16 num) ;
#ifdef TSTAT_ZIGBEE  
void USART_SendDataStringB(u16 num) ;
#endif 

#ifdef TSTAT_CO2  
void USART_SendDataStringC(u16 num) ;
#endif //TSTAT_CO2

extern u8 uart_send[USART_SEND_LEN] ;
#ifdef TSTAT_ZIGBEE 
extern u8 uart_sendB[USART_SEND_LEN] ;
#endif 
extern u8 Station_NUM;
#endif//TSTAT_ARM

#endif
