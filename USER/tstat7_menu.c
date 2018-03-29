
#include "tstat7_menu.h"
#include "tstat7_lcd.h"
#include "eepDefine.h"
#include "setpoint.h"
#include "pid.h"
#include "rtc.h"
#include "24cxx.h" 
#include "stdlib.h"
#include "string.h"
#include "delay.h"
#include "control_logic.h"
#include "revision.h"
#include "TstatFunctions.h"
#include "modbus.h"
#include "usart.h"
#include "bacnet.h"
#include "rs485.h"
#include "store.h"
#include "stmflash.h"

#ifdef TSTAT7_ARM
extern uint8 heat_cool_flag;
extern uint8 setpoint_change_flag;
// unsigned char uart_test;
#define  UP_CODE       0x20
#define  DOWN_CODE     0x10 
#define  MODE_CODE     0x08 
#define  MENU_CODE     0x01
#define  ENTER_CODE    0x02 
#define  OCCU_CODE     0x04

#define PIR_SENSOR_ENABLE       1
#define PIR_SENSOR_DISABLE      0

enum
{
	MENUSCHEDULE = 1,	
	CLOCK,
	ADVANCED
};

unsigned char const  MODEMENU1[2][9] =
{ 
	"HEATCOOL",
	"FANMODE ", 
};

unsigned char const  FANMODE[5][9]  =
{ 
	"MAN OFF ",
	"MAN LO  ",
	"MAN MED ",
	"MAN HI  ", 
	" -AUTO- ",
};
unsigned char const  HEATCOOL[3][9] =
{
	"MAN HEAT",
	"MAN COOL", 
	"AUTO H/C", 
}; 

enum
{
	HEATCOOLE,
	FANMODEE,
};	


enum
{ 
	MENUSTART,
//	WALL ,
//	CEILING, 
//	OUTDOOR,
//	AVERAGE,	 
//	NIGHT_HEAT,
//	NIGHT_COOL,	 
//	RHset, 		 	
	MENU_UNITS,		//17
	TIME_FORMAT, 
	NET_ID,
//	CAL,
	SOP,
	OCC_SEL,
	OCCTimer,
	BAUDRATE_MENU,
	PROTOCOL,
	BACK,
	SENSOR ,
};  

unsigned char const  MainMenu[4][8] = {
{"        "},
{"SCHEDULE"},	 
{"  CLOCK "},
{"ADVANCED"}, 	 
};

unsigned char const  SchweekMenu[3][8] = {
 
	{"WORK DAY"},
	{"WEEK END"},
	{" < BACK"},
 
} ;


union time{
	uint16 hm;
	uint8 tm[2];
};
 
 
enum
{ 
	YEAR = 1,
	MONTH,
	WEEK,
	DATE,
	HOUR,
	MINUTE,
	BACKy
	
}; 

enum
{
//	NULL,
	DAY = 1,		//0
	S_TIME1,
	S_MINUTE1,
	S_TIME2,
	S_MINUTE2,
	S_TIME3,
	S_MINUTE3,
	S_TIME4,
	S_MINUTE4,
 
	//BACK
};


enum
{
	TEMPE,
	TEMP,
	RHP,
 	NUMBER,
	TIME, 
	FORMAT,
	TEMP_UNITS,
	DISP_BAUDTATE,
	DISP_PROTOCOL,
	ERROR_1
};


unsigned char  const  SubMenu[14][9] = {//Added setpoint and fan mode to menu
	{"        "},
//	{" SENSOR "},
//	{"  WALL  "},		//00 CAL single point calibration of temperature
//	{" CEILING"},		//01 Temperature Sensor Select
//	{" OUTDOOR"},
//	{" AVERAGE"},		//02 Temperature filter
//	{"unocHEAT"},		//03 Analog input 1  
//	{"unocCOOL"},		//04 Analog input 2
//	{"  RH    "},		//05 Digital input 1 
	{" UNITS "},		//15 Cooling deadband
	{"Time Dis"},		//16 Heating deadband
//	{"HTstages"},		//17 Degree C or degree F
//	{"CLstages"},		//19 Select fan speed number 
	{" Net ID "},
//	{"   CAL  "},
	{"   SOP  "},
	{"OCC SEL"},
	{"OCCTIMER"},
	{"BAUDRATE"},
	{"PPROTOCOL"},
	{" < BACK "},

}; 
union list
{
  int16    new_setpoint ;
  int16    new_parameter ;
};

static union list menubuffer;

uint8 event_count = 0;
//*********************************start menu system defination************************************//
uint8 gucDay;
uint8 gucEvent = 0;
int16 gsiPara;
uint8 gucHour;
uint8 gucMinute;
uint8 gbSetSchedule = 0;
uint8 const guMonth[12][3] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};


uint8 gucItemClockMenu,gucItemScheduleMenu,gucItemAdvancedMenu,gucItemModeMenu;
uint8 gbModeMenu;
uint8 gucSelectButton =0;
uint8 gucItemMainMenu;
uint8 tempDay = 1;
uint8 guTime[7]; 
uint8 DispBuf[9]; 

uint8 gbClock = 0;
uint8 gucMenuKey =0;
uint8 gbScheduleMenu,gbClockMenu,gbAdvancedMenu;
uint8 STOP_DECIMAL = 0;
uint8 MenuOnLcd = 0;
uint8 gbHmFlag = 1;
uint8 gucMenuevent = 0;
uint8 gbScheduleTime = 0;
uint8 gbFlashTime=0;

uint8 gbFlashHour;
uint8 gbFlashMinute;
uint8 gbFlashWeek;
uint8 gbFlashMonth;
uint8 gbFlashDate;
uint8 gbFlashYear;

uint8 gbFlashHourEnable;
uint8 gbFlashMinuteEnable;
uint8 gbFlashWeekEnable;
uint8 gbFlashMonthEnable;
uint8 gbFlashDateEnable;
uint8 gbFlashYearEnable;
uint8 Fire;
uint8 Snow;
uint8 Lowfan;
uint8 Midfan;
uint8 Highfan;
uint8 Setting;

uint8 gucPointValue ;
static uint8 setpoint_adjust_flag = 0;
extern et_menu_parameter item_to_adjust;

extern uint8  const code init_big_ticks[ TOTAL_TIMERS ];
extern void init_extern_operation_eeprom(unsigned char sop_num);
extern void ConverteCF(void);
extern void SoftReset(void);
extern void refresh_setpoint(uint8 day_night);

 
unsigned char new_read_eeprom(unsigned int ReadAddr)
{
	unsigned char read_temp = 0;
	read_temp = AT24CXX_ReadOneByte(ReadAddr);
	return read_temp;
}


void new_write_eeprom(unsigned int WriteAddr, unsigned char DataToWrite)
{
	AT24CXX_WriteOneByte(WriteAddr, DataToWrite);
}
static void init_timers( )
{
//	et_timer timer_no ;
	int8 timer_no;
    for( timer_no = 0 ; timer_no < TOTAL_TIMERS ; timer_no++ )
	{
		 b.big_ticks[ timer_no ] = DEFAULT_TIMEOUT;
	}

}


///////////////////////////////////////////////////
// -------------start_timer -----------------------------
// the routine start up a timer event , and the event run after timeout.
static void start_timer( et_timer timer_no , unsigned char timeout )
						// timer_no is the software timer to start
						// timeout is the value	to set the timer to
						//     if its DEFAULT_TIMEOUT then go to the normal timer table	
						//     if its nonzero, then go with the	value stored in	'timeout'
{
	if( timeout == DEFAULT_TIMEOUT )	// DEFAULT_TIMEOUT = 0
	{
		b.big_ticks[ timer_no] = init_big_ticks[timer_no ];	//load the timer with the normal default timer value
	}
	else
	{
		b.big_ticks[ timer_no ]	= timeout;    //load the timer with a custom value
	}
}


///---------------stop_timer -----------------
//  stop timer event immedately.
static void stop_timer( et_timer timer_no )
{
  
  b.big_ticks[ timer_no	] = DEFAULT_TIMEOUT;

}


static void push_event( et_event event_type )
{
	if( event_count	< EVENT_QUEUE_SIZE )  //MAximum	of three events	QUEUE'd
	{
		b.event_queue[ event_count++ ] = event_type;
	}
} 

static void start_normal_mode(	void )
{ 
	b.event_function[ UP_KEY ]	= 	(et_jump_function)INCREASE_COOLING_SETPOINT; 
	b.event_function[ DOWN_KEY	] = (et_jump_function)DECREASE_COOLING_SETPOINT; 
	b.event_function[ MODE_KEY	] = (et_jump_function)MODE_SWITCH; 
	
	b.event_function[ MENU_KEY ] = 	(et_jump_function)START_MENU_MODE ;  
	b.event_function[ ENTER_KEY ] = (et_jump_function)SELECT_FUNCTION;  
	  
	b.event_function[ OCCU_KEY ] = 	(et_jump_function)OCCUPIED_UNOCCUPIED_SWITCH;
} 



//there are several type data could be dislay on the LCD,temp or clcok or number
unsigned char  *DealwithPara(signed int value,unsigned char type) 
{
 

	if(type == TEMPE)
	{
		DispBuf[0] = ' ';
		DispBuf[1] = ' ';
		DispBuf[7] = ' ';
		DispBuf[2] = value/1000;
		DispBuf[3] = (value - DispBuf[2]*1000)/100 + 0x30;
		if(DispBuf[2] == 0)
		DispBuf[2] = ' ';
		else
		DispBuf[2] += 0x30;
		DispBuf[5] = '.';
		DispBuf[4] = value%100/10 + 0x30;
		DispBuf[6] = value%10 + 0x30;
	}

	else if(type == TEMP)
	{
		DispBuf[0] = ' ';
		DispBuf[1] = ' ';
		DispBuf[7] = ' ';
		DispBuf[2] = value/1000;
		DispBuf[3] = (value - DispBuf[2]*1000)/100 + 0x30;
		if(DispBuf[2] == 0)
		DispBuf[2] = ' ';
		else
		DispBuf[2] += 0x30;
		DispBuf[4] = '.';
		DispBuf[5] = value%100/10 + 0x30;
		DispBuf[6] = value%10 + 0x30;
	}
	else if(type == RHP)
	{
		DispBuf[0] = ' ';
		DispBuf[1] = ' ';
		DispBuf[7] = 37;
		DispBuf[2] = value/1000;
		DispBuf[3] = (value - DispBuf[2]*1000)/100 + 0x30;
		if(DispBuf[2] == 0)
		DispBuf[2] = ' ';
		else
		DispBuf[2] += 0x30;
		DispBuf[4] = '.';
		DispBuf[5] = value%100/10 + 0x30;
		DispBuf[6] = value%10 + 0x30;
	}

	else if(type == NUMBER)
	{		 
		DispBuf[2] = ' ';	 
		if(value < 10)
		{
				DispBuf[5] = ' ';		 
				DispBuf[4] = ' ';
				DispBuf[3] = value%10 + 0x30;
		}
		else if(value < 100)
		{
				DispBuf[5] = ' ';		 
				DispBuf[3] = value/10 + 0x30;
				DispBuf[4] = value%10 + 0x30;
		}
		else
		{
				DispBuf[3] = value/100 + 0x30;		 
				DispBuf[4] = value%100/10 + 0x30;
				DispBuf[5] = value%10 + 0x30;
		}
		DispBuf[6] = ' ';
	DispBuf[0] = ' ';
	DispBuf[1] = ' ';
	DispBuf[7] = ' ';
	 
	}
	
	else if(type == DISP_BAUDTATE)
	{
		switch(value)
		{
			case BAUDRATE_9600 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = ' ';
				DispBuf[2] = '9';
				DispBuf[3] = '6';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;
			case BAUDRATE_19200 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '1';
				DispBuf[2] = '9';
				DispBuf[3] = '2';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;
			case BAUDRATE_38400 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '3';
				DispBuf[2] = '8';
				DispBuf[3] = '4';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;
			
			case BAUDRATE_57600 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '5';
				DispBuf[2] = '7';
				DispBuf[3] = '6';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;			
			case BAUDRATE_76800 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '7';
				DispBuf[2] = '6';
				DispBuf[3] = '8';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;				

			case BAUDRATE_115200 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '1';
				DispBuf[2] = '1';
				DispBuf[3] = '5';
				DispBuf[4] = '2';
				DispBuf[5] = '0';
				DispBuf[6] = '0';
				DispBuf[7] = ' ';
			break;
			case BAUDRATE_1200 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = ' ';
				DispBuf[2] = '1';
				DispBuf[3] = '2';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;	
			case BAUDRATE_4800 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = ' ';
				DispBuf[2] = '4';
				DispBuf[3] = '8';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;	
			case BAUDRATE_14400 :
				DispBuf[0] = ' ';		 
				DispBuf[1] = '1';
				DispBuf[2] = '4';
				DispBuf[3] = '4';
				DispBuf[4] = '0';
				DispBuf[5] = '0';
				DispBuf[6] = ' ';
				DispBuf[7] = ' ';
			break;			
			
		}
	}		
	
	

	else if(type == DISP_PROTOCOL)
	{
	 if(value == 0)//modbus protocol
	 {
		DispBuf[0] = ' ';
		DispBuf[1] = 'M';
		DispBuf[2] = 'O';
		DispBuf[3] = 'D';
		DispBuf[4] = 'B';
		DispBuf[5] = 'U';	
		DispBuf[6] = 'S';
		DispBuf[7] = ' ';		 
	 }
	 else
	 {
		DispBuf[0] = ' ';
		DispBuf[1] = 'B';
		DispBuf[2] = 'A';
		DispBuf[3] = 'C';
		DispBuf[4] = 'N';
		DispBuf[5] = 'E';	
		DispBuf[6] = 'T';
		DispBuf[7] = ' ';		 
	 }
	}

	else if(type == TIME)
	{
	DispBuf[0] = ' ';
	DispBuf[1] = ' ';
	DispBuf[7] = ' ';
		 
		DispBuf[2] = gucHour/10 + 0X30;
		DispBuf[3] = gucHour%10 + 0X30;		 
		DispBuf[4] = ':';
		DispBuf[5] = gucMinute/10 + 0X30;
		DispBuf[6] = gucMinute%10 + 0X30;
	}
 
	else if(type == FORMAT)
	{
	DispBuf[0] = ' ';
	DispBuf[1] = ' ';
	DispBuf[7] = ' ';
		if(value == 0)
		{			 
			DispBuf[2] = '1';		 
			DispBuf[3] = '2';
		 
		}
		else
		{		
			DispBuf[2] = '2';		 
			DispBuf[3] = '4';
		
		}
 		DispBuf[4] = ' ';
		DispBuf[5] = 'H';
		DispBuf[6] = 'r';
	}
	else if(type == TEMP_UNITS)
	{
		if(value == 0)
		{			 	 
			DispBuf[4] = 'C';			 
			 
		}
		else
		{			 
			DispBuf[4] = 'F';
		
		}
	DispBuf[0] = ' ';
	DispBuf[1] = ' ';
	DispBuf[7] = ' ';
 		DispBuf[2] = ' ';
		DispBuf[3] = ' ';	
		DispBuf[5] = ' ';
		DispBuf[6] = ' ';
	}
	else //if(type == ERROR)
	{
			if(value == 0)
			{
				DispBuf[0] = 'W';
				DispBuf[1] = 'A';		 
				DispBuf[2] = 'K';
			}
			else if(value == 1)
			{
				DispBuf[0] = 'A';
				DispBuf[1] = 'W';	 
				DispBuf[2] = 'A';
			}
			else if(value == 2)
			{
				DispBuf[0] = 'H';
				DispBuf[1] = 'O';	 
				DispBuf[2] = 'M';
			}
			DispBuf[3] = gucHour/10 + 0X30;
			DispBuf[4] = gucHour%10 + 0X30;		 
			DispBuf[5] = ':';
			DispBuf[6] = gucMinute/10 + 0X30;
			DispBuf[7] = gucMinute%10 + 0X30;	
	} 
	
	DispBuf[8] = 0;	
	return DispBuf;
 
}

