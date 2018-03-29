#ifndef _DEFINE_H
#define _DEFINE_H

#define DEFAULT_FILTER   5
#define int16		signed short int
#define TSTAT_ZIGBEE
//#define TSTAT8_HUNTER
#define TSTAT_CO2
//#define TSTAT_OCC
//#define OCCTEST 

//#define COLOR_TEST
#ifdef TSTAT_ARM
//#define TSTAT7_ARM
//#define ZIGBEE_MODE_DEBUG

#include "types.h"
#include "stm32f10x.h"
#include "bitmap.h"


#define HW_VERSION   4

#define RS485_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
#define RS485_LED_OFF			GPIO_SetBits(GPIOE, GPIO_Pin_1);
#define RELAY1_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_2);
#define RELAY1_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_2);
#define RELAY2_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
#define RELAY2_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_3);
#define RELAY3_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_4);
#define RELAY3_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_4);
#define RELAY4_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_5);
#define RELAY4_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_5);
#define RELAY5_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_6);
#define RELAY5_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_6);

#define DO1_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_7);
#define DO1_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_7);
#define DO2_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
#define DO2_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_8);

#define AI1_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
#define AI1_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_9);
#define AI2_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_10);
#define AI2_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_10);
#define AI3_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_11);
#define AI3_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_11);
#define AI4_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
#define AI4_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_12);
#define AI5_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_13);
#define AI5_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_13);
#define AI6_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
#define AI6_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_14);
#define AI7_LED_ON			GPIO_ResetBits(GPIOE, GPIO_Pin_15);
#define AI7_LED_OFF		GPIO_SetBits(GPIOE, GPIO_Pin_15);





#ifdef TSTAT7_ARM
#define BK_ON    0
#define BK_OFF   1
#else
#define BK_ON    1 
#define BK_OFF   0
#endif


#define ICON_CONTROL_AUTO    0
#define ICON_CONTROL_MANUAL  1
#define ICON_CONTROL_OUTPUT  2

#define DAY_ICON     		0
#define NIGHT_ICON     	1
#define OCC_ICON     		2
#define UNOCC_ICON    	3
#define HEAT_ICON     	4
#define COOL_ICON     	5
#define FAN1_ICON     	6
#define FAN2_ICON     	7
#define FAN3_ICON     	8

#define ICON_OUTPUT1    1
#define ICON_OUTPUT2    2
#define ICON_OUTPUT3    3
#define ICON_OUTPUT4    4
#define ICON_OUTPUT5    5
#define ICON_OUTPUT6    6
#define ICON_OUTPUT7    7


#define ENABLE_FRC						1
#define DISABLE_FRC						0

#define FRC_STATUS_OK						0
#define FRC_STATUS_ERR					1

#define FRC_UPDATE_TIME				60

#define FRC_LOCAL_SENSOR  		0
#define FRC_REMOTE_SENSOR			1

#define FRC_TEMPERATURE_BASE   0
#define FRC_ENTHALPY_BASE   	 1

#define FRC_CONFIG_PASS        0
#define FRC_CONFIG_FAIL        1

#define FRC_OUTPUT_AO1			0
#define FRC_OUTPUT_AO2			1

#define FRC_MODE_NOT_OK					0
#define FRC_MODE_NO_FREECOOL		1
#define FRC_MODE_OK             2

#define FRC_FUNCTION_START			1
#define FRC_FUNCTION_NOT_START			0


#define FRC_SRC_AVERAGE_TEMPERATURE_INTERNAL_AI1	  	3
#define FRC_SRC_HUM_TEMPERATURE 4
#define FRC_SRC_AI1   			5
#define FRC_SRC_AI2  			 	6
#define FRC_SRC_AI3   			7
#define FRC_SRC_AI4   			8
#define FRC_SRC_AI5   			9
#define FRC_SRC_AI6   			10
#define FRC_SRC_AI7   			11
#define FRC_SRC_AI8   			12
#define FRC_SRC_INTERNAL   	2
#define FRC_SRC_REMOTETEM   20
#define FRC_SRC_HUMIDITY   	14
#define FRC_SRC_REMOTEHUM   21


#define RS485_ENABLE   	1
#define RS485_DISABLE   0

#define SENDUART1 0
#define SENDUART4 1

#define PIR_NOTTRIGGERED   0
#define PIR_TRIGGERED   1

#define OVERRIDE_NOTTRIGGER   0
#define OVERRIDE_TRIGGERED   1
#define OVERRIDE_WORKING   2

typedef struct
{
uint8 first_line[10];
uint8 second_line[10];	
}MENUNAME;

#ifdef TSTAT7_ARM
		enum GPIO_NUM{
		 UART_TX,				
		 UART_RX,				
		 RS485_TXEN,	
		 EEP_SCL,				
		 EEP_SDA,				
		 EEP_WP,				
		 KEYPAD_HIGH,
		 KEY1,
		 KEY2,	
		 KEY3,
		 KEY4, 
		 KEY5,
		 KEY6,
		 INPUT_TYPE0,
		 INPUT_TYPE1,
		 CHSEL0,
		 CHSEL1,
		 CHSEL2,
		 AI_MODE_SEL,
		 GPIO_DEFINE_END	
		}; 
	#else


//06 06 00 0A 06 08 B5 6E 01 00
enum GPIO_NUM{
 UART_TX,				
 UART_RX,				
 RS485_TXEN,	
 EEP_SCL,				
 EEP_SDA,				
 EEP_WP,				
 KEYPAD_HIGH,
 KEY1,
 KEY2,	
 KEY3,
 KEY4,
 INPUT_TYPE0,
 INPUT_TYPE1,
 CHSEL0,
 CHSEL1,
 CHSEL2,
 AI_MODE_SEL,
 GPIO_DEFINE_END	
};
#endif
#define MAX_AI_CHANNEL	 8

#define SERIALDEALTIME_DELAY		10
#define SCHEDULE_ON 	1
#define SCHEDULE_OFF  0

#define  RELAY1 GPIO_Pin_6
#define  RELAY2 GPIO_Pin_7
//#ifdef TSTAT_OCC
//#define  RELAY3 GPIO_Pin_4
//#define  RELAY4 GPIO_Pin_5
//#else
//#define  RELAY3 GPIO_Pin_8
//#define  RELAY4 GPIO_Pin_9
//#endif
#define  RELAY5 GPIO_Pin_12

#define  RELAY_ON			1
#define  RELAY_OFF		0

//Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode
#define AI_FUNCTION_NORMAL							0
#define AI_FUNCTION_FREEZEPROTECT				1
#define AI_FUNCTION_OCCSENSOR						2
#define AI_FUNCTION_SWEEPOFF						3
#define AI_FUNCTION_CLOCK								4
#define AI_FUNCTION_CHANGEOVER					5
#define AI_FUNCTION_OUTTEM							6

#define BLINK_ON        1
#define BLINK_OFF       0

#define HC_CFG_AUTO			0
#define HC_CFG_COOL			1
#define HC_CFG_HEAT			2


#define HC_CTL_USER			0
#define HC_CTL_AI1			1
#define HC_CTL_AI2			2
#define HC_CTL_AI3			3
#define HC_CTL_AI4			4
#define HC_CTL_AI5			5
#define HC_CTL_AI6			6
#define HC_CTL_AI7			7
#define HC_CTL_AI8			8

#define CH_HEIGHT													36

#define THERM_METER_POS										5

#define SETPOINT_POS											108
#define FAN_MODE_POS											SETPOINT_POS+CH_HEIGHT+7
#define SYS_MODE_POS											FAN_MODE_POS+CH_HEIGHT+7

#define MENU_ITEM1      SETPOINT_POS+0
#define MENU_ITEM2      FAN_MODE_POS+0

#define TIME_POS										      SYS_MODE_POS + CH_HEIGHT + 7
#define MENU_ITEM_POS											130
#define MENU_VALUE_POS										SYS_MODE_POS//MENU_ITEM_POS + CH_HEIGHT
#define ICON_POS													274
#define ICON_XPOS 												2

#define FIRST_ICON_POS									  ICON_XPOS
#define SECOND_ICON_POS                   FIRST_ICON_POS + 60
#define THIRD_ICON_POS                    SECOND_ICON_POS + 60
#define FOURTH_ICON_POS                   THIRD_ICON_POS + 60
#define FIFTH_ICON_POS                    FOURTH_ICON_POS + 40

#define ICON_XDOTS						55
#define ICON_YDOTS						45

#define FANBLADE_XDOTS						40
#define FANBLADE_YDOTS						45

#define FANSPEED_XDOTS						15
#define FANSPEED_YDOTS						45

#define THERM_METER_XPOS									39
#define TEMP_FIRST_BLANK						      0//30  //+= blank width
#define FIRST_CH_POS											TEMP_FIRST_BLANK + THERM_METER_XPOS
#define SECOND_CH_POS											FIRST_CH_POS+48
#define THIRD_CH_POS											SECOND_CH_POS+48+16
#define UNIT_POS													THIRD_CH_POS + 48+ 15
#define BUTTON_DARK_COLOR   							0X0BA7
#define BTN_OFFSET												CH_HEIGHT+7


#define TOP_AREA_DISP_ITEM_TEMPERATURE   	0
#define TOP_AREA_DISP_ITEM_HUM					 	1
#define TOP_AREA_DISP_ITEM_CO2				   	2

#define TOP_AREA_DISP_UNIT_C   					 	0
#define TOP_AREA_DISP_UNIT_F					 	 	1
#define TOP_AREA_DISP_UNIT_PPM				   	2
#define TOP_AREA_DISP_UNIT_PERCENT			 	3

#ifdef COLOR_TEST
#define TSTAT8_BACK_COLOR1  0x9df5//0xc67b// 0x7557 //0x2589
extern uint32 TSTAT8_BACK_COLOR;   //
extern uint32 TSTAT8_MENU_COLOR2;
extern uint32 TANGLE_COLOR;
#else
#define TSTAT8_BACK_COLOR1  0x7E19
#define TSTAT8_BACK_COLOR   0x7E19//
#define TSTAT8_MENU_COLOR2  0x7e17
#define TANGLE_COLOR        0xbe9c
#endif

#define TSTAT8_CH_COLOR   	0xffff //0xd6e0
#define TSTAT8_MENU_COLOR   0x7e17//0x3bef//0x43f2//0x14a9


#define SCH_COLOR  0xffff//0XB73F
#define SCH_BACK_COLOR  0x3bef//0x43f2//0x14E9

#define PRODUCT_ID 				9//TSTAT8
#define HW_VER						16
#define HIGH_LEVEL  					1
#define LOW_LEVEL							0 

#define BAUDRATE_9600						0
#define BAUDRATE_19200					1
#define BAUDRATE_38400          2
#define BAUDRATE_57600					3
#define BAUDRATE_115200					4
#define BAUDRATE_76800					5
#define BAUDRATE_1200						6
#define BAUDRATE_4800						7
#define BAUDRATE_14400					8

#define BAC_BAUDRATE_9600						96
#define BAC_BAUDRATE_19200					192
#define BAC_BAUDRATE_38400          384
#define BAC_BAUDRATE_57600					576
#define BAC_BAUDRATE_115200					1152
#define BAC_BAUDRATE_76800					768
#define BAC_BAUDRATE_1200						12
#define BAC_BAUDRATE_4800						48
#define BAC_BAUDRATE_14400					144

#define STM32F10X

#ifdef STM32F10X
#define far 
#define xdata
#define idata
#endif



//#define T322AI		80
//#define T38AI8AO6DO   81
//#define T39AO13DO	82
//#define T311AI11AO	83

#define SW_VER 20150112

//#ifdef T322AI 
//#define PRODUCT_ID 		9//T322AI
//#define HW_VER			8
////#define MAX_AI_CHANNEL	 22
//#define MAX_AIS         MAX_AI_CHANNEL
//#define MAX_INS  MAX_AIS 
//#define MAX_OUTS				1

////IO操作函数	 
//#define CHA_SEL0				PAout(0)	
//#define CHA_SEL1				PAout(1)		 
//#define CHA_SEL2				PAout(2)		
//#define CHA_SEL3				PAout(3)		
//#define CHA_SEL4				PAout(4)

//#define RANGE_SET0			PCout(8)
//#define RANGE_SET1			PCout(9)
//#endif 

//#ifdef T38AI8AO6DO 
//#define PRODUCT_ID 		12
//#define HW_VER			8
//#define MAX_AI_CHANNEL	 22
//#define MAX_AIS         MAX_AI_CHANNEL
//#define MAX_INS  				MAX_AIS 
//#define MAX_OUTS				1

////IO操作函数	 
//#define CHA_SEL0				PAout(0)	//SCL
//#define CHA_SEL1				PAout(1)	//SDA	 
//#define CHA_SEL2				PAout(2)		//输入SDA
//#define CHA_SEL3				PAout(3)		//输入SDA
//#define CHA_SEL4				PAout(4)

//#define RANGE_SET0			PCout(8)
//#define RANGE_SET1			PCout(9)



//#define PRODUCT_ID 		T38AI8AO6DO
//#define HW_VER			8
//#define MAX_AI_CHANNEL	 8
//#define MAX_AIS         MAX_AI_CHANNEL
//#define MAX_INS  MAX_AIS

//#define MAX_AOS					8
//#define MAX_DOS					6 
//#define MAX_OUTS				(MAX_AOS+MAX_DOS)


//IO操作函数	 
//#define CHA_SEL0				PCout(0)		//		PIN15
//#define CHA_SEL1				PCout(1)	//	 		PIN16
//#define CHA_SEL2				PCout(2)		//	PIN13



//#define CHA_SEL4				PAout(7)
//#define RANGE_SET0			PAout(4)
//#define RANGE_SET1			PAout(5)
//#endif 


typedef struct {
uint16 hm;
uint8 tm[2];
}SCHEDULE_TIME;

typedef struct  {
uint8 location;
uint8 name[9];
}DISPTEMP;

#define LCD_CS		PDout(0) //PD0
#define LCD_SCL		PDout(1) //PD1
#define LCD_SDA		PDout(3) ///PD3
#define LCD_RES		PDout(15)// PD15

#ifdef TSTAT_OCC
#define LCD_RS		PBout(6) //PD2
#else
#define LCD_RS		PDout(2) //PD2
#endif
 //struct str_time time;

#endif //TSTAT_ARM

/**********************************************************************************************/
/**********************************************************************************************/					
/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/	

#ifndef TSTAT_ARM
typedef signed long      int32;
typedef unsigned long		uint32;

typedef signed int       	int16;
typedef unsigned int		uint16;

typedef signed char			int8;
typedef unsigned char		uint8;
typedef bit					bool;


#define GOAL_CPU  //brings in header files for the goal cpu

#ifdef GOAL_CPU
	#include <SM59128_C.H>
#else
	#include <REG54.H>
	sfr WDTC = 0x8F;
#endif
#include "modbus.h"
#include <Intrins.h>


typedef struct DISPTEMP {
			 		uint8 location;
					uint8 name[9];
					};

#define NULL     0					
sbit RS232STATE = P1 ^ 2;

typedef union time{
uint16 hm;
uint8 tm[2];
};