// void CheckScheduleSet(number)
//{
//	uint8 temp;
//	union time current,previous; 
//	temp = (gucDay - 1)*8 + (gucEvent - 1)*2;
// 	current.tm[0] = gucHour;
//	current.tm[1] = gucMinute;
//	previous.tm[0]= new_read_eeprom(EEP_DAY1_EVENT1_HI + temp - 2);
//	previous.tm[1]= new_read_eeprom(EEP_DAY1_EVENT1_LO + temp - 2); 
//	if(current.hm <= previous.hm)
//	{ 
//		putText(0,(uint8 *)"OVERLAP!");
//		gucHour = previous.tm[0];
//		gucMinute = previous.tm[1];	  
//		DealwithPara(number,ERROR_1);
//		putText(1,DispBuf); 
//		gucEvent--;
//		gbScheduleTime = 1;
// 	}	
//	else
//		gbScheduleTime = 0;
//	number = 0;
//}




static void start_setpoint_mode( )
{
	int16 temp_signed_long ; 
	
	setpoint_adjust_flag = 1 ;  //	user just hit the keypad, 
								// this	flag lets us use the first hit to 
								// switch modes, don't adjust the setpoint on first hit
	STOP_DECIMAL = 1; 
	switch ( item_to_adjust	)
	{
	  case ADJUSTING_COOLING :
		if(b.eeprom[EEP_DISPLAY ] < 3  || b.eeprom[EEP_DISPLAY ] > 4)
				temp_signed_long = ((int16)EEP_DaySpHi<<8) + EEP_DaySpLo;
		else
			temp_signed_long = ((int16)b.eeprom[EEP_UNIVERSAL_SET  + 1]<<8)+b.eeprom[EEP_UNIVERSAL_SET] ;	
		menubuffer.new_setpoint = (int16)temp_signed_long;

	   	putDigitals2(temp_signed_long/10); 

		break ; 
		
	} //--------------endswitch--------- 
	//when the setpoint mode times out, the	new setpoint will be accepted
 	b.jump_function[ KEYPAD_ACTIVE_TIMER ] = ACCEPT_NEW_SETTING;  // This will accept the last value shown on the display								//in normal use, the user has no "OK" key
	start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT );
}  //end of start_setpoint_mode() ----------
 

 

//=================================================
//  Accept a new setpoint from the user
//  can	share this with	all setpoints and fan speed adjustmentsz
//=================================================
static void accept_new_setting( )
{
//	uint16 itemp,temp_i;
	STOP_DECIMAL = 0;
	switch ( item_to_adjust	)
	{
	//cc 09-28-2012 change below code to match new setpoint function 
		case ADJUSTING_COOLING :  //use this for single setpoint, and for cooling on 2 setpoint systems
//			if(EEP_Display < 3 || EEP_Display > 4)
//			{ 
					EEP_DaySpLo = menubuffer.new_setpoint&0xff;
					EEP_DaySpHi =( menubuffer.new_setpoint>>8)&0xff;
		      loop_setpoint[0] = menubuffer.new_setpoint;
					Set_day_setpoint(EEP_DaySpHi, EEP_DaySpLo);
					init_PID_flag |= 0x01;
					refresh_setpoint(DAY_MODE);
					setpoint_change_flag = 0;
//			}
//			else
//			{
//				EEP_UniversalSetpointLo = menubuffer.new_setpoint&0xff;
//				EEP_UniversalSetpointHi =( menubuffer.new_setpoint>>8)&0xff;
//				new_write_eeprom( EEP_UNIVERSAL_SET , EEP_UniversalSetpointLo) ;
//				new_write_eeprom( EEP_UNIVERSAL_SET + 1 , EEP_UniversalSetpointHi) ;
//			} 
//			setpoint();
	break ; 
	case ADJUSTING_HEATING :  //use this for heating of two setpoint systems only
    	 
		break; 
	}
	stop_timer( KEYPAD_ACTIVE_TIMER	) ;
	setpoint_adjust_flag = 0 ;  //when user presses heat/cool/heat, we must take care 
								// that this flag is not destroyed

	//for some reason, we need a comment here, compiler is complaining
//	stop_blink() ;
//	if(b.eeprom[EEP_SPECIAL_MENU_LOCK - MAXEEPCONSTRANGE] == 3)
//	{
//		b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
//		start_timer( KEYPAD_ACTIVE_TIMER , MENU_TIMEOUT	);
//		if(item_to_adjust == ADJUSTING_COOLING)
//		item_to_adjust = EEP_FACTORY_DEFAULTS + 2;
//		else if(item_to_adjust == ADJUSTING_FAN_SPEED)
//		item_to_adjust = EEP_FACTORY_DEFAULTS + 3;
//		start_menu();
//	}
//	else
		start_normal_mode() ; 
} //----end of accept_new_setting( ) ------------


//=================================================
//  Increase the cooling_setpoint by one Degree
//=================================================
static void increase_cooling_setpoint(	)
{ 
	int16   temp_signed_long ;
	if(setpoint_adjust_flag)
	{ 
		if(item_to_adjust != ADJUSTING_COOLING )	//This is the first time we are in this routine, 
		{
			accept_new_setting( );
			item_to_adjust = ADJUSTING_COOLING ; 
			start_setpoint_mode() ;  //start flashing mode
		} 
		else
		{
			if(b.eeprom[EEP_AUTO_ONLY ] != 2)
			{
				if(b.eeprom[EEP_DISPLAY ] < 3 || b.eeprom[EEP_DISPLAY ] > 4)
					menubuffer.new_setpoint = menubuffer.new_setpoint + b.eeprom[EEP_SETPOINT_INCREASE ] ;
				else//PID2 setpoint
				{
					/*if(b.eeprom[EEP_INPUT1_SELECT - MAXEEPCONSTRANGE])
						{		
							if(menubuffer.new_setpoint	< (int)((new_read_eeprom(EEP_TABLE1_FIVE + 1 + (b.eeprom[EEP_INPUT1_SELECT - MAXEEPCONSTRANGE] - 1)*22) << 8) + new_read_eeprom(EEP_TABLE1_FIVE + (b.eeprom[EEP_INPUT1_SELECT - MAXEEPCONSTRANGE] - 1)*22)))
							menubuffer.new_setpoint++ ;
							else
							menubuffer.new_setpoint = (int)((new_read_eeprom(EEP_TABLE1_FIVE + 1 + (b.eeprom[EEP_INPUT1_SELECT - MAXEEPCONSTRANGE] - 1)*22) << 8) + new_read_eeprom(EEP_TABLE1_FIVE + (b.eeprom[EEP_INPUT1_SELECT - MAXEEPCONSTRANGE] - 1)*22));
						}*/	
					if(b.eeprom[EEP_DISPLAY ] >= 3 && b.eeprom[EEP_DISPLAY ] <= 4)
						menubuffer.new_setpoint++ ;			
				}
			}

			temp_signed_long = (int16)(b.eeprom[EEP_MAX_SETPOINT ]*10);

			if(b.eeprom[EEP_DISPLAY ] < 3 || b.eeprom[EEP_DISPLAY] > 4)//now only temperature has setpoint limitation
				if( menubuffer.new_setpoint	> temp_signed_long)/* && b.eeprom[EEP_KEYPAD_SELECT - MAXEEPCONSTRANGE] == 3 */
				{
					menubuffer.new_setpoint	= temp_signed_long;
				}
			temp_signed_long = menubuffer.new_setpoint;//mul( menubuffer.new_setpoint , 10 ) ;
			// If the setpoint is greater than 99.9 degrees, we need to shift the decimal point over		 
			setpoint_change_flag = 1;
			loop_setpoint[0] = temp_signed_long;
			putDigitals2(temp_signed_long/10);
			start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT );
		} 
	}
	else
	{
		item_to_adjust = ADJUSTING_COOLING ; 		
		start_setpoint_mode() ;  
	}  
} 
//=================================================
//  Decrease the cooling setpoint by one Degree
//=================================================
 
static void decrease_cooling_setpoint()
{ 
	int16 temp_signed_long ; 
	//setpoint_adjust_flag && b.eeprom[EEP_SETPOINT_CONTROL - MAXEEPCONSTRANGE] == 0)	     //user has	hit the	keypad,	first hit just switches	to this	adjusting mode
	if(setpoint_adjust_flag)
	{ 
		if(item_to_adjust != ADJUSTING_COOLING)		
		{
			accept_new_setting( );
			item_to_adjust = ADJUSTING_COOLING; 
			start_setpoint_mode() ;  
		}

		else
		{
			if(b.eeprom[EEP_AUTO_ONLY ] != 2 )
			{
				if(b.eeprom[EEP_DISPLAY ] < 3 || b.eeprom[EEP_DISPLAY ] > 4)//temperture setpoint
				menubuffer.new_setpoint = menubuffer.new_setpoint - b.eeprom[EEP_SETPOINT_INCREASE ] ;
				else//PID2 setpoint
				//if(menubuffer.new_setpoint > 0 && b.eeprom[EEP_DISPLAY - MAXEEPCONSTRANGE] >= 3 && b.eeprom[EEP_DISPLAY - MAXEEPCONSTRANGE] <= 4)
				if(b.eeprom[EEP_DISPLAY ] >= 3 && b.eeprom[EEP_DISPLAY ] <= 4)
				menubuffer.new_setpoint-- ;
			}

			temp_signed_long = (int16)(b.eeprom[EEP_MIN_SETPOINT ]*10);

			if(b.eeprom[EEP_DISPLAY ] < 3 || b.eeprom[EEP_DISPLAY ] > 4)//now only temperature has setpoint limitation
				if(	menubuffer.new_setpoint	< temp_signed_long)
				{  
					menubuffer.new_setpoint	= temp_signed_long; 
				}

			temp_signed_long = menubuffer.new_setpoint;		//mul(	menubuffer.new_setpoint	, 10 ) ;
			// If the setpoint is greater than 99.9 degrees, we need to shift the decimal point over
			loop_setpoint[0] = temp_signed_long;
			setpoint_change_flag = 1; 
			putDigitals2(temp_signed_long/10);  
			start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT) ; 
		}
	}
	else
	{
		item_to_adjust = ADJUSTING_COOLING ; 		
		start_setpoint_mode() ;  
	}  
}