#endif //TSTAT_ARM
/**********************************************************************************************/
/**********************************************************************************************/					
/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/					
//#define DEBUG
//#define EEPWRITEERROR
//#define INFOTEST
//#define CRCRECHECK
//#define TDSTEST
//#define FLOATINGTEST
//#define 	REVS_T
//#define 	DIGITAL
//#define 	ANALOG
//#define   RUNAR
//#define 	CONSTCODE
//#define 	CCDEBUG
//#define  	OCC_TEST
//#define  	WIRELESS_PIR
//#define  	COM_DEBUG
//#define 	STAGEDEBUG
//#define 	COMDEBUG
//#define 	ZIGBEE
//#define 	IDDEBUG
//#define 	CALTEST

#define SET_YEAR				0
#define SET_MONTH				1
#define SET_WEEK				2
#define SET_DAY					3
#define SET_HOUR				4
#define SET_MINUTE			5
#define SET_SECOND			6



#define DISP_COST_MODE   0
#define DISP_COOL_MODE   1
#define DISP_HEAT_MODE   2


#define SPLIMIT			0
#define SPUNLIMIT			1

#define THERMISTER_RANGE  18
#define THERMISTER_TYPE2  2
#define THERMISTER_TYPE3  19
#define THERMISTER_TYPE50K 20

#define SN_MOVE      0
#define SN_BACKUP    1
#define SN_RECOVERY  2

#ifdef 	ZIGBEE
#define  COMMUNICATION_MODE 		1
#else
#define  COMMUNICATION_MODE 		0
#endif //ZIGBEE

//define analog input range here
#define SEL_AI1														0
#define SEL_AI2														1
#define SEL_AI3														2
#define SEL_AI4														3
#define SEL_AI5														4
#define SEL_AI6														5
#define SEL_AI7														6
#define SEL_AI8														7

#define AI_RANGE_UNUSED										0
#define AI_RANGE_10K_THERMISTOR_TYPE2			1
#define AI_RANGE_PERCNETAGE								2
#define AI_RANGE_ON_OFF										3
#define AI_RANGE_USER											4
#define AI_RANGE_OFF_ON										5
#define AI_RANGE_CUSTOMER_SENSOR			  	6		
#define AI_RANGE_OPEN_CLOSE								7
#define AI_RANGE_CLOSE_OPEN								8
#define AI_RANGE_OCCUPIED_UNOCCUPIED		  9
#define AI_RANGE_UNOCCUPIED_OCCUPIED			10
#define AI_RANGE_10K_THERMISTOR_TYPE3			11
#define AI_RANGE_MA												12
#define AI_RANGE_50K_THERMISTOR						13
#define AI_RANGE_VOLTAGE_5V      					14

#define AI_RANGE_UNUSED_10V								128	
#define AI_RANGE_PERCNETAGE_10V					  130							
#define AI_RANGE_ON_OFF_10V								131		
#define AI_RANGE_USER_10V									132	
#define AI_RANGE_OFF_ON_10V							  133		
#define AI_RANGE_CUSTOMER_SENSOR_10V			134  		
#define AI_RANGE_OPEN_CLOSE_10V						135		
#define AI_RANGE_CLOSE_OPEN_10V						136		
#define AI_RANGE_OCCUPIED_UNOCCUPIED_10V	137  
#define AI_RANGE_UNOCCUPIED_OCCUPIED_10V	138
#define AI_RANGE_VOLTAGE_10V              139

// 	#define DEBUGCO2 
//  #define DEBUGSCAN //for solving the serial number error 
// 	#define  ALLINCO2
//  #define  ROBERT
//	#define HUM_CALIBRATION_ENABLE
	#define TSTAT6_CODE
//	#define ZIGBEE

	#define  T5E_NEW
	#define  pic_ver_14_up
	#define  RTC8563
	#define  VAV
  

	
	
	#define TEM_COMPATIBLE_AI1						1
	#define TEMP_INTERNAL_TEMPERATURE	  	2
	#define TEMP_AVERAGE_TEMPERATURE_INTERNAL_AI1	  	3
	#define TEMP_HUM_TEMPERATURE	  	4
	#define TEMP_AI1	  				5
	#define TEMP_AI2	  				6
	#define TEMP_AI3	  				7
	#define TEMP_AI4	  				8
	#define TEMP_AI5	  	    	9
	#define TEMP_AI6	  				10
	#define TEMP_AI7	  				11
	#define TEMP_AI8	  				12
	#define TEMP_CO2	  				13
	#define TEMP_HUM	  				14
	#define TEMP_AIRFLOW_SENSOR	  	    15
	#define TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI2	  	16
	#define TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI3	  	17
	#define TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI4	  	18


	#define MANUAL_ICON_DISABLE     0
	#define MANUAL_ICON_ENABLE    	1

  #define NOT_IGNORE 			0
  #define IGNORE 				1
	#define ANALOG_OUTPUT6_TRANSDUCER   6
	#define ANALOG_OUTPUT7_TRANSDUCER   7
	
	#define TRANSDUCER_INTERNAL		5  
	#define TRANSDUCER_SETPOINT     6
	#define TRANSDUCER_AI1		    7
	#define TRANSDUCER_AI2     		8
	#define TRANSDUCER_AI3     		9
	#define TRANSDUCER_AI4     		10
	#define TRANSDUCER_AI5     		11
	#define TRANSDUCER_AI6     		12
	#define TRANSDUCER_AI7     		13
	#define TRANSDUCER_AI8     		14
	#define TRANSDUCER_HUM     		15
	#define TRANSDUCER_CO2     		16
//	#define TRANSDUCER_INTERNAL_AI1 17
//	#define TRANSDUCER_AI1_TO_AI2   18
//	#define TRANSDUCER_AI1_TO_AI3   19
//	#define TRANSDUCER_AI1_TO_AI4   20




	#define ID_ENABLE 			1
	#define ID_DISABLE			0
	#define DISABLE_UART		0
	#define ENABLE_UART			1

	#define CLOCK_PRESENT		1
	#define HUMIDITY_PRESENT	2
	#define CO2_PRESENT	   		3
	#define CO_PRESENT	    	4
	#define SPARE_PRESENT	    5

	#define FLASH_OK     		0
	#define EEPROM_OK			1 

	#define NEED_TO_UNLOCK  1
	#define NEED_TO_LOCK  	0
	#define NO_PID	  		9
	#define PID_LOOP1	  	0
	#define PID_LOOP2	  	1
	#define PID_LOOP3	  	2

	//these items indicate which input will be used for PID loop

	#define PID_INTERNAL_TEMPERATURE   	2
	#define PID_AVERAGE_TEMPERATURE	  	3
	#define PID_HUM_TEMPERATURE		    4
	#define PID_AI1						5
	#define PID_AI2						6
	#define PID_AI3						7
	#define PID_AI4						8
	#define PID_AI5						9
	#define PID_AI6						10
	#define PID_AI7						11
	#define PID_AI8						12
	#define PID_CO2						13
	#define PID_HUM						14
	#define PID_AIRFLOW_SENSOR			15

	//define display unit
	#define DISP_UNIT_TEM				0
	#define DISP_UNIT_PPM				1
	#define DISP_UNIT_PERCENT			2
	#define DISP_UNIT_V					3
	#define DISP_UNIT_MA				4

	//indicate which setpoint wll be shown as current setpoint 
	#define DISPLAY_TEM_SP   0
	#define DISPLAY_CO2_SP	 1
	#define DISPLAY_HUM_SP	 2

	#define DAY_MODE  		1
	#define NIGHT_MODE		0
	#define WRITE_DISABLE    99 
	#define RS485_WRITE_DISABLE  0
	#define RS485_WRITE_ENABLE  1  
	#define DATA_LEN 6
	#ifdef SANJAY
	#define ITERM_DEFAULT   10
	#define TEM_SENSOR 1
	#define DEFAULT_SOP 1
	
	#else
	#define TEM_SENSOR 2
	#define DEFAULT_SOP 1
	#endif

	#define MAX_DUAL_VAV_ENABLE	    	1
	#define MAX_DUAL_VAV_DISABLE 		0 
	#define INPUT_VAV				    7

	#define FLOATING_COOLING			1
	#define FLOATING_HEATING			2
	#define OUTPUT_NO_FLOATING 			0
	#define OUTPUT_FLOATING				1

#ifdef TSTAT7_ARM
	#define MAX_INPUT_CHANNEL			4
#else
	#define MAX_INPUT_CHANNEL			8
#endif	
	#define   CMD_TEMP_CAL         		0xe5
	#define   CMD_HUM_CAL            	0xe6
	#define   CMD_HEATING_CONTROL      	0xd5
	#define   CMD_CLEAR_TABLE         	0xe4

	#define OUTPUT_FUNCTION_ON_OFF			0
	#define OUTPUT_FUNCTION_ROTATING  		1
	#define OUTPUT_FUNCTION_LIGHTING      	2
	#define OUTPUT_FUNCTION_VAV      	    3


//	#define OUTPUT_RANGE_ON_OFF				1
	#define OUTPUT_RANGE_PWM      			3
	#define OUTPUT_RANGE_TRANSDUCER			4

	#define OUTPUT_ANALOG_RANGE_ONOFF					0
	#define OUTPUT_ANALOG_RANGE_0TO10V				1
    #define OUTPUT_ANALOG_RANGE_PWM         17 

	#define ID_WRITE_ENABLE      0
	#define ID_WRITE_DISABLE     1
//	#define MINUTE 	1
//	#define HOUR 	0

	#define CLOCK_ITEM_YEAR	      0
	#define CLOCK_ITEM_MONTH	  1
	#define CLOCK_ITEM_DAY  	  2
	#define CLOCK_ITEM_WEEK		  3
	#define CLOCK_ITEM_HOUR	      4
	#define CLOCK_ITEM_MINUTE	  5
	#define CLOCK_ITEM_SECOND	  6

	#define SCHEDULE_ITEM_WORKDAY_DAY_HOUR     	0
	#define SCHEDULE_ITEM_WORKDAY_DAY_MINUTE   	1
	#define SCHEDULE_ITEM_WORKDAY_NIGHT_HOUR  	2
	#define SCHEDULE_ITEM_WORKDAY_NIGHT_MINUTE  3
	#define SCHEDULE_ITEM_WEEKEND_DAY_HOUR  	4
	#define SCHEDULE_ITEM_WEEKEND_DAY_MINUTE  	5
	#define SCHEDULE_ITEM_WEEKEND_NIGHT_HOUR  	6
	#define SCHEDULE_ITEM_WEEKEND_NIGHT_MINUTE  7
	
	#define AI8   					0
	#define NO_PIR_SENSOR 			0
	#define INTERNAL_PIR_SENSOR		1
	#define WIRELESS_PIR_SENSOR		2

	/////////////////////////////// Useful defines
	#define ZIGBEE_ENABLE   0
	#define WORK_MODE_AUTO  1
	#define WORK_MODE_COOLING  2
	#define WORK_MODE_HEATING  3

//	#define RELAY_ON_TO_OFF    1
//	#define RELAY_OFF_TO_ON    0 

//	#define DISPLAY_NUMBER_LCD
	#define SELECT_ANALOG_IN1     	1
	#define SELECT_ANALOG_IN2     	2
	#define SELECT_HUM     			3
	#define SELECT_CO2     			4 
	#define TENBITS        			10
	#define SIXTEENBITS    			16
	#define HUM_EXIST      			1
	#define HUM_NOT_EXIST  			0
	#define CLOCK_EXIST    			1
	#define CLOCK_NOT_EXIST   		0
	#define FACTORY        			0

 	#define TEMP_INFO	 		1
	#define USER_INFO	 		12
	#define RTC_INFO_DATE     	13
	#define RTC_INFO_TIME     	14
	#define DISP_CO2			15
	#define DISP_HUM			16

#define 	MAX_MENU        EEP_FACTORY_DEFAULTS  
#define 	MIN_MENU        EEP_MENU_ITEM_BEGINNING
#define 	DEV_ADDR     		0x80
#define 	I2C_WR       		0x00
#define 	I2C_RD       		0x01
#define   CMD_READ_PARAMS            0xc1
#define   CMD_READ_VERSION         0xc2
#define   CMD_OVERRIDE_CAL         0xd3
#define   CMD_READ_CAL_PT            0xd4
#define   CMD_HEATING_CONTROL         0xd5
#define   CMD_CLEAR_TABLE            0xe4
#define   CMD_TEMP_CAL            0xe5
#define   CMD_HUM_CAL               0xe6
#define CMD_TRIGGER_TEMP_HOLD_MASTER    0xe3
#define CMD_TRIGGER_HUM_HOLD_MASTER     0xe5
#define CMD_TRIGGER_TEMP_NO_HOLD_MASTER 0xf3
#define CMD_TRIGGER_HUM_NO_HOLD_MASTER  0xf5
#define CMD_WRITE_USER_REGISTER         0xe6
#define CMD_READ_USER_REGISTER          0xe7
#define CMD_SOFT_RESET                  0xfe
#define SEL_TEMP                 0x00
#define SEL_HUM                  0x02
	
    #define OLD   0
	#define NEW   1	
	#define FALSE 0
	#define TRUE 1
//cc there is no gridconfig cunftion any more, so block this	
//	#define EEPROM_OFFSET			800//divide the e2chip into two  parts ,one for normal config another for gridpoint config 
	
	#define TOTAL_EEP_SIZE 			2047
	
//	#define MANUAL_COOL_VALVE_HI	TOTAL_EEP_SIZE - 5
//	#define MANUAL_COOL_VALVE_LO	TOTAL_EEP_SIZE - 6
//	#define MANUAL_HEAT_VALVE_HI	TOTAL_EEP_SIZE - 7
//	#define MANUAL_HEAT_VALVE_LO	TOTAL_EEP_SIZE - 8
//	#define MANUAL_RELAY	   		TOTAL_EEP_SIZE - 9
//	#define LIGHTING_STAGE	   		TOTAL_EEP_SIZE - 10
//	
//	#define EEP_SERINALNUMBER_WRITE_FLAG  TOTAL_EEP_SIZE - 11	
//	
//	#define MANUAL_VALVE   		TOTAL_EEP_SIZE - 20
//	
//	#define MANUAL_ANALOG1_HI	   	TOTAL_EEP_SIZE - 21
//	#define MANUAL_ANALOG1_LO	   	TOTAL_EEP_SIZE - 22
//	#define MANUAL_ANALOG2_HI	   	TOTAL_EEP_SIZE - 23
//	#define MANUAL_ANALOG2_LO	   	TOTAL_EEP_SIZE - 24
//	#define MANUAL_ANALOG3_HI	   	TOTAL_EEP_SIZE - 25
//	#define MANUAL_ANALOG3_LO	   	TOTAL_EEP_SIZE - 26
//	#define MANUAL_ANALOG4_HI	   	TOTAL_EEP_SIZE - 27
//	#define MANUAL_ANALOG4_LO	   	TOTAL_EEP_SIZE - 28
//	#define MANUAL_ANALOG5_HI	   	TOTAL_EEP_SIZE - 29
//	#define MANUAL_ANALOG5_LO	   	TOTAL_EEP_SIZE - 30
//	#define MANUAL_ANALOG6_HI	   	TOTAL_EEP_SIZE - 31
//	#define MANUAL_ANALOG6_LO	   	TOTAL_EEP_SIZE - 32
//	#define MANUAL_ANALOG7_HI	   	TOTAL_EEP_SIZE - 33
//	#define MANUAL_ANALOG7_LO	   	TOTAL_EEP_SIZE - 34
//	#define MANUAL_ANALOG8_HI	   	TOTAL_EEP_SIZE - 35
//	#define MANUAL_ANALOG8_LO	   	TOTAL_EEP_SIZE - 36
//	#define MANUAL_INTERNAL_HI	   	TOTAL_EEP_SIZE - 37
//	#define MANUAL_INTERNAL_LO	   	TOTAL_EEP_SIZE - 38