static uint8 get_inverse(uint8 value)
{
	if(value == 0)	return 1;
	else 	return 0;
}


static void ShowTime( void )
{
	 
// 	unsigned char xdata DispBuf[9];
	if(gbFlashYearEnable == 1)
	{
		gbFlashYear =  get_inverse(gbFlashYear);
	}
	else
		gbFlashYear  = 0;
	if(gbFlashMonthEnable == 1)
	{
		gbFlashMonth =  get_inverse(gbFlashMonth);
	}
	else
	gbFlashMonth = 0;
	if(gbFlashDateEnable == 1)
	{
		gbFlashDate =  get_inverse(gbFlashDate);
	}
	else
	gbFlashDate  = 0;			   
	
	if(gbFlashHourEnable )
	{ 
		gbFlashHour = get_inverse(gbFlashHour);
	}
	else
	{
		gbFlashHour = 0;
	}
	if(gbFlashMinuteEnable )
	{

		gbFlashMinute = get_inverse(gbFlashMinute);
	}
	else
	{
		gbFlashMinute = 0;
	}
	 
	RTC_Get();
	
	if(gbFlashYearEnable == 0)
		guTime[0] = calendar.w_year;//year
	if(gbFlashMonthEnable == 0)
		guTime[1] = calendar.w_month;//month
	if(gbFlashWeekEnable == 0)
		guTime[2] = calendar.week;//week
	if(gbFlashDateEnable == 0)
		guTime[3] = calendar.w_date;//date
	if(gbFlashHourEnable ==0 )
		guTime[4] = calendar.hour;//hour
	if(gbFlashMinuteEnable == 0)
		guTime[5] = calendar.min;//minute
		guTime[6] = calendar.sec;
	
		if(b.eeprom[EEP_SYSTEM_TIME_FORMAT ] == 0)  	// 12h
		{
			if(guTime[4] == 12) 	putTime(guTime[4],guTime[5],gucPointValue,gbFlashHour,gbFlashMinute); 	//  chelsea 
		 	else putTime(guTime[4] % 12,guTime[5],gucPointValue,gbFlashHour,gbFlashMinute); 					//  chelsea 
			//if(gbFlash)
			//putTime(0,guTime[5],1);
		}
		else   // 24h
		{
			putTime(guTime[4],guTime[5],gucPointValue,gbFlashHour,gbFlashMinute); //  chelsea 
		}
	
		putWeek(guTime[2]);
	
	
	//	if(override_timer == OFF)
		//{
			DispBuf[0] = 0x20;
		 	DispBuf[1] = guMonth[guTime[1]-1][0];
			DispBuf[2] = guMonth[guTime[1]-1][1];
			DispBuf[3] = guMonth[guTime[1]-1][2];
			
			DispBuf[4] = 0x20;
			DispBuf[5] = 0x30 + guTime[3]/10;
			DispBuf[6] = 0x30 + guTime[3]%10;
			DispBuf[7] = 0x20;
	
			if(gbFlashMonth == 1)
			{
				DispBuf[1] = 0x20;
				DispBuf[2] = 0x20;
				DispBuf[3] = 0x20;
			}
			if(gbFlashDate== 1)
			{
				DispBuf[5] = 0x20;
				DispBuf[6] = 0x20;	 
			}
			DispBuf[8] = 0;
		if(gbClock)
			putText(0,(unsigned char*)DispBuf); 
	if(gbFlashYearEnable == 1)
	{
	
	
			DispBuf[0] = 0x20;
		 	DispBuf[1] = 0x20;
			DispBuf[2] = 0x30+2;
			DispBuf[3] = 0x30+0;
			
			DispBuf[4] = 0x30+(guTime[0]%100)/10;
			DispBuf[5] = 0x30 + guTime[0]%10;
			DispBuf[6] = 0x20;
			DispBuf[7] = 0x20;
			DispBuf[8] = 0;
	
	if(gbFlashYear == 1)
	{
			DispBuf[0] = 0x20;
		 	DispBuf[1] = 0x20;
			DispBuf[2] = 0x30+2;
			DispBuf[3] = 0x30+0;
			
			DispBuf[4] = 0x20;
			DispBuf[5] = 0x20;
			DispBuf[6] = 0x20;
			DispBuf[7] = 0x20;
			DispBuf[8] = 0;
	}
		putText(1,DispBuf); 
	
	}
	if((MenuOnLcd == 0)&&(gbFlashMonthEnable == 1|| gbFlashDateEnable == 1 || gbFlashWeekEnable == 1 || gbFlashHourEnable == 1 || gbFlashMinuteEnable == 1))
	{
	
			DispBuf[0] = 0x20;
		 	DispBuf[1] = 0x20;
			DispBuf[2] = 0x30+2;
			DispBuf[3] = 0x30+0;
			
			DispBuf[4] = 0x30+(guTime[0]%100)/10;
			DispBuf[5] = 0x30 + guTime[0]%10;
			DispBuf[6] = 0x20;
			DispBuf[7] = 0x20;
			DispBuf[8] = 0;
	 
		putText(1,DispBuf); 
	
	} 	 
}


static void FlashTime(unsigned char minute,unsigned char hour,unsigned char hm)
{
	gbFlashTime =  get_inverse(gbFlashTime);

	DispBuf[0] = ' ';
	DispBuf[1] = ' ';
	DispBuf[7] = ' ';
	DispBuf[8] = 0;	 
	DispBuf[2] = hour/10 + 0X30;
	DispBuf[3] = hour%10 + 0X30;		 
	DispBuf[4] = ':';
	DispBuf[5] = minute/10 + 0X30;
	DispBuf[6] = minute%10 + 0X30;
	if(gbFlashTime )
	{
		if(hm)//houre flashing
		{
			DispBuf[2] = ' ';
			DispBuf[3] = ' ';	
		}
		else //minute flashing
		{
			DispBuf[5] = ' ';
			DispBuf[6] = ' ';	
		}
	}
 
	putText(1,DispBuf);

}

static void ScheduleMenu( void )
{
	unsigned char temp;
//	unsigned char xdata DispBuf[9];
	gbSetSchedule = 0;
	switch(gucItemScheduleMenu)
	{
//		case DAY:
//			gsiPara = tempDay;
//			gucDay = tempDay;
//			putText(1,(uint8 *)SchweekMenu[gsiPara-1]);
//			if(tempDay == 3)
//					putText(0,(uint8 *)"   GO   ");
//			else
//				putText(0,(uint8 *)"DAY SET ");
//			break;
//		/*case EVENT:
//			gsiPara = tempEvent;
//			gucEvent  = tempEvent;
//			putText(1,eventMenu[gsiPara-1]);
//		break;*/   
//		case S_TIME1:
//		case S_TIME2:
//		case S_TIME3:
//		case S_TIME4:
//			temp = gucItemScheduleMenu - S_TIME1;
//		 	temp = temp >> 1;
//			
//			if(temp == 0)   strcpy((char *)DispBuf,(char *)"OCCUPIED");
//			else 			strcpy((char *)DispBuf,(char *)"UNOCCUP");
//				
////			for(i=0;i<8;i++)
////				DispBuf[i] = new_read_eeprom(WAKE_CHAR1 - (temp)*8 - i);	
//		
//			DispBuf[8] =  0;
//			putText(0,DispBuf);  
//			delay_ms(4); 
//			if(gucMenuevent == 0)
//			{
//				if(gucEvent < 2)	gucEvent ++;	
//				else gucEvent = 1;	
//				}
//		
//			temp = (gucDay - 1)*8 + (gucEvent - 1)*2;// chelsea
//			gsiPara = new_read_eeprom(EEP_DAY1_EVENT1_HI + temp);   
//			if(gsiPara > 23)
//			gsiPara  = 12;
//			gucHour = gsiPara;
//			gucMinute = new_read_eeprom(EEP_DAY1_EVENT1_LO + temp); 
//		 	gbSetSchedule = 1;
//		 //	putText(1,DealwithPara(gsiPara,TIME));
//		//	FlashTime(gucMinute,gucHour,1);
//			gbHmFlag = 1;
//			break;
//		case S_MINUTE1:
//		case S_MINUTE2:
//	 	case S_MINUTE3:
//		case S_MINUTE4:
//			temp = gucItemScheduleMenu - S_MINUTE1;	
//			temp = temp >> 1;
//		
//			if(temp == 0)   strcpy((char *)DispBuf,(char *)"OCCUPIED");
//			else 			strcpy((char *)DispBuf,(char *)"UNOCCUP");
////			for(i=0;i<8;i++)
////			DispBuf[i] = new_read_eeprom(WAKE_CHAR1 - (temp)*8 - i);		 
//			
//		    DispBuf[8] =  0;

//			putText(0,DispBuf); 
//			delay_us(4000); 

//			temp = ( gucDay - 1)*8 + (gucEvent - 1)*2;// chelsea
//			gsiPara =  new_read_eeprom(EEP_DAY1_EVENT1_LO + temp);  
//			if(gsiPara > 59)
//			gsiPara  = 0;
//			gucHour = new_read_eeprom(EEP_DAY1_EVENT1_HI + temp); 
//			gucMinute = gsiPara;
//	 		gbSetSchedule = 1;
//		 //putText(1,DealwithPara(gsiPara,TIME));
//			//FlashTime(gucMinute,gucHour,0);
//			gbHmFlag = 0;
//			break;

		default:
			break;
	}
 	gucMenuevent = 0;
}







static void AdvancedMenu(void)
{
 
//	unsigned char xdata DispBuf[9];
	gbSetSchedule = 0;
	if(gucItemAdvancedMenu == BACK)
	{
		putText(0,(uint8 *)"   GO   ");		 
		putText(1,(uint8 *)" < BACK "); 
	}
//	else if(gucItemAdvancedMenu == SENSOR)
//	{
//		putText(0," SENSOR ");		 
//		for(i=0;i<8;i++)
//		DispBuf[i] = new_read_eeprom(INTERNAL_CHAR1 - i);
//		putText(1,DispBuf);  
//		gsiPara = 0;
//	}
//	else if(gucItemAdvancedMenu < UNITS)	
//	{
//		gsiPara = (signed int)(b.eeprom[EEP_WALL_SETPOINT- MAXEEPCONSTRANGE + 1 + ((gucItemAdvancedMenu - 2) << 1)] << 8) + b.eeprom[EEP_WALL_SETPOINT- MAXEEPCONSTRANGE + ((gucItemAdvancedMenu - 2) << 1)];
// 
//		if(gucItemAdvancedMenu < 6)
//		{
//			for(i=0;i<8;i++)
//			DispBuf[i] = new_read_eeprom(WALL_CHAR1 - (gucItemAdvancedMenu-2)*8 - i);
//			DispBuf[8] =0;
//			putText(0,DispBuf); 
//		}
//		else
//			putText(0,SubMenu[gucItemAdvancedMenu]);
//	
//		if(gucItemAdvancedMenu<RHset)
//			putText(1,DealwithPara(gsiPara,TEMP));
// 		if(gucItemAdvancedMenu == RHset)
//			putText(1,DealwithPara(gsiPara,RHP));
//
//	}
	 
	else if(gucItemAdvancedMenu < BACK)
	{
		switch(gucItemAdvancedMenu)
		{
			case MENU_UNITS:
				gsiPara = b.eeprom[EEP_DEGC_OR_F];
				DealwithPara(gsiPara,TEMP_UNITS);
				putText(1,DispBuf);
			break;
			case TIME_FORMAT:
				gsiPara = b.eeprom[EEP_SYSTEM_TIME_FORMAT ];
				DealwithPara(gsiPara,FORMAT);
				putText(1,DispBuf);
			break;
			 
			case NET_ID:
				gsiPara = laddress;		//b.eeprom[EEP_COOL_ORIGINAL_TABLE - MAXEEPCONSTRANGE];
				DealwithPara(gsiPara,NUMBER);
				putText(1,DispBuf);
			 
			break;
		 
			case SOP:
			 	gsiPara = b.eeprom[EEP_SEQUENCE];		//b.eeprom[EEP_COOL_ORIGINAL_TABLE - MAXEEPCONSTRANGE];
				DealwithPara(gsiPara,NUMBER);
				putText(1,DispBuf);
				break;
			case OCC_SEL:
				gsiPara = new_read_eeprom(EEP_PIR_SENSOR_SELECT);		   		
				if(gsiPara == PIR_SENSOR_ENABLE)/*&&(schedule_on_off ==OFF)*/	
					putText(1,(uint8 *)" ENABLE"); 
				else
					putText(1,(uint8 *)"DISABLE");
				break;
			case OCCTimer:
				gsiPara = EEP_OverrideTimer;
				DealwithPara(gsiPara,NUMBER);
				putText(1,DispBuf);
			break;
			
			case BAUDRATE_MENU://advance menu
				gsiPara = EEP_Baudrate;
				DealwithPara(gsiPara,DISP_BAUDTATE);
				putText(1,DispBuf);	
			break;

			case PROTOCOL://advance menu
				if(modbus.com_config[0] == MODBUS)
					gsiPara = 0;
				else if(modbus.com_config[0] == BAC_MSTP)
					gsiPara = 1;
				
				DealwithPara(gsiPara,DISP_PROTOCOL);
				putText(1,DispBuf);	
			break;			
			
			default:break;
		}		
  
		putText(0,(uint8 *)SubMenu[gucItemAdvancedMenu]);
	}

 //gucMenuevent = 0;
}
//=================================================
//  Menu (program) mode	functions
//=================================================
// it is prepare to start up menu 
static void start_menu( )
{ 
	if(gbAdvancedMenu == 1)
	{
		if(gucMenuKey == 0)		
		gucItemAdvancedMenu ++; 
		if(gucItemAdvancedMenu > BACK)
		gucItemAdvancedMenu = 1;
	 
		b.event_function[ UP_KEY ] 		= INCREASE_PARAMETER; //Just put a value never be used
		b.event_function[ DOWN_KEY ] 	= DECREASE_PARAMETER; 
		gucSelectButton = 0x21; 
		b.event_function[ENTER_KEY ] = ACCEPT_PARAMETER;
		AdvancedMenu( );
		
	}
	else if(gbScheduleMenu == 1)
	{
		if(gucMenuKey == 0)		
		gucItemScheduleMenu ++; 
//		if(gucItemScheduleMenu > CaculateSchedule())

		if(gucItemScheduleMenu > 5)
		gucItemScheduleMenu = 1;
	 
		b.event_function[ UP_KEY ] 		= INCREASE_PARAMETER; //Just put a value never be used
		b.event_function[ DOWN_KEY ] 	= DECREASE_PARAMETER; 
		gucSelectButton = 0x21; 
		b.event_function[ENTER_KEY ] = ACCEPT_PARAMETER;
		ScheduleMenu( );
		
	}
	else
	{
		MenuOnLcd = 1;
		gbFlashYearEnable = 0;
		gbFlashMonthEnable = 0;
		gbFlashDateEnable = 0;
		gbFlashWeekEnable = 0;
		gbFlashHourEnable = 0;
		gbFlashMinuteEnable = 0;
		gucItemMainMenu ++; 
		if(gucItemMainMenu > 3)
			gucItemMainMenu = 1;//the gucItemMainMenu increas2e when start this routine,so here should be 1 	 
		gucItemAdvancedMenu = 0;
		gucItemScheduleMenu = 0;
		gucItemClockMenu = 0;
		gbClock  = 0;
		//--remap the keypad --
		b.event_function[ UP_KEY ] 		= NOTHING; //Just put a value never be used
		b.event_function[ DOWN_KEY ] 	= NOTHING; 
		gucSelectButton = 0x20; 
		b.event_function[ENTER_KEY ] = SELECT_FUNCTION;
	 	putText(0,(uint8 *)"MAINMENU");
		putText(1,(uint8 *)MainMenu[gucItemMainMenu]);  
 	}		
	gucMenuKey = 0;
	b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
	start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT	) ;	
     //put the current parameter on the display 	 
	
}
 
static void start_menu_mode()
{ 
	MenuOnLcd = 1;	  
	start_menu() ;
 	stop_timer( LCD_SCREEN_ROTATION_TIMER ); 
	 
}
 
static void exit_menu_mode( )
{
 
 	stop_timer(KEYPAD_ACTIVE_TIMER) ; 
 	start_normal_mode() ;
 	setpoint_adjust_flag = 0 ; 
	gucSelectButton  = 0x00;
	MenuOnLcd = 0;
	gucItemScheduleMenu = 0;
	gucItemClockMenu = 0;
	gucItemAdvancedMenu = 0;
	gucItemMainMenu = 0;
	gbScheduleMenu  = 0;
	gbClockMenu = 0;
	gbAdvancedMenu = 0;	 
	gbFlashMinuteEnable = 0;  
	gbFlashYearEnable = 0;
	gbFlashMonthEnable = 0;
	gbFlashWeekEnable = 0;
	gbFlashDateEnable = 0;
	gbFlashHourEnable = 0;
 	gbSetSchedule = 0;
	gbClock = 0;
	start_timer( LCD_SCREEN_ROTATION_TIMER , DEFAULT_TIMEOUT ) ;
	start_timer(TEMP_DISPLAY_TIMER,DEFAULT_TIMEOUT);
}
 

 
 