//	#define RS485_WRITEFLAG     	TOTAL_EEP_SIZE - 39	

	//mode for gridpoint, changed by keypad ,cycle from OFF To emer
	#define OFF						0
//	#define RELAY_ON            1
	#define COOL					1	
	#define HEAT					2	
	#define EMER					3


	#define AUTO					4
	
	
	#define ORIGINALADDRESSVALUE	100 
	// starting point to where to read in the flash for data transfer
	// RL 3/11/04
	#define DATA_TO_ISP						60928	// 0xEE00 = 60928
	#define DATA_TO_FIRMWARE				61184	// 0xEF00 = 61184
	
	#define DATA_FAC_DEFAULT                0xEC00  // 512bytes from 0xec00 are used for strore FAC default what is sent by *.TXT
	
	#define DATA_FAC_FLAG55                 DATA_FAC_DEFAULT 
	#define DATA_FAC_FLAGAA                 DATA_FAC_DEFAULT + 1
	#define DATA_FAC_START                  DATA_FAC_DEFAULT + 2
	
	
	#define NO 0
	#define YES 1
	
	
	#define BINARY_OUTPUT	0	  //RANGES FOR OUTPUTS
	#define FLOATING  1
	#define PWM    2
	
	//TBD: RANGES FOR INPUTS
	
	
	#define RELAY_ON_TO_OFF    1
	#define RELAY_OFF_TO_ON    0 
	#define NO_ANALOG	1
	#define NO_PWM		1
	#define PWM5D		2		
	#define PWM5A		3
	#define PWM_AQ		4
//	#define TSTAT6		6  //CC ADD 07212011
//	#define TSTAT7		7  //CC ADD 07212011
	
	
	
	// The event queue is only for button pushes, not timers
	#define EVENT_QUEUE_SIZE 3      //maximum of three events can be ready
	
	#define DEFAULT_TIMEOUT 0
	
	#define MENU_TIMEOUT 0
	#define UNOCC_COOL_SETPOINT 400 // x 10 deg C  MDOCT21 x 10 value
	#define UNOCCUPIED 0
	#define PIR_OCCUPIED       1
	#define PIR_UNOCCUPIED     0
	#define  DAY_FLAG    	 0x01
	#define  NIGHT_FLAG      0x02
	#define  TEMP_UP_FLAG  	 0x04
	#define  TEMP_DOWN_FLAG  0x08
	#define  DAY_KEY_FLAG    0x10
	#define  NIGHT_KEY_FLAG  0x20
	
	#define LIGHTON  0
	#define LIGHTOFF 1
	
	#define TOTALREFRESHNUM		4            // 3 LED + 1 EIGHT LIGHT
	
	#define MAX_TEMP 3000  //maximum temp to display        
	#define MIN_TEMP  -500   //minimum temp to display
	
	
	#define RELAYON		1
	#define RELAYOFF	0
	
	#define KEYPAD_2A		0
	#define KEYPAD_2B		5
	#define KEYPAD_4A		1
	#define KEYPAD_4B		4
	#define KEYPAD_6A		2
	#define KEYPAD_6B		3
	#define KEYPAD_2C		6
	#define KEYPAD_6C		7
	#define KEYPAD_4C	    8


	#ifdef CONFIGURE_TOOL
		enum
		{
		RECEIVE_PC_DATA,
		RECEIVE_TSTAT_DATA  	 
		};	
	#endif

	enum
	{
	DEGC  = 0,
	DEGF
	};
	
	
	enum
	{
	DEGREE_F,
	DEGREE_C, 
	E2E,
	VER,
	SRS,
	BUT_6A,
	BUT_6B,
	BUT_4B,
	BUT_2B,
	DIS_NO,
	DIS_YES,
	DIS_OFF,
	ddE,
	AdE,
	bdE,
	BUT_2C,
	BUT_6C
	};
	
	
	enum
	{
	HC_AUTOMATIC,
	HC_KEYPAD, 
	HC_DI_ACTIVE_HIGH,
	HC_DI_ACTIVE_LOW,
	HC_ANALOG_IN1,
	HC_ANALOG_IN2
	};
	#ifndef TSTAT7_ARM
	enum
	{
	//	BACK = 1,
	WALL =1,
	CEILING, 
	OUTDOOR,
	AVERAGE,	 
	NIGHT_HEAT,
	NIGHT_COOL,	 
	RHset, 
	FAN,
	DAY,		//8
	//	EVENT,
	S_TIME1,
	S_MINUTE1,
	S_TIME2,
	S_MINUTE2,
	S_TIME3,
	S_MINUTE3,
	S_TIME4,
	S_MINUTE4,
	//	ACTION,
	TSTAT_UNITS,		//17
	RTC_TIME_FORMAT,
	//	HEAT_STAGES,
	//	COOL_STAGES,
	NET_ID,
	CAL,
	SOP,
	BACK
	};

	#endif
	
	enum
	{
	NONE,						//00
	RELAY1_LED_CONTROL,			//01
	RELAY2_LED_CONTROL,			//02
	RELAY3_LED_CONTROL,			//03
	RELAY4_LED_CONTROL,			//04
	RELAY5_LED_CONTROL,			//05
	COASTING_LED_CONTROL,		//06
	COOLING1_LED_CONTROL,		//07
	COOLING2_LED_CONTROL,		//08
	COOLING3_LED_CONTROL,		//09
	HEATING1_LED_CONTROL,		//10
	HEATING2_LED_CONTROL,		//11
	HEATING3_LED_CONTROL,		//12
	COOLING_MODE_LED_CONTROL,	//13
	HEATING_MODE_LED_CONTROL,	//14
	FANOFF_LED_CONTROL,			//15
	FANLOW_LED_CONTROL,			//16
	FANMED_LED_CONTROL,			//17
	FANHI_LED_CONTROL,			//18
	FANAUTO_LED_CONTROL,		//19
	HEAT1_OR_COOL1_CONTROL,		//20
	HEAT2_OR_COOL2_CONTROL,		//21
	HEAT3_OR_COOL3_CONTROL,		//22
	COOL1_2_OR_3_CONTROL,		//23
	HEAT1_2_OR_3_CONTROL,		//24
	OCCUPANCY_CONTROL,			//25
	
	COOLorHEAT_1_2_3,			//26
	COOLorHEAT_2_3,				//27
	FAN_GRID,					//28
	UTIL,						//29
	HOLD,						//30
	MODE_SWITCH,				//31
	//	OFF_GRID,					//32
	//	COOL_GRID,					//33
	//	HEAT_GRID,					//34
	
	TOTAL_LED_CONTROL_STATES	//
	};

	typedef /*idata*/ enum
	{
	UPDATE_TEMPERATURE , 	//01
	SHOW_TEMPERATURE ,           //02
	CONTROL_LOGIC ,       //03
	UPDATE_PID,        	//04 update the PID controllers
	REFRESH_VALVE_POSITION,
	REFRESH_OUTPUTS ,     //05
	BLINK_FLIP ,          //06  flip the display on and off during blinking mode
	ACCEPT_NEW_SETTING ,	//07
	GRIDPOINT_MODE_SWITCH,//the defalt decrease setpoint button be set mode switch in gridpoint tstat
	INCREASE_COOLING_SETPOINT , //9  used for cooling setpt only
	DECREASE_COOLING_SETPOINT , //10
	INCREASE_HEATING_SETPOINT , //OCCUPIED_UNOCCUPIED_SWITCH , //11 ///cc change this for clock//  
	DECREASE_HEATING_SETPOINT ,//SELECT_FUNCTION, //12 ///cc change this for clock  //	
	INCREASE_FAN_SPEED , //13
	#ifdef TSTAT7_ARM
	DECREASE_FAN_SPEED , //14
	#else
	INCREASE_SYS_MODE,//	DECREASE_FAN_SPEED , //14
	#endif
	START_MENU_MODE ,//17
	EXIT_MENU_MODE ,//18
	START_NEXT_MENU , //19
	START_PREVIOUS_MENU , //20	
	INCREASE_PARAMETER , //22
	DECREASE_PARAMETER ,//23
	ACCEPT_PARAMETER ,          //24 OK key 
	ACCEPT_EXIT_PARAMETER ,         //25 ESC key 
	INIT_CONTROL_LOGIC ,   //26 take care of the controller logic, set outputs etc.
	INIT_REFRESH_OUTPUTS ,   //27 refresh the capacitors on the analog outputs
	INIT_UPDATE_PID ,     //28
	LEDS_STATUS,//                   // 29 SHOW EIGHT LED STATUS
	COM_DEALWITH,		// 30	
	OUTPUT1_ROUTINE,
	OUTPUT2_ROUTINE,
	OUTPUT3_ROUTINE,
	OUTPUT4_ROUTINE,
	OUTPUT5_ROUTINE,
	OUTPUT6_ROUTINE, //add timer for analog output ON/OFF & PWM mode
	OUTPUT7_ROUTINE,	
	STATE_SWITCH_ROUTINE,
	COOLING_LOCKOUT_ROUTINE,
	HEATING_LOCKOUT_ROUTINE,
	OVERRIDE_TIMER_ROUTINE,
	OPEN_RELAY_ROUTINE,
	FREEZE_DELAY_ON_ROUTINE,
	ONE_SECOND_ROUTINE,		
	WRITE_CONFIGURE,
	TOTAL_JUMP_FUNCTIONS,//31 
	LCD_TURN_OFF_ROUTINE,
	LCD_SCREEN_ROTATION_ROUTINE,
	RUN_SCHEDULE, //cc add for schedule
	CLOCK_DATE_BLINK,
	CLOCK_TIME_BLINK,
	SCHEDULE_TIME_BLINK,
#ifdef TSTAT7_ARM
	SELECT_FUNCTION,
	OCCUPIED_UNOCCUPIED_SWITCH,
	CLOCK_MENU,
	NOTHING,
#endif
	
	} et_jump_function ;

/* -----------------------------------------------------
Event types
----------------------------------------------------- */
	
	typedef /*idata*/ enum
	{
	NULL_EVENT ,     	//0  no event pending
	COOL_UP_KEY ,    	//1 COOL_UP button pressed
	COOL_DOWN_KEY ,  	//2 COOL_DOWN button pressed
	HEAT_UP_KEY ,    	//3 HEAT_UP button pressed
	HEAT_DOWN_KEY ,  	//4 HEAT_DOWN button pressed
	FAN_UP_KEY  ,    	//5 FAN_UP button pressed
	FAN_DOWN_KEY ,   	//6 FAN_DOWN button pressed
	OCC_KEY  ,    	//7 OCCPUIED button pressed   //FUTURE: in certain sequences, let the keypad behave like this
	UNOCC_KEY ,   	//8 UNOCCPUIED button pressed //FUTURE: in certain sequences, let the keypad behave like this
	MENU_KEY ,    	//9 menu trapdoor 
	
	TEMP_UP_KEY ,	//10    //TBD: this should be COOL_UP_KEY
	TEMP_DOWN_KEY ,	//11   //TBD: this should be COOL_DOWN_KEY
#ifdef TSTAT7_ARM
	UP_KEY ,    		//1 UP button pressed
	DOWN_KEY ,  		//2 DOWN button pressed
	MODE_KEY ,    		//3 SELECT button pressed 
	ENTER_KEY  ,    	//5 MODE button pressed
	OCCU_KEY ,   		//6 OCCU/UNOCCU button pressed on pressed //FUTURE: in certain sequences, let the keypad behave like this
#endif		
  	TOTAL_EVENTS
	} et_event ;


	typedef /*idata*/ enum
	{
	TEMPERATURE_TIMER ,  	//1 update temperature reading
	TEMP_DISPLAY_TIMER ,    //2 update display
	CONTROL_LOGIC_TIMER ,   //3 do the control logic
	UPDATE_PID_TIMER , 		//4 Recalc I-term and do logic 
	REFRESH_VALVE_POSITION_TIMER,
	REFRESH_OUTPUTS_TIMER , //5 Refresh the analog outputs
	BLINK_TIMER,    		//6 for blinking display
	KEYPAD_ACTIVE_TIMER ,  	//7 for inactivity timeout
	LEDS_TIMER,             //9 FOR Eight Led Status
	COM_TIMER,				//10 FOR COM   to do dealwith
	OUTPUT1_TIMER,			//11
	OUTPUT2_TIMER,			//12
	OUTPUT3_TIMER,			//13
	OUTPUT4_TIMER,			//14
	OUTPUT5_TIMER,			//15
	OUTPUT6_TIMER,
	OUTPUT7_TIMER,
	STATE_SWITCH_TIMER,		//16
	COOLING_LOCKOUT_TIMER,	//17
	HEATING_LOCKOUT_TIMER,	//18
	OVERRIDE_TIMER,			//19
	OPEN_RELAY_TIMER,
	FREEZE_DELAY_ON_TIMER, 	
	ONE_SECOND_TIMER,	
	LCD_TURN_OFF_TIMER,
	LCD_SCREEN_ROTATION_TIMER,
	CHECK_SCHEDULE_TIMER,  ///cc add this  for clock 
	CLOCK_DATE_BLINK_TIMER,
	CLOCK_TIME_BLINK_TIMER,
	SCHEDULE_TIME_BLINK_TIMER,
//	COMMUNICATION_CHECK_TIMER,
	TOTAL_TIMERS  			//
	} et_timer ;
/* --------------Begin EEprom info -----------------------------
Menu types

	- CAUTION: This table must agree with menu_jump_tb in CONST.H

	- also used for eeprom address
--------------------------------------------------------------- */
	typedef enum 
	{ 
	DAYTIME_COASTING ,  // room is satisfied
	DAYTIME_COOLING1 ,  // daytime, room is hot
	DAYTIME_COOLING2 ,  // daytime, room is med hot
	DAYTIME_COOLING3 ,  // daytime, room is really hot
	DAYTIME_COOLING4 ,  // MHF:2005/4/11,added new stages to suit for variable cases
	DAYTIME_COOLING5 ,  	  // for example ,0 heating and 6 cooling,1 heating ang 5 cooling ...
	DAYTIME_COOLING6 ,
	DAYTIME_HEATING1 ,  // daytime, room is cool
	DAYTIME_HEATING2 ,  // daytime, room is cold
	DAYTIME_HEATING3 ,  // daytime, room is really cold  
	DAYTIME_HEATING4 ,
	DAYTIME_HEATING5 ,
	DAYTIME_HEATING6
	} et_mode_of_operation ;

// ----- item on the display-----------------------------------
	typedef /*idata*/ enum 
	{
	EEP_SERIALNUMBER_LOWORD,             
	
	EEP_SERIALNUMBER_HIWORD  = 2  ,
	
	EEPROM_VERSION_NUMBER_LO   = 4  ,
	EEPROM_VERSION_NUMBER_HI,
	EEP_ADDRESS,	      //6
	EEP_PRODUCT_MODEL,	  //7
	EEP_HARDWARE_REV,	  //8
	EEP_INFO_BYTE,		  //9
	EEPROM_TEST_ADDRESS, //10 
	EEP_UPDATE_STATUS ,		//11 reg 16 August Ron
	EEP_UPDATE_PTR_HI,			//12 reg 17 
	EEP_UPDATE_PTR_LO,			//13 reg 18  

	EEP_UNIVERSAL_HEAT_DB  = EEP_UPDATE_PTR_LO + 2 ,	 //15

	EEP_UNIVERSAL_COOL_DB = EEP_UNIVERSAL_HEAT_DB + 2 ,  //17	

	EEP_UNITS1_HIGH = EEP_UNIVERSAL_COOL_DB + 2 , //19                 //store the custom sensors's units

	EEP_UNITS1_LOW = EEP_UNITS1_HIGH + 2 , //21

	EEP_UNITS2_HIGH = EEP_UNITS1_LOW + 2 , //23                 //store the custom sensors's units

	EEP_UNITS2_LOW = EEP_UNITS2_HIGH + 2 , //25

	EEP_TABLE1_ZERO = EEP_UNITS2_LOW + 2 ,                                 //A changable look up table for "custom" sensor 

	EEP_TABLE1_HALFONE = EEP_TABLE1_ZERO + 2, //29             //the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval

	EEP_TABLE1_ONE = EEP_TABLE1_HALFONE + 2,               //the value's unit in the table is different according to different sensor

	EEP_TABLE1_HALFTWO = EEP_TABLE1_ONE + 2, //33                

	EEP_TABLE1_TWO = EEP_TABLE1_HALFTWO + 2,

	EEP_TABLE1_HALFTHREE = EEP_TABLE1_TWO + 2, //37

	EEP_TABLE1_THREE = EEP_TABLE1_HALFTHREE + 2,  //39

	EEP_TABLE1_HALFFOUR = EEP_TABLE1_THREE + 2,

	EEP_TABLE1_FOUR = EEP_TABLE1_HALFFOUR + 2, //43

	EEP_TABLE1_HALFFIVE = EEP_TABLE1_FOUR + 2,

	EEP_TABLE1_FIVE = EEP_TABLE1_HALFFIVE + 2, //47	

	EEP_TABLE2_ZERO = EEP_TABLE1_FIVE + 2,    //51              //A changable look up table for "custom" sensor 

	EEP_TABLE2_HALFONE = EEP_TABLE2_ZERO + 2,               //the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval

	EEP_TABLE2_ONE = EEP_TABLE2_HALFONE + 2,                //the value's unit in the table is different according to different sensor

	EEP_TABLE2_HALFTWO = EEP_TABLE2_ONE + 2,                 

	EEP_TABLE2_TWO = EEP_TABLE2_HALFTWO + 2,

	EEP_TABLE2_HALFTHREE = EEP_TABLE2_TWO + 2, //61

	EEP_TABLE2_THREE = EEP_TABLE2_HALFTHREE + 2,

	EEP_TABLE2_HALFFOUR = EEP_TABLE2_THREE + 2,

	EEP_TABLE2_FOUR = EEP_TABLE2_HALFFOUR + 2,

	EEP_TABLE2_HALFFIVE = EEP_TABLE2_FOUR + 2,

	EEP_TABLE2_FIVE = EEP_TABLE2_HALFFIVE + 2,//71			

	EEP_CALIBRATION_ANALOG1 = EEP_TABLE2_FIVE + 2,

	EEP_CALIBRATION_ANALOG2 = EEP_CALIBRATION_ANALOG1 + 2,

	EEP_CALIBRATION_ANALOG3 = EEP_CALIBRATION_ANALOG2 + 2,

	EEP_CALIBRATION_ANALOG4 = EEP_CALIBRATION_ANALOG3 + 2,

	EEP_CALIBRATION_ANALOG5 = EEP_CALIBRATION_ANALOG4 + 2, //81

	EEP_CALIBRATION_ANALOG6 = EEP_CALIBRATION_ANALOG5 + 2,

	EEP_CALIBRATION_ANALOG7 = EEP_CALIBRATION_ANALOG6 + 2,

	EEP_CALIBRATION_ANALOG8 = EEP_CALIBRATION_ANALOG7 + 2,	

	EEP_CALIBRATION_EXTERNAL = EEP_CALIBRATION_ANALOG8 + 2,

	EEP_CALIBRATION_ANALOG_IN2 = EEP_CALIBRATION_EXTERNAL + 2,

	EEP_CALIBRATION_INTERNAL_THERMISTOR = EEP_CALIBRATION_ANALOG_IN2 + 2,//91

	EEP_CALIBRATION = EEP_CALIBRATION_INTERNAL_THERMISTOR + 2,  // 00 thermistor calibration term 	
	EEP_MENU_ITEM_BEGINNING, //	EEP_CALIBRATION_EXTERNAL_HI = EEP_MENU_ITEM_BEGINNING, 	
	MAXEEPCONSTRANGE, 	EEP_TEMP_SELECT  = MAXEEPCONSTRANGE, 	// 	01 (ACCORDING TO DEFINE.C) temperature select, 0 internal, 1 external and 4 changeover sensor
	EEP_FILTER,   //2                 
	EEP_ANALOG_IN1,//3
	EEP_ANALOG_IN2,//4                        
	EEP_OVERRIDE_TIMER,//5
	EEP_DAC_OFFSET ,//6						//  DAC OFFSET
	EEP_BAUDRATE,      //7 					//  BAUD RATE	
	EEP_CYCLING_DELAY,//8
	EEP_CHANGOVER_DELAY, //9  					// 				
	EEP_COOLING_PTERM ,     //10				//  cooling P term
	EEP_COOLING_ITERM ,   					//  cooling I term  													 
	EEP_SEQUENCE ,  						//  sequence of operation , tstat behaves differently according to sequence
	EEP_HEAT_COOL_CONFIG,					//
  EEP_DAY_COOLING_DEADBAND ,    //  cooling deadband
  EEP_DAY_HEATING_DEADBAND ,   	// 09 heating deadband
	EEP_DEGC_OR_F ,    						// 	temperature units  0 = DegC, 1 = DegF 
	EEP_FAN_MODE ,  						//  auto. semi or manual fan operation
	EEP_NIGHT_HEATING_DEADBAND ,  			//  night heating setback
	EEP_NIGHT_COOLING_DEADBAND ,			// 	night cooling setback
  EEP_NIGHT_HEATING_SETPOINT,	   //20

  EEP_NIGHT_COOLING_SETPOINT = EEP_NIGHT_HEATING_SETPOINT + 2,

	EEP_APPLICATION = EEP_NIGHT_COOLING_SETPOINT + 2,       				//  HOTEL, OFFICE, HOME, ....
	EEP_POWERUP_SETPOINT , 					//  POWER UP COOLING SETPOINT
	EEP_POWERUP_ON_OFF , 					//  Powerup on or off 
	EEP_KEYPAD_SELECT ,						//  four or six  butten
	EEP_AUTO_ONLY ,							//  auto have or not	
	EEP_OUTPUT1_SCALE ,  					// 
	EEP_OUTPUT2_SCALE ,  	//30				//		
	EEP_MAX_SETPOINT,             			//  max cooling setpoint
	EEP_MIN_SETPOINT,						// 	min heating setpoint 
	EEP_SPECIAL_MENU_LOCK,			
	EEP_DISPLAY,
	EEP_VALVE_TRAVEL_TIME,
	EEP_RS485_MODE,
	EEP_DEMAND_RESPONSE,//
	EEP_CLOCK_YEAR,
  EEP_CLOCK_MONTH,
	EEP_CLOCK_DAY,
	EEP_CLOCK_WEEK,
	EEP_CLOCK_HOUR,
	EEP_CLOCK_MINUTE,
	EEP_CLOCK_SECOND,	
	EEP_SCHEDULE_WORK_DAYTIME_HOUR,
	EEP_SCHEDULE_WORK_DAYTIME_MINUTE,
	EEP_SCHEDULE_WORK_NIGHTTIME_HOUR,
	EEP_SCHEDULE_WORK_NIGHTTIME_MINUTE,
	EEP_SCHEDULE_WEEKEND_DAYTIME_HOUR,
	EEP_SCHEDULE_WEEKEND_DAYTIME_MINUTE,
	EEP_SCHEDULE_WEEKEND_NIGHTTIME_HOUR,
	EEP_PROTOCOL_SEL,//EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE,
	EEP_STORE_DEFAULTS,    					// the value  means nothing,here only occupy one possition for not change the e2 chip store sequence when the user set parameters by keypad
	EEP_FACTORY_DEFAULTS,         			// factory defaults  //not sure why this is stored in EEprom
  EEP_DAY_COOLING_SETPOINT ,		//40    //I think this is (same) as your variables, no menu for this though. 

  EEP_DAY_HEATING_SETPOINT = EEP_DAY_COOLING_SETPOINT + 2,  	//    //can you use your menu items, Shi and Slo to set these by the keypad. then a lot of work is done for you already. 
  
	EEP_NIGHT_SETPOINT = EEP_DAY_HEATING_SETPOINT + 2,

	EEP_FAN_SPEED = EEP_NIGHT_SETPOINT + 2,  		//
	EEP_HEAT_PID3_TABLE,
  EEP_COOL_PID3_TABLE,
	EEP_HEAT_UNIVERSAL_TABLE ,    			// 	how many heat stages in the universal PID table
	EEP_COOL_UNIVERSAL_TABLE ,    			//  how many cool stages in the universal PID table
	EEP_HEAT_ORIGINAL_TABLE ,     			//  store the number of heat satges in  the original PID TABLE
	EEP_COOL_ORIGINAL_TABLE ,     			//50  caution: Don't add anything between EEP_HEAT_UNIVERSAL_TABLE and EEP_COOL_ORIGINAL_TABLE
	EEP_INPUT1_SELECT ,           			//  select which compose is the analog input 
	EEP_UNIVERSAL_PTERM  ,        			//	universal sensor P term

	EEP_UNIVERSAL_ITERM = EEP_UNIVERSAL_PTERM + 2 ,     //	universal sensor I term  	

	EEP_UNIVERSAL_SET = EEP_UNIVERSAL_ITERM + 2 ,       //	setpoint for universal PID   	

	EEP_UNIVERSAL_NIGHTSET = EEP_UNIVERSAL_SET + 2 , 	// 	night setpoint for universal pid

	EEP_LED1_CONTROL = EEP_UNIVERSAL_NIGHTSET + 2 ,//60,  
	EEP_LED2_CONTROL,						//
	EEP_LED3_CONTROL,
	EEP_LED4_CONTROL,
	EEP_LED5_CONTROL,
	EEP_LED6_CONTROL,	 
	EEP_LED7_CONTROL,
	EEP_PID_OUTPUT1 ,             			// 	select which type PID to be caculated for control output
	EEP_PID_OUTPUT2 ,            			// 
	EEP_PID_OUTPUT3 , //  
	EEP_PID_OUTPUT4 ,//70   
	EEP_PID_OUTPUT5 ,						  
	EEP_PID_OUTPUT6 ,      
	EEP_PID_OUTPUT7 , 
	EEP_RANGE_OUTPUT1 ,                 	// 	Decide the OUTPUT_1TO5 mode ,ON/OFF or floating valve
	EEP_RANGE_OUTPUT2 ,   				   	//	cc 2011/01/18 chage EEP_MODE_OUTPUT to EEP_RANGE_OUTPUT
	EEP_RANGE_OUTPUT3 ,   
	EEP_RANGE_OUTPUT4 ,   
	EEP_RANGE_OUTPUT5 , 
	EEP_INTERLOCK_OUTPUT1 ,// 				// 	interlock for each output.these registers store the input type correspond to the interlock.
	EEP_INTERLOCK_OUTPUT2 ,//80					// 	there are four cases : 0, interlock always ON; 1, DI1 determine the interlock status
	EEP_INTERLOCK_OUTPUT3 ,					//  2, AI1 determine the interlock status 3, AI2 determine the interlock status					                		//	4,timer or.5,timer and.6,timer delay from off to on 
	EEP_INTERLOCK_OUTPUT4 ,
	EEP_INTERLOCK_OUTPUT5 ,
	EEP_INTERLOCK_OUTPUT6 ,
	EEP_INTERLOCK_OUTPUT7 ,		
	EEP_SETPOINT_INCREASE ,					// 	increse or decrease step when adjust setpoint
	EEP_FREEZE_TEMP_SETPOINT,				// 	temperature setpoitn in freeze protect
	EEP_FREEZE_DELAY_ON,					// 	delay several seconds to sure the temp less than setpoint
	EEP_FREEZE_DELAY_OFF,//					// 	the Tstat will heat several minutes
	EEP_ANALOG1_FUNCTION,//90					// 	AI1 is used for what function.0,normal 1,freeze protect 2,occupacy 
	EEP_ANALOG2_FUNCTION, 					//	AI2 is used for what function.3,sweep off 4,clock 5,change over mode
	EEP_TIMER_ON,							// 	the timer will on how long time

	EEP_TIMER_OFF = EEP_TIMER_ON + 2,		// 	the timer will off how long time

	EEP_MIN_ADDRESS = EEP_TIMER_OFF + 2,

	EEP_MAX_ADDRESS = EEP_MIN_ADDRESS + 2, 	//

	EEP_TIMER_UNITS = EEP_MAX_ADDRESS + 2,  //100	
	EEP_INPUT_MANU_ENABLE,
	EEP_INTERNAL_SENSOR_MANUAL,
	EEP_OUTPUT_MANU_ENABLE,  
	EEP_BASE_ADDRESS,   
	EEP_DEAD_MASTER,
	EEP_ROUND_DISPLAY, 
	EEP_CONFIGURATION,
	EEP_TIMER_SELECT,	   					//	select interval timer or rotation timer for that user timer
	EEP_OUTPUT1_FUNCTION,//					//  use one byte to store the function for each output instaed of one one byte for all outputs
	EEP_OUTPUT2_FUNCTION,//110
	EEP_OUTPUT3_FUNCTION,
	EEP_OUTPUT4_FUNCTION,
	EEP_OUTPUT5_FUNCTION,          			// 	0 = normal,1 = rotation,2 = lighting , 3 = PWM
	EEP_DEFAULT_SETPOINT,
	EEP_SETPOINT_CONTROL,					
	EEP_PIR_STAGE,
	EEP_SYSTEM_TIME_FORMAT,
	EEP_ANALOG1_RANGE,
	EEP_ANALOG2_RANGE,//
	EEP_ANALOG3_RANGE,//120
	EEP_ANALOG4_RANGE,
	EEP_ANALOG5_RANGE,
	EEP_ANALOG6_RANGE,
	EEP_ANALOG7_RANGE,						
	EEP_ANALOG8_RANGE,
	EEP_LCD_TURN_OFF,
	EEP_FREE_COOL_CONFIG,
	EEP_LOCK_REGISTER,
	EEP_CUSTOM_ADDRESS,//
	EEP_LCD_SCREEN1,  //130
	EEP_LCD_SCREEN2,
	EEP_SUN_ICON_CONTROL,
	EEP_MOON_ICON_CONTROL,
	EEP_DAYSETPOINT_OPTION,
	EEP_WORK_MODE,
	EEP_INPUT1_FILTER,
	EEP_INPUT2_FILTER,
	EEP_INPUT3_FILTER,
	EEP_INPUT4_FILTER,
	EEP_INPUT5_FILTER,
	EEP_INPUT6_FILTER,
	EEP_INPUT7_FILTER,
	EEP_INPUT8_FILTER,
	EEP_CO2_FILTER,
	EEP_HUM_FILTER,
	EEP_CO2_CALIBRATION,

	TOTAL_EE_PARAMETERS = EEP_CO2_CALIBRATION + 2, 	EEP_FAN0_OPER_TABLE_COAST = TOTAL_EE_PARAMETERS,      // TOTAL_EE_PARAMETERS + 0 = 36 + 0
	EEP_FAN0_OPER_TABLE_COOL1,		// 
	EEP_FAN0_OPER_TABLE_COOL2,      // 
	EEP_FAN0_OPER_TABLE_COOL3,      // 
	EEP_FAN0_OPER_TABLE_HEAT1,      // 
	EEP_FAN0_OPER_TABLE_HEAT2,      // 
	EEP_FAN0_OPER_TABLE_HEAT3,      // 	
	EEP_FAN1_OPER_TABLE_COAST,      // TOTAL_EE_PARAMETERS + 7
	EEP_FAN1_OPER_TABLE_COOL1,		// 
	EEP_FAN1_OPER_TABLE_COOL2,      // 
	EEP_FAN1_OPER_TABLE_COOL3,      // 
	EEP_FAN1_OPER_TABLE_HEAT1,      //240 
	EEP_FAN1_OPER_TABLE_HEAT2,      // 
	EEP_FAN1_OPER_TABLE_HEAT3,      // 
	EEP_FAN2_OPER_TABLE_COAST,      // TOTAL_EE_PARAMETERS + 14
	EEP_FAN2_OPER_TABLE_COOL1,		// 
	EEP_FAN2_OPER_TABLE_COOL2,      // 
	EEP_FAN2_OPER_TABLE_COOL3,      // 
	EEP_FAN2_OPER_TABLE_HEAT1,      // 
	EEP_FAN2_OPER_TABLE_HEAT2,      // 
	EEP_FAN2_OPER_TABLE_HEAT3,      // 	
	EEP_FAN3_OPER_TABLE_COAST,      // TOTAL_EE_PARAMETERS + 21
	EEP_FAN3_OPER_TABLE_COOL1,		// 
	EEP_FAN3_OPER_TABLE_COOL2,      // 250
	EEP_FAN3_OPER_TABLE_COOL3,      // 
	EEP_FAN3_OPER_TABLE_HEAT1,      // 
	EEP_FAN3_OPER_TABLE_HEAT2,      // 
	EEP_FAN3_OPER_TABLE_HEAT3,      // 	
	EEP_FANAUT_OPER_TABLE_COAST,    // TOTAL_EE_PARAMETERS + 28 = 
	EEP_FANAUT_OPER_TABLE_COOL1,	// 
	EEP_FANAUT_OPER_TABLE_COOL2,    // 
	EEP_FANAUT_OPER_TABLE_COOL3,    // 
	EEP_FANAUT_OPER_TABLE_HEAT1,    // 
	EEP_FANAUT_OPER_TABLE_HEAT2,    // 
	EEP_FANAUT_OPER_TABLE_HEAT3,    // 	260
	EEP_UNIVERSAL_OUTPUT_BEGIN,	EEP_UNIVERSAL_OUTPUT_COAST = EEP_UNIVERSAL_OUTPUT_BEGIN,
	EEP_UNIVERSAL_OUTPUT_COOL1,
	EEP_UNIVERSAL_OUTPUT_COOL2,
	EEP_UNIVERSAL_OUTPUT_COOL3,
	EEP_UNIVERSAL_OUTPUT_HEAT1,
	EEP_UNIVERSAL_OUTPUT_HEAT2,
	EEP_UNIVERSAL_OUTPUT_HEAT3,	
	EEP_VALVE_OPER_TABLE_BEGIN,	EEP_VALVE_OPER_TABLE_COAST = EEP_VALVE_OPER_TABLE_BEGIN,
	EEP_VALVE_OPER_TABLE_COOL1,
	EEP_VALVE_OPER_TABLE_COOL2,	 //270
	EEP_VALVE_OPER_TABLE_COOL3,
	EEP_VALVE_OPER_TABLE_HEAT1,
	EEP_VALVE_OPER_TABLE_HEAT2,
	EEP_VALVE_OPER_TABLE_HEAT3,
	EEP_UNIVERSAL_VALVE_BEGIN,	EEP_UNIVERSAL_VALVE_COAST = EEP_UNIVERSAL_VALVE_BEGIN,
	EEP_UNIVERSAL_VALVE_COOL1,
	EEP_UNIVERSAL_VALVE_COOL2,
	EEP_UNIVERSAL_VALVE_COOL3,
	EEP_UNIVERSAL_VALVE_HEAT1,
	EEP_UNIVERSAL_VALVE_HEAT2,		//280
	EEP_UNIVERSAL_VALVE_HEAT3,
	EEP_VALVE_OFF_TABLE_BEGIN,	EEP_VALVE_OFF_TABLE_COAST = EEP_VALVE_OFF_TABLE_BEGIN,
	EEP_VALVE_OFF_TABLE_COOL1,
	EEP_VALVE_OFF_TABLE_COOL2,
	EEP_VALVE_OFF_TABLE_COOL3,
	EEP_VALVE_OFF_TABLE_HEAT1,
	EEP_VALVE_OFF_TABLE_HEAT2,
	EEP_VALVE_OFF_TABLE_HEAT3,
	EEP_OUTPUT_DELAY_OFF_TO_ON,	EEP_OUTPUT1_DELAY_OFF_TO_ON = EEP_OUTPUT_DELAY_OFF_TO_ON,
	EEP_OUTPUT2_DELAY_OFF_TO_ON,  //290
	EEP_OUTPUT3_DELAY_OFF_TO_ON,
	EEP_OUTPUT4_DELAY_OFF_TO_ON,
	EEP_OUTPUT5_DELAY_OFF_TO_ON,
	EEP_OUTPUT6_DELAY_OFF_TO_ON,
	EEP_OUTPUT7_DELAY_OFF_TO_ON,
	EEP_OUTPUT_DELAY_ON_TO_OFF,	EEP_OUTPUT1_DELAY_ON_TO_OFF  = EEP_OUTPUT_DELAY_ON_TO_OFF,
	EEP_OUTPUT2_DELAY_ON_TO_OFF,
	EEP_OUTPUT3_DELAY_ON_TO_OFF,
	EEP_OUTPUT4_DELAY_ON_TO_OFF,
	EEP_OUTPUT5_DELAY_ON_TO_OFF,
	EEP_OUTPUT6_DELAY_ON_TO_OFF,
	EEP_OUTPUT7_DELAY_ON_TO_OFF,
	EEP_SCHEDULE_DHOME_HOUR,// at home (sun + man)				//schedule
	EEP_SCHEDULE_DHOME_MIN,//
	EEP_SCHEDULE_WORK_HOUR,// at work (sun + no man)
	EEP_SCHEDULE_WORK_MIN,//
	EEP_SCHEDULE_NHOME_HOUR,// at home (sun + man)				//schedule
	EEP_SCHEDULE_NHOME_MIN,//	
	EEP_SCHEDULE_SLEEP_HOUR,// sleeping (moon + man)
	EEP_SCHEDULE_SLEEP_MIN,//
	EEP_SCHEDULE_HOLIDAY_BYEAR,
	EEP_SCHEDULE_HOLIDAY_BMONTH,// away/holiday (no sun&moon + no man)  
	EEP_SCHEDULE_HOLIDAY_BDAY,//
	EEP_SCHEDULE_HOLIDAY_EYEAR,
	EEP_SCHEDULE_HOLIDAY_EMONTH,// 					
	EEP_SCHEDULE_HOLIDAY_EDAY,//
	EEP_DAY2_EVENT4_HI,  //34
	EEP_DAY2_EVENT4_LO,	
	EEP_OUTPUT_PWM_AUTO_BEGIN,	EEP_OUTPUT_PWM_AUTO_COAST = EEP_OUTPUT_PWM_AUTO_BEGIN,
	EEP_OUTPUT_PWM_AUTO_COOL1,	 
	EEP_OUTPUT_PWM_AUTO_COOL2,
	EEP_OUTPUT_PWM_AUTO_COOL3,
	EEP_OUTPUT_PWM_AUTO_HEAT1,
	EEP_OUTPUT_PWM_AUTO_HEAT2,
	EEP_OUTPUT_PWM_AUTO_HEAT3,	
	EEP_ENDS,				   //
  EXTERNAL_SENSOR_1,		                // MDF 6/7/04 Added EXTERNAL_SENSOR_1
	ANALOG_INPUT1,
	ANALOG_INPUT2,
	ANALOG_INPUT3,	//310
	ANALOG_INPUT4,
	ANALOG_INPUT5,
	ANALOG_INPUT6,
	ANALOG_INPUT7,
	ANALOG_INPUT8,
	ADJUSTING_COOLING,		        		//110 not stored in b.eeprom   b is strcut link.
	ADJUSTING_HEATING,
	ADJUSTING_PIR,
	ADJUSTING_FAN_SPEED ,
	ADJUSTING_GRID_MODE, 
	ADJUSTING_SYSTEM_MODE, //system mode: heating/cooling/auto
	ADDRESS_PLUG_N_PLAY ,                   // 0
	TEMPRATURE_SENSOR ,                       // 1
	COOLING_VALVE ,                         // 2
	HEATING_VALVE ,                         // 3
	PID ,									// 4	 120
	PID_UNIVERSAL ,
	COOL_HEAT_MODE ,						// 6
	MODE_OPERATION ,						// 7
	PID2_MODE_OPERATION,
	EEP_PID_SAMPLE_TIME,//DIGITAL_OUTPUT_STATUS ,		//330			// 8 //out1,2,3 and if on-off type, 4 and 5
  EEP_DTERM,                                      //EXPLOREADDRESS,	
	EEP_OVERRIDE_TIMER_LEFT	,
	EEP_LAST_KEY_TIMER,//130 whenever press a key,the timer start to count,from 0 to 65535
	EEP_KEYPAD_VALUE,
	EEP_DISPLAY_HUNDRED,
	EEP_DISPLAY_TEN,
	EEP_DISPLAY_DIGITAL,
	EEP_DISPLAY_STATUS,		  //340
	EEP_INTERNAL_THERMISTOR,
	EEP_VALVE_PERCENT,
    EEP_MANU_RELAY1,
    EEP_MANU_RELAY2,
    EEP_MANU_RELAY3,
    EEP_MANU_RELAY4,
    EEP_MANU_RELAY5, 
	EEP_EEPROM_SIZE,
    EEP_LINE1_CHAR1,					//	150

    EEP_LINE1_CHAR2 = EEP_LINE1_CHAR1 + 2,

    EEP_LINE1_CHAR3 = EEP_LINE1_CHAR2 + 2,

    EEP_LINE1_CHAR4 = EEP_LINE1_CHAR3 + 2,

    EEP_LINE2_CHAR1 = EEP_LINE1_CHAR4 + 2,					

    EEP_LINE2_CHAR2 = EEP_LINE2_CHAR1 + 2,

    EEP_LINE2_CHAR3 = EEP_LINE2_CHAR2 + 2,

    EEP_LINE2_CHAR4 = EEP_LINE2_CHAR3 + 2, 

	EEP_INTERNAL_CHAR1 = EEP_LINE2_CHAR4 + 2,

	EEP_INTERNAL_CHAR2 = EEP_INTERNAL_CHAR1 + 2,

	EEP_INTERNAL_CHAR3 = EEP_INTERNAL_CHAR2 + 2,

	EEP_INTERNAL_CHAR4 = EEP_INTERNAL_CHAR3 + 2,
	
	EEP_AI1_CHAR1 = EEP_INTERNAL_CHAR4 + 2,

	EEP_AI1_CHAR2 = EEP_AI1_CHAR1 + 2,

	EEP_AI1_CHAR3 = EEP_AI1_CHAR2 + 2,

	EEP_AI1_CHAR4 = EEP_AI1_CHAR3 + 2, 
	
	EEP_AI2_CHAR1 = EEP_AI1_CHAR4 + 2,

	EEP_AI2_CHAR2 = EEP_AI2_CHAR1 + 2,

	EEP_AI2_CHAR3 = EEP_AI2_CHAR2 + 2,

	EEP_AI2_CHAR4 = EEP_AI2_CHAR3 + 2,

	EEP_AI3_CHAR1 = EEP_AI2_CHAR4 + 2,

	EEP_AI3_CHAR2 = EEP_AI3_CHAR1 + 2,

	EEP_AI3_CHAR3 = EEP_AI3_CHAR2 + 2,

	EEP_AI3_CHAR4 = EEP_AI3_CHAR3 + 2, 

	EEP_AI4_CHAR1 = EEP_AI3_CHAR4 + 2,

	EEP_AI4_CHAR2 = EEP_AI4_CHAR1 + 2,

	EEP_AI4_CHAR3 = EEP_AI4_CHAR2 + 2,

	EEP_AI4_CHAR4 = EEP_AI4_CHAR3 + 2,

	EEP_AI5_CHAR1 = EEP_AI4_CHAR4 + 2,

	EEP_AI5_CHAR2 = EEP_AI5_CHAR1 + 2,

	EEP_AI5_CHAR3 = EEP_AI5_CHAR2 + 2,

	EEP_AI5_CHAR4 = EEP_AI5_CHAR3 + 2, 

	EEP_AI6_CHAR1 = EEP_AI5_CHAR4 + 2,

	EEP_AI6_CHAR2 = EEP_AI6_CHAR1 + 2,

	EEP_AI6_CHAR3 = EEP_AI6_CHAR2 + 2,

	EEP_AI6_CHAR4 = EEP_AI6_CHAR3 + 2,

	EEP_AI7_CHAR1 = EEP_AI6_CHAR4 + 2,

	EEP_AI7_CHAR2 = EEP_AI7_CHAR1 + 2,

	EEP_AI7_CHAR3 = EEP_AI7_CHAR2 + 2,

	EEP_AI7_CHAR4 = EEP_AI7_CHAR3 + 2, 

	EEP_AI8_CHAR1 = EEP_AI7_CHAR4 + 2,

	EEP_AI8_CHAR2 = EEP_AI8_CHAR1 + 2,

	EEP_AI8_CHAR3 = EEP_AI8_CHAR2 + 2,

	EEP_AI8_CHAR4 = EEP_AI8_CHAR3 + 2,

	EEP_OUT1_CHAR1 = EEP_AI8_CHAR4 + 2,

	EEP_OUT1_CHAR2 = EEP_OUT1_CHAR1 + 2,

	EEP_OUT1_CHAR3 = EEP_OUT1_CHAR2 + 2,

	EEP_OUT1_CHAR4 = EEP_OUT1_CHAR3 + 2,

	EEP_OUT2_CHAR1 = EEP_OUT1_CHAR4 + 2,

	EEP_OUT2_CHAR2 = EEP_OUT2_CHAR1 + 2,

	EEP_OUT2_CHAR3 = EEP_OUT2_CHAR2 + 2,

	EEP_OUT2_CHAR4 = EEP_OUT2_CHAR3 + 2,

	EEP_OUT3_CHAR1 = EEP_OUT2_CHAR4 + 2,

	EEP_OUT3_CHAR2 = EEP_OUT3_CHAR1 + 2,

	EEP_OUT3_CHAR3 = EEP_OUT3_CHAR2 + 2,

	EEP_OUT3_CHAR4 = EEP_OUT3_CHAR3 + 2,

	EEP_OUT4_CHAR1 = EEP_OUT3_CHAR4 + 2,

	EEP_OUT4_CHAR2 = EEP_OUT4_CHAR1 + 2,

	EEP_OUT4_CHAR3 = EEP_OUT4_CHAR2 + 2,

	EEP_OUT4_CHAR4 = EEP_OUT4_CHAR3 + 2,

	EEP_OUT5_CHAR1 = EEP_OUT4_CHAR4 + 2,

	EEP_OUT5_CHAR2 = EEP_OUT5_CHAR1 + 2,

	EEP_OUT5_CHAR3 = EEP_OUT5_CHAR2 + 2,

	EEP_OUT5_CHAR4 = EEP_OUT5_CHAR3 + 2,

	EEP_OUT6_CHAR1 = EEP_OUT5_CHAR4 + 2,

	EEP_OUT6_CHAR2 = EEP_OUT6_CHAR1 + 2,

	EEP_OUT6_CHAR3 = EEP_OUT6_CHAR2 + 2,

	EEP_OUT6_CHAR4 = EEP_OUT6_CHAR3 + 2,

	EEP_OUT7_CHAR1 = EEP_OUT6_CHAR4 + 2,

	EEP_OUT7_CHAR2 = EEP_OUT7_CHAR1 + 2,

	EEP_OUT7_CHAR3 = EEP_OUT7_CHAR2 + 2,

	EEP_OUT7_CHAR4 = EEP_OUT7_CHAR3 + 2,

	EEP_SCHEDULEA_CHAR1 = EEP_OUT7_CHAR4 + 2,							// 		543
	EEP_SCHEDULEA_CHAR2,
	EEP_SCHEDULEA_CHAR3,
	EEP_SCHEDULEA_CHAR4,
	EEP_SCHEDULEB_CHAR1,							
	EEP_SCHEDULEB_CHAR2,
	EEP_SCHEDULEB_CHAR3,
	EEP_SCHEDULEB_CHAR4,
	EEP_SCHEDULEC_CHAR1,							
	EEP_SCHEDULEC_CHAR2,
	EEP_SCHEDULEC_CHAR3,
	EEP_SCHEDULEC_CHAR4,
	EEP_SCHEDULED_CHAR1,							
	EEP_SCHEDULED_CHAR2,
	EEP_SCHEDULED_CHAR3,
	EEP_SCHEDULED_CHAR4, 
	EEP_WALL_CHAR1,							
	EEP_WALL_CHAR2,
	EEP_WALL_CHAR3,
	EEP_WALL_CHAR4,
	EEP_CEILING_CHAR1,						
	EEP_CEILING_CHAR2,
	EEP_CEILING_CHAR3,
	EEP_CEILING_CHAR4,
	EEP_OUTDOOR_CHAR1,					
	EEP_OUTDOOR_CHAR2,
	EEP_OUTDOOR_CHAR3,
	EEP_OUTDOOR_CHAR4,
	EEP_AVERAGE_CHAR1,						
	EEP_AVERAGE_CHAR2,
	EEP_AVERAGE_CHAR3,
	EEP_AVERAGE_CHAR4,
	EEP_HEAT_COOL,
	EEP_PWM_OUT4,
	EEP_PWM_OUT5,
  EEP_DELAY1_ON_TO_OFF_TIME_CURRENT,      
	EEP_DELAY2_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY3_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY4_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY5_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY6_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY7_ON_TO_OFF_TIME_CURRENT,
	EEP_DELAY1_OFF_TO_ON_TIME_CURRENT,
	EEP_DELAY2_OFF_TO_ON_TIME_CURRENT,
	EEP_DELAY3_OFF_TO_ON_TIME_CURRENT,
	EEP_DELAY4_OFF_TO_ON_TIME_CURRENT, //380
	EEP_DELAY5_OFF_TO_ON_TIME_CURRENT,
	EEP_DELAY6_OFF_TO_ON_TIME_CURRENT,
	EEP_DELAY7_OFF_TO_ON_TIME_CURRENT,
	MAXEXTERNADDRESS,	
    EEP_DEADMASTER_AUTO_MANUAL,	
    EEP_DEADMASTER_OUTPUT_STATE,
    EEP_DEADMASTER_COOL_OUTPUT,

    EEP_DEADMASTER_HEAT_OUTPUT = EEP_DEADMASTER_COOL_OUTPUT + 2,
	
	EEP_DAY_SETPOINT = EEP_DEADMASTER_HEAT_OUTPUT + 2,	

	EEP_WINDOW_INTERLOCK_COOLING_SETPOINT = EEP_DAY_SETPOINT + 2,

	EEP_WINDOW_INTERLOCK_HEATING_SETPOINT = EEP_WINDOW_INTERLOCK_COOLING_SETPOINT + 2,

	TEST1 = EEP_WINDOW_INTERLOCK_HEATING_SETPOINT + 2,
	TEST2 ,
	TEST3 ,
	EEP_RESERVED,
  CURRENT_SETPOINT,
  CURRENT_SETPOINT_OFFSET,
	EEP_TWO_BYTE_SETPOINT,
	HUM_PIC_VERSION,
	EEP_ICON_DAY_OUTPUT_CONTROL,
	EEP_ICON_NIGHT_OUTPUT_CONTROL,
	EEP_ICON_OCC_OUTPUT_CONTROL,
	EEP_ICON_UNOCC_OUTPUT_CONTROL,
	EEP_ICON_HEAT_OUTPUT_CONTROL,
	EEP_ICON_COOL_OUTPUT_CONTROL,
	EEP_ICON_FAN1_OUTPUT_CONTROL,
	EEP_ICON_FAN2_OUTPUT_CONTROL,
	EEP_ICON_FAN3_OUTPUT_CONTROL,
	EEP_OUTPUT_PWM_OFF_BEGIN,	EEP_OUTPUT_PWM_OFF_COAST = EEP_OUTPUT_PWM_OFF_BEGIN,
	EEP_OUTPUT_PWM_OFF_COOL1,	 
	EEP_OUTPUT_PWM_OFF_COOL2,
	EEP_OUTPUT_PWM_OFF_COOL3,
	EEP_OUTPUT_PWM_OFF_HEAT1,
	EEP_OUTPUT_PWM_OFF_HEAT2,
	EEP_OUTPUT_PWM_OFF_HEAT3,
	EEP_SPARE,	
//	HUMRH1_H, //

//	HUMCOUNT2_H = HUMRH1_H + 2,		

//	HUMRH2_H 	= HUMCOUNT2_H + 2,	

//	HUMCOUNT3_H = HUMRH2_H+ 2,		

	HUMRH3_H,//	= HUMCOUNT3_H + 2, //411

	HUMCOUNT4_H = HUMRH3_H + 2,	//

	HUMRH4_H	= HUMCOUNT4_H + 2,

	HUMCOUNT5_H = HUMRH4_H + 2,	

	HUMRH5_H	= HUMCOUNT5_H + 2,

	HUMCOUNT6_H = HUMRH5_H + 2 , //421				

	HUMRH6_H	= HUMCOUNT6_H + 2, //

	HUMCOUNT7_H = HUMRH6_H + 2 ,

	HUMRH7_H	= HUMCOUNT7_H + 2,

	HUMCOUNT8_H = HUMRH7_H + 2 ,

	HUMRH8_H	= HUMCOUNT8_H + 2, //431

	HUMCOUNT9_H = HUMRH8_H + 2 ,   //

	HUMRH9_H	= HUMCOUNT9_H + 2,

	HUMCOUNT10_H = HUMRH9_H + 2 ,			

	HUMRH10_H	= HUMCOUNT10_H +2 , //439

    EEP_HARDWARE_INFORMATION = HUMRH10_H + 2, //L TO H

	EEP_YEAR    = EEP_HARDWARE_INFORMATION + 2,
	EEP_MONTH,	
	EEP_WEEK,
	EEP_DAY,	
	EEP_HOUR,
	EEP_MINUTE,
 	EEP_SECOND,	 
	EEP_DIAGNOSTIC_ALARM,
	EEP_ANALOG3_FUNCTION,//future
	EEP_ANALOG4_FUNCTION,//future
	EEP_ANALOG5_FUNCTION,//future
	EEP_ANALOG6_FUNCTION,//future
	EEP_ANALOG7_FUNCTION,//future
	EEP_ANALOG8_FUNCTION,//future
	EEP_OUTPUT6_FUNCTION,//future
	EEP_OUTPUT7_FUNCTION,//future
	EEP_ECOMONY_COOLING_SETPOINT,  

	EEP_ECOMONY_HEATING_SETPOINT = EEP_ECOMONY_COOLING_SETPOINT + 2,  	

	EEP_ICON = EEP_ECOMONY_HEATING_SETPOINT + 2,
	HUM_OFFSETFLAG,
	EEP_MANUAL_AO1,

	EEP_MANUAL_AO2 = EEP_MANUAL_AO1 + 2,

	EEP_EXTERNAL_SENSOR1 = EEP_MANUAL_AO2 + 2,

	EEP_EXTERNAL_SENSOR2 = EEP_EXTERNAL_SENSOR1 + 2,

	HUM_LOCK_A = EEP_EXTERNAL_SENSOR2 + 2,//= EEP_USER_INFO_SCREEN2_LINE2_CHAR_78 + 2,
	HUM_LOCK_B,
	EEP_LCD_ROTATE_ENABLE,
	EEP_SCHEDULE_ON_OFF,
	EEP_DISP_ITEM_TEMPERATURE, 
	EEP_DISP_ITEM_SETPOINT,	   
	EEP_DISP_ITEM_AI1,		   
	EEP_DISP_ITEM_AI2,		   
	EEP_DISP_ITEM_AI3,		   
	EEP_DISP_ITEM_AI4,		   
	EEP_DISP_ITEM_AI5,		   
	EEP_DISP_ITEM_AI6,		   
	EEP_DISP_ITEM_AI7,		   
	EEP_DISP_ITEM_AI8,		   
	EEP_DISP_ITEM_MODE,
	EEP_DISP_ITEM_USER_INFO,
	EEP_DISP_ITEM_CLOCK_DATE,
	EEP_DISP_ITEM_CLOCK_TIME,
	EEP_DISP_ITEM_EXTENAL_SENSOR1,
	EEP_DISP_ITEM_EXTENAL_SENSOR2,
	EEP_DISP_ITEM_EXTENAL_SENSOR3,
	EEP_DISP_ITEM_EXTENAL_SENSOR4,
	EEP_DISP_ITEM_OUT1,
	EEP_DISP_ITEM_OUT2,
	EEP_DISP_ITEM_OUT3,
	EEP_DISP_ITEM_OUT4,
	EEP_DISP_ITEM_OUT5,
	EEP_DISP_ITEM_OUT6,
	EEP_DISP_ITEM_OUT7,
	EEP_EXT_SENSOR_TEMP_CAL,

	EEP_BUTTON_LEFT1 = EEP_EXT_SENSOR_TEMP_CAL + 2,
	EEP_BUTTON_LEFT2,
	EEP_BUTTON_LEFT3,
	EEP_BUTTON_LEFT4,
	EEP_HUM_INPUT_MANUAL_ENABLE,
	EEP_HUM_INPUT_MANUAL_VALUE,

	EEP_CO2_INPUT_MANUAL_ENABLE = EEP_HUM_INPUT_MANUAL_VALUE + 2,
	EEP_CO2_INPUT_MANUAL_VALUE,

	EEP_UNIVERSAL_OFF_TABLE_BEGIN = EEP_CO2_INPUT_MANUAL_VALUE + 2,	EEP_UNIVERSAL_OFF_TABLE_COAST = EEP_UNIVERSAL_OFF_TABLE_BEGIN,
	EEP_UNIVERSAL_OFF_TABLE_COOL1,
	EEP_UNIVERSAL_OFF_TABLE_COOL2,
	EEP_UNIVERSAL_OFF_TABLE_COOL3,
	EEP_UNIVERSAL_OFF_TABLE_HEAT1,
	EEP_UNIVERSAL_OFF_TABLE_HEAT2,
	EEP_UNIVERSAL_OFF_TABLE_HEAT3,		
	EEP_UNIVERSAL_OFF_VALVE_BEGIN,	EEP_UNIVERSAL_OFF_VALVE_COAST = EEP_UNIVERSAL_OFF_VALVE_BEGIN,
	EEP_UNIVERSAL_OFF_VALVE_COOL1,
	EEP_UNIVERSAL_OFF_VALVE_COOL2,
	EEP_UNIVERSAL_OFF_VALVE_COOL3,
	EEP_UNIVERSAL_OFF_VALVE_HEAT1,
	EEP_UNIVERSAL_OFF_VALVE_HEAT2,
	EEP_UNIVERSAL_OFF_VALVE_HEAT3,
	EEP_KEYPAD_REVERSE,
	EEP_LIGHT_SENSOR,  //select which item will be on AI8
	EEP_PIR_SENSOR_SELECT,
	EEP_PIR_SENSOR_VALUE,
	EEP_PIR_SENSOR_ZERO,
	EEP_HUM_HEATING_CONTROL,
	EEP_ID_WRITE_ENABLE,
	EEP_COMMUNICATION_CHECK_ENABLE,
	EEP_PIR_TIMER,
	EEP_SUPPLY_SETPOINT,

	EEP_MAX_SUPPLY_SETPOINT = EEP_SUPPLY_SETPOINT + 2, 

	EEP_MIN_SUPPLY_SETPOINT = EEP_MAX_SUPPLY_SETPOINT + 2,
	 
	EEP_MAX_AIRFLOW_COOLING = EEP_MIN_SUPPLY_SETPOINT + 2,

	EEP_MAX_AIRFLOW_HEATING = EEP_MAX_AIRFLOW_COOLING + 2,
	 
	EEP_OCC_MIN_AIRFLOW = EEP_MAX_AIRFLOW_HEATING + 2,
	 
	EEP_AIRFLOW_SETPOINT = EEP_OCC_MIN_AIRFLOW + 2,

	EEP_VAV_CONTROL = EEP_AIRFLOW_SETPOINT + 2,
	EEP_PID3_INPUT_SELECT,
	EEP_PID3_VALVE_OPER_TABLE_BEGIN,	EEP_PID3_VALVE_OPER_TABLE_COAST = EEP_PID3_VALVE_OPER_TABLE_BEGIN,
	EEP_PID3_VALVE_OPER_TABLE_COOL1,
	EEP_PID3_VALVE_OPER_TABLE_COOL2,
	EEP_PID3_VALVE_OPER_TABLE_COOL3,
	EEP_PID3_VALVE_OPER_TABLE_HEAT1,
	EEP_PID3_VALVE_OPER_TABLE_HEAT2,
	EEP_PID3_VALVE_OPER_TABLE_HEAT3,
	EEP_PID3_COOLING_DB,
	EEP_PID3_HEATING_DB,
	EEP_PID3_PTERM,
	EEP_PID3_ITERM,
	EEP_PID3_OUTPUT,
	EEP_PID3_OUTPUT_BEGIN,
	EEP_PID3_OUTPUT_COOL1,
	EEP_PID3_OUTPUT_COOL2,
	EEP_PID3_OUTPUT_COOL3,
	EEP_PID3_OUTPUT_HEAT1,
	EEP_PID3_OUTPUT_HEAT2,
	EEP_PID3_OUTPUT_HEAT3,
	EEP_PID3_VALVE_OFF_TABLE_BEGIN,	EEP_PID3_VALVE_OFF_TABLE_COAST = EEP_PID3_VALVE_OFF_TABLE_BEGIN,
	EEP_PID3_VALVE_OFF_TABLE_COOL1,
	EEP_PID3_VALVE_OFF_TABLE_COOL2,
	EEP_PID3_VALVE_OFF_TABLE_COOL3,
	EEP_PID3_VALVE_OFF_TABLE_HEAT1,
	EEP_PID3_VALVE_OFF_TABLE_HEAT2,
	EEP_PID3_VALVE_OFF_TABLE_HEAT3,
	EEP_PID3_OFF_OUTPUT_BEGIN,
	EEP_PID3_OFF_OUTPUT_COOL1,
	EEP_PID3_OFF_OUTPUT_COOL2,
	EEP_PID3_OFF_OUTPUT_COOL3,
	EEP_PID3_OFF_OUTPUT_HEAT1,
	EEP_PID3_OFF_OUTPUT_HEAT2,
	EEP_PID3_OFF_OUTPUT_HEAT3,
	EEP_WIRELESS_PIR_RESPONSE1,
	EEP_WIRELESS_PIR_RESPONSE2,
	EEP_WIRELESS_PIR_RESPONSE3,
	EEP_WIRELESS_PIR_RESPONSE4,
	EEP_WIRELESS_PIR_RESPONSE5,
	EEP_PRESSURE_VALUE,	
	EEP_PRESSURE_MANUAL_VALUE,
	EEP_PRESSURE_MANUAL_ENABLE,
	EEP_AQ_VALUE,
	EEP_AQ_MANUAL_VALUE	,
	EEP_AQ_MANUAL_ENABLE,
	EEP_TEMPERATURE_OF_PRESSURE_SENSOR,
	EEP_TEMPERATURE_OF_HUM_SENSOR,
	EEP_SP_DISPLAY_SELECT,
	EEP_PID3_DAY_SP,
	EEP_PID3_NIGHT_SP = EEP_PID3_DAY_SP + 2,

	NEW_EEP_SERIALNUMBER_LOWORD = EEP_PID3_NIGHT_SP + 2, 
	            
	EEP_UNOCC_MIN_AIRFLOW  = NEW_EEP_SERIALNUMBER_LOWORD + 2,

	NEW_EEP_ADDRESS = EEP_UNOCC_MIN_AIRFLOW + 2, //829
	NEW_EEP_PRODUCT_MODEL,
	NEW_EEP_HARDWARE_REV,
	BK_SN,
	BK_EEP_SERIALNUMBER_LOWORD,

	BK_EEP_SERIALNUMBER_HIWORD  = BK_EEP_SERIALNUMBER_LOWORD + 2,
	
	BK_HW = BK_EEP_SERIALNUMBER_HIWORD + 2,
	BK_SPARE,
	EEP_TSTAT_NAME_ENABLE,
	EEP_TSTAT_NAME1,

	EEP_TSTAT_NAME2 = EEP_TSTAT_NAME1 + 2,

	EEP_TSTAT_NAME3 = EEP_TSTAT_NAME2 + 2,

	EEP_TSTAT_NAME4 = EEP_TSTAT_NAME3 + 2,

	EEP_TSTAT_NAME5 = EEP_TSTAT_NAME4 + 2,

	EEP_TSTAT_NAME6 = EEP_TSTAT_NAME5 + 2,

	EEP_TSTAT_NAME7 = EEP_TSTAT_NAME6 + 2,

	EEP_TSTAT_NAME8 = EEP_TSTAT_NAME7 + 2,

	EEP_TSTAT_NAME9 = EEP_TSTAT_NAME8 + 2,

	EEP_TSTAT_NAME10 = EEP_TSTAT_NAME9 + 2,

	EEP_SHOW_ID = EEP_TSTAT_NAME10 + 2,
	EEP_TRANSDUCER_RANGE_MIN,
	EEP_TRANSDUCER_RANGE_MAX,
	EEP_ICON_MANUAL_MODE,
	EEP_ICON_MANUAL_VALUE, 
	EEP_SETPOINT_UNLIMIT,
	MANUAL_COOL_VALVE_HI,
	MANUAL_COOL_VALVE_LO,
	MANUAL_HEAT_VALVE_HI,
	MANUAL_HEAT_VALVE_LO,
	MANUAL_RELAY,
	LIGHTING_STAGE,	
	EEP_SERINALNUMBER_WRITE_FLAG,	
	MANUAL_VALVE,
	MANUAL_ANALOG1_HI,
	MANUAL_ANALOG1_LO,
	MANUAL_ANALOG2_HI,
	MANUAL_ANALOG2_LO,
	MANUAL_ANALOG3_HI,
	MANUAL_ANALOG3_LO,
	MANUAL_ANALOG4_HI,
	MANUAL_ANALOG4_LO,
	MANUAL_ANALOG5_HI,
	MANUAL_ANALOG5_LO,
	MANUAL_ANALOG6_HI,
	MANUAL_ANALOG6_LO,
	MANUAL_ANALOG7_HI,
	MANUAL_ANALOG7_LO,
	MANUAL_ANALOG8_HI,
	MANUAL_ANALOG8_LO,
	MANUAL_INTERNAL_HI,
	MANUAL_INTERNAL_LO,
	RS485_WRITEFLAG,
	EEP_HUM_CALIBRATION,
	
	EEP_MODBUS_BACNET_SWITCH = EEP_HUM_CALIBRATION + 2,
	USER_SETTING,
	EEP_MODE1_NAME1,

	EEP_MODE1_NAME2 = EEP_MODE1_NAME1 + 2,
	
	EEP_MODE1_NAME3 = EEP_MODE1_NAME2 + 2,
	
	EEP_MODE1_NAME4 = EEP_MODE1_NAME3 + 2,
	
	EEP_MODE2_NAME1 = EEP_MODE1_NAME4 + 2,
	
	EEP_MODE2_NAME2 = EEP_MODE2_NAME1 + 2,
	
	EEP_MODE2_NAME3 = EEP_MODE2_NAME2 + 2,
	
	EEP_MODE2_NAME4 = EEP_MODE2_NAME3 + 2,
	
	EEP_MODE3_NAME1 = EEP_MODE2_NAME4 + 2,
	
	EEP_MODE3_NAME2 = EEP_MODE3_NAME1 + 2,
	
	EEP_MODE3_NAME3 = EEP_MODE3_NAME2 + 2,
	
	EEP_MODE3_NAME4 = EEP_MODE3_NAME3 + 2,

	EEP_MODE4_NAME1 = EEP_MODE3_NAME4 + 2,
	
	EEP_MODE4_NAME2 = EEP_MODE4_NAME1 + 2,
	
	EEP_MODE4_NAME3 = EEP_MODE4_NAME2 + 2,
	
	EEP_MODE4_NAME4 = EEP_MODE4_NAME3 + 2,
	
	EEP_MODE5_NAME1 = EEP_MODE4_NAME4 + 2,
	
	EEP_MODE5_NAME2 = EEP_MODE5_NAME1 + 2,
	
	EEP_MODE5_NAME3 = EEP_MODE5_NAME2 + 2,
	
	EEP_MODE5_NAME4 = EEP_MODE5_NAME3 + 2,

	EEP_HC_USER = EEP_MODE5_NAME4 + 2,
	EEP_SLEEP_SP,
	
	EEP_HOLIDAY_SP = EEP_SLEEP_SP + 2,
	
	EEP_MAX_WORK_SP = EEP_HOLIDAY_SP + 2,//max work setpoint  40C  
	EEP_MIN_WORK_SP,//min work setpoint  16C
	EEP_MAX_SLEEP_SP,//max sleeping setpoint 30C		
	EEP_MIN_SLEEP_SP,//min sleeping setpoint 15C	
	EEP_MAX_HOLIDAY_SP,//
	EEP_MIN_HOLIDAY_SP,//min holiday setpoint   >5C
	EEP_PIR_SENSETIVITY,
	EEP_SCHEDULE_MONDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_MONDAY_EVENT1_M,//
	EEP_SCHEDULE_MONDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_MONDAY_EVENT2_M,// 
	EEP_SCHEDULE_MONDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_MONDAY_EVENT3_M,//
	EEP_SCHEDULE_MONDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_MONDAY_EVENT4_M,//
	EEP_SCHEDULE_MONDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_MONDAY_EVENT5_M,//
	EEP_SCHEDULE_MONDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_MONDAY_EVENT6_M,//
	EEP_SCHEDULE_TUESDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_TUESDAY_EVENT1_M,//
	EEP_SCHEDULE_TUESDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_TUESDAY_EVENT2_M,// 
	EEP_SCHEDULE_TUESDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_TUESDAY_EVENT3_M,//
	EEP_SCHEDULE_TUESDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_TUESDAY_EVENT4_M,//
	EEP_SCHEDULE_TUESDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_TUESDAY_EVENT5_M,//
	EEP_SCHEDULE_TUESDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_TUESDAY_EVENT6_M,//
	EEP_SCHEDULE_WENSDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_WENSDAY_EVENT1_M,//
	EEP_SCHEDULE_WENSDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_WENSDAY_EVENT2_M,// 
	EEP_SCHEDULE_WENSDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_WENSDAY_EVENT3_M,//
	EEP_SCHEDULE_WENSDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_WENSDAY_EVENT4_M,//
	EEP_SCHEDULE_WENSDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_WENSDAY_EVENT5_M,//
	EEP_SCHEDULE_WENSDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_WENSDAY_EVENT6_M,//		
	EEP_SCHEDULE_THURSDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_THURSDAY_EVENT1_M,//
	EEP_SCHEDULE_THURSDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_THURSDAY_EVENT2_M,// 
	EEP_SCHEDULE_THURSDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_THURSDAY_EVENT3_M,//
	EEP_SCHEDULE_THURSDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_THURSDAY_EVENT4_M,//
	EEP_SCHEDULE_THURSDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_THURSDAY_EVENT5_M,//
	EEP_SCHEDULE_THURSDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_THURSDAY_EVENT6_M,//	
	EEP_SCHEDULE_FRIDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_FRIDAY_EVENT1_M,//
	EEP_SCHEDULE_FRIDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_FRIDAY_EVENT2_M,// 
	EEP_SCHEDULE_FRIDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_FRIDAY_EVENT3_M,//
	EEP_SCHEDULE_FRIDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_FRIDAY_EVENT4_M,//
	EEP_SCHEDULE_FRIDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_FRIDAY_EVENT5_M,//
	EEP_SCHEDULE_FRIDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_FRIDAY_EVENT6_M,//		
	EEP_SCHEDULE_SATDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SATDAY_EVENT1_M,//
	EEP_SCHEDULE_SATDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_SATDAY_EVENT2_M,// 
	EEP_SCHEDULE_SATDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SATDAY_EVENT3_M,//
	EEP_SCHEDULE_SATDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SATDAY_EVENT4_M,//
	EEP_SCHEDULE_SATDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SATDAY_EVENT5_M,//
	EEP_SCHEDULE_SATDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SATDAY_EVENT6_M,//	
	EEP_SCHEDULE_SUNDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SUNDAY_EVENT1_M,//
	EEP_SCHEDULE_SUNDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_SUNDAY_EVENT2_M,// 
	EEP_SCHEDULE_SUNDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SUNDAY_EVENT3_M,//
	EEP_SCHEDULE_SUNDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SUNDAY_EVENT4_M,//
	EEP_SCHEDULE_SUNDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SUNDAY_EVENT5_M,//
	EEP_SCHEDULE_SUNDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_SUNDAY_EVENT6_M,//		
	EEP_SCHEDULE_HOLIDAY_EVENT1_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_HOLIDAY_EVENT1_M,//
	EEP_SCHEDULE_HOLIDAY_EVENT2_H,// at work (sun + no man)
	EEP_SCHEDULE_HOLIDAY_EVENT2_M,// 
	EEP_SCHEDULE_HOLIDAY_EVENT3_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_HOLIDAY_EVENT3_M,//
	EEP_SCHEDULE_HOLIDAY_EVENT4_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_HOLIDAY_EVENT4_M,//
	EEP_SCHEDULE_HOLIDAY_EVENT5_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_HOLIDAY_EVENT5_M,//
	EEP_SCHEDULE_HOLIDAY_EVENT6_H,// at home (sun + man)				//EEP_SCHEDULE
	EEP_SCHEDULE_HOLIDAY_EVENT6_M,//
	EEP_SCHEDULE_MONDAY_FLAG,
	EEP_SCHEDULE_TUESDAY_FLAG = EEP_SCHEDULE_MONDAY_FLAG + 3,
	EEP_SCHEDULE_WENSDAY_FLAG = EEP_SCHEDULE_TUESDAY_FLAG + 3,
	EEP_SCHEDULE_THURSDAY_FLAG = EEP_SCHEDULE_WENSDAY_FLAG + 3,
	EEP_SCHEDULE_FRIDAY_FLAG = EEP_SCHEDULE_THURSDAY_FLAG + 3,
	EEP_SCHEDULE_SATDAY_FLAG = EEP_SCHEDULE_FRIDAY_FLAG + 3,
	EEP_SCHEDULE_SUNDAY_FLAG = EEP_SCHEDULE_SATDAY_FLAG + 3,
	EEP_SCHEDULE_HOLIDAY_FLAG = EEP_SCHEDULE_SUNDAY_FLAG + 3,
	EEP_SCHEDULE_DAY_BEGAIN = EEP_SCHEDULE_HOLIDAY_FLAG + 3,
	EEP_SCHEDULE_DAY_END = EEP_SCHEDULE_DAY_BEGAIN + 46,
	EEP_SLEEP_COOLING_DB,
	EEP_SLEEP_HEATING_DB,
	EEP_SLEEP_COOLING_SP,
	
	EEP_SLEEP_HEATING_SP = EEP_SLEEP_COOLING_SP + 2,
	
	EEP_AWAY_SP = EEP_SLEEP_HEATING_SP+2,
	
	EEP_AWAY_COOLING_SP = EEP_AWAY_SP + 2,
	
	EEP_AWAY_HEATING_SP = EEP_AWAY_COOLING_SP + 2,
	
	EEP_AWAY_COOL_DB = EEP_AWAY_HEATING_SP + 2,
	EEP_AWAY_HEAT_DB,
	
	EEP_FRC_ENABLE ,//free cooling  function enable/disable 0:DIABLE  1:ENABLE
	EEP_FRC_BASE_SELECT, //FREE COOLING BASE SELECT 0: TEMPERATUERE BASE 1: ENTHALPY BASE
	EEP_FRC_SPACETEM_UNIT,
	EEP_FRC_SUPPLYTEM_UNIT,
	EEP_FRC_OUTDOORTEM_UNIT,
	EEP_FRC_MIN_FRESH_AIR, // MINIMAL FRESH AIR, UNIT PERCENTAGE
	EEP_FRC_MIN_FRESH_AIRTEM_L, // MINIMAL FRESH AIR TEMPERATURE, UNIT DEGREE C/F
	EEP_FRC_MIN_FRESH_AIRTEM_H, // MINIMAL FRESH AIR TEMPERATURE, UNIT DEGREE C/F
	EEP_FRC_OUTPUT_SELECT, //SELECT USE WHICH OUTPUT TO CONTROL THE VALVE, AO1 OR AO2
	EEP_FRC_OUTPUT_CONFIG,  // FREE COOLING OUTPUT CHECK, OUTPUT SOURCE, OUTPUT FUNCTION
	EEP_FRC_PID2_CONFIG,   // FREE COOLING PID2 CONFIG CHECK, CHECK IF PID2 HAS INPUT FROM SUPPLY SENSOR, ALSO CHECK IF IT IS SET TO NEGATIVE COOLING
//	EEP_FRC_COOLING_DEADBAND, // FREE COOLING DEADBAND UNIT: DEGREE C/F
	EEP_FRC_OUTPUT_MODE,  // SET FREE COOLING OUTPUT MODE
//	EEP_FRC_FULL_FRESH,// SET FULL FRESH AIR INPUT
	EEP_FRC_TOTAL_CONFIG,// MINIMAL SUPPLY AIR TEMPERATURE UNIT: DEGREE C/F
	EEP_FRC_PRESENT_MODE,//	
//	EEP_BOARD_SELECT,
	EEP_INPUT_VOLTAGE_TERM,
//	EEP_ICON_DAY_OUTPUT_CONTROL,
//	EEP_ICON_NIGHT_OUTPUT_CONTROL,
//	EEP_ICON_OCC_OUTPUT_CONTROL,
//	EEP_ICON_UNOCC_OUTPUT_CONTROL,
//	EEP_ICON_HEAT_OUTPUT_CONTROL,
	TOTAL_EEP_VARIABLES ,
		
	EEP_FRC_SPACETEM_SOURCE,// FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
	EEP_FRC_SPACETEM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	
//	EEP_FRC_SPACETEM_LASTUPDATE = EEP_FRC_SPACETEM_ID +2,// FREE COOLING SPACE TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//	EEP_FRC_SPACETEM_STATUE,// CHECK IF SPACE TEMPERATURE SOURCE IS WORK WELL
//	EEP_FRC_SPACETEM_CONFIG,// FREE COOLING SPACE TEMPERATURE STATUS, CHECK RANGE,UNIT 	
	
	EEP_FRC_SUPPLYTEM_SOURCE = EEP_FRC_SPACETEM_ID + 2,// FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
	EEP_FRC_SUPPLYTEM_ID,// COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	
//	EEP_FRC_SUPPLYTEM_LASTUPDATE = EEP_FRC_SUPPLYTEM_ID + 2,// FREE COOLING SUPPLY TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//	EEP_FRC_SUPPLYTEM_STATUE,// CHECK IF SUPPLY TEMPERATURE SOURCE IS WORK WELL
//	EEP_FRC_SUPPLYTEM_CONFIG,// FREE COOLING SUPPLY TEMPERATURE STATUS, CHECK RANGE,UNIT 
	
	EEP_FRC_OUTDOORTEM_SOURCE = EEP_FRC_SUPPLYTEM_ID + 2,// FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
	EEP_FRC_OUTDOORTEM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	
//	EEP_FRC_OUTDOORTEM_LASTUPDATE = EEP_FRC_OUTDOORTEM_ID +2,// FREE COOLING OUTDOOR TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//	EEP_FRC_OUTDOORTEM_STATUE,// CHECK IF OUTDOOR TEMPERATURE SOURCE IS WORK WELL
//	EEP_FRC_OUTDOORTEM_CONFIG,// FREE COOLING OUTDOOR TEMPERATURE STATUS, CHECK RANGE,UNIT 	

	EEP_FRC_INDOORHUM_SOURCE = EEP_FRC_OUTDOORTEM_ID + 2,// FREE COOLING INDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
	EEP_FRC_INDOORHUM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
	
	EEP_FRC_INDOORHUM_LASTUPDATE = EEP_FRC_INDOORHUM_ID +2 ,// FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
	EEP_FRC_INDOORHUM_STATUE,// CHECK IF INDOOR HUMIDITY SOURCE IS WORK WELL
	EEP_FRC_INDOORHUM_CONFIG,// FREE COOLING INDOOR HUMIDITY STATUS, CHECK RANGE,UNIT
	
	EEP_FRC_OUTDOORHUM_SOURCE,// FREE COOLING OUTDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
	EEP_FRC_OUTDOORHUM_ID,//  COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK

	EEP_PWM_DUTY = EEP_FRC_OUTDOORHUM_ID + 2,
	EEP_SCHEDULE_MODE_NUM,
	EEP_STATION_NUMBER,
	EEP_INSTANCE1,
	EEP_INSTANCE2,
	EEP_INSTANCE3,
	EEP_INSTANCE4,
	EEP_4TO20MA_BOTTOM = EEP_INSTANCE4 + 2 ,                                 //A changable look up table for "custom" sensor 
           //the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
	EEP_4TO20MA_TOP = EEP_4TO20MA_BOTTOM + 2, //47		
		
	EEP_4TO20MA_UNIT_HI = EEP_4TO20MA_TOP + 2,
	
	EEP_4TO20MA_UNIT_LO = EEP_4TO20MA_UNIT_HI + 2,
	
	BAC_TEST1 = EEP_4TO20MA_UNIT_LO + 2,
	BAC_TEST2,
	BAC_TEST3,
	BAC_TEST4,
	BAC_TEST5,
	BAC_TEST6,
	BAC_TEST7,
	BAC_TEST8,
	TEST6,
	TEST7,
	TEST8,
	TEST9,
	TEST10
	
/*********************END*************************/


  	} et_menu_parameter ;