void ClockMenu1(void)
{ 
	
	gbFlashHour = 0;
	gbFlashMinute = 0;
	gbFlashWeek = 0;
	gbFlashMonth = 0;
	gbFlashDate = 0;
	gbFlashYear = 0;

	gbFlashHourEnable = 0;
	gbFlashMinuteEnable = 0;
	gbFlashWeekEnable = 0;
	gbFlashMonthEnable = 0;
	gbFlashDateEnable = 0;
	gbFlashYearEnable  = 0;
	
	if(gucItemClockMenu == 7)
	{
		putText(0,(uint8 *)"   GO   ");		 
		putText(1,(uint8 *)" < BACK "); 
//		gbFlashGlobal = 0;
     	gbClock  = 0;
		MenuOnLcd = 1;	
	}
	else
	{
		MenuOnLcd = 0;
		RTC_Get(); 
		switch(gucItemClockMenu)
		{
			case YEAR:
//				gsiPara = DealBCD(read_clock(8) & 0xFF);//58872345
				gsiPara = calendar.w_year; 
				gbFlashYearEnable = 1;
				ShowTime( );


			break;
			case MONTH:
				//gsiPara = DealBCD(read_clock(7) & 0x1F);
				gsiPara = calendar.w_month; 
				gbFlashMonthEnable = 1;
			break;
			case WEEK:
	//			gsiPara = DealBCD(read_clock(6) & 0x07);
				calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date);
				gsiPara = calendar.week; 
				gbFlashWeekEnable = 1;
			break;
			case DATE:
//				gsiPara = DealBCD(read_clock(5) & 0x3F);
//				if(gsiPara > Get_MaxDay(DealBCD(read_clock(8) & 0xFF),DealBCD(read_clock(7) & 0x1F)))   // chelsea
//					gsiPara = Get_MaxDay(DealBCD(read_clock(8) & 0xFF),DealBCD(read_clock(7) & 0x1F));
				gsiPara = calendar.w_date; 
				gbFlashDateEnable = 1;
			break;
			case HOUR:
//				gsiPara = DealBCD(read_clock(4) & 0x3F);
				gsiPara = calendar.w_year; 
				gbFlashHourEnable = 1;
			break;
			case MINUTE:
//				gsiPara = DealBCD(read_clock(3) & 0x7F);
				gsiPara = calendar.w_year; 
				gbFlashMinuteEnable = 1;
			break;
			default:break;
		}  
// 		gbFlashGlobal = 1;
		//putText(0,ClockMenu[gucItemClockMenu]);
		if(gucItemClockMenu == WEEK)
			;//putText(1,weekMenu[gsiPara-1]); 
		else
			;//putText(1,DealwithPara(gsiPara,NUMBER));  
	}
}
//---------accept_parameter()-----------
//Accept a new value for the current paramter being displayed
static void accept_parameter( )
{ 
	unsigned char   temp;

	gbFlashMinuteEnable = 0;
	gbFlashYearEnable = 0;
	gbFlashMonthEnable = 0;
	gbFlashWeekEnable = 0;
	gbFlashDateEnable = 0;
	gbFlashHourEnable = 0;
//	gbSetSchedule = 0;
//	start_timer(KEYPAD_ACTIVE_TIMER,0);
	gucMenuKey = 1;
	b.event_function[MENU_KEY] = START_MENU_MODE;
	if(gbModeMenu == 1)
 	{		
		switch(gucItemModeMenu)
			{				 
				case HEATCOOLE:
				 		if(gsiPara == 0)
						{				 
							heat_cool_flag = 1 ;
							b.eeprom[EEP_HEAT_COOL_CONFIG] = HC_KEYPAD;
						}
						else if(gsiPara == 1)
						{
								heat_cool_flag = 0 ;
								b.eeprom[EEP_HEAT_COOL_CONFIG] = HC_KEYPAD;
						}
						else
						{
								b.eeprom[EEP_HEAT_COOL_CONFIG] = HC_AUTOMATIC;
				
						}
						new_write_eeprom(EEP_HEAT_COOL_CONFIG, b.eeprom[EEP_HEAT_COOL_CONFIG]);
				
						control_logic() ;
//						StatusLeds();
						break;


				case FANMODEE:
					EEP_FanSpeed = gsiPara;
					fan_speed_user = EEP_FanSpeed;
					if(fan_speed_user == 0)
					{
						info_byte &= 0xfe;
						occupied = 0;
						write_eeprom(EEP_INFO_BYTE,info_byte);
						override_timer = OFF;
						override_timer_time = 0;
						setpoint_select = NIGHT_SP;
					}
					else 
					{
						if(override_timer == OFF)
						{
							info_byte |= 0x01;
							occupied = 1;
							write_eeprom(EEP_INFO_BYTE,info_byte);//occupied mode unoccupied mode
						}
						write_eeprom(EEP_INFO_BYTE,info_byte);
						setpoint_select = DHOME_SP;
					}	 		
			 			write_eeprom(EEP_FAN_SPEED,fan_speed_user);	

						break; 
				default:
						break;
			}
 
		
	}
	else if(gbScheduleMenu == 1)
	{
		switch(gucItemScheduleMenu)
			{
//				case DAY:
//					gucDay = gsiPara;
//					if(gsiPara == 3)
//					{
//						gbScheduleMenu = 0;
//						gbClockMenu  = 0;
//						gbAdvancedMenu = 0;
//						tempDay = 1;
//						start_menu();
//					}
//				break;
//		 
//				case S_TIME1: 
//				temp = (gucDay - 1)*8 + (gucEvent - 1)*2;			 	
//				new_write_eeprom(EEP_DAY1_EVENT1_HI + temp,gsiPara);
//				gucMenuevent = 1;
//				break;

//				case S_TIME2:case S_TIME3:case S_TIME4:
//				temp = (gucDay - 1)*8 + (gucEvent - 1)*2;
//	 			if(gbScheduleTime == 0)
//				new_write_eeprom(EEP_DAY1_EVENT1_HI + temp,gsiPara);
//				gucMenuevent = 1;
//				break;

//				case S_MINUTE1:
//				temp = (gucDay - 1)*8 + (gucEvent - 1)*2;	
//				new_write_eeprom(EEP_DAY1_EVENT1_LO + temp,gsiPara);

//				break;

//				case S_MINUTE2:case S_MINUTE3:case S_MINUTE4:
//				temp = (gucDay - 1)*8 + (gucEvent - 1)*2;			 	 
//				if(gbScheduleTime == 0)
//				new_write_eeprom(EEP_DAY1_EVENT1_LO + temp,gsiPara);
//				break;
		 
				default:break;
			}	
	
	}
	else if(gbClockMenu == 1)
	{
		if(gucItemClockMenu == 7)
		{
			gbScheduleMenu = 0;
			gbClockMenu  = 0;
			gbAdvancedMenu = 0;
			start_menu( );
		}
		else if(gucItemClockMenu < 8)
		{
			switch(gucItemClockMenu)
			{				 
				case YEAR:
//				temp = GetWeekDay(2000+gsiPara,guTime[1],guTime[3]);
//	 			write_clock(8 - (gucItemClockMenu - YEAR),HexToBcd(gsiPara));
//	 			write_clock(6,HexToBcd(temp));
				calendar.w_year = gsiPara;
				RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
				break;

				case MONTH:
//				temp = GetWeekDay(2000+guTime[0],gsiPara,guTime[3]);
//	 			write_clock(8 - (gucItemClockMenu - YEAR),HexToBcd(gsiPara));
//	 			write_clock(6,HexToBcd(temp));
				calendar.w_month = gsiPara;
				RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
				break;

				case WEEK:
//	 			write_clock(8 - (gucItemClockMenu - YEAR),HexToBcd(gsiPara));
				break;

				case DATE:
//				temp = GetWeekDay(2000+guTime[0],guTime[1],gsiPara);
//	 			write_clock(8 - (gucItemClockMenu - YEAR),HexToBcd(gsiPara));
//	 			write_clock(6,HexToBcd(temp));
				calendar.w_date = gsiPara;
				RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
				break;

				case HOUR:
					calendar.hour = gsiPara;
					RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
					break;
				case MINUTE:
//			 	write_clock(8 - (gucItemClockMenu - YEAR),HexToBcd(gsiPara));
					calendar.min = gsiPara;
					RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
				break;
		 
				default:break;
			}

		if(gucItemClockMenu == 2)
		gucItemClockMenu += 2; 
		else
		gucItemClockMenu ++; 
		if(gucItemClockMenu > 7)
		gucItemClockMenu = 1;
	 
		b.event_function[ UP_KEY ] 		= INCREASE_PARAMETER; //Just put a value never be used
		b.event_function[ DOWN_KEY ] 	= DECREASE_PARAMETER; 
		gucSelectButton = 0x21; 
		b.event_function[ENTER_KEY ] = ACCEPT_PARAMETER;// 235 
		ClockMenu1( );
		}
	}
	else if(gbAdvancedMenu == 1)
	{
		if(gucItemAdvancedMenu == BACK)
		{
			gbScheduleMenu = 0;
			gbClockMenu  = 0;
			gbAdvancedMenu = 0;
			start_menu( );
		}
//		else if(gucItemAdvancedMenu == SENSOR)
//		{
//				AcceptSensor( );	
//		}
//		else if(gucItemAdvancedMenu < UNITS)
//		{
//			switch(gucItemAdvancedMenu)
//			{				 
//				case WALL:
//				case CEILING:
//				case OUTDOOR:
//				case AVERAGE:
//				case NIGHT_HEAT:
//				case NIGHT_COOL:
//				case RHset:
//				b.eeprom[EEP_WALL_SETPOINT - MAXEEPCONSTRANGE + 1 + ((gucItemAdvancedMenu - 1) << 1)] = (gsiPara >> 8) & 0xff;
//				b.eeprom[EEP_WALL_SETPOINT - MAXEEPCONSTRANGE + ((gucItemAdvancedMenu - 1) << 1)] = gsiPara & 0xff;
//				 new_write_eeprom(EEP_WALL_SETPOINT + 1 + ((gucItemAdvancedMenu - 1) << 1) ,b.eeprom[EEP_WALL_SETPOINT - MAXEEPCONSTRANGE + 1 + ((gucItemAdvancedMenu - 1) << 1)]);
//				 new_write_eeprom(EEP_WALL_SETPOINT + ((gucItemAdvancedMenu - 1) << 1),b.eeprom[EEP_WALL_SETPOINT - MAXEEPCONSTRANGE + ((gucItemAdvancedMenu - 1) << 1)]);
//				break;
//		 
//				default:break;
//			}
//		}
	 
 
		else if(gucItemAdvancedMenu < BACK)
		{
	 		switch(gucItemAdvancedMenu)
			{				 
				case MENU_UNITS:				
			 
					if(gsiPara != b.eeprom[EEP_DEGC_OR_F])			 
			 		{ 
						new_write_eeprom(EEP_DEGC_OR_F,gsiPara);
				  		EEP_DEGCorF = gsiPara; 
						ConverteCF( );
					}			 	 
				break;
				case TIME_FORMAT:
			 	b.eeprom[EEP_SYSTEM_TIME_FORMAT] = gsiPara;
			 	new_write_eeprom(EEP_SYSTEM_TIME_FORMAT,gsiPara);
				break;
			/*	case HEAT_STAGES:
				b.eeprom[EEP_HEAT_ORIGINAL_TABLE - MAXEEPCONSTRANGE] = gsiPara;
			 	new_write_eeprom(EEP_HEAT_ORIGINAL_TABLE,gsiPara);
			 	init_PID_flag = 1;	
				break;
				case COOL_STAGES:
				b.eeprom[EEP_COOL_ORIGINAL_TABLE - MAXEEPCONSTRANGE] = gsiPara ;
			 	new_write_eeprom(EEP_COOL_ORIGINAL_TABLE,gsiPara);
				init_PID_flag = 1;			 	 
				break;*/
				case NET_ID:
				laddress = gsiPara ; 
				Station_NUM = laddress;
//			 	new_write_eeprom(EEP_ADDRESS,gsiPara); 
//				iap_erase_block(DATA_TO_ISP/512);							
//				for( i=0; i<=15; i++)
//				{
//					temp = new_read_eeprom(i);
//					iap_program_data_byte(temp, DATA_TO_ISP + i);
//				}
//				iap_program_data_byte(0x00, DATA_TO_ISP + 16);	  	
				ID_Lock = ID_WRITE_ENABLE; 
				flash_buf[0] = laddress;	
					STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);
				new_write_eeprom(EEP_ADDRESS , laddress) ; 			
				break;
			/*	case CAL:
			if (b.eeprom[EEP_TEMP_SELECT - MAXEEPCONSTRANGE] == 1 )
			{
				if(temperature < 0)
				analog_calibration[0] += (-analog_input[0] - gsiPara);
				else
				analog_calibration[0] += (gsiPara - analog_input[0]);
				//analog_calibration[0] = 500+(originalTemp - analog_calibration[0] + menubuffer.new_parameter)- analog_input[0];
				//analog_calibration[0] = menubuffer.new_parameter;
				new_write_eeprom( EEP_CALIBRATION_ANALOG1 , analog_calibration[0]&0xFF ) ;
				new_write_eeprom( EEP_CALIBRATION_ANALOG1 + 1, analog_calibration[0]/256 );
			}
			else if (b.eeprom[EEP_TEMP_SELECT - MAXEEPCONSTRANGE] == 2)
			{

				if(temperature < 0)
				analog_calibration[4] += (-analog_input[4] - gsiPara);
				else
				analog_calibration[4] += (gsiPara - analog_input[4]);
			//	analog_calibration[2] = 500+(originalTemp - analog_calibration[2] + menubuffer.new_parameter)- analog_input[2];
			//	analog_calibration[2] = menubuffer.new_parameter;
				new_write_eeprom( EEP_CALIBRATION_ANALOG5 , analog_calibration[4]&0xFF ) ;
				new_write_eeprom( EEP_CALIBRATION_ANALOG5 + 1, analog_calibration[4]/256 ) ;
			}
				break;*/
		  		case SOP:
			 		Sys_Sequence = gsiPara;			 
					write_eeprom(EEP_SEQUENCE ,gsiPara);//
					init_extern_operation_eeprom(gsiPara);
					Tstat7_Restart();
					delay_ms(10);
					SoftReset();
				break;
				case OCC_SEL:
					if(gsiPara == PIR_SENSOR_ENABLE)
					{
						//schedule_on_off = OFF;
						//new_write_eeprom(EEP_SCHEDULE_ON_OFF,OFF);
						PirSensorSelect = 1;	
						new_write_eeprom(EEP_PIR_SENSOR_SELECT,1);						
					}
					else
					{
						PirSensorSelect = 0;	
						new_write_eeprom(EEP_PIR_SENSOR_SELECT,0);	
					}
												
					break;
				case OCCTimer:
					EEP_OverrideTimer = gsiPara;
					new_write_eeprom(EEP_OVERRIDE_TIMER,gsiPara);
					break; 
				
				case BAUDRATE_MENU: //accept parameter
					EEP_Baudrate = gsiPara;
				  new_write_eeprom(EEP_BAUDRATE,gsiPara);
				if(EEP_Baudrate == BAUDRATE_19200)
				{
				uart1_init(19200);
				modbus.baudrate = 19200;
				}
			else if(EEP_Baudrate == BAUDRATE_9600)
				{
				uart1_init(9600);
				modbus.baudrate = 9600;
				}

			else if(EEP_Baudrate == BAUDRATE_38400)
				{
				uart1_init(38400);
				modbus.baudrate = 38400;
				}
			else if(EEP_Baudrate == BAUDRATE_57600)
				{
				uart1_init(57600);
				modbus.baudrate = 57600;
				}
			else if(EEP_Baudrate == BAUDRATE_115200)
				{
				uart1_init(115200);
				modbus.baudrate = 115200;
				}
			else if(EEP_Baudrate == BAUDRATE_76800)
				{
				uart1_init(76800);
				modbus.baudrate = 76800;
				}	
			else if(EEP_Baudrate == BAUDRATE_1200)
				{
				uart1_init(1200);
				modbus.baudrate = 1200;
				}
			else if(EEP_Baudrate == BAUDRATE_4800)
				{
				uart1_init(4800);
				modbus.baudrate = 4800;
				}
			else if(EEP_Baudrate == BAUDRATE_14400)
				{
				uart1_init(14400);
				modbus.baudrate = 14400;
				}
				serial_restart();
			break;
			case PROTOCOL:
				//new_write_eeprom(EEP_PROTOCOL_SEL, gsiPara);
				if(gsiPara == 1)//
				{
					modbus.com_config[0] = 0;
					write_eeprom(EEP_PROTOCOL_SEL, 1);
					Recievebuf_Initialize(0);
				}
				else
				{
					modbus.com_config[0] = 3;
					write_eeprom(EEP_PROTOCOL_SEL, 0);
//						if(EEP_Baudrate == BAUDRATE_19200)
//						uart1_init(19200);
//					else if(EEP_Baudrate == BAUDRATE_9600)
//						uart1_init(9600);
//					else if(EEP_Baudrate == BAUDRATE_38400)
//						uart1_init(38400);
//					else if(EEP_Baudrate == BAUDRATE_57600)
//						uart1_init(57600);
//					else if(EEP_Baudrate == BAUDRATE_115200)
//						uart1_init(115200);
//					else if(EEP_Baudrate == BAUDRATE_76800)
//						uart1_init(76800);
				}
				//new_write_eeprom(EEP_OVERRIDE_TIMER,gsiPara);		
			break;
				default:break;				
			}
		}
		
	}	

	 
 
	//MenuOnLcd = 0;
//	putDigitals2(0xffff);
	
}// end of accept_parameter( )----------





//function:  get max day of every month
//create:	chelsea

static uint8 Get_MaxDay(uint16 year,uint8 month)
{
	uint8 leap = 0;
	uint8 xdata maxday = 0;
	if(year % 100 == 0)
	{
		if(year % 400 == 0)	leap = 1;
		else leap = 0;
	}
	else
	{
		if(year % 4 == 0)	leap = 1;
		else leap = 0;
	}
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: 	maxday = 31;	break;
		case 2:		if(leap)	maxday = 29;
					else 	maxday = 28;
					break;
		case 4:
		case 6:
		case 9:
		case 11:	maxday = 30;	break;
		default:	break;
	}
	return maxday;
}

// -----------------increase_parameter ------------------------
// increase parameter at current menu.
static void increase_parameter( )
{
//	unsigned char  temp; 
	b.jump_function[ KEYPAD_ACTIVE_TIMER] =	ACCEPT_EXIT_PARAMETER ;	 
  	start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT ) ;
	b.event_function[ ENTER_KEY ] 	=  ACCEPT_PARAMETER;
	b.event_function[ MENU_KEY ] = ACCEPT_PARAMETER;
	if(gbModeMenu == 1)
	{		
		switch(gucItemModeMenu)
			{				 
				case HEATCOOLE:
		 		gsiPara++;
				if(gsiPara > 2)
				gsiPara = 0;
				putText(1,(uint8 *)HEATCOOL[gsiPara]);
				break; 
				case FANMODEE:
				if(gsiPara == AUTO)
					gsiPara  = 0;
				else
				{
	 				gsiPara++;
					if(gsiPara > b.eeprom[ EEP_FAN_MODE ])
					gsiPara = AUTO;
				}

				if((b.eeprom[ EEP_FAN_MODE ] < 2 || b.eeprom[ EEP_AUTO_ONLY ] == 1) && gsiPara <= AUTO)
				{				 
					if(gsiPara == OFF)
						putText(1,(uint8 *)"MAN OFF ");
					else if(gsiPara == AUTO && b.eeprom[ EEP_FAN_MODE ] == 1)
						putText(1,(uint8 *)" -AUTO- ");
					else 
						putText(1,(uint8 *)" MAN ON ");
				}
				else
				{					 
					putText(1,(uint8 *)FANMODE[gsiPara]);
			    }				
				break;
		 
		 
				default:break;
			}
 
		
	}