#define TOTAL_MENU_PARAMETERS	(EEP_FACTORY_DEFAULTS + 1 - EEP_MENU_ITEM_BEGINNING)
 
//--------------End of EEPROM data ------------------------------

typedef enum 
{ 
	TEST_MODE ,   //For testing of tstats
	FAN_COIL_COMMON_SEQUENCE, //fan coil common sequence
	CUSTOM_SEQUENCE, //custom squence.
	STANDARD_A_C_UNIT_SEQUENCE,
	HEAT_PUMP_SEQUENCE_FOR_HEATING,//heat pump sequence,
	HEAT_PUMP_SEQUENCE_FOR_COOLING,//heat pump sequence with reversing valve on for cooling
  TRANSDUCER_MODE ,   //6 custon tstat for Taiwan
	HVAC_TYPE,//add for gridpoint,heatpump mode
	TSTAT_SOP_TOTAL
	} et_sequence_of_operation ;



#define CALIBRATION_OFFSET 500 //allows us to store EEP_CALIBRATION as an unsigned int
#define CAL_EXTERN_OFFSET  500 //allows us to store CAL_EXTERN_OFFSET

#define CALIBRATION_DEFAULT 500
#define EXT_CALIBRATION_DEFAULT 500

#define DAC_OFFSET 100 

	//--------- default eeprom parameters --------------

#define LOWER  		0
#define UPPER  		1

#define DISP_COSTING_MODE   0
#define DISP_COOLING_MODE   1
#define DISP_HEATING_MODE   2

	
#define FAN_OFF 	0
#define FAN_AUTO 	4
#define FAN_ON		1
#define FAN_SPEED1 1
#define FAN_SPEED2 2
#define FAN_SPEED3 3

#define FAN_OFF_4C  		0
#define FAN_HEAT_4C  		1
#define FAN_COOL_4C  		2
#define FAN_GAS_4C  		3
#define FAN_AUTO_4C  		4

#define RECEIVE		0
#define SEND		1

#define COOLING3_PID	14   //these are the switching points for each stage
#define COOLING2_PID	26
#define COOLING1_PID	38
#define COOLING0_PID	50
#define HEATING0_PID	50   
#define HEATING1_PID	62
#define HEATING2_PID	74
#define HEATING3_PID	86

// following variables are for control vlave by pulse
#define TIMER_HIGH_BYTE		0xDC		//timer2 1 ms.    0xdc00 = 10ms,0xfc66 = 1ms
#define TIMER_LOW_BYTE		0x00


//#define RUN				0
//#define STOP			1

#define OPEN_MOTOR		1
#define CLOSE_MOTOR		0