else if(gbScheduleMenu == 1)
	{		
		switch(gucItemScheduleMenu)
			{				 
				case DAY:
				gsiPara++;
				if(gsiPara > 3)
				gsiPara = 1;
				tempDay = gsiPara;  // chelsea
				if(tempDay == 3)
					putText(0,(uint8 *)"   GO   ");
				else
					putText(0,(uint8 *)"DAY SET ");

				putText(1,(uint8 *)SchweekMenu[gsiPara-1]);
				
				break;
				/*case EVENT:
				gsiPara++;
				if(gsiPara > 4)
				gsiPara = 1;
				//tempEvent = gsiPara;  // chelsea
				putText(1,eventMenu[gsiPara-1]);
				break;*/
				case S_TIME1:
				case S_TIME2:
				case S_TIME3:
				case S_TIME4:	
				gsiPara++;		 
				if(gsiPara > 23)
				gsiPara = 0;
				gucHour = gsiPara;
				//putText(1,DealwithPara(gsiPara,TIME));
				//FlashTime(gucMinute,gucHour,1);
			 	gbSetSchedule = 1;
				gbHmFlag = 1;
				break;
				case S_MINUTE1:
				case S_MINUTE2: 
				case S_MINUTE3:
				case S_MINUTE4:
				gsiPara++;		 
				if(gsiPara > 59)
				gsiPara = 0;
				gucMinute = gsiPara;
				//putText(1,DealwithPara(gsiPara,TIME));
				//FlashTime(gucMinute,gucHour,0);
			 	gbSetSchedule = 1;
				gbHmFlag = 0;
				break;
			 
		 
				default:break;
			}
 
		
	}
 
	else if(gbClockMenu == 1)
	{
	 
			switch(gucItemClockMenu)
			{
				case YEAR:				 				 
					gsiPara++;	
				if(gsiPara > 99)
					gsiPara = 0;
				guTime[0] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));			 
				break;
				case MONTH:
				gsiPara++;	
				if(gsiPara > 12)
					gsiPara = 1;
				guTime[1] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));	
				break;
				case WEEK:
				gsiPara++;
				if(gsiPara > 7) 					 
					gsiPara = 1;
				guTime[2] = gsiPara;
				break;
				case DATE:
					gsiPara++;				//add leap year
				//--------------- chelsea	

				if(gsiPara > Get_MaxDay(calendar.w_year,calendar.w_month)) 					 
					gsiPara = 1;
				guTime[3] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));	
				break;
				case HOUR:
				gsiPara++;
				if(gsiPara > 23) 					 
					gsiPara = 0;
					 guTime[4] = gsiPara;
				break;
				case MINUTE:
				gsiPara++;
				if(gsiPara > 59) 					 
					gsiPara = 0;
				guTime[5] = gsiPara;
				break;
			 
				default:break;
			}
		 
	}
	else if(gbAdvancedMenu == 1)
	{
//		if(gucItemAdvancedMenu == SENSOR)
//		{
//				IncreaseSensor( );	
//		}
//		else if(gucItemAdvancedMenu > SENSOR && gucItemAdvancedMenu < UNITS)
//		{
//			switch(gucItemAdvancedMenu)
//			{
//				case WALL:
//				case CEILING:
//				case OUTDOOR:
//				case AVERAGE:
//				case NIGHT_HEAT:
//				case NIGHT_COOL:
//				gsiPara += 100;
//				if(gsiPara  > (signed int)(b.eeprom[EEP_MAX_SETPOINT - MAXEEPCONSTRANGE]*100))
//
//				gsiPara = (signed int)(b.eeprom[EEP_MAX_SETPOINT - MAXEEPCONSTRANGE]*100);
//				putText(1,DealwithPara(gsiPara,TEMP)); 
//				break;
//				case RHset:
//
//				gsiPara += 100; // 105 
//			
//				if(gsiPara  > 9000)
//
//				gsiPara = 9000;	
//				putText(1,DealwithPara(gsiPara,RHP)); 				 
//				break;			 
//
//				default:break;
//			}
//			//putText(1,DealwithPara(gsiPara,TEMP)); 
//		}
 
 
	    if(gucItemAdvancedMenu < BACK)
		{
			switch(gucItemAdvancedMenu)
			{				 
				case MENU_UNITS:				
					gsiPara++;	
					if(gsiPara > 1)
					gsiPara =0;	
					DealwithPara(gsiPara,TEMP_UNITS);
					putText(1,DispBuf);	 
				break;
				case TIME_FORMAT:
					gsiPara++;	
					if(gsiPara > 1)
					gsiPara =0;	
					DealwithPara(gsiPara,FORMAT);
					putText(1,DispBuf);		 
				break;
			/*	case HEAT_STAGES:
				gsiPara++;	
				if(gsiPara > 2)
				gsiPara =0;	
				putText(1,DealwithPara(gsiPara,NUMBER));		 
				break;
				case COOL_STAGES:
				gsiPara++;	
				if(gsiPara > 2)
				gsiPara =0;	
				putText(1,DealwithPara(gsiPara,NUMBER));	 
				break;*/
				case NET_ID:
					gsiPara++;	
					if(gsiPara > 254)
					gsiPara = 1;
					DealwithPara(gsiPara,NUMBER);	
					putText(1,DispBuf);	 
				break;
				//case CAL:
				//gsiPara++;
			 	
				//putText(1,DealwithPara(gsiPara,TEMPE));	
			//	display(gsiPara); 
				//break;
				case SOP:
					gsiPara = get_inverse(gsiPara);
				 	DealwithPara(gsiPara,NUMBER);
					putText(1,DispBuf);	
					//display(gsiPara); 
					break;
					
				case OCC_SEL:										   		   		
					if(gsiPara != NO_PIR_SENSOR)
					{						
						gsiPara	=NO_PIR_SENSOR;
						putText(1,(uint8 *)"DISABLE");
					} 
					else
					{
						gsiPara = PIR_SENSOR_ENABLE;
						putText(1,(uint8 *)" ENABLE");
					}
						
					break;
				case OCCTimer:
					gsiPara++;	
					if(gsiPara > 254)
					gsiPara = 1;
					DealwithPara(gsiPara,NUMBER);	
					putText(1,DispBuf);
					break;  

				case BAUDRATE_MENU: //increase parameter
					
					if(gsiPara == BAUDRATE_1200)
						gsiPara = BAUDRATE_4800;
					else if(gsiPara == BAUDRATE_4800)
						gsiPara = BAUDRATE_9600;
					else if(gsiPara == BAUDRATE_9600)
						gsiPara = BAUDRATE_14400;
					else if(gsiPara == BAUDRATE_14400)
						gsiPara = BAUDRATE_19200;
					else if(gsiPara == BAUDRATE_19200)
						gsiPara = BAUDRATE_38400;
					else if(gsiPara == BAUDRATE_38400)
						gsiPara = BAUDRATE_57600;
					else if(gsiPara == BAUDRATE_57600)
						gsiPara = BAUDRATE_76800;
					else if(gsiPara == BAUDRATE_76800)
						gsiPara = BAUDRATE_115200;
					else if(gsiPara == BAUDRATE_115200)
						gsiPara = BAUDRATE_1200;
				
					DealwithPara(gsiPara,DISP_BAUDTATE);	
					putText(1,DispBuf);
				break;

				case PROTOCOL:
					if(gsiPara == 0)
						gsiPara = 1;
					else
						gsiPara = 0;
					DealwithPara(gsiPara,DISP_PROTOCOL);	
					putText(1,DispBuf);
				break;
				default:break;

				
			}
		}
	
	}
}

 




// ------------------decrease_parameter ---------------------
// decrease parameter at current menu. 
static void decrease_parameter( )
{
// 	unsigned char xdata temp;
  	b.jump_function[ KEYPAD_ACTIVE_TIMER] =	ACCEPT_EXIT_PARAMETER ;	 
  	start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT ) ;
	b.event_function[ ENTER_KEY ] 	=  ACCEPT_PARAMETER;
	b.event_function[ MENU_KEY ] = ACCEPT_PARAMETER;
	if(gbModeMenu == 1)
	{		
		switch(gucItemModeMenu)
			{				 
			case HEATCOOLE:
		 		
				if(gsiPara > 0)
					gsiPara--;
				else 
					gsiPara = 2;
				putText(1,(uint8 *)HEATCOOL[gsiPara]);
				break;


				case FANMODEE:
	 		 
					if(b.eeprom[ EEP_AUTO_ONLY] == 1)
					{
						if(gsiPara == 0)
							gsiPara = 4;
						else 
							gsiPara = 0;
 
					}			 
					else if( gsiPara > EEP_FanMode)
				 	{	
		 				 gsiPara = EEP_FanMode ;
					}
					else if( gsiPara > 0)
				 	{	
		 				 gsiPara-- ;
						if( gsiPara == 0)
						 gsiPara = 4;
					}
					else
					{
						gsiPara = 4;
					}



					if((b.eeprom[ EEP_FAN_MODE ] < 2 || b.eeprom[ EEP_AUTO_ONLY ] == 1) && gsiPara <= AUTO)
					{				 
						if(gsiPara == OFF)
							putText(1,(uint8 *)"MAN OFF ");
						else if(gsiPara == AUTO && b.eeprom[ EEP_FAN_MODE] == 1)
							putText(1,(uint8 *)" -AUTO- ");
						else 
							putText(1,(uint8 *)" MAN ON ");
					}
					else
					{					 
						putText(1,(uint8 *)FANMODE[gsiPara]);
				    }				 
								
				break; 
				default:
					break;
			} 	
	}
	else if(gbScheduleMenu == 1)
	{		
			switch(gucItemScheduleMenu)
			{				 
				case DAY:
				gsiPara--;
				if(gsiPara== 0)
				gsiPara = 3;
				tempDay = gsiPara;  // chelsea
				if(tempDay == 3)
					putText(0,(uint8 *)"   GO   ");
				else
					putText(0,(uint8 *)"DAY SET ");
				putText(1,(uint8 *)SchweekMenu[gsiPara-1]);
				break;	    
				case S_TIME1:
				if(gsiPara == 0)
				gsiPara = 23;			 
				else if(gsiPara > 0)
				gsiPara--;
				gucHour = gsiPara;
				DealwithPara(gsiPara,TIME);
				putText(1,DispBuf);
				gbHmFlag = 1;
				break;
				case S_TIME2:
				case S_TIME3:
				case S_TIME4:
				if(gsiPara == 0)
				gsiPara = 23;			 
				else if(gsiPara > 0)
				gsiPara--;
				gucHour = gsiPara;
				DealwithPara(gsiPara,TIME);
				putText(1,DispBuf);
				start_timer(CHECK_SCHEDULE_TIMER,DEFAULT_TIMEOUT);
				gbHmFlag = 1;
				break;
				case S_MINUTE1: 
				if(gsiPara == 0)
				gsiPara = 59;			 
				else if(gsiPara > 0)
				gsiPara--;
				gucMinute= gsiPara;
			//	putText(1,DealwithPara(gsiPara,TIME));
			 	//FlashTime(gucMinute,gucHour,1);
			 	gbSetSchedule = 1;
				gbHmFlag = 0;
				break;
				case S_MINUTE2:
				case S_MINUTE3:
				case S_MINUTE4:
				if(gsiPara == 0)
				gsiPara = 59;			 
				else if(gsiPara > 0)
				gsiPara--;
				gucMinute= gsiPara;
			//	putText(1,DealwithPara(gsiPara,TIME));
				//FlashTime(gucMinute,gucHour,0);
			 	gbSetSchedule = 1;
				gbHmFlag = 0;
				start_timer(CHECK_SCHEDULE_TIMER,DEFAULT_TIMEOUT);
				break;
			 
		 
				default:break;
			}


		//b.event_function[ UP_KEY ] 		= 100;//INCREASE_PARAMETER; //Just put a value never be used
		//b.event_function[ DOWN_KEY ] 	= 100;//DECREASE_PARAMETER; 
		//gucSelectButton = 0x21; 
		//b.event_function[ENTER_KEY ] = ACCEPT_PARAMETER;
		
	}
	else if(gbClockMenu == 1)
	{
			switch(gucItemClockMenu)
			{
				case YEAR:
				if(gsiPara > 0) 				 
					gsiPara--;	
				else
					gsiPara = 99;
				guTime[0] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));		 
				break;
				case MONTH:
				if(gsiPara > 1) 				 
					gsiPara--;	
				else
					gsiPara = 12;
				guTime[1] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));	
				break;
				case WEEK:
				if(gsiPara > 1) 				 
					gsiPara--;	
				else
					gsiPara = 7;
				guTime[2] = gsiPara;
				break;
				case DATE:// here should fix ,add leap  year arithmetic
				if(gsiPara > 1) 				 
					gsiPara--;	
				else
					gsiPara = Get_MaxDay(calendar.w_year,calendar.w_month);
				guTime[3] = gsiPara;