#define REST        0
#define DRIVE_VALVE	1
#define INITIALIZE  2

#define EXTRA_CLOSE_MILLI_SECONDS	     5000		// 3 s = 3 * 1000 ms, this is the pulse at the end of travel to make sure the valve is open or closed. 
#define EXTRA_OPEN_MILLI_SECONDS	     5000		// 2 s = 2 * 1000 ms, this is the pulse at the end of travel to make sure the valve is open or closed. 
#define DAY_TICKS  (24L * 60 * 60 * 1000)
#define POSITION_TOLERANCE	5 
#define MIN_VALVE_POSITION	5 
#define MAX_VALVE_POSITION	95 
#define VALVE_MOVE_TRIGGER	1  //need xx stable readings before we trigger a new position

#define B1 0.0014
#define B2 0.1325
#define B3 (0.0317)
#define B4 (3.0876)
#define HUM_PERCENT 1
#define HUM_FREQ 0
#define HUM_FLOAT  100.0

#define ONOFF				0
#define ONE_TENVOLTS		1
#define ZERO_FIVEVOLTS		2
#define TWO_TENVOLTS		3
#define TEN_ZEROVOLTS		4


#define SERIAL_COM_IDLE		0
//#define INVALID_PACKET		1
//#define VALID_PACKET		2

//#define READ_VARIABLES      3
//#define WRITE_VARIABLES     6
//#define MULTIPLE_WRITE_VARIABLES 16
//#define CHECKONLINE			25

//#define DATABUFLEN			8
//#define DATABUFLEN_SCAN		12 //SERIAL BUFFER USE DURING SCAN
//#define DATABUFLEN_MUL		25 //SERIAL BUFFER MULTIPLE WRITES

//#define SENDPOOLLEN         8
#define RESPONSERANDVALUE	1

#define INIT_HEARTBEAT_COUNT 39  


#define  ONE_SECOND    10
#define  THREE_SECOND  30
#define  FIVE_SECOND   50

#define BLINK_OFF_TIME 3
#define BLINK_ON_TIME 5


#define STR_IN_DESCRIPTION_LENGTH  21
#define STR_IN_LABEL                9
//typedef  struct
//{
//	int8_t description[STR_IN_DESCRIPTION_LENGTH]; 	      /* (21 bytes; string)*/
//	int8_t label[STR_IN_LABEL];		      	/* (9 bytes; string)*/
//	int32_t value;		     						/* (4 bytes; int32_t)*/
//	uint8_t filter;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32, 6=64,7=128,)*/
//	uint8_t decom;/* (1 bit; 0=ok, 1=point decommissioned)*/
//	uint8_t sub_id;/* (1 bit)*/
//	uint8_t sub_product;  /* (1 bit)*/
//	uint8_t control; /*  (1 bit; 0=OFF, 1=ON)*/
//	uint8_t auto_manual; /* (1 bit; 0=auto, 1=manual)*/
//	uint8_t digital_analog ; /* (1 bit; 1=analog, 0=digital)*/
//	uint8_t calibration_sign; /* (1 bit; sign 0=positiv 1=negative )*/
//	uint8_t sub_number;//extend IO number 
//	uint8_t calibration_h; /* (5 bits - spare )*/  //??? unused ? calibration??????
//	uint8_t calibration_l;  /* (8 bits; -25.6 to 25.6 / -256 to 256 )*/
//	uint8_t range;	      			/* (1 uint8_t ; input_range_equate)*/
//	
//} Str_in_point; 
/*
description: free cooling input sensor structure
number:
source: local sensor or remote sensor
id: if use local sensor, then it will be AI1-AI8, internal sensor humidity and so on, if use remote sensor, it will show the number and AI of mini pannel
value: current value
update:last update time if use remote sensor
status:indicate if current value is legal and remote sensor come on time
cofig: indicate if the configuration of block is legal or not 
*/
typedef struct
{
	uint8 source;
	uint16 id;
	int16 value;
	uint8 update;
	uint8 status;
	uint8 config;
}STR_FRC_INPUT;




#ifndef TSTAT_ARM
typedef struct link
{
	uint8 eeprom[ TOTAL_EE_PARAMETERS  - MAXEEPCONSTRANGE] ;//parameter[12]  
	et_jump_function event_function[ TOTAL_EVENTS ] ; //TOTAL_EVENTS=8
	et_jump_function jump_function[ TOTAL_TIMERS ] ;  //TOTAL_TIMERS=6
	et_event event_queue[ EVENT_QUEUE_SIZE ]  ;       //EVENT_QUEUE_SIZE=3
	uint8 big_ticks[ TOTAL_TIMERS ] ;//9      //number of heartbeats x small_ticks, high gear notched evey ~ ms
};

typedef struct map_id_addr_
{
	et_menu_parameter id;
	int addr;
}map_id_addr;

#elif defined  TSTAT7_ARM
	typedef struct link
	{
		uint8 eeprom[TOTAL_EEP_VARIABLES];
		et_jump_function event_function[ TOTAL_EVENTS ] ; //TOTAL_EVENTS=8
		et_jump_function jump_function[ TOTAL_TIMERS ] ;  //TOTAL_TIMERS=6
		et_event event_queue[ EVENT_QUEUE_SIZE ]  ;       //EVENT_QUEUE_SIZE=3
		uint8 big_ticks[ TOTAL_TIMERS ] ;//9      //number of heartbeats x small_ticks, high gear notched evey ~ ms

	 }LINK;
#else

typedef struct
	{ 
	unsigned char eeprom[TOTAL_EEP_VARIABLES];
	}LINK;

#endif //nTSTAT_ARM

#endif   // _DEFINE_H_