//				temp = GetWeekDay(guTime[0],guTime[1],guTime[3]);
//	 			write_clock(6,HexToBcd(temp));	
				break;
				case HOUR:
				if(gsiPara > 0) 				 
					gsiPara--;	
				else
					gsiPara = 23;
				guTime[4] = gsiPara;
				break;
				case MINUTE:
				if(gsiPara > 0) 				 
					gsiPara--;	
				else
					gsiPara = 59;
				guTime[5] = gsiPara;
				break;
			 
				default:break;
			}										   ; 
	}
	else if(gbAdvancedMenu == 1)
	{
//		if(gucItemAdvancedMenu == SENSOR)
//		{
//				DecreaseSensor( );	
//		}
//		if(gucItemAdvancedMenu > SENSOR && gucItemAdvancedMenu < UNITS)
//		{
//	 
//			switch(gucItemAdvancedMenu)
//			{
//				case WALL:
//				case CEILING:
//				case OUTDOOR:
//				case AVERAGE:
//				case NIGHT_HEAT:
//				case NIGHT_COOL:
//				gsiPara -= 100;
//				if(gsiPara  < (signed int)(b.eeprom[EEP_MIN_SETPOINT - MAXEEPCONSTRANGE]*100))
//
//				gsiPara = (signed int)(b.eeprom[EEP_MIN_SETPOINT - MAXEEPCONSTRANGE]*100);
//				putText(1,DealwithPara(gsiPara,TEMP)); 
//				break;
//				case RHset:
//
//				gsiPara -= 100; 
//			
//				if(gsiPara  < 1000)
//
//				gsiPara = 1000;	
//				putText(1,DealwithPara(gsiPara,RHP));				 
//				break;
//			 
//				default:break;
//			}
//	 
//		} 
 
		if(gucItemAdvancedMenu < BACK)
		{
			switch(gucItemAdvancedMenu)
			{				 
				case MENU_UNITS:				
					if(gsiPara == 0)
						gsiPara = 1;			 
					else if(gsiPara > 0)
						gsiPara--;
					DealwithPara(gsiPara,TEMP_UNITS);
					putText(1,DispBuf);	 
				break;
				case TIME_FORMAT:
					if(gsiPara == 0)
						gsiPara = 1;			 
					else if(gsiPara > 0)
						gsiPara--;
					DealwithPara(gsiPara,FORMAT);
					putText(1,DispBuf);		 
				break;
				/*case HEAT_STAGES:
				if(gsiPara == 0)
				gsiPara = 2;			 
				else if(gsiPara > 0)
				gsiPara--;

				putText(1,DealwithPara(gsiPara,NUMBER));		 
				break;
				case COOL_STAGES:
				if(gsiPara == 0)
				gsiPara = 2;			 
				else if(gsiPara > 0)
				gsiPara--;

				putText(1,DealwithPara(gsiPara,NUMBER));	 
				break;*/
				case NET_ID:
					if(gsiPara == 1)
					gsiPara = 254;			 
					else if(gsiPara > 1)
					gsiPara--;
					DealwithPara(gsiPara,NUMBER);
					putText(1,DispBuf);	 
				break;
				//case CAL:
				//gsiPara--;
			 	
				//putText(1,DealwithPara(gsiPara,TEMPE));	
				//display(gsiPara); 
				//break;
		  		case SOP:
					gsiPara = get_inverse(gsiPara);
				 	DealwithPara(gsiPara,NUMBER);
					putText(1,DispBuf);	
					//display(gsiPara); 
				break;
				case OCC_SEL:										   		   		
					if(gsiPara != NO_PIR_SENSOR)
					{					
						gsiPara	=NO_PIR_SENSOR;
						putText(1,(uint8 *)"DISABLE");
					} 
					else
					{
						gsiPara = PIR_SENSOR_ENABLE;
						putText(1,(uint8 *)" ENABLE");
					}
						
					break;
				case OCCTimer:
					if(gsiPara == 1)
						gsiPara = 254;			 
					else if(gsiPara > 1)
						gsiPara--;
					DealwithPara(gsiPara,NUMBER);
				case BAUDRATE_MENU://decrease
					
				if(gsiPara == BAUDRATE_9600)
					gsiPara = BAUDRATE_4800;
				else if(gsiPara == BAUDRATE_4800)
					gsiPara = BAUDRATE_1200;
				else if(gsiPara == BAUDRATE_19200)
					gsiPara = BAUDRATE_14400;
				else if(gsiPara == BAUDRATE_14400)
					gsiPara = BAUDRATE_9600;
				else if(gsiPara == BAUDRATE_115200)
					gsiPara = BAUDRATE_76800;
				else if(gsiPara == BAUDRATE_76800)
					gsiPara = BAUDRATE_57600;
				else if(gsiPara == BAUDRATE_57600)
					gsiPara = BAUDRATE_38400;	
				else if(gsiPara == BAUDRATE_38400)
					gsiPara = BAUDRATE_19200;					
				else if(gsiPara == BAUDRATE_1200)
					gsiPara = BAUDRATE_115200;
				else
					gsiPara--;
				DealwithPara(gsiPara,DISP_BAUDTATE);
					putText(1,DispBuf);	
				break;

				case PROTOCOL:
					if(gsiPara == 0)
						gsiPara = 1;
					else
						gsiPara = 0;
					DealwithPara(gsiPara,DISP_PROTOCOL);	
					putText(1,DispBuf);
				break;				
				  
				default:break;				
			}
			 
		}    
	}
}

 


static void switch_mode( void )
{
 	b.event_function[ UP_KEY ]	= INCREASE_PARAMETER;// 3,put a never be used value
	b.event_function[ DOWN_KEY] = DECREASE_PARAMETER;// 4,
	b.event_function[ ENTER_KEY] = ACCEPT_PARAMETER;// 4,
	MenuOnLcd = 1;
  stop_timer(LCD_SCREEN_ROTATION_TIMER) ;
	gucItemModeMenu++;
	if(gucItemModeMenu > 1)
	
	gucItemModeMenu = 0;
	
	gbModeMenu = 1;
	gbScheduleMenu = 0;
	gbClock  = 0;	
	gbAdvancedMenu  = 0;
	gbClockMenu = 0;	
	putText(0,(uint8 *)MODEMENU1[gucItemModeMenu]);
//	putText(0,"FANMODE");
	if(gucItemModeMenu)
	{
		gsiPara =  b.eeprom[EEP_FAN_SPEED ];
		if(gsiPara == AUTO)
		{
			if(b.eeprom[EEP_FAN_MODE ] == 0)/* || (b.eeprom[EEP_AUTO_ONLY ] == 1))*/
				putText(1,(uint8 *)" MAN ON ");
			else
				putText(1,(uint8 *)" -AUTO- ");
		}
		else
			putText(1,(uint8 *)FANMODE[gsiPara]);
		//	putText(1,FANMODE[gsiPara]);
	}
	else
	{
		gsiPara =  0;//new_read_eeprom(COOLHEATMODE);//0;//b.eeprom[EEP_FAN_SPEED - MAXEEPCONSTRANGE];
		if(gsiPara > 2)
		gsiPara = 2;
		putText(1,(uint8 *)HEATCOOL[gsiPara]);
	}
		b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
		start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT) ; 
}









static void SelectButton(void)    
{ 
   if(gucSelectButton == 0x20)
	{
		b.event_function[ UP_KEY ]	= INCREASE_PARAMETER;// 3,put a never be used value
		b.event_function[ DOWN_KEY] = DECREASE_PARAMETER;// 4,
		b.event_function[ ENTER_KEY] = ACCEPT_PARAMETER;// 4,
		MenuOnLcd = 1;

		stop_timer(	LCD_SCREEN_ROTATION_TIMER ) ;
		switch(gucItemMainMenu)
		{
			case MENUSCHEDULE:
				gbScheduleMenu = 1;
				gbClock  = 0;	
				gbAdvancedMenu  = 0;
				gbClockMenu = 0;
				gbModeMenu = 0;
				gucItemScheduleMenu  = 1;
				gucItemClockMenu = 0;
				gucItemAdvancedMenu = 0;  
		  		//tempDay = 1; 
				gucEvent = 0; 
				ScheduleMenu( ); 
				 	 
			break;			 
	
			case CLOCK: 
				MenuOnLcd = 0;
				gbClockMenu = 1;
				gbClock     = 1;
gbModeMenu = 0;
				gbScheduleMenu = 0;	
				gbAdvancedMenu  = 0;
				gucItemScheduleMenu  = 0;
				gucItemClockMenu = 1;
				gucItemAdvancedMenu = 0; 
				RTC_Get();	
//				gsiPara = DealBCD(read_clock(8) & 0xFF);
				gsiPara = calendar.w_year;
				gbFlashYearEnable = 1;
 				ShowTime( );
			break;
			case ADVANCED: 	 
				gbClock  = 0;
				gbAdvancedMenu  = 1;
				gbScheduleMenu = 0;
gbModeMenu = 0;
				gbClockMenu = 0;
				gucItemScheduleMenu  = 0;
				gucItemClockMenu = 0;
				gucItemAdvancedMenu = 1;				 
				AdvancedMenu( );
			break;		 
		  
			default:
			gbClock  = 0;
			break;
		}
		b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
		start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT) ;	
	}
 
} //  done with the heating setpoint



//					DisplayIcon(DT7,ON);//show run human
//					DisplayIcon(DT4,OFF);//show moon	
//					DisplayIcon(DT6,OFF); //show human
//					DisplayIcon(DT3,OFF); //show sunshine
//	DisplayIcon(DT11,OFF);
//	DisplayIcon(DT12,OFF);
//	DisplayIcon(DT10,ON);

static void Icon_Control(void)
{	
	uint8 mode_state; 
	mode_state = get_current_mode();
  
	if(ICON_ManualMode == 0)
	{
		DisplayFanSpeedIcon(setpoint_select);
		DisplayHeat_CoolIcon(mode_state);
	}
	else
	{
		if(GetByteBit(&ICON_ManualValue,1))//cool icon on
			DisplayIcon(DT2,ON);	
		else
			DisplayIcon(DT2,OFF);//cool icon off
		
		if(GetByteBit(&ICON_ManualValue,2))//heat icon on
			DisplayIcon(DT8,ON);//	
		else
			DisplayIcon(DT8,OFF);//heat icon off

		if(GetByteBit(&ICON_ManualValue,3))//fan blade 1 icon on
			DisplayIcon(DT11,ON);//	
		else
			DisplayIcon(DT11,OFF);//fan blade 1 off

		if(GetByteBit(&ICON_ManualValue,4))//fan blade 2 icon on
			DisplayIcon(DT12,ON);//	
		else
			DisplayIcon(DT12,OFF);//fan blade 2 icon off

		if(GetByteBit(&ICON_ManualValue,5))//sun icon on
			DisplayIcon(DT3,ON);//	
		else
			DisplayIcon(DT3,OFF);//sun icon off		
		
		if(GetByteBit(&ICON_ManualValue,6))//moon icon on
			DisplayIcon(DT4,ON);//	
		else
			DisplayIcon(DT4,OFF);//moon icon off

		if(GetByteBit(&ICON_ManualValue,7))//occupied icon on
		{
			DisplayIcon(DT6,ON);//	
			DisplayIcon(DT5,ON);//
		}
		else
		{
			DisplayIcon(DT6,OFF);//occupied icon off
			//DisplayIcon(DT5,OFF);//
		}


		if(GetByteBit(&ICON_ManualValue,0))//unoccupied icon on
		{
			DisplayIcon(DT7,ON);//	
			DisplayIcon(DT5,ON);//	
		}
		else
		{
			DisplayIcon(DT7,OFF);//unoccupied icon off
			//DisplayIcon(DT5,OFF);//
		}	

		if((GetByteBit(&ICON_ManualValue,0) == 0) && (GetByteBit(&ICON_ManualValue,7) == 0))//occupied and unoccupied both are off
		{
			DisplayIcon(DT5,OFF);
		}
			
	}
	 
}

static void show_temprature(void)
{
	//static uint8 i;
	//temperature = 100;
	putDigitals1(temperature,EEP_DEGCorF);
	//i++;
	putDigitals2(loop_setpoint[0]/10);
	//putDigitals2(i);
	Icon_Control();
}

static void displayFull(uint16 temp_x10) 
	// for F only: 0 = temp	delta; 1=temperature 
{

	uint16   display_temp ;
	
//  	bit bt;
	display_temp = temp_x10; 
		DispBuf[0] = 0x20;
		DispBuf[1] = 0x20;
	if(display_temp < 100)
	{		
		DispBuf[2] = display_temp/10 + 0x30 ; 
		DispBuf[3] = 0x2e;
		DispBuf[4] = (display_temp%10) +0x30;
		DispBuf[5] = 0X20;
	}
	else
	{
	    DispBuf[2] = (display_temp%1000)/100 + 0x30;
		DispBuf[3] = (display_temp%100)/10 + 0x30;  
		DispBuf[4] = 0x2e;
		DispBuf[5] = (display_temp%10) + 0x30;
	
	}
	if(b.eeprom[EEP_DEGC_OR_F] == 0)
		DispBuf[6] = 'C';
	else
		DispBuf[6] = 'F';
	DispBuf[7] = 0X20;
 
	DispBuf[8] = 0;
	putText(1,DispBuf); 
}
// ------ jump to numbered function -----  
static void run_function( et_jump_function func_no )
{
	unsigned char i  ; 

	switch(	func_no	) 
	{ 
		case SHOW_TEMPERATURE:
			show_temprature();	 

			gucPointValue = get_inverse(gucPointValue);

			ShowTime();
		
			if(gbSetSchedule)
				FlashTime(gucMinute,gucHour,gbHmFlag);
		
			start_timer(TEMP_DISPLAY_TIMER , DEFAULT_TIMEOUT ) ;
		break;
		
		case INCREASE_COOLING_SETPOINT : 
			increase_cooling_setpoint() ;
		break ;

		case DECREASE_COOLING_SETPOINT : 
			decrease_cooling_setpoint() ;
		break ;

		case MODE_SWITCH :
		 	switch_mode() ;
		break ; 
 
		case SELECT_FUNCTION : 
			SelectButton() ;
		break ;
		case INCREASE_FAN_SPEED	:
		 		;
		break ;

		case DECREASE_FAN_SPEED	:
	 		 ;
		break ;
 
		case ACCEPT_NEW_SETTING	: 
			accept_new_setting(); 
		break ;

		case START_MENU_MODE : 
			start_menu_mode();  
		break ;

		case EXIT_MENU_MODE : 
			exit_menu_mode() ;
		break ;

		case START_NEXT_MENU : 
			;
		break ;

		case START_PREVIOUS_MENU : 
//			start_previous_menu() ;
		break ;

		case INCREASE_PARAMETER	: 
			increase_parameter() ;
		break ;

		case DECREASE_PARAMETER	: 
			decrease_parameter() ;
		break ;

		case ACCEPT_PARAMETER : 
//			if(gucItemScheduleMenu == S_TIME2 || gucItemScheduleMenu  == S_MINUTE2)
//				CheckScheduleSet(0);
//			else if(gucItemScheduleMenu  == S_TIME3 || gucItemScheduleMenu  == S_MINUTE3)
//				CheckScheduleSet(1);
//			else if(gucItemScheduleMenu  == S_TIME4 || gucItemScheduleMenu  == S_MINUTE4)
//				CheckScheduleSet(2);
//			accept_parameter() ; //keypad active timer has reached zero, time to update the	setpoints
//		 
			 
	    break ;

		case ACCEPT_EXIT_PARAMETER : 
			accept_parameter() ;
			exit_menu_mode() ; 	
		break ;
   
	 
//		case CLOCK_MENU:
//			 ShowTime();
//			gucPointValue = ~gucPointValue;
//			if(gbSetSchedule)
//			FlashTime(gucMinute,gucHour,gbHmFlag);
//			start_timer(CLOCK_MENU_TIMER, DEFAULT_TIMEOUT);
//		break;

		case LCD_SCREEN_ROTATION_ROUTINE:

			putText(0,(uint8 *)"  TEMP  "); 
			displayFull(temperature);			
			start_timer(LCD_SCREEN_ROTATION_TIMER, DEFAULT_TIMEOUT);	   		
		break;
 
		default:
			break;

	}	// end switch

}	// end run_function()



static void task_init(void)
{
	b.jump_function[TEMP_DISPLAY_TIMER] = SHOW_TEMPERATURE;
	b.jump_function[LCD_SCREEN_ROTATION_TIMER] =LCD_SCREEN_ROTATION_ROUTINE;
	start_timer( TEMP_DISPLAY_TIMER , DEFAULT_TIMEOUT ) ;
	start_timer(LCD_SCREEN_ROTATION_TIMER, DEFAULT_TIMEOUT);
}

 
static void displayinteger(uint16 integer  ) 
	// for F only: 0 = temp	delta; 1=temperature 
{
// 		unsigned char xdata DispBuf[9];
		putText(1,(uint8 *)"        ");
		DispBuf[0] = 0x20;
		DispBuf[1] = 0x20;
		DispBuf[2] = 0x20;
		DispBuf[3] = 0x20;	
		DispBuf[4] = 0x20;

		//Only set these digits if they are not 0
		if ((integer%1000)/100 != 0)
			DispBuf[3] = (integer%1000)/100 + 0x30;
		if ((integer%100)/10 != 0 || DispBuf[0] != 0xFF)
			DispBuf[4] =  (integer%100)/10 + 0x30; 
		DispBuf[5] =  (integer%10) + 0x30;
		DispBuf[6] = 0x20;
		DispBuf[7] = 0x20;   	//
		DispBuf[8] = 0 ;
		putText(1,&DispBuf[0]);  	// 		  
}
 

static void DisplayVer(void)
{ 
	  
	putText(1,(uint8 *)"        ");
	putText(0,(uint8 *)"VERSION ");		  
	displayinteger(EEPROM_VERSION);		   
	delay_ms(2000); 
	putText(1,(uint8 *)"        ");
	putText(0,(uint8 *)"  ADDR  ");	
	//laddress = new_read_eeprom(EEP_ADDRESS );	
	STMFLASH_Read(FLASH_MODBUS_ID, flash_buf, 2);	
	laddress = flash_buf[0];//new_read_eeprom(EEP_ADDRESS );
	flash_buf[0] = 254;
	if((laddress == 0xff)||(laddress == 0)) 
	{
		STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);	
		write_eeprom(EEP_ADDRESS , 254);	
	}
	displayinteger(laddress);	 
	delay_ms(2000); 
    
}
void vTstat7_Menu( void *pvParameters )
{ 
	uint8 i;
	uint8 gucFanCounter = 0;
 	uint8 event;
	uint8 func_no = 0; 
	uint8 timer_no;
	init_timers();
	task_init();
	start_normal_mode(); 
	Tstat7_ClearScreen();
	delay_ms(100);
	DisplayVer(); 	
	for(;;)
	{   
		for( timer_no =	0 ; timer_no < TOTAL_TIMERS; timer_no++	)
		{
									
			if( b.big_ticks[timer_no] != 0 ) 
			{			  
				b.big_ticks[timer_no]-- ;		   

				if( b.big_ticks[timer_no] == 0 ) 
					{			  
						func_no =	b.jump_function[timer_no] ; //func_no is an enumerated type, need this before heading into run_function	
						 
						run_function( (et_jump_function)func_no )	;
					}
			} 
		} // end for loop for software timers: TOTAL_TIMERS  

		// ----- event dispatch	-----------------------------------------
		while( event_count )	   //main event	dispatcher for commands
		{	
			event_count-- ;
			event =	b.event_queue[event_count] ;
			// ------------- run_event_function ----------------
			func_no	 = b.event_function[ event ] ;	

			run_function( (et_jump_function)func_no )	;
		}
		// ----- done dispatching events --------------------------------
		
		gucFanCounter++;
		if(gucFanCounter>4 )
			gucFanCounter = 0;
	 
//		if( current_mode_of_operation[0] == DAYTIME_COASTING )
//		{
//			gucFanCounter = 8;
//			NoFanIcon();
//		 
//		}
		
		if(ICON_ManualMode == 0)
		{
			switch(gucFanCounter/2)
			{
				case 0:
					Fan3Icon(); 
					break;
				case 1:
					Fan2Icon();
					break;
				case 2:
					Fan1Icon();
					break;	 
				default:
				break;
			}
		}
		delay_ms(100);
		
//		i++;
// 	    if(i==10) 
//		{
//			LED_TEST =1; 
//			
//		}
//		else if(i>20)
//		{
//			i = 0;
//			LED_TEST = 0;
//			
//		}
	}
}


void Keypad_Receive(uint8 prev_keypad_state)
{	
	static uint8 keypad_count = 0; 
	uint8 temp;
	switch(prev_keypad_state)
	{ 
		case UP_CODE:
			push_event( UP_KEY ) ; 
			break;
		case DOWN_CODE:
			push_event( DOWN_KEY ); 
			break;
		case ENTER_CODE:
			push_event( ENTER_KEY );
			break;
		case MENU_CODE:
			push_event( MENU_KEY); 
			break;
		case MODE_CODE:
			push_event( MODE_KEY);   
			break;
		 default:
			break; 
	} 
	if(prev_keypad_state == OCCU_CODE)				//winter judge menu delay
	{ 
		keypad_count++ ;
		if(keypad_count > 1) //0.5 sec
		{					               
			 keypad_count = 0 ;
			 temp = PirSensorSelect;//new_read_eeprom(EEP_PIR_SENSOR_SELECT);
			stop_timer( LCD_SCREEN_ROTATION_TIMER); 
			stop_timer(TEMP_DISPLAY_TIMER);
			if (occupied == 0)//EEP_OverrideTimer/*override_timer == OFF && EEP_OverrideTimer && occupied == 0 */)
			{				
	//			occupancy = 1;
				occupied = 1;	  
        setpoint_select = DHOME_SP;				
				//mhf 11-11-05 register 184 should show occupied ,but does not change bit occupied status in the EEPROM
				info_byte |= 0x01;
				new_write_eeprom(EEP_INFO_BYTE,info_byte);
			
				// Also switch the Fan to AUTO when changing the tstat to unoccupied
					
				//	Also switch the Fan to AUTO when changing to occupied mode
				fan_speed_user = AUTO;//fan_table[b.eeprom[EEP_FAN_GRIDPOINT - MAXEEPCONSTRANGE]][b.eeprom[EEP_MODE_GRIDPOINT - MAXEEPCONSTRANGE]] ;
			 
				EEP_FanSpeed = fan_speed_user ;
				new_write_eeprom(EEP_FAN_SPEED,fan_speed_user);	
//				b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
//				start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT) ;
//				MenuOnLcd = 1;
				
				if(1)//(((temp == PIR_SENSOR_DISABLE)||(temp == PIR_SENSOR_ENABLE))/*&&(schedule_on_off ==OFF)*/)  
				{	  
//					override_timer = ON;
					override_timer_time = (unsigned int)60*b.eeprom[EEP_OVERRIDE_TIMER];
//					CalDesireTime(b.eeprom[EEP_OVERRIDE_TIMER - MAXEEPCONSTRANGE],1,0);
//					start_timer( OVERRIDE_TIMER , DEFAULT_TIMEOUT );			

					//new_write_eeprom(EEP_PIR_SENSOR_SELECT,PIR_SENSOR_ENABLE);
					putText(0,"OCCUPIED");  

					DispBuf[0] = 'T';
					DispBuf[1] = 'I';
					DispBuf[2] = 'M';
					DispBuf[3] = 'E';
					DispBuf[4] = 'R';
			
					if(EEP_OverrideTimer > 99)
					{
						DispBuf[5] = 0x30 + EEP_OverrideTimer/100;
						DispBuf[6] = 0x30 +(EEP_OverrideTimer%100)/10;
						DispBuf[7] = 0x30 + EEP_OverrideTimer%10;
					}
					else
					{
						DispBuf[5] = ' ';
						DispBuf[6] = 0x30 + EEP_OverrideTimer/10;
						DispBuf[7] = 0x30 + EEP_OverrideTimer%10;
					}
					DispBuf[8] = 0;
					putText(1,(unsigned char*)DispBuf); 
				}
//				else
//				{
////					override_timer = OFF;
////					override_timer_time = 0;
//					putText(0,(uint8 *)"  OCC   ");
//					putText(1,(uint8 *)"Disable");
//				} 			 
			}	  	  	 
			else
			{
//				if(EEP_OverrideTimer )
//				{
				
					occupied = 0;
					info_byte &= 0xFE;
					new_write_eeprom(EEP_INFO_BYTE,info_byte); 
					setpoint_select = NIGHT_SP;
//					 occupancy = 0;
//					override_timer = OFF;
//					override_timer_time = 0;  
					fan_speed_user = 0;//fan_table[b.eeprom[EEP_FAN_GRIDPOINT - MAXEEPCONSTRANGE]][b.eeprom[EEP_MODE_GRIDPOINT - MAXEEPCONSTRANGE]] ;				 
				 
					EEP_FanSpeed = fan_speed_user ;
					new_write_eeprom(EEP_FAN_SPEED,fan_speed_user); 
          override_timer_time = 0;
					if(temp == PIR_SENSOR_ENABLE)/*&&(schedule_on_off ==OFF))*/
					{	 
						new_write_eeprom(EEP_PIR_SENSOR_SELECT,PIR_SENSOR_DISABLE);
					}
//					putText(0,(uint8 *)"  OCC   ");
//					putText(1,(uint8 *)"Disable");
//				}	
					putText(0,	(uint8 *)" UNOCC  ");
					putText(1,	(uint8 *)"        ");					
			}
			//start_timer(LCD_SCREEN_ROTATION_TIMER, DEFAULT_TIMEOUT);	  	 
		}
		b.jump_function[ KEYPAD_ACTIVE_TIMER] =	EXIT_MENU_MODE;
		start_timer( KEYPAD_ACTIVE_TIMER , DEFAULT_TIMEOUT) ;
		MenuOnLcd = 1; 
		Icon_Control();		
	}			
	else 
	{ 
		keypad_count = 0 ;
	}
}


#endif //TSTAT7_ARM
